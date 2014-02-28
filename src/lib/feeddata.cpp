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

#include "feeddata.h"
#include "feeddata_p.h"

FeedData::FeedData()
    : d(new FeedDataPrivate())
{
}

FeedData::FeedData(const QString &name, const QUrl &source, const QDateTime &lastUpdated)
    : d(new FeedDataPrivate())
{
    d->name = name;
    d->source = source;
    d->lastUpdated = lastUpdated;
}

FeedData::FeedData(const FeedData &other)
    : d(other.d)
{
}

FeedData::~FeedData()
{
}

FeedData & FeedData::operator=(const FeedData &other)
{
    d = other.d;
    return *this;
}

bool FeedData::operator==(const FeedData &other) const
{
    if (d->name != other.name()) {
        return false;
    }

    if (d->source != other.source()) {
        return false;
    }

    if (d->lastUpdated != other.lastUpdated()) {
        return false;
    }

    return true;
}

bool FeedData::isNull() const
{
    return d->name.isEmpty() && d->source.isEmpty() && d->lastUpdated.isNull();
}

QString FeedData::name() const
{
    return d->name;
}

void FeedData::setName(const QString &name)
{
    d->name = name;
}

QUrl FeedData::source() const
{
    return d->source;
}

void FeedData::setSource(const QUrl &source)
{
    d->source = source;
}

QDateTime FeedData::lastUpdated() const
{
    return d->lastUpdated;
}

void FeedData::setLastUpdated(const QDateTime &lastUpdated)
{
    d->lastUpdated = lastUpdated;
}
