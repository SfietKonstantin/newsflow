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

#include "feed.h"
#include "feed_p.h"

FeedPrivate::FeedPrivate(Feed *q):
    q_ptr(q)
{
}

Feed::Feed(QObject *parent):
    QObject(parent), d_ptr(new FeedPrivate(this))
{
}

Feed::Feed(FeedPrivate &dd, QObject *parent):
    QObject(parent), d_ptr(&dd)
{
}

Feed::~Feed()
{
}

Feed * Feed::create(const FeedData &feedData, QObject *parent)
{
    Feed * returned = new Feed(parent);
    returned->d_func()->data = feedData;
    return returned;
}

Feed * Feed::create(const QString &name, const QUrl &source, const QDateTime &lastUpdated, QObject *parent)
{
    Feed * returned = new Feed(parent);
    returned->d_func()->data.setName(name);
    returned->d_func()->data.setSource(source);
    returned->d_func()->data.setLastUpdated(lastUpdated);
    return returned;
}

QString Feed::name() const
{
    Q_D(const Feed);
    return d->data.name();
}

QUrl Feed::source() const
{
    Q_D(const Feed);
    return d->data.source();
}

QDateTime Feed::lastUpdated() const
{
    Q_D(const Feed);
    return d->data.lastUpdated();
}

FeedData Feed::toData() const
{
    Q_D(const Feed);
    return d->data;
}
