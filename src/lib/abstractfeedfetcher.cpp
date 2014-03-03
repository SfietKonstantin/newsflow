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

#include "abstractfeedfetcher.h"
#include "abstractfeedfetcher_p.h"

AbstractFeedFetcherPrivate::AbstractFeedFetcherPrivate(AbstractFeedFetcher *q)
    : QObject(), networkAccessManager(0), threadPool(0), q_ptr(q)
{
}

AbstractFeedFetcherPrivate::~AbstractFeedFetcherPrivate()
{
}

////// End of private class //////

AbstractFeedFetcher::AbstractFeedFetcher(QObject *parent)
    : QObject(parent), d_ptr(new AbstractFeedFetcherPrivate(this))
{
}

AbstractFeedFetcher::AbstractFeedFetcher(QNetworkAccessManager *networkAccessManager,
                                         QThreadPool *threadPool, QObject *parent)
    : QObject(parent), d_ptr(new AbstractFeedFetcherPrivate(this))
{
    Q_D(AbstractFeedFetcher);
    d->networkAccessManager = networkAccessManager;
    d->threadPool = threadPool;
}

AbstractFeedFetcher::AbstractFeedFetcher(AbstractFeedFetcherPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

AbstractFeedFetcher::~AbstractFeedFetcher()
{
}

QNetworkAccessManager * AbstractFeedFetcher::networkAccessManager() const
{
    Q_D(const AbstractFeedFetcher);
    return d->networkAccessManager;
}

void AbstractFeedFetcher::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    Q_D(AbstractFeedFetcher);
    d->networkAccessManager = networkAccessManager;
}

QThreadPool * AbstractFeedFetcher::threadPool() const
{
    Q_D(const AbstractFeedFetcher);
    return d->threadPool;
}

void AbstractFeedFetcher::setThreadPool(QThreadPool *threadPool)
{
    Q_D(AbstractFeedFetcher);
    d->threadPool = threadPool;
}

QUrl AbstractFeedFetcher::source() const
{
    Q_D(const AbstractFeedFetcher);
    return d->source;
}

void AbstractFeedFetcher::setSource(const QUrl &source)
{
    Q_D(AbstractFeedFetcher);
    d->source = source;
}

QString AbstractFeedFetcher::errorString() const
{
   Q_D(const AbstractFeedFetcher);
    return d->errorString;
}

FeedData AbstractFeedFetcher::feedInfo() const
{
    Q_D(const AbstractFeedFetcher);
    return d->feedInfo;
}

QList<ArticleData> AbstractFeedFetcher::feed() const
{
    Q_D(const AbstractFeedFetcher);
    return d->feed;
}

void AbstractFeedFetcher::setErrorString(const QString &errorString)
{
    Q_D(AbstractFeedFetcher);
    d->errorString = errorString;
}

void AbstractFeedFetcher::setFeedInfo(const FeedData &feedInfo)
{
    Q_D(AbstractFeedFetcher);
    d->feedInfo = feedInfo;
}

void AbstractFeedFetcher::setFeed(const QList<ArticleData> &feed)
{
    Q_D(AbstractFeedFetcher);
    d->feed = feed;
}
