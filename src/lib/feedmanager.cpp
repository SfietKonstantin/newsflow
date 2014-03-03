/*
 * Copyright (C) 2014 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "feedmanager.h"
#include "feedmanager_p.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QPluginLoader>
#include <QtCore/QStandardPaths>
#include <QtCore/QThreadPool>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "abstractarticlescorer.h"
#include "articlesmodel.h"
#include "feeddata.h"
#include "feedmodel.h"
#include "ifeedsource.h"
#include "iarticlescorer.h"

static const char *FEEDS_CONFIG_FILE = "feeds.json";
static const char *NAME_KEY = "name";
static const char *SOURCE_KEY = "source";
static const char *LAST_UPDATED_KEY = "lastUpdated";
static const char *PROVIDER_KEY = "provider";

bool sortScore(ScoredArticle *article1, ScoredArticle *article2)
{
    return article1->score > article2->score;
}

FeedManagerItem::~FeedManagerItem()
{
    feedFetcher->deleteLater();
}

FeedManagerItemScorer::FeedManagerItemScorer(QObject *parent)
    : QObject(parent)
{
}

FeedManagerItemScorer::~FeedManagerItemScorer()
{
    qDeleteAll(scoredArticles);
    qDeleteAll(mappedScoredArticles.keys());
}

void FeedManagerItemScorer::slotLoaded(bool ok)
{
    AbstractArticleScorer *scorer = qobject_cast<AbstractArticleScorer *>(sender());
    ScoredArticle *article = mappedScoredArticles.value(scorer);
    if (!scorer) {
        return;
    }

    if (!article) {
        scorer->deleteLater();
        return;
    }

    if (ok) {
        article->score *= scorer->score();
    }

    mappedScoredArticles.remove(scorer);
    scorer->deleteLater();

    // All scores has been fetched
    if (mappedScoredArticles.isEmpty()) {
        emit finished();
    }
}

FeedManagerPrivate::FeedManagerPrivate(FeedManager *q)
    : QObject(), networkAccessManager(0), threadPool(0), q_ptr(q)
{
}

FeedManagerPrivate::~FeedManagerPrivate()
{
    clear();
    qDeleteAll(m_feedFetcherToItem.keys());
}

void FeedManagerPrivate::addFeed(const QString &provider, const FeedData &feed,
                                 AbstractFeedFetcher *feedFetcher, FeedManager *feedManager)
{
    FeedManagerItem *item = new FeedManagerItem;
    item->provider = provider;
    item->feed = feed;
    item->feedFetcher = feedFetcher;
    item->feedFetcher->setSource(item->feed.source());
    feedManager->d_func()->m_feedFetcherToItem.insert(item->feedFetcher, item);
    connect(item->feedFetcher, &AbstractFeedFetcher::feedLoaded,
            feedManager->d_func(), &FeedManagerPrivate::slotFeedLoaded);
    feedManager->d_func()->m_items.append(item);
    feedManager->d_func()->save();

    foreach (FeedModel *model, feedManager->d_func()->m_feedModels) {
        model->addFeed(feed);
    }
}

void FeedManagerPrivate::registerFeedModel(FeedModel *model, FeedManager *feedManager)
{
    feedManager->d_func()->m_feedModels.insert(model);
    QList<FeedData> feeds;
    foreach (FeedManagerItem *item, feedManager->d_func()->m_items) {
        feeds.append(item->feed);
    }
    model->setFeeds(feeds);
}

void FeedManagerPrivate::unregisterFeedModel(FeedModel *model, FeedManager *feedManager)
{
    feedManager->d_func()->m_feedModels.remove(model);
}

void FeedManagerPrivate::registerArticleModel(ArticlesModel *model, const FeedData &feed,
                                              FeedManager *feedManager)
{
    FeedManagerItem *item = 0;
    bool found = false;
    foreach (item, feedManager->d_func()->m_items) {
        if (item->feed == feed) {
            found = true;
            break;
        }
    }

    if (!found) {
        return;
    }

    item->articleModels.insert(model);
    model->setArticles(item->articles);
}

void FeedManagerPrivate::unregisterArticleModel(ArticlesModel *model, const FeedData &feed,
                                                FeedManager *feedManager)
{
    FeedManagerItem *item = 0;
    bool found = false;
    foreach (item, feedManager->d_func()->m_items) {
        if (item->feed == feed) {
            found = true;
            break;
        }
    }

    if (!found) {
        return;
    }

    item->articleModels.remove(model);
}

void FeedManagerPrivate::init()
{
    Q_Q(FeedManager);
    QDir dir = configPath();
    QFile file (dir.absoluteFilePath(FEEDS_CONFIG_FILE));
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open feeds config";
        return;
    }

    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "Error parsing feeds config" << error.errorString();
        return;
    }

    clear();

    QJsonArray array = document.array();
    foreach (const QJsonValue &value, array) {
        QJsonObject valueObject = value.toObject();
        QString name = valueObject.value(NAME_KEY).toString();
        QString source = valueObject.value(SOURCE_KEY).toString();
        QString lastUpdatedString = valueObject.value(LAST_UPDATED_KEY).toString();
        QDateTime lastUpdated = QDateTime::fromString(lastUpdatedString, Qt::ISODate);
        QString provider = valueObject.value(PROVIDER_KEY).toString();

        AbstractFeedFetcher *feedFetcher = FeedManagerPrivate::getFeedFetcher(provider, q);

        if (feedFetcher) {
            FeedManagerItem *item = new FeedManagerItem;
            item->provider = provider;
            item->feed = FeedData(name, QUrl(source), lastUpdated);
            item->feedFetcher = feedFetcher;
            item->feedFetcher->setSource(item->feed.source());
            m_feedFetcherToItem.insert(item->feedFetcher, item);
            connect(item->feedFetcher, &AbstractFeedFetcher::feedLoaded,
                    this, &FeedManagerPrivate::slotFeedLoaded);
            m_items.append(item);
            qDebug() << "Feed found:" << name << source << lastUpdated << provider;
            qDebug() << "FeedFetcher created:" << feedFetcher;
        }
    }
}

void FeedManagerPrivate::save()
{
    QJsonDocument document;
    QJsonArray array;

    foreach (FeedManagerItem *item, m_items) {
        QJsonObject object = FeedManagerPrivate::feedToObject(item->feed);
        object.insert(PROVIDER_KEY, item->provider);
        array.append(object);
    }
    document.setArray(array);

    QDir dir = FeedManagerPrivate::configPath();
    QFile file (dir.absoluteFilePath(FEEDS_CONFIG_FILE));
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open feeds config";
        return;
    }

    file.write(document.toJson());
    file.close();
}

void FeedManagerPrivate::clear()
{
    qDeleteAll(m_items);
    m_items.clear();
}

AbstractFeedFetcher *FeedManagerPrivate::getFeedFetcher(const QString &type,
                                                        FeedManager *feedManager)
{
    AbstractFeedFetcher *feedFetcher = 0;
    foreach (QObject *staticPlugin, QPluginLoader::staticInstances()) {
        IFeedSource *feedSource = qobject_cast<IFeedSource *>(staticPlugin);
        if (feedSource && feedSource->name() == type) {
            feedFetcher = feedSource->feedFetcher(feedManager->d_func()->networkAccessManager,
                                                  feedManager->d_func()->threadPool,
                                                  feedManager->d_func());
        }
    }
    return feedFetcher;
}

QDir FeedManagerPrivate::configPath()
{
    QString organizationName = QCoreApplication::instance()->organizationName();
    QString applicationName = QCoreApplication::instance()->applicationName();
    QDir dir = QDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if (!dir.exists(organizationName)) {
        Q_ASSERT(!dir.mkdir(organizationName));
    }
    dir.cd(organizationName);
    if (!dir.exists(applicationName)) {
        Q_ASSERT(dir.mkdir(applicationName));
    }
    dir.cd(applicationName);
    return dir;
}

QJsonObject FeedManagerPrivate::feedToObject(const FeedData &feed)
{
    QJsonObject object;
    object.insert(NAME_KEY, feed.name());
    object.insert(SOURCE_KEY, feed.source().toString());
    object.insert(LAST_UPDATED_KEY, feed.lastUpdated().toString(Qt::ISODate));
    return object;
}

void FeedManagerPrivate::slotFeedLoaded(bool ok)
{
    AbstractFeedFetcher *feedFetcher = qobject_cast<AbstractFeedFetcher *>(sender());
    if (m_feedFetcherToItem.contains(feedFetcher)) {
        FeedManagerItem *item = m_feedFetcherToItem.value(feedFetcher);
        if (!ok) {
            qDebug() << "Error fetching feed for" << item->feed.name();
            qDebug() << "Error:" << feedFetcher->errorString();
            return;
        }

        beginScore(item, feedFetcher->feed());
    }
}

void FeedManagerPrivate::slotScoringFinished()
{
    FeedManagerItemScorer *itemScorer = qobject_cast<FeedManagerItemScorer *>(sender());
    if (!itemScorer) {
        return;
    }

    FeedManagerItem *item = itemScorer->item;
    item->articles.clear();

    std::sort(itemScorer->scoredArticles.begin(), itemScorer->scoredArticles.end(), sortScore);
    foreach (ScoredArticle *scoredArticle, itemScorer->scoredArticles) {
        item->articles.append(scoredArticle->article);
    }

    foreach (ArticlesModel *model, item->articleModels) {
        model->setArticles(item->articles);
    }
    qDebug() << "Setting" << item->articles.count() << "articles for" << item->feed.name();
    qDebug() << item->articleModels.count() << "models registered";


    itemScorer->deleteLater();
}

void FeedManagerPrivate::beginScore(FeedManagerItem *item, const QList<ArticleData> &articles)
{
    QList<IArticleScorer *> scorers;
    foreach (QObject *object, QPluginLoader::staticInstances()) {
        IArticleScorer *scorer = qobject_cast<IArticleScorer *>(object);
        if (scorer) {
            scorers.append(scorer);
        }
    }

    FeedManagerItemScorer *itemScorer = new FeedManagerItemScorer(this);
    itemScorer->item = item;
    foreach (const ArticleData &article, articles) {
        ScoredArticle* scoredArticle = new ScoredArticle;
        scoredArticle->article = article;
        scoredArticle->score = 1.;
        foreach (IArticleScorer *scorer, scorers) {
            AbstractArticleScorer *articleScorer = scorer->articleScorer(networkAccessManager,
                                                                         threadPool, this);
            articleScorer->setArticle(article);
            connect(articleScorer, &AbstractArticleScorer::loaded,
                    itemScorer, &FeedManagerItemScorer::slotLoaded);
            itemScorer->mappedScoredArticles.insert(articleScorer, scoredArticle);
        }

        itemScorer->scoredArticles.append(scoredArticle);
    }

    connect(itemScorer, &FeedManagerItemScorer::finished,
            this, &FeedManagerPrivate::slotScoringFinished);

    foreach (AbstractArticleScorer *articleScorer, itemScorer->mappedScoredArticles.keys()) {
        articleScorer->load();
    }
}

////// End of private class //////

FeedManager::FeedManager(QObject *parent) :
    QObject(parent), d_ptr(new FeedManagerPrivate(this))
{
    Q_D(FeedManager);
    d->networkAccessManager = new QNetworkAccessManager(this);
    d->threadPool = new QThreadPool(this);
    d->threadPool->setMaxThreadCount(1);
    d->init();
}

FeedManager::~FeedManager()
{
}

void FeedManager::load()
{
    Q_D(FeedManager);
    foreach (FeedManagerItem *item, d->m_items) {
        item->feedFetcher->loadFeed();
        qDebug() << "Starting refreshing feed for" << item->feed.name();
    }
}

void FeedManager::removeFeed(Feed *feed)
{
    Q_D(FeedManager);
    FeedData data = feed->toData();
    FeedManagerItem *item;
    foreach (item, d->m_items) {
        if (item->feed == data) {
            break;
        }
    }

    d->m_items.removeAll(item);
    delete item;

    QList<FeedData> feedData;
    foreach (item, d->m_items) {
        feedData.append(item->feed);
    }

    foreach (FeedModel *model, d->m_feedModels) {
        model->setFeeds(feedData);
    }
    d->save();
}
