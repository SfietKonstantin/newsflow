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

#ifndef FEEDMANAGER_P_H
#define FEEDMANAGER_P_H

#include <QtCore/QDir>
#include <QtCore/QJsonObject>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QSet>
#include "abstractfeedfetcher.h"
#include "articledata.h"
#include "feeddata.h"
#include "feedmanager.h"

class QNetworkAccessManager;
class QNetworkReply;
class QThreadPool;
class ArticlesModel;
class FeedAdder;
class FeedModel;
class AbstractArticleScorer;

struct FeedManagerItem
{
    virtual ~FeedManagerItem();
    QString provider;
    FeedData feed;
    AbstractFeedFetcher *feedFetcher;
    QList<ArticleData> articles;
    QSet<ArticlesModel *> articleModels;
};

struct ScoredArticle
{
    ArticleData article;
    float score;
};

class FeedManagerItemScorer: public QObject
{
    Q_OBJECT
public:
    explicit FeedManagerItemScorer(QObject *parent = 0);
    virtual ~FeedManagerItemScorer();
    void slotLoaded(bool ok);
    FeedManagerItem *item;
    QList<ScoredArticle *> scoredArticles;
    QMap<AbstractArticleScorer *, ScoredArticle *> mappedScoredArticles;
Q_SIGNALS:
    void finished();
};

class FeedManagerPrivate: public QObject
{
    Q_OBJECT
public:
    explicit FeedManagerPrivate(FeedManager *q);
    virtual ~FeedManagerPrivate();
    static void addFeed(const QString &provider, const FeedData &feed,
                        AbstractFeedFetcher *feedFetcher, FeedManager *feedManager);
    static void registerFeedModel(FeedModel *model, FeedManager *feedManager);
    static void unregisterFeedModel(FeedModel *model, FeedManager *feedManager);
    static void registerArticleModel(ArticlesModel *model, const FeedData &feed,
                                     FeedManager *feedManager);
    static void unregisterArticleModel(ArticlesModel *model, const FeedData &feed,
                                       FeedManager *feedManager);
    void init();
    void save();
    void clear();
    static AbstractFeedFetcher * getFeedFetcher(const QString &type, FeedManager *feedManager);
    static QDir configPath();
    static QJsonObject feedToObject(const FeedData &feed);
    QNetworkAccessManager *networkAccessManager;
    QThreadPool *threadPool;
protected:
    FeedManager * const q_ptr;
private Q_SLOTS:
    void slotFeedLoaded(bool ok);
    void slotScoringFinished();
private:
    void beginScore(FeedManagerItem *item, const QList<ArticleData> &articles);
    QList<FeedManagerItem *> m_items;
    QSet<FeedModel *> m_feedModels;
    QMap<AbstractFeedFetcher *, FeedManagerItem *> m_feedFetcherToItem;
    Q_DECLARE_PUBLIC(FeedManager)
};

#endif // FEEDMANAGER_P_H
