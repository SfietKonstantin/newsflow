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

#include "feedadder.h"
#include "feedadder_p.h"
#include <QtNetwork/QNetworkReply>
#include "feedmanager_p.h"
#include "processerror.h"

FeedAdderPrivate::FeedAdderPrivate(FeedAdder *q)
    : feedManager(0), feedFetcher(0), q_ptr(q)
{
}

FeedAdderPrivate::~FeedAdderPrivate()
{
}

void FeedAdderPrivate::setStatus(FeedAdder::Status newStatus)
{
    Q_Q(FeedAdder);
    if (status != newStatus) {
        status = newStatus;
        emit q->statusChanged();
    }
}

void FeedAdderPrivate::slotFinished(bool ok)
{
    Q_Q(FeedAdder);
    if (!ok) {
        setStatus(FeedAdder::Error);
        return;
    }

    feed = feedFetcher->feedInfo();
    if (feedName != feed.name()) {
        feedName = feed.name();
        emit q->feedNameChanged();
    }

    setStatus(FeedAdder::Idle);
}

////// End of private class //////

FeedAdder::FeedAdder(QObject *parent)
    : QObject(parent), d_ptr(new FeedAdderPrivate(this))
{
}

FeedAdder::FeedAdder(FeedAdderPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

FeedAdder::~FeedAdder()
{
}

FeedAdder::Status FeedAdder::status() const
{
    Q_D(const FeedAdder);
    return d->status;
}

FeedManager * FeedAdder::feedManager() const
{
    Q_D(const FeedAdder);
    return d->feedManager;
}

void FeedAdder::setFeedManager(FeedManager *feedManager)
{
    Q_D(FeedAdder);
    if (d->feedManager != feedManager) {
        d->feedManager = feedManager;
        emit feedManagerChanged();
    }
}

QString FeedAdder::type() const
{
    Q_D(const FeedAdder);
    return d->type;
}

void FeedAdder::setType(const QString &type)
{
    Q_D(FeedAdder);
    if (d->type != type) {
        d->type = type;
        emit typeChanged();
    }
}

QUrl FeedAdder::source() const
{
    Q_D(const FeedAdder);
    return d->source;
}

void FeedAdder::setSource(const QUrl &source)
{
    Q_D(FeedAdder);
    if (d->source != source) {
        d->source = source;
        emit sourceChanged();
    }
}

QString FeedAdder::feedName() const
{
    Q_D(const FeedAdder);
    return d->feedName;
}

QString FeedAdder::feedAlternateName() const
{
    Q_D(const FeedAdder);
    return d->feedAlternateName;
}

void FeedAdder::setFeedAlternateName(const QString &feedAlternateName)
{
    Q_D(FeedAdder);
    if (d->feedAlternateName != feedAlternateName) {
        d->feedAlternateName = feedAlternateName;
        emit feedAlternateNameChanged();
    }
}

bool FeedAdder::addFeed()
{
    Q_D(FeedAdder);
    if (d->status == Loading) {
        return false;
    }

    if (!d->feedManager) {
        return false;
    }

    d->feedFetcher = FeedManagerPrivate::getFeedFetcher(d->type, d->feedManager);
    d->addedType = d->type;
    if (!d->feedFetcher) {
        return false;
    }

    d->feedFetcher->setSource(d->source);
    connect(d->feedFetcher, &AbstractFeedFetcher::feedInfoLoaded, d, &FeedAdderPrivate::slotFinished);
    d->feedFetcher->loadFeedInfo();

    d->setStatus(Loading);
    return true;
}

bool FeedAdder::saveFeed()
{
    Q_D(FeedAdder);
    if (!d->feedManager) {
        return false;
    }

    if (!d->feedFetcher) {
        return false;
    }

    if (d->status != FeedAdder::Idle) {
        return false;
    }

    d->feed.setName(d->feedAlternateName);
    FeedManagerPrivate::addFeed(d->addedType, d->feed, d->feedFetcher, d->feedManager);
    return true;
}
