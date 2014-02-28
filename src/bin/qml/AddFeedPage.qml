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

Dialog {
    id: container
    property Item main
    property FeedManager feedManager

    FeedAdder {
        id: feedAdder
        feedManager: container.feedManager
        source: source.text
    }

    acceptDestination: typeComboBox.currentItem.objectName == "rss" ? rss : null
    canAccept: (typeComboBox.currentItem.objectName == "rss" && source.text.length > 0)
    onAccepted: {
        feedAdder.type = typeComboBox.currentItem.objectName
        if (typeComboBox.currentItem.objectName == "rss") {
            feedAdder.addFeed()
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        Column {
            anchors.left: parent.left; anchors.right: parent.right
            DialogHeader {
                //: Text for the accept entry of the dialog used to add feed
                //% "Add feed"
                acceptText: qsTrId("newsflow-addfeedpage-accept")
            }

            ComboBox {
                id: typeComboBox
                //: Text for the combo box used to select feed type
                //% "Feed type"
                label: qsTrId("newsflow-addfeedpage-combo-feedtype")
                menu: ContextMenu {
                    MenuItem {
                        objectName: "rss"
                        //: Text for the menu entry for selecting RSS feed type
                        //% "RSS"
                        text: qsTrId("newsflow-addfeedpage-menu-rss")
                    }
                    MenuItem {
                        enabled: false
                        objectName: "facebook"
                        //: Text for the menu entry for selecting Facebook feed type
                        //% "Facebook"
                        text: qsTrId("newsflow-addfeedpage-menu-facebook")
                    }
                    MenuItem {
                        enabled: false
                        objectName: "twitter"
                        //: Text for the menu entry for selecting Twitter feed type
                        //% "Twitter"
                        text: qsTrId("newsflow-addfeedpage-menu-twitter")
                    }
                }
            }

            TextField {
                id: source
                visible: typeComboBox.currentItem.objectName == "rss"
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium

                //: Text for the text field to put the url to the RSS feeds
                //% "Source"
                placeholderText: qsTrId("newsflow-addfeedpage-source")
                label: qsTrId("newsflow-addfeedpage-source")
            }
        }
    }

    Component {
        id: rss
        Dialog {
            id: rssDialog
            canAccept: feedAdder.status == FeedAdder.Idle && rssName.text.length > 0
            acceptDestination: container.main
            acceptDestinationAction: PageStackAction.Pop
            onAccepted: {
                feedAdder.feedAlternateName = rssName.text
                feedAdder.saveFeed()
            }

            BusyIndicator {
                id: busyIndicator
                running: feedAdder.status == FeedAdder.Loading
                anchors.centerIn: parent
                size: BusyIndicatorSize.Large
            }


            SilicaFlickable {
                anchors.fill: parent
                contentHeight: rssColumn.height

                Column {
                    id: rssColumn
                    visible: feedAdder.status == FeedAdder.Idle
                    anchors.left: parent.left; anchors.right: parent.right
                    DialogHeader {
                        acceptText: qsTrId("newsflow-addfeedpage-accept")
                    }

                    TextField {
                        id: rssName
                        anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                        anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium

                        //: Text for the text field to put the name of the RSS feeds
                        //% "Name"
                        placeholderText: qsTrId("newsflow-addfeedpage-rss-name")
                        label: qsTrId("newsflow-addfeedpage-rss-name")
                        text: feedAdder.feedName
                    }
                }

                ViewPlaceholder {
                    id: placeHolder
                    enabled: feedAdder.status == FeedAdder.Error
                    text: qsTrId("newsflow-addfeedpage-rss-error-placeholder")
                }
            }
        }
    }
}
