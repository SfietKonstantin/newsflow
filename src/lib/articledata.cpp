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

#include "articledata.h"
#include "articledata_p.h"

ArticleData::ArticleData()
    : d(new ArticleDataPrivate())
{
}

ArticleData::ArticleData(const QString &title, const QString &description, const QUrl &link,
                         const QString &image, const QDateTime &timestamp)
    : d(new ArticleDataPrivate())
{
    d->title = title;
    d->description = description;
    d->link = link;
    d->image = image;
    d->timestamp = timestamp;
}

ArticleData::ArticleData(const ArticleData &other)
    : d(other.d)
{
}

ArticleData::~ArticleData()
{
}

ArticleData & ArticleData::operator=(const ArticleData &other)
{
    d = other.d;
    return *this;
}

bool ArticleData::operator==(const ArticleData &other) const
{
    if (d->title != other.title()) {
        return false;
    }

    if (d->description != other.description()) {
        return false;
    }

    if (d->link != other.link()) {
        return false;
    }

    if (d->image != other.image()) {
        return false;
    }

    if (d->timestamp != other.timestamp()) {
        return false;
    }

    return true;
}

bool ArticleData::isNull() const
{
    return d->title.isEmpty() && d->description.isEmpty() && d->link.isEmpty() && d->image.isNull()
           && d->timestamp.isNull();
}

QString ArticleData::title() const
{
    return d->title;
}

void ArticleData::setTitle(const QString &title)
{
    d->title = title;
}

QString ArticleData::description() const
{
    return d->description;
}

void ArticleData::setDescription(const QString &description)
{
    d->description = description;
}

QUrl ArticleData::link() const
{
    return d->link;
}

void ArticleData::setLink(const QUrl &link)
{
    d->link = link;
}

QString ArticleData::image() const
{
    return d->image;
}

void ArticleData::setImage(const QString &image)
{
    d->image = image;
}

QDateTime ArticleData::timestamp() const
{
    return d->timestamp;
}

void ArticleData::setTimestamp(const QDateTime &timestamp)
{
    d->timestamp = timestamp;
}
