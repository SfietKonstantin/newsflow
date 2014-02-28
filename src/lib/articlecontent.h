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

#ifndef ARTICLECONTENT_H
#define ARTICLECONTENT_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QMetaType>

class ArticleParagraphData;
class ArticleParagraph
{
public:
    explicit ArticleParagraph();
    explicit ArticleParagraph(const QString &content);
    ArticleParagraph(const ArticleParagraph &other);
    virtual ~ArticleParagraph();
    ArticleParagraph & operator=(const ArticleParagraph &other);
    QString content() const;
    void setContent(const QString &content);
private:
    QSharedDataPointer<ArticleParagraphData> data;
};

Q_DECLARE_METATYPE(ArticleParagraph)

class ArticlePictureData;
class ArticlePicture
{
    public:
        explicit ArticlePicture();
        explicit ArticlePicture(const QString &source);
        ArticlePicture(const ArticlePicture &other);
        virtual ~ArticlePicture();
        ArticlePicture & operator=(const ArticlePicture &other);
        QString source() const;
        void setSource(const QString &source);
    private:
        QSharedDataPointer<ArticlePictureData> data;
};

Q_DECLARE_METATYPE(ArticlePicture)

#endif // ARTICLECONTENT_H
