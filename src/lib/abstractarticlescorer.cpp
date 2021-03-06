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

#include "abstractarticlescorer.h"
#include "abstractarticlescorer_p.h"

AbstractArticleScorerPrivate::AbstractArticleScorerPrivate(AbstractArticleScorer *q)
    : score(0), q_ptr(q)
{
}

AbstractArticleScorerPrivate::~AbstractArticleScorerPrivate()
{
}

////// End of private class //////

AbstractArticleScorer::AbstractArticleScorer(QObject *parent) :
    QObject(parent), d_ptr(new AbstractArticleScorerPrivate(this))
{
}

AbstractArticleScorer::AbstractArticleScorer(QNetworkAccessManager *networkAccessManager,
                                             QThreadPool *threadPool, QObject *parent)
    : QObject(parent), d_ptr(new AbstractArticleScorerPrivate(this))
{
    Q_D(AbstractArticleScorer);
    d->networkAccessManager = networkAccessManager;
    d->threadPool = threadPool;
}

AbstractArticleScorer::AbstractArticleScorer(AbstractArticleScorerPrivate &dd, QObject *parent)
    : QObject(parent), d_ptr(&dd)
{
}

AbstractArticleScorer::~AbstractArticleScorer()
{
}

QNetworkAccessManager * AbstractArticleScorer::networkAccessManager() const
{
    Q_D(const AbstractArticleScorer);
    return d->networkAccessManager;
}

void AbstractArticleScorer::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    Q_D(AbstractArticleScorer);
    d->networkAccessManager = networkAccessManager;
}

QThreadPool * AbstractArticleScorer::threadPool() const
{
    Q_D(const AbstractArticleScorer);
    return d->threadPool;
}

void AbstractArticleScorer::setThreadPool(QThreadPool *threadPool)
{
    Q_D(AbstractArticleScorer);
    d->threadPool = threadPool;
}

ArticleData AbstractArticleScorer::article() const
{
    Q_D(const AbstractArticleScorer);
    return d->article;
}

void AbstractArticleScorer::setArticle(const ArticleData &article)
{
    Q_D(AbstractArticleScorer);
    d->article = article;
}

QString AbstractArticleScorer::errorString() const
{
   Q_D(const AbstractArticleScorer);
    return d->errorString;
}

float AbstractArticleScorer::score() const
{
    Q_D(const AbstractArticleScorer);
    return d->score;
}

void AbstractArticleScorer::setErrorString(const QString &errorString)
{
    Q_D(AbstractArticleScorer);
    d->errorString = errorString;
}

void AbstractArticleScorer::setScore(float score)
{
    Q_D(AbstractArticleScorer);
    d->score = score;
}
