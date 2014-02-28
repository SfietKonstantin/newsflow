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

#ifndef FEEDADDER_H
#define FEEDADDER_H

#include "newsflow_global.h"
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include "feedmanager.h"

class FeedAdderPrivate;
class NEWSFLOW_EXPORT FeedAdder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(FeedManager * feedManager READ feedManager WRITE setFeedManager NOTIFY feedManagerChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString feedName READ feedName NOTIFY feedNameChanged)
    Q_PROPERTY(QString feedAlternateName READ feedAlternateName WRITE setFeedAlternateName
               NOTIFY feedAlternateNameChanged)
    Q_ENUMS(Status)
public:
    enum Status {
        Idle,
        Loading,
        Error
    };
    explicit FeedAdder(QObject *parent = 0);
    virtual ~FeedAdder();
    Status status() const;
    FeedManager * feedManager() const;
    void setFeedManager(FeedManager *feedManager);
    QString type() const;
    void setType(const QString &type);
    QUrl source() const;
    void setSource(const QUrl &source);
    QString feedName() const;
    QString feedAlternateName() const;
    void setFeedAlternateName(const QString &feedAlternateName);
public Q_SLOTS:
    bool addFeed();
    bool saveFeed();
Q_SIGNALS:
    void statusChanged();
    void feedManagerChanged();
    void typeChanged();
    void sourceChanged();
    void feedNameChanged();
    void feedAlternateNameChanged();
protected:
    explicit FeedAdder(FeedAdderPrivate &dd, QObject *parent = 0);
    const QScopedPointer<FeedAdderPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(FeedAdder)

};

#endif // FEEDADDER_H
