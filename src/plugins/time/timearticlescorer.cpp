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

#include "timearticlescorer.h"
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <math.h>

static const int MONTH = 2592000; // A month

TimeArticleScorer::TimeArticleScorer(QObject *parent)
    : AbstractArticleScorer(parent)
{
}

TimeArticleScorer::TimeArticleScorer(QNetworkAccessManager *networkAccessManager,
                                     QThreadPool *threadPool, QObject *parent)
    : AbstractArticleScorer(networkAccessManager, threadPool, parent)
{
}

void TimeArticleScorer::load()
{
    if (article().isNull()) {
        emit loaded(false);
        return;
    }

    if (!article().timestamp().isValid()) {
        emit loaded(false);
        return;
    }

    int secondsFromNow = article().timestamp().secsTo(QDateTime::currentDateTime());
    float score = qBound<float>(0., 1. - (float) secondsFromNow / (float) MONTH, 1.);

    setScore(score);
    emit loaded(true);
}
