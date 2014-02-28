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

#ifndef ARTICLESMODEL_H
#define ARTICLESMODEL_H

#include <QtCore/QAbstractListModel>
#include "feed.h"
#include "feedmanager.h"

class ArticleData;
class ArticlesModelPrivate;
class ArticlesModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(FeedManager * feedManager READ feedManager WRITE setFeedManager NOTIFY feedManagerChanged)
    Q_PROPERTY(Feed * feed READ feed WRITE setFeed NOTIFY feedChanged)
public:
    enum Role {
        ArticleRole,
        SummaryRole
    };
    explicit ArticlesModel(QObject *parent = 0);
    virtual ~ArticlesModel();
    QHash<int, QByteArray> roleNames() const;
    int count() const;
    FeedManager * feedManager() const;
    void setFeedManager(FeedManager *feedManager);
    Feed * feed() const;
    void setFeed(Feed *feed);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void setArticles(const QList<ArticleData> &articles);
Q_SIGNALS:
    void feedManagerChanged();
    void feedChanged();
    void countChanged();
protected:
    explicit ArticlesModel(ArticlesModelPrivate &dd, QObject *parent = 0);
    const QScopedPointer<ArticlesModelPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(ArticlesModel)
};

#endif // ARTICLESMODEL_H
