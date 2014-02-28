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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.newsflow 1.0

Page {
    id: container
    property FeedManager feedManager
    property var feed

    SilicaListView {
        anchors.fill: parent
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        model: ArticlesModel {
            feedManager: container.feedManager
            feed: container.feed
        }

        delegate: BackgroundItem {
            height: container.height

            Image {
                id: image
                anchors.top: parent.top;
                anchors.left: parent.left; anchors.right: parent.right
                visible: model.article.image.length > 0
                source: model.article.image
                height: visible ? container.height / 2 : 0
            }

            Label {
                id: title
                anchors.top: image.bottom; anchors.topMargin: Theme.paddingMedium
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                text: model.article.title
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeLarge
            }

            Label {
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                anchors.top: title.bottom; anchors.topMargin: Theme.paddingMedium
                anchors.bottom: parent.bottom; anchors.bottomMargin: Theme.paddingMedium
                text: model.summary
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
                textFormat: Text.StyledText
                elide: Text.ElideRight
            }
            onClicked: pageStack.push(Qt.resolvedUrl("ArticlePage.qml"), {"url": model.article.link})
        }
    }
}
