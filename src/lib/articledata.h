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

#ifndef ARTICLEDATA_H
#define ARTICLEDATA_H

#include "newsflow_global.h"
#include <QtCore/QSharedDataPointer>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>

class ArticleDataPrivate;
class NEWSFLOW_EXPORT ArticleData
{
public:
    explicit ArticleData();
    explicit ArticleData(const QString &title, const QString &description, const QUrl &link,
                         const QString &image, const QDateTime &timestamp);
    ArticleData(const ArticleData &other);
    virtual ~ArticleData();
    ArticleData & operator=(const ArticleData &other);
    bool operator==(const ArticleData &other) const;
    bool isNull() const;
    QString title() const;
    void setTitle(const QString &title);
    QString description() const;
    void setDescription(const QString &description);
    QUrl link() const;
    void setLink(const QUrl &link);
    QString image() const;
    void setImage(const QString &image);
    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &timestamp);
private:
    QSharedDataPointer<ArticleDataPrivate> d;
};
#endif // ARTICLEDATA_H
