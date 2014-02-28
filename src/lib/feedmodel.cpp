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

#include "feedmodel.h"
#include "feedmodel_p.h"
#include "feedmanager_p.h"
#include "feeddata.h"

FeedModelData::~FeedModelData()
{
    if (feed) {
        feed->deleteLater();
    }
}

FeedModelPrivate::FeedModelPrivate(FeedModel * q)
    : feedManager(0), q_ptr(q)
{
}

FeedModelPrivate::~FeedModelPrivate()
{
}

void FeedModelPrivate::clear()
{
    Q_Q(FeedModel);
    if (m_data.count() > 0) {
        q->beginRemoveRows(QModelIndex(), 0, q->rowCount() - 1);
        qDeleteAll(m_data);
        m_data.clear();
        q->endRemoveRows();
        emit q->countChanged();
    }
}

void FeedModelPrivate::appendData(const QList<FeedModelData *> &data)
{
    Q_Q(FeedModel);
    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), q->rowCount(), q->rowCount() + data.count() - 1);
        m_data.append(data);
        emit q->countChanged();
        q->endInsertRows();
    }
}

////// End of private class //////

FeedModel::FeedModel(QObject *parent)
    : QAbstractListModel(parent), d_ptr(new FeedModelPrivate(this))
{
}

FeedModel::FeedModel(FeedModelPrivate &dd, QObject *parent)
    : QAbstractListModel(parent), d_ptr(&dd)
{
}

FeedModel::~FeedModel()
{
    Q_D(FeedModel);
    if (d->feedManager) {
        FeedManagerPrivate::unregisterFeedModel(this, d->feedManager);
    }
}

QHash<int, QByteArray> FeedModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(FeedRole, "feed");
    roles.insert(PictureRole, "picture");
    roles.insert(StatusRole, "status");
    return roles;
}

int FeedModel::count() const
{
    return rowCount();
}

FeedManager * FeedModel::feedManager() const
{
    Q_D(const FeedModel);
    return d->feedManager;
}

void FeedModel::setFeedManager(FeedManager *feedManager)
{
    Q_D(FeedModel);
    if (d->feedManager != feedManager) {
        if (d->feedManager) {
            FeedManagerPrivate::unregisterFeedModel(this, d->feedManager);
        }
        d->feedManager = feedManager;
        if (d->feedManager) {
            FeedManagerPrivate::registerFeedModel(this, d->feedManager);
        }
        emit feedManagerChanged();
    }
}

int FeedModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const FeedModel);
    Q_UNUSED(parent)
    return d->m_data.count();
}

QVariant FeedModel::data(const QModelIndex &index, int role) const
{
    Q_D(const FeedModel);
    if (index.row() < 0 or index.row() >= rowCount()) {
        return QVariant();
    }
    FeedModelData *data = d->m_data.at(index.row());

    switch (role) {
    case FeedRole:
        return QVariant::fromValue(data->feed);
        break;
    case PictureRole:
        return data->picture;
        break;
    case StatusRole:
        return data->status;
        break;
    default:
        return QVariant();
        break;
    }
}

void FeedModel::setFeeds(const QList<FeedData> &feeds)
{
    Q_D(FeedModel);
    QList<FeedModelData *> data;
    foreach (const FeedData &feed, feeds) {
        FeedModelData *item = new FeedModelData;
        item->feed = Feed::create(feed, this);
        item->status = Idle;
        data.append(item);
    }
    d->clear();
    d->appendData(data);
}

void FeedModel::addFeed(const FeedData &feed)
{
    Q_D(FeedModel);
    QList<FeedModelData *> data;
    FeedModelData *item = new FeedModelData;
    item->feed = Feed::create(feed, this);
    item->status = Idle;
    data.append(item);
    d->appendData(data);
}

void FeedModel::setFeed(int row, const FeedData &feed)
{
    Q_D(FeedModel);
    if (row < 0 || row >= rowCount()) {
        return;
    }

    FeedModelData *data = d->m_data.at(row);
    if (data->feed) {
        data->feed->deleteLater();
    }
    data->feed = Feed::create(feed, this);
    emit dataChanged(index(row), index(row));
}

void FeedModel::setFeedPicture(int row, const QString &picture)
{
    Q_D(FeedModel);
    if (row < 0 || row >= rowCount()) {
        return;
    }

    FeedModelData *data = d->m_data.at(row);
    data->picture = picture;
    emit dataChanged(index(row), index(row));
}

void FeedModel::setFeedStatus(int row, Status status)
{
    Q_D(FeedModel);
    if (row < 0 || row >= rowCount()) {
        return;
    }

    FeedModelData *data = d->m_data.at(row);
    data->status = status;
    emit dataChanged(index(row), index(row));
}
