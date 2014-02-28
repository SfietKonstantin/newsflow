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

#include "articlesmodel.h"
#include "articlesmodel_p.h"
#include "feedmanager_p.h"
#include "articledata.h"
#include "articleformatter.h"

ArticlesModelData::~ArticlesModelData()
{
}

ArticlesModelPrivate::ArticlesModelPrivate(ArticlesModel * q)
    : feedManager(0), feed(0), q_ptr(q)
{
}

ArticlesModelPrivate::~ArticlesModelPrivate()
{
}

void ArticlesModelPrivate::clear()
{
    Q_Q(ArticlesModel);
    if (m_data.count() > 0) {
        q->beginRemoveRows(QModelIndex(), 0, q->rowCount() - 1);
        qDeleteAll(m_data);
        m_data.clear();
        q->endRemoveRows();
        emit q->countChanged();
    }
}

void ArticlesModelPrivate::prependData(const QList<ArticlesModelData *> &data)
{
    Q_Q(ArticlesModel);
    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), 0, data.count() - 1);
        QList<ArticlesModelData *> oldData = m_data;
        m_data = data;
        m_data.append(oldData);
        emit q->countChanged();
        q->endInsertRows();
    }
}

void ArticlesModelPrivate::appendData(const QList<ArticlesModelData *> &data)
{
    Q_Q(ArticlesModel);
    if (data.count() > 0) {
        q->beginInsertRows(QModelIndex(), q->rowCount(), q->rowCount() + data.count() - 1);
        m_data.append(data);
        emit q->countChanged();
        q->endInsertRows();
    }
}

////// End of private class //////

ArticlesModel::ArticlesModel(QObject *parent)
    : QAbstractListModel(parent), d_ptr(new ArticlesModelPrivate(this))
{
}

ArticlesModel::ArticlesModel(ArticlesModelPrivate &dd, QObject *parent)
    : QAbstractListModel(parent), d_ptr(&dd)
{
}

ArticlesModel::~ArticlesModel()
{
    Q_D(ArticlesModel);
    if (d->feedManager && d->feed) {
        FeedManagerPrivate::unregisterArticleModel(this, d->feed->toData(), d->feedManager);
    }
    qDeleteAll(d->m_data);
}

QHash<int, QByteArray> ArticlesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ArticleRole, "article");
    roles.insert(SummaryRole, "summary");
    return roles;
}

int ArticlesModel::count() const
{
    return rowCount();
}

FeedManager * ArticlesModel::feedManager() const
{
    Q_D(const ArticlesModel);
    return d->feedManager;
}

void ArticlesModel::setFeedManager(FeedManager *feedManager)
{
    Q_D(ArticlesModel);
    if (d->feedManager != feedManager) {
        if (d->feedManager && d->feed) {
            FeedManagerPrivate::unregisterArticleModel(this, d->feed->toData(), d->feedManager);
        }
        d->feedManager = feedManager;
        if (d->feedManager && d->feed) {
            FeedManagerPrivate::registerArticleModel(this, d->feed->toData(), d->feedManager);
        }
        emit feedManagerChanged();
    }
}

Feed * ArticlesModel::feed() const
{
    Q_D(const ArticlesModel);
    return d->feed;
}

void ArticlesModel::setFeed(Feed *feed)
{
    Q_D(ArticlesModel);
    if (d->feed != feed) {
        if (d->feedManager && d->feed) {
            FeedManagerPrivate::unregisterArticleModel(this, d->feed->toData(), d->feedManager);
        }
        d->feed = feed;
        if (d->feedManager && d->feed) {
            FeedManagerPrivate::registerArticleModel(this, d->feed->toData(), d->feedManager);
        }
        emit feedChanged();
    }
}

int ArticlesModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const ArticlesModel);
    Q_UNUSED(parent)
    return d->m_data.count();
}

QVariant ArticlesModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ArticlesModel);
    if (index.row() < 0 or index.row() >= rowCount()) {
        return QVariant();
    }
    ArticlesModelData *data = d->m_data.at(index.row());

    switch (role) {
    case ArticleRole:
        return QVariant::fromValue(data->article);
        break;
    case SummaryRole:
        return data->summary;
        break;
    default:
        return QVariant();
        break;
    }
}

void ArticlesModel::setArticles(const QList<ArticleData> &articles)
{
    Q_D(ArticlesModel);
    QList<ArticlesModelData *> data;
    foreach (const ArticleData &article, articles) {
        ArticlesModelData *item = new ArticlesModelData;
        item->article = Article::create(article, this);
        item->summary = ArticleFormatter::getDescription(article.description());
        data.append(item);
    }
    d->clear();
    d->appendData(data);
}
