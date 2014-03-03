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

#ifndef ABSTRACTARTICLESCORER_H
#define ABSTRACTARTICLESCORER_H

#include "newsflow_global.h"
#include <QtCore/QObject>
#include "articledata.h"

class QNetworkAccessManager;
class QThreadPool;
class AbstractArticleScorerPrivate;
class NEWSFLOW_EXPORT AbstractArticleScorer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractArticleScorer(QObject *parent = 0);
    explicit AbstractArticleScorer(QNetworkAccessManager *networkAccessManager,
                                   QThreadPool *threadPool, QObject *parent = 0);
    virtual ~AbstractArticleScorer();
    /**
     * @brief Network access
     * @return QNetworkAccessManager used to perform network access.
     */
    QNetworkAccessManager * networkAccessManager() const;
    /**
     * @brief Set the network access
     * @param networkAccessManager QNetworkAccessManager used to perform network access.
     */
    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);
    /**
     * @brief Thread pool
     * @return QThreadPool used to provide a thread pool.
     */
    QThreadPool * threadPool() const;
    /**
     * @brief Set the thread pool
     * @param threadPool QThreadPool used to provide a thread pool.
     */
    void setThreadPool(QThreadPool *threadPool);
    ArticleData article() const;
    void setArticle(const ArticleData &article);
    QString errorString() const;
    float score() const;
public Q_SLOTS:
    virtual void load() = 0;
Q_SIGNALS:
    void loaded(bool ok);
protected:
    void setErrorString(const QString &errorString);
    void setScore(float score);
    explicit AbstractArticleScorer(AbstractArticleScorerPrivate &dd, QObject *parent = 0);
    QScopedPointer<AbstractArticleScorerPrivate> d_ptr;
private:
    Q_DECLARE_PRIVATE(AbstractArticleScorer)
};

#endif // ABSTRACTARTICLESCORER_H
