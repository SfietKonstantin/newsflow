/*
 * Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
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

#include "abstractmodel.h"
#include "abstractmodel_p.h"
#include "osignal.h"
#include <QtCore/QDebug>

AbstractModelPrivate::AbstractModelPrivate(AbstractModel *q)
    : QObject(), manager(0), q_ptr(q), m_status(AbstractModel::Idle), m_reply(0)
{
}

void AbstractModelPrivate::setReply(QNetworkReply *reply)
{
    clearReply();
    m_reply = reply;
    connect(m_reply, &QNetworkReply::finished, this, &AbstractModelPrivate::slotFinished);
    setStatus(AbstractModel::Loading);
}

void AbstractModelPrivate::clearReply()
{
    if (m_reply) {
        m_reply->disconnect(this);
        m_reply->deleteLater();
        m_reply = 0;
    }
}

void AbstractModelPrivate::setStatus(AbstractModel::Status status)
{
    Q_Q(AbstractModel);
    if (m_status != status) {
        m_status = status;
        emit q->statusChanged();
    }
}

void AbstractModelPrivate::clear()
{
    Q_Q(AbstractModel);
    if (m_data.count() > 0) {
        q->beginRemoveRows(QModelIndex(), 0, q->rowCount() - 1);
        qDeleteAll(m_data);
        m_data.clear();
        q->endRemoveRows();
        emit q->countChanged();
    }
}

void AbstractModelPrivate::handleFinished(QNetworkReply *reply)
{
    Q_UNUSED(reply)
}

void AbstractModelPrivate::handleError(QNetworkReply *reply, QNetworkReply::NetworkError error,
                                       const QString &errorString)
{
    Q_UNUSED(reply)
    qWarning() << "Network error:" << error << errorString;
    clearReply();
    setStatus(AbstractModel::Error);
}

void AbstractModelPrivate::addData(const QList<QObject *> &data)
{
    Q_Q(AbstractModel);
    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), q->rowCount(), q->rowCount() + data.count() - 1);
        m_data.append(data);
        emit q->countChanged();
        q->endInsertRows();
    }
}

void AbstractModelPrivate::slotFinished()
{
    if (m_reply->error() == QNetworkReply::NoError) {
        handleFinished(m_reply);
    } else {
        handleError(m_reply, m_reply->error(), m_reply->errorString());
    }
}

////// End of private class //////

AbstractModel::AbstractModel(AbstractModelPrivate &dd, QObject *parent)
    : QAbstractListModel(parent), d_ptr(&dd)
{
    Q_D(AbstractModel);
    d->manager = new Manager(this);
}

AbstractModel::~AbstractModel()
{
}

QHash<int, QByteArray> AbstractModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(DataRole, "data");
    return roles;
}

AbstractModel::Status AbstractModel::status() const
{
    Q_D(const AbstractModel);
    return d->m_status;
}

int AbstractModel::count() const
{
    return rowCount();
}

Manager * AbstractModel::manager() const
{
    Q_D(const AbstractModel);
    return d->manager;
}

void AbstractModel::setManager(Manager *manager)
{
    Q_D(AbstractModel);
    if (d->manager != manager) {
        d->manager = manager;
        emit managerChanged();
    }
}

int AbstractModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const AbstractModel);
    Q_UNUSED(parent)
    return d->m_data.count();
}

QVariant AbstractModel::data(const QModelIndex &index, int role) const
{
    Q_D(const AbstractModel);
    if (index.row() < 0 or index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {
    case DataRole:
        return QVariant::fromValue(d->m_data.at(index.row()));
        break;
    default:
        return QVariant();
        break;
    }
}

QVariant AbstractModel::getData(int row)
{
    Q_D(const AbstractModel);
    if (row < 0 or row >= rowCount()) {
        return QVariant();
    }

    return QVariant::fromValue(d->m_data.at(row));
}


