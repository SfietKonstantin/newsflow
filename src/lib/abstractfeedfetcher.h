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

#ifndef ABSTRACTFEEDFETCHER_H
#define ABSTRACTFEEDFETCHER_H

#include "newsflow_global.h"
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include "articledata.h"
#include "feeddata.h"

class QNetworkAccessManager;
class QThreadPool;
class AbstractFeedFetcherPrivate;

/**
 * @brief Base class for class used to fetch feeds
 *
 * A feed fetcher is used to fetch, process and format
 * a certain type of feed. One instance of this class is
 * created for each feed registered in news flow.
 *
 * This class provides a series of useful properties used
 * to process feeds. Source of the feed is accessed via
 * source(). Currently only QUrl are supported for source,
 * but this might change in the future.
 *
 * Access to network is done via networkAccessManager(),
 * and access to a thread pool is done via threadPool().
 * AbstractFeedFetcher works in the application thread,
 * so there shouldn't be any heavy processing done in
 * implementations of this class. Prefer using QRunnable
 * and launch them in the thread pool.
 *
 * AbstractFeedFetcher have two functions to reimplement:
 * - loadFeedInfo()
 * - loadFeed()
 *
 * The first method should start an operation that fetches
 * the feed information and store them in a FeedData. The
 * second method should start an operation that load and
 * process feeds, and store them in a lost of ArticleData.
 *
 * feedInfoLoaded() and feedLoaded() must be emitted when
 * the corresponding operation has finished.
 *
 * Remark: network operations should be done in the
 * main thread. Having them running in a thread pool can
 * have unpredicted results.
 */
class NEWSFLOW_EXPORT AbstractFeedFetcher : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor
     * @param parent parent object.
     */
    explicit AbstractFeedFetcher(QObject *parent = 0);
    /**
     * @brief Overloaded constructor
     * @param networkAccessManager network access.
     * @param threadPool thread pool.
     * @param parent parent object.
     */
    explicit AbstractFeedFetcher(QNetworkAccessManager *networkAccessManager,
                                 QThreadPool *threadPool, QObject *parent = 0);
    /**
     * @brief Destructor
     */
    virtual ~AbstractFeedFetcher();
    /**
     * @brief Network access
     * @return QNetworkAccessManager used to perform network access.
     */
    QNetworkAccessManager * networkAccessManager() const;
    /**
     * @brief Set the network access
     * @param networkAccessManager QNetworkAccessManager used to perform network access.
     */
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);
    /**
     * @brief Thread pool
     * @return QThreadPool used to provide a thread pool.
     */
    QThreadPool * threadPool() const;
    /**
     * @brief Set the thread pool
     * @param threadPool QThreadPool used to provide a thread pool.
     */
    void setThreadPool(QThreadPool *threadPool);
    QUrl source() const;
    void setSource(const QUrl &source);
    QString errorString() const;
    FeedData feedInfo() const;
    QList<ArticleData> feed() const;
public Q_SLOTS:
    virtual void loadFeedInfo() = 0;
    virtual void loadFeed() = 0;
Q_SIGNALS:
    void feedInfoLoaded(bool ok);
    void feedLoaded(bool ok);
protected:
    void setErrorString(const QString &errorString);
    void setFeedInfo(const FeedData &feedInfo);
    void setFeed(const QList<ArticleData> &feed);
    explicit AbstractFeedFetcher(AbstractFeedFetcherPrivate &dd, QObject *parent = 0);
    QScopedPointer<AbstractFeedFetcherPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(AbstractFeedFetcher)
};

#endif // ABSTRACTFEEDFETCHER_H
