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
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QPluginLoader>
#include <QtCore/QStandardPaths>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QDebug>
#include "articlesmodel.h"
#include "feeddata.h"
#include "feedmodel.h"
#include "ifeedsource.h"
#include "iofflinearticlescorer.h"

static const char *FEEDS_CONFIG_FILE = "feeds.json";
static const char *NAME_KEY = "name";
static const char *SOURCE_KEY = "source";
static const char *LAST_UPDATED_KEY = "lastUpdated";
static const char *PROVIDER_KEY = "provider";

bool sortScore(const ScoredArticle &article1, const ScoredArticle &article2)
{
    return article1.score > article2.score;
}

FeedManagerPrivate::FeedManagerPrivate(FeedManager *q)
    : networkAccessManager(0), q_ptr(q)
{
}

FeedManagerPrivate::~FeedManagerPrivate()
{
    clear();
    qDeleteAll(m_downloadFeedReplies.keys());
}

QNetworkReply * FeedManagerPrivate::getAddFeedReply(const QString &type, const QUrl &source,
                                                    FeedManager *feedManager)
{
    IFeedSource *plugin = qobject_cast<IFeedSource *>(FeedManagerPrivate::getPlugin(type));
    if (!plugin) {
        return 0;
    }

    qDebug() << "Plugin:" << plugin;
    qDebug() << "Loading feed with URL" << source;

    QNetworkAccessManager *networkAccessManager = feedManager->d_func()->networkAccessManager;
    return plugin->downloadFeedInfo(source, networkAccessManager);
}

void FeedManagerPrivate::addFeed(const FeedData &feed, QObject *plugin, FeedManager *feedManager)
{
    FeedManagerItem *item = new FeedManagerItem;
    item->feed = feed;
    item->plugin = plugin;
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

        QObject *plugin = FeedManagerPrivate::getPlugin(provider);

        if (plugin) {
            FeedManagerItem *item = new FeedManagerItem;
            item->feed = FeedData(name, QUrl(source), lastUpdated);
            item->plugin = plugin;
            m_items.append(item);
            qDebug() << "Feed found:" << name << source << lastUpdated << provider;
            qDebug() << "Plugin:" << plugin;
        }
    }
}

void FeedManagerPrivate::save()
{
    QJsonDocument document;
    QJsonArray array;

    foreach (FeedManagerItem *item, m_items) {
        IFeedSource *feedSource = qobject_cast<IFeedSource *>(item->plugin);
        if (feedSource) {
            QJsonObject object = FeedManagerPrivate::feedToObject(item->feed);
            object.insert(PROVIDER_KEY, feedSource->name());
            array.append(object);
        }
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

QObject * FeedManagerPrivate::getPlugin(const QString &type)
{
    QObject *plugin = 0;
    foreach (QObject *staticPlugin, QPluginLoader::staticInstances()) {
        IFeedSource *feedSource = qobject_cast<IFeedSource *>(staticPlugin);
        if (feedSource && feedSource->name() == type) {
            plugin = staticPlugin;
        }
    }
    return plugin;
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

void FeedManagerPrivate::slotDownloadFeedFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (m_downloadFeedReplies.contains(reply)) {
        FeedManagerItem *item = m_downloadFeedReplies.value(reply);
        IFeedSource *feedSource = qobject_cast<IFeedSource *>(item->plugin);
        if (feedSource) {
            // TODO: merge articles instead of replacing
            item->articles = score(feedSource->processFeed(item->feed, reply));
            foreach (ArticlesModel *model, item->articleModels) {
                model->setArticles(item->articles);
            }
            qDebug() << "Setting" << item->articles.count() << "articles for" << item->feed.name();
            qDebug() << item->articleModels.count() << "models registered";
        }
    }

    if (reply) {
        reply->deleteLater();
    }
}

QList<ArticleData> FeedManagerPrivate::score(const QList<ArticleData> &articles)
{
    QList<IOfflineArticleScorer *> scorers;
    foreach (QObject *object, QPluginLoader::staticInstances()) {
        IOfflineArticleScorer *scorer = qobject_cast<IOfflineArticleScorer *>(object);
        if (scorer) {
            scorers.append(scorer);
        }
    }

    QList<ArticleData> returnedArticles;
    QList<ScoredArticle> scoredArticles;
    foreach (const ArticleData &article, articles) {
        ScoredArticle scoredArticle;
        scoredArticle.article = article;
        scoredArticle.score = 1.;
        foreach (IOfflineArticleScorer *scorer, scorers) {
            scoredArticle.score = scoredArticle.score * scorer->score(article);
        }

        scoredArticles.append(scoredArticle);
    }
    std::sort(scoredArticles.begin(), scoredArticles.end(), sortScore);


    foreach (const ScoredArticle &scoredArticle, scoredArticles) {
        returnedArticles.append(scoredArticle.article);
    }

    return returnedArticles;
}

////// End of private class //////

FeedManager::FeedManager(QObject *parent) :
    QObject(parent), d_ptr(new FeedManagerPrivate(this))
{
    Q_D(FeedManager);
    d->networkAccessManager = new QNetworkAccessManager(this);
    d->init();
}

FeedManager::~FeedManager()
{
}

void FeedManager::load()
{
    Q_D(FeedManager);
    foreach (FeedManagerItem *item, d->m_items) {
        IFeedSource *feedSource = qobject_cast<IFeedSource *>(item->plugin);
        if (feedSource) {
            QNetworkReply *reply = feedSource->downloadFeed(item->feed, d->networkAccessManager);
            connect(reply, &QNetworkReply::finished, d, &FeedManagerPrivate::slotDownloadFeedFinished);
            d->m_downloadFeedReplies.insert(reply, item);
            qDebug() << "Starting refreshing feed for" << item->feed.name();
        }
    }
}

//void FeedManager::deleteFeed(Feed *feed)
//{

//}
