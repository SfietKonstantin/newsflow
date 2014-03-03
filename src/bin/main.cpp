/*
  Copyright (C) 2013 Jolla Ltd.
  Contact: Thomas Perl <thomas.perl@jollamobile.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <sailfishapp/sailfishapp.h>
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QTranslator>
#include <QtCore/QtPlugin>
#include <QtGui/QGuiApplication>
#include <QtQml/qqml.h>
#include <QtQuick/QQuickView>
#include <article.h>
#include <articlesmodel.h>
#include <feed.h>
#include <feedadder.h>
#include <feedmanager.h>
#include <feedmodel.h>

static const char *URI = "harbour.newsflow";
static const char *TRANSLATION_PATH = "/usr/share/harbour-newsflow/translations";
static const char *UNCREATABLE = "Uncreatable";


Q_IMPORT_PLUGIN(RssPlugin)
Q_IMPORT_PLUGIN(TimePlugin)

void defineImports()
{
    // @uri harbour.newsflow
    qmlRegisterType<ArticlesModel>(URI, 1, 0, "ArticlesModel");
    qmlRegisterType<FeedAdder>(URI, 1, 0, "FeedAdder");
    qmlRegisterType<FeedManager>(URI, 1, 0, "FeedManager");
    qmlRegisterType<FeedModel>(URI, 1, 0, "FeedModel");
    qmlRegisterUncreatableType<Article>(URI, 1, 0, "Article", UNCREATABLE);
    qmlRegisterUncreatableType<Feed>(URI, 1, 0, "Feed", UNCREATABLE);
}

int main(int argc, char *argv[])
{
    defineImports();

    QScopedPointer<QTranslator> engineeringEnglish(new QTranslator);
    QScopedPointer<QTranslator> translator(new QTranslator);
#ifndef DESKTOP
    engineeringEnglish->load("newsflow-engineering-english", TRANSLATION_PATH);
#else
    engineeringEnglish->load("newsflow-engineering-english", ":/");
#endif
    translator->load(QLocale(), "newsflow", "_", TRANSLATION_PATH);
    QGuiApplication *app = SailfishApp::application(argc, argv);
    app->setOrganizationName("SfietKonstantin");
    app->setApplicationName("NewsFlow");

    app->installTranslator(engineeringEnglish.data());
    app->installTranslator(translator.data());

    QQuickView *view = SailfishApp::createView();
#ifndef DESKTOP
    view->setSource(SailfishApp::pathTo("qml/harbour-newsflow.qml"));
    view->showFullScreen();
#else
    view->setSource(QUrl("qrc:/qml/harbour-newsflow.qml"));
    view->show();
#endif

    int result = app->exec();
    app->removeTranslator(translator.data());
    app->removeTranslator(engineeringEnglish.data());
    return result;
}

//#include "main.moc"
