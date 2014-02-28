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

#ifndef FEEDMODEL_H
#define FEEDMODEL_H

#include <QtCore/QAbstractListModel>
#include "feedmanager.h"

class FeedData;
class FeedModelPrivate;
class FeedModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(FeedManager * feedManager READ feedManager WRITE setFeedManager NOTIFY feedManagerChanged)
public:
    enum Role {
        FeedRole,
        PictureRole,
        StatusRole
    };
    enum Status {
        Idle,
        Loading,
        Error
    };
    explicit FeedModel(QObject *parent = 0);
    virtual ~FeedModel();
    QHash<int, QByteArray> roleNames() const;
    int count() const;
    FeedManager * feedManager() const;
    void setFeedManager(FeedManager *feedManager);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void setFeeds(const QList<FeedData> &feeds);
    void addFeed(const FeedData &feed);
    void setFeed(int row, const FeedData &feed);
    void setFeedPicture(int row, const QString &picture);
    void setFeedStatus(int row, Status status);
Q_SIGNALS:
    void countChanged();
    void feedManagerChanged();
protected:
    explicit FeedModel(FeedModelPrivate &dd, QObject *parent = 0);
    const QScopedPointer<FeedModelPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(FeedModel)
};


#endif // FEEDMODEL_H
