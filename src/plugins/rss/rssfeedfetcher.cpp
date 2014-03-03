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

#include "rssfeedfetcher.h"
#include "rssfeedfetcher_p.h"
#include <QtCore/QDebug>
#include <QtCore/QStack>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <rsssupport.h>

static const char *BUSY_ERROR = "Cannot load: operation already in progress";
static const char *NETWORK_ERROR = "Cannot load: network error: %1";
static const char *PARSE_ERROR = "Cannot load: parse error: %1";

static const char *CHANNEL_TAG = "channel";
static const char *ITEM_TAG = "item";
static const char *TITLE_TAG = "title";
static const char *DESCRIPTION_TAG = "description";
static const char *LINK_TAG = "link";
static const char *PUBDATE_TAG = "pubDate";
static const char *ENCLOSURE_TAG = "enclosure";
static const char *TYPE_KEY = "type";
static const char *URL_KEY = "url";
static const char *IMAGE_TYPE_VALUE = "image";
static const char *RFC822 = "ddd, dd MMM yyyy hh:mm:ss";

RssFeedFetcherPrivate::RssFeedFetcherPrivate(RssFeedFetcher *q)
    : AbstractFeedFetcherPrivate(q), feedInfoReply(0), feedReply(0)
{
}

void RssFeedFetcherPrivate::slotFeedInfoFinished()
{
    Q_Q(RssFeedFetcher);
    if (!feedInfoReply) {
        qDebug() << "Feed info loaded without reply";
        emit q->feedInfoLoaded(false);
        return;
    }

    if (feedInfoReply->error() != QNetworkReply::NoError) {
        q->setErrorString(QString(NETWORK_ERROR).arg(feedInfoReply->errorString()));
        feedInfoReply->deleteLater();
        feedInfoReply = 0;
        emit q->feedInfoLoaded(false);
        return;
    }

    // Info about the feed
    QString title;
    QString description;

    bool inChannel = false;
    bool shouldBreak = false;
    QStack<QStringRef> currentElementStack;
    QXmlStreamReader reader (feedInfoReply);
    while (!reader.atEnd() && !shouldBreak) {
        QXmlStreamReader::TokenType type = reader.readNext();
        switch (type) {
        case QXmlStreamReader::StartElement:
            {
                QStringRef currentElement;
                if (!currentElementStack.isEmpty()) {
                    currentElement = currentElementStack.top();
                }
                QStringRef newElement = reader.name();
                if (currentElement == CHANNEL_TAG) {
                    inChannel = true;
                }
                if (currentElement == ITEM_TAG) {
                    shouldBreak = true;
                }

                currentElementStack.push(newElement);
            }
            break;
        case QXmlStreamReader::EndElement:
            {
                inChannel = false;
                currentElementStack.pop();
            }
            break;
        case QXmlStreamReader::Characters:
            {
                QStringRef currentElement;
                if (!currentElementStack.isEmpty()) {
                    currentElement = currentElementStack.top();
                }
                if (inChannel) {
                    if (currentElement == TITLE_TAG) {
                        title = reader.text().toString();
                    }
                    if (currentElement == DESCRIPTION_TAG) {
                        description = reader.text().toString();
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    if (reader.error() != QXmlStreamReader::NoError) {
        q->setErrorString(QString(PARSE_ERROR).arg(reader.errorString()));
        feedInfoReply->deleteLater();
        feedInfoReply = 0;
        emit q->feedInfoLoaded(false);
        return;
    }

    q->setFeedInfo(FeedData(title, feedInfoReply->url()));
    feedInfoReply->deleteLater();
    feedInfoReply = 0;
    emit q->feedInfoLoaded(true);
}

void RssFeedFetcherPrivate::slotFeedFinished()
{
    Q_Q(RssFeedFetcher);
    if (!feedReply) {
        qDebug() << "Feed loaded without reply";
        emit q->feedLoaded(false);
        return;
    }

    if (feedReply->error() != QNetworkReply::NoError) {
        q->setErrorString(QString(NETWORK_ERROR).arg(feedReply->errorString()));
        feedReply->deleteLater();
        feedReply = 0;
        emit q->feedLoaded(false);
        return;
    }

    QLocale locale (QLocale::English, QLocale::UnitedStates);
    QList<ArticleData> articles;

    // Info about current article
    QString articleTitle;
    QString articleDescription;
    QString articleLink;
    QString articleTimestamp;
    QString articleImage;
    RssSupport support;

    bool inItem = false;
    QStack<QStringRef> currentElementStack;
    QXmlStreamReader reader (feedReply);
    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType type = reader.readNext();
        switch (type) {
        case QXmlStreamReader::StartElement:
            {
                QStringRef currentElement;
                if (!currentElementStack.isEmpty()) {
                    currentElement = currentElementStack.top();
                }
                QStringRef nextElement = reader.name();
                if (currentElement == ITEM_TAG) {
                    inItem = true;
                }

                if (inItem) {
                    if (nextElement == ENCLOSURE_TAG) {
                        if (reader.attributes().value(TYPE_KEY).startsWith(IMAGE_TYPE_VALUE)) {
                            articleImage = reader.attributes().value(URL_KEY).toString();
                        }
                    }
                }
                currentElementStack.push(nextElement);
            }
            break;
        case QXmlStreamReader::EndElement:
            {
                QStringRef currentElement;
                if (!currentElementStack.isEmpty()) {
                    currentElement = currentElementStack.top();
                }
                if (currentElement == ITEM_TAG) {
                    if (articleImage.isEmpty() && !support.images().isEmpty())  {
                        articleImage = support.images().first();
                    }

                    QDateTime timestamp = locale.toDateTime(articleTimestamp.left(QString(RFC822).length()), RFC822);
                    articles.append(ArticleData(articleTitle, articleDescription, QUrl(articleLink),
                                                articleImage, timestamp));

                    articleTitle.clear();
                    articleDescription.clear();
                    articleLink.clear();
                    articleTimestamp.clear();
                    articleImage.clear();
                }
                inItem = false;
                currentElementStack.pop();
            }
            break;
        case QXmlStreamReader::Characters:
            {
                QStringRef currentElement;
                if (!currentElementStack.isEmpty()) {
                    currentElement = currentElementStack.top();
                }
                if (inItem) {
                    if (currentElement == TITLE_TAG) {
                        articleTitle = reader.text().toString();
                    }
                    if (currentElement == LINK_TAG) {
                        articleLink = reader.text().toString();
                    }
                    if (currentElement == DESCRIPTION_TAG) {
                        QString description = reader.text().toString();
                        support.setHtml(description);
                        articleDescription = support.processedHtml();
                    }
                    if (currentElement == PUBDATE_TAG) {
                        articleTimestamp = reader.text().toString();
                    }
                }
            }
            break;
        default:
            break;
        }
    }

    if (reader.error() != QXmlStreamReader::NoError) {
        q->setErrorString(QString(PARSE_ERROR).arg(reader.errorString()));
        feedReply->deleteLater();
        feedReply = 0;
        emit q->feedLoaded(false);
        return;
    }

    q->setFeed(articles);
    feedReply->deleteLater();
    feedReply = 0;
    emit q->feedLoaded(true);
}

////// End of private class //////

RssFeedFetcher::RssFeedFetcher(QObject *parent)
    : AbstractFeedFetcher(*(new RssFeedFetcherPrivate(this)), parent)
{
}

RssFeedFetcher::RssFeedFetcher(QNetworkAccessManager *networkAccessManager, QThreadPool *threadPool,
                               QObject *parent)
    : AbstractFeedFetcher(*(new RssFeedFetcherPrivate(this)), parent)
{
    Q_D(RssFeedFetcher);
    d->networkAccessManager = networkAccessManager;
    d->threadPool = threadPool;
}

void RssFeedFetcher::loadFeedInfo()
{
    Q_D(RssFeedFetcher);
    if (d->feedInfoReply) {
        setErrorString(BUSY_ERROR);
        emit feedInfoLoaded(false);
        return;
    }
    d->feedInfoReply = d->networkAccessManager->get(QNetworkRequest(d->source));
    connect(d->feedInfoReply, &QNetworkReply::finished,
            d, &RssFeedFetcherPrivate::slotFeedInfoFinished);
}

void RssFeedFetcher::loadFeed()
{
    Q_D(RssFeedFetcher);
    if (d->feedReply) {
        setErrorString(BUSY_ERROR);
        emit feedLoaded(false);
        return;
    }
    d->feedReply = d->networkAccessManager->get(QNetworkRequest(d->source));
    connect(d->feedReply, &QNetworkReply::finished,
            d, &RssFeedFetcherPrivate::slotFeedFinished);
}
