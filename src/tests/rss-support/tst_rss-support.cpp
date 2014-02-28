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

#include <QtTest/QtTest>
#include <rsssupport.h>
#include <articlecontent.h>
#include <QtCore/QRegularExpression>

class RssSupportTests: public QObject
{
    Q_OBJECT
private slots:

    /*
    void testP()
    {
        RssSupport support;
        QString input = "<p>This is a test text</p>";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(), input);

        input = "<p>First paragraph</p><p>Second paragraph</p>";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(), input);

        input = "First paragraph</p><p>Second paragraph</p>Third paragraph";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(),
                 QString("<p>First paragraph</p><p>Second paragraph</p><p>Third paragraph</p>"));

        input = "Simple test";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(), QString("<p>Simple test</p>"));
    }

    void testSimpleImg()
    {
        RssSupport support;
        QString input = "<img src=\"source.png\"/>";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(), input);

        input = "<img alt=\"Alt description\" width=300 src=\"source.png\"/>";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(), QString("<img src=\"source.png\"/>"));
    }

    void testImgAndP()
    {
        RssSupport support;
        QString input = "<p>Test 1<img src=\"source.png\" />Test 2</p>";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(),
                 QString("<p>Test 1</p><img src=\"source.png\"/><p>Test 2</p>"));
    }

    void testComment()
    {
        RssSupport support;
        QString input = "<!--test-->";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(), QString(""));

        input = "<!--test <!-- test2 -->";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(), QString(""));

        input = "<!--test --test2-->";
        support.setHtml(input);
        QCOMPARE(support.processedHtml(), QString(""));
    }
    */

    void test()
    {
//        RssSupport support;
//        support.setHtml("<p><i><span style=\"color: #3d85c6;\">In which we test</span></i></p>");
//        support.setHtml("<p><i><span style=\"color: #3d85c6;\">In which we talk about being sick, organization, the value of it, the point of workflow. A quick recap of the release plans and how it will work. We touch on the subject of icons and the coming icons for Plasma and about shapes then round up about what's coming.</span></i><br /><i><span style=\"color: #3d85c6;\"><br /></span></i></p>");
//        qDebug() << support.processedHtml();
//        RssSupport support;
//        support.setHtml("<p>test<span>test2</span></p>");
//        qDebug() << support.processedHtml();
//        support.setHtml("<p>test<img alt=\"test\" src=\"test.png\"> other stuff</p>");




//        RssSupport support;
//        support.setHtml("<p>Next in Kubuntu:</p><p>Improved on-demand installation and notifications for localization, as well as improved internal handling to increase reliability of things.</p><a href=\"http://apachelog.wordpress.com/2014/02/25/localization/#gallery-1206-1-slideshow\">Click to view slideshow.</a><img alt=\"\" border=\"0\" height=\"1\" src=\"http://stats.wordpress.com/b.gif?host=apachelog.wordpress.com&#038;blog=12425881&#038;post=1206&#038;subd=apachelog&#038;ref=&#038;feed=1\" width=\"1\" />");
//        qDebug() << support.processedHtml();


//        QRegularExpression expression ("\\w+=\\w+");
//        QRegularExpressionMatchIterator iterator = expression.globalMatch("a=b c=d");
//        while (iterator.hasNext()) {
//            qDebug() << iterator.next();
//        }
//        qDebug() << expression.match("");
    }
};

QTEST_MAIN(RssSupportTests)

#include "tst_rss-support.moc"
