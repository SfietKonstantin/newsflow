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

#include "articlecontent.h"
#include "articlecontent_p.h"

ArticleParagraph::ArticleParagraph()
    : data(new ArticleParagraphData)
{
}

ArticleParagraph::ArticleParagraph(const QString &content)
    : data(new ArticleParagraphData)
{
    data->content = content;
}

ArticleParagraph::ArticleParagraph(const ArticleParagraph &other)
    : data(other.data)
{
}

ArticleParagraph::~ArticleParagraph()
{
}

ArticleParagraph & ArticleParagraph::operator=(const ArticleParagraph &other)
{
    if (this != &other) {
        data = other.data;
    }
    return *this;
}

QString ArticleParagraph::content() const
{
    return data->content;
}

void ArticleParagraph::setContent(const QString &content)
{
    data->content = content;
}

ArticlePicture::ArticlePicture()
    : data(new ArticlePictureData)
{
}

ArticlePicture::ArticlePicture(const QString &source)
    : data(new ArticlePictureData)
{
    data->source = source;
}

ArticlePicture::ArticlePicture(const ArticlePicture &other)
    : data(other.data)
{
}

ArticlePicture::~ArticlePicture()
{
}

ArticlePicture & ArticlePicture::operator=(const ArticlePicture &other)
{
    if (this != &other) {
        data = other.data;
    }
    return *this;
}

QString ArticlePicture::source() const
{
    return data->source;
}

void ArticlePicture::setSource(const QString &source)
{
    data->source = source;
}
