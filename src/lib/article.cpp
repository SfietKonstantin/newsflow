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

#include "article.h"
#include "article_p.h"

ArticlePrivate::ArticlePrivate(Article *q):
    q_ptr(q)
{
}

Article::Article(QObject *parent):
    QObject(parent), d_ptr(new ArticlePrivate(this))
{
}

Article::Article(ArticlePrivate &dd, QObject *parent):
    QObject(parent), d_ptr(&dd)
{
}

Article::~Article()
{
}

Article * Article::create(const ArticleData &articleData, QObject *parent)
{
    Article * returned = new Article(parent);
    returned->d_func()->data = articleData;
    return returned;
}

Article * Article::create(const QString &title, const QString &description, const QUrl &link,
                          const QDateTime &timestamp, const QString &image, QObject *parent)
{
    Article * returned = new Article(parent);
    returned->d_func()->data.setTitle(title);
    returned->d_func()->data.setDescription(description);
    returned->d_func()->data.setLink(link);
    returned->d_func()->data.setTimestamp(timestamp);
    returned->d_func()->data.setImage(image);
    return returned;
}

QString Article::title() const
{
    Q_D(const Article);
    return d->data.title();
}

QString Article::description() const
{
    Q_D(const Article);
    return d->data.description();
}

QUrl Article::link() const
{
    Q_D(const Article);
    return d->data.link();
}

QDateTime Article::timestamp() const
{
    Q_D(const Article);
    return d->data.timestamp();
}

QString Article::image() const
{
    Q_D(const Article);
    return d->data.image();
}

ArticleData Article::toData() const
{
    Q_D(const Article);
    return d->data;
}
