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

#ifndef FEEDDATA_H
#define FEEDDATA_H

#include "newsflow_global.h"
#include <QtCore/QSharedData>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>

class FeedDataPrivate;
class NEWSFLOW_EXPORT FeedData
{
public:
    explicit FeedData();
    explicit FeedData(const QString &name, const QUrl &source,
                      const QDateTime &lastUpdated = QDateTime());
    FeedData(const FeedData &other);
    virtual ~FeedData();
    FeedData & operator=(const FeedData &other);
    bool operator==(const FeedData &other) const;
    bool isNull() const;
    QString name() const;
    void setName(const QString &name);
    QUrl source() const;
    void setSource(const QUrl &source);
    QDateTime lastUpdated() const;
    void setLastUpdated(const QDateTime &lastUpdated);
private:
    QSharedDataPointer<FeedDataPrivate> d;
};

#endif // FEEDDATA_H
