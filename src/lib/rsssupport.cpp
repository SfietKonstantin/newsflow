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

#include "rsssupport.h"
#include "rsssupport_p.h"
#include <QtCore/QDebug>
#include "articlecontent.h"

//static const char *IMG_TAG = "<img";
static const char *SRC_KEY = "src";
static const char *HREF_KEY = "href";

RssSupportPrivate::RssSupportPrivate()
    : tag("<(\\w+)([^>]*)>([^<]+)</([^>]+)>")
    , arguments("(\\w+)=([\"'][^\"']*[\"']|\\w*)")
    , comment("<!--(.*?)-->")
    , title("h[1-6]")
    , unicode("&#(\\d+);")
{
}

QString RssSupportPrivate::taggedText(const QString &tag, const QString &text)
{
    QString newText = text;
    newText = newText.replace("<br/>", "\n").trimmed();
    if (!newText.isEmpty()) {
        return QString("<%1>%2</%1>").arg(tag, text);
    }

    return QString();
}

QString RssSupportPrivate::fromHtmlCleaned(const QString &rich)
{
    QString plain = rich;
    plain = plain.replace("&Aacute;", QChar(0x00C1));
    plain = plain.replace("&aacute;", QChar(0x00E1));
    plain = plain.replace("&Acirc;", QChar(0x00C2));
    plain = plain.replace("&acirc;", QChar(0x00E2));
    plain = plain.replace("&acute;", QChar(0x00B4));
    plain = plain.replace("&AElig;", QChar(0x00C6));
    plain = plain.replace("&aelig;", QChar(0x00E6));
    plain = plain.replace("&Agrave;", QChar(0x00C0));
    plain = plain.replace("&agrave;", QChar(0x00E0));
    plain = plain.replace("&alefsym;", QChar(0x2135));
    plain = plain.replace("&Alpha;", QChar(0x0391));
    plain = plain.replace("&alpha;", QChar(0x03B1));
    plain = plain.replace("&amp;", QChar(0x0026));
    plain = plain.replace("&and;", QChar(0x2227));
    plain = plain.replace("&ang;", QChar(0x2220));
    plain = plain.replace("&Aring;", QChar(0x00C5));
    plain = plain.replace("&aring;", QChar(0x00E5));
    plain = plain.replace("&asymp;", QChar(0x2248));
    plain = plain.replace("&Atilde;", QChar(0x00C3));
    plain = plain.replace("&atilde;", QChar(0x00E3));
    plain = plain.replace("&Auml;", QChar(0x00C4));
    plain = plain.replace("&auml;", QChar(0x00E4));
    plain = plain.replace("&bdquo;", QChar(0x201E));
    plain = plain.replace("&Beta;", QChar(0x0392));
    plain = plain.replace("&beta;", QChar(0x03B2));
    plain = plain.replace("&brvbar;", QChar(0x00A6));
    plain = plain.replace("&bull;", QChar(0x2022));
    plain = plain.replace("&cap;", QChar(0x2229));
    plain = plain.replace("&Ccedil;", QChar(0x00C7));
    plain = plain.replace("&ccedil;", QChar(0x00E7));
    plain = plain.replace("&cedil;", QChar(0x00B8));
    plain = plain.replace("&cent;", QChar(0x00A2));
    plain = plain.replace("&Chi;", QChar(0x03A7));
    plain = plain.replace("&chi;", QChar(0x03C7));
    plain = plain.replace("&circ;", QChar(0x02C6));
    plain = plain.replace("&clubs;", QChar(0x2663));
    plain = plain.replace("&cong;", QChar(0x2245));
    plain = plain.replace("&copy;", QChar(0x00A9));
    plain = plain.replace("&crarr;", QChar(0x21B5));
    plain = plain.replace("&cup;", QChar(0x222A));
    plain = plain.replace("&curren;", QChar(0x00A4));
    plain = plain.replace("&Dagger;", QChar(0x2021));
    plain = plain.replace("&dagger;", QChar(0x2020));
    plain = plain.replace("&dArr;", QChar(0x21D3));
    plain = plain.replace("&darr;", QChar(0x2193));
    plain = plain.replace("&deg;", QChar(0x00B0));
    plain = plain.replace("&Delta;", QChar(0x0394));
    plain = plain.replace("&delta;", QChar(0x03B4));
    plain = plain.replace("&diams;", QChar(0x2666));
    plain = plain.replace("&divide;", QChar(0x00F7));
    plain = plain.replace("&Eacute;", QChar(0x00C9));
    plain = plain.replace("&eacute;", QChar(0x00E9));
    plain = plain.replace("&Ecirc;", QChar(0x00CA));
    plain = plain.replace("&ecirc;", QChar(0x00EA));
    plain = plain.replace("&Egrave;", QChar(0x00C8));
    plain = plain.replace("&egrave;", QChar(0x00E8));
    plain = plain.replace("&empty;", QChar(0x2205));
    plain = plain.replace("&emsp;", QChar(0x2003));
    plain = plain.replace("&ensp;", QChar(0x2002));
    plain = plain.replace("&Epsilon;", QChar(0x0395));
    plain = plain.replace("&epsilon;", QChar(0x03B5));
    plain = plain.replace("&equiv;", QChar(0x2261));
    plain = plain.replace("&Eta;", QChar(0x0397));
    plain = plain.replace("&eta;", QChar(0x03B7));
    plain = plain.replace("&ETH;", QChar(0x00D0));
    plain = plain.replace("&eth;", QChar(0x00F0));
    plain = plain.replace("&Euml;", QChar(0x00CB));
    plain = plain.replace("&euml;", QChar(0x00EB));
    plain = plain.replace("&euro;", QChar(0x20AC));
    plain = plain.replace("&exist;", QChar(0x2203));
    plain = plain.replace("&fnof;", QChar(0x0192));
    plain = plain.replace("&forall;", QChar(0x2200));
    plain = plain.replace("&frac12;", QChar(0x00BD));
    plain = plain.replace("&frac14;", QChar(0x00BC));
    plain = plain.replace("&frac34;", QChar(0x00BE));
    plain = plain.replace("&frasl;", QChar(0x2044));
    plain = plain.replace("&Gamma;", QChar(0x0393));
    plain = plain.replace("&gamma;", QChar(0x03B3));
    plain = plain.replace("&ge;", QChar(0x2265));
    plain = plain.replace("&gt;", QChar(0x003E));
    plain = plain.replace("&hArr;", QChar(0x21D4));
    plain = plain.replace("&harr;", QChar(0x2194));
    plain = plain.replace("&hearts;", QChar(0x2665));
    plain = plain.replace("&hellip;", QChar(0x2026));
    plain = plain.replace("&Iacute;", QChar(0x00CD));
    plain = plain.replace("&iacute;", QChar(0x00ED));
    plain = plain.replace("&Icirc;", QChar(0x00CE));
    plain = plain.replace("&icirc;", QChar(0x00EE));
    plain = plain.replace("&iexcl;", QChar(0x00A1));
    plain = plain.replace("&Igrave;", QChar(0x00CC));
    plain = plain.replace("&igrave;", QChar(0x00EC));
    plain = plain.replace("&image;", QChar(0x2111));
    plain = plain.replace("&infin;", QChar(0x221E));
    plain = plain.replace("&int;", QChar(0x222B));
    plain = plain.replace("&Iota;", QChar(0x0399));
    plain = plain.replace("&iota;", QChar(0x03B9));
    plain = plain.replace("&iquest;", QChar(0x00BF));
    plain = plain.replace("&isin;", QChar(0x2208));
    plain = plain.replace("&Iuml;", QChar(0x00CF));
    plain = plain.replace("&iuml;", QChar(0x00EF));
    plain = plain.replace("&Kappa;", QChar(0x039A));
    plain = plain.replace("&kappa;", QChar(0x03BA));
    plain = plain.replace("&Lambda;", QChar(0x039B));
    plain = plain.replace("&lambda;", QChar(0x03BB));
    plain = plain.replace("&lang;", QChar(0x2329));
    plain = plain.replace("&laquo;", QChar(0x00AB));
    plain = plain.replace("&lArr;", QChar(0x21D0));
    plain = plain.replace("&larr;", QChar(0x2190));
    plain = plain.replace("&lceil;", QChar(0x2308));
    plain = plain.replace("&ldquo;", QChar(0x201C));
    plain = plain.replace("&le;", QChar(0x2264));
    plain = plain.replace("&lfloor;", QChar(0x230A));
    plain = plain.replace("&lowast;", QChar(0x2217));
    plain = plain.replace("&loz;", QChar(0x25CA));
    plain = plain.replace("&lrm;", QChar(0x200E));
    plain = plain.replace("&lsaquo;", QChar(0x2039));
    plain = plain.replace("&lsquo;", QChar(0x2018));
    plain = plain.replace("&lt;", QChar(0x003C));
    plain = plain.replace("&macr;", QChar(0x00AF));
    plain = plain.replace("&mdash;", QChar(0x2014));
    plain = plain.replace("&micro;", QChar(0x00B5));
    plain = plain.replace("&middot;", QChar(0x00B7));
    plain = plain.replace("&minus;", QChar(0x2212));
    plain = plain.replace("&Mu;", QChar(0x039C));
    plain = plain.replace("&mu;", QChar(0x03BC));
    plain = plain.replace("&nabla;", QChar(0x2207));
    plain = plain.replace("&nbsp;", QChar(0x00A0));
    plain = plain.replace("&ndash;", QChar(0x2013));
    plain = plain.replace("&ne;", QChar(0x2260));
    plain = plain.replace("&ni;", QChar(0x220B));
    plain = plain.replace("&not;", QChar(0x00AC));
    plain = plain.replace("&notin;", QChar(0x2209));
    plain = plain.replace("&nsub;", QChar(0x2284));
    plain = plain.replace("&Ntilde;", QChar(0x00D1));
    plain = plain.replace("&ntilde;", QChar(0x00F1));
    plain = plain.replace("&Nu;", QChar(0x039D));
    plain = plain.replace("&nu;", QChar(0x03BD));
    plain = plain.replace("&Oacute;", QChar(0x00D3));
    plain = plain.replace("&oacute;", QChar(0x00F3));
    plain = plain.replace("&Ocirc;", QChar(0x00D4));
    plain = plain.replace("&ocirc;", QChar(0x00F4));
    plain = plain.replace("&OElig;", QChar(0x0152));
    plain = plain.replace("&oelig;", QChar(0x0153));
    plain = plain.replace("&Ograve;", QChar(0x00D2));
    plain = plain.replace("&ograve;", QChar(0x00F2));
    plain = plain.replace("&oline;", QChar(0x203E));
    plain = plain.replace("&Omega;", QChar(0x03A9));
    plain = plain.replace("&omega;", QChar(0x03C9));
    plain = plain.replace("&Omicron;", QChar(0x039F));
    plain = plain.replace("&omicron;", QChar(0x03BF));
    plain = plain.replace("&oplus;", QChar(0x2295));
    plain = plain.replace("&or;", QChar(0x2228));
    plain = plain.replace("&ordf;", QChar(0x00AA));
    plain = plain.replace("&ordm;", QChar(0x00BA));
    plain = plain.replace("&Oslash;", QChar(0x00D8));
    plain = plain.replace("&oslash;", QChar(0x00F8));
    plain = plain.replace("&Otilde;", QChar(0x00D5));
    plain = plain.replace("&otilde;", QChar(0x00F5));
    plain = plain.replace("&otimes;", QChar(0x2297));
    plain = plain.replace("&Ouml;", QChar(0x00D6));
    plain = plain.replace("&ouml;", QChar(0x00F6));
    plain = plain.replace("&para;", QChar(0x00B6));
    plain = plain.replace("&part;", QChar(0x2202));
    plain = plain.replace("&permil;", QChar(0x2030));
    plain = plain.replace("&perp;", QChar(0x22A5));
    plain = plain.replace("&Phi;", QChar(0x03A6));
    plain = plain.replace("&phi;", QChar(0x03C6));
    plain = plain.replace("&Pi;", QChar(0x03A0));
    plain = plain.replace("&pi;", QChar(0x03C0));
    plain = plain.replace("&piv;", QChar(0x03D6));
    plain = plain.replace("&plusmn;", QChar(0x00B1));
    plain = plain.replace("&pound;", QChar(0x00A3));
    plain = plain.replace("&Prime;", QChar(0x2033));
    plain = plain.replace("&prime;", QChar(0x2032));
    plain = plain.replace("&prod;", QChar(0x220F));
    plain = plain.replace("&prop;", QChar(0x221D));
    plain = plain.replace("&Psi;", QChar(0x03A8));
    plain = plain.replace("&psi;", QChar(0x03C8));
    plain = plain.replace("&quot;", QChar(0x0022));
    plain = plain.replace("&radic;", QChar(0x221A));
    plain = plain.replace("&rang;", QChar(0x232A));
    plain = plain.replace("&raquo;", QChar(0x00BB));
    plain = plain.replace("&rArr;", QChar(0x21D2));
    plain = plain.replace("&rarr;", QChar(0x2192));
    plain = plain.replace("&rceil;", QChar(0x2309));
    plain = plain.replace("&rdquo;", QChar(0x201D));
    plain = plain.replace("&real;", QChar(0x211C));
    plain = plain.replace("&reg;", QChar(0x00AE));
    plain = plain.replace("&rfloor;", QChar(0x230B));
    plain = plain.replace("&Rho;", QChar(0x03A1));
    plain = plain.replace("&rho;", QChar(0x03C1));
    plain = plain.replace("&rlm;", QChar(0x200F));
    plain = plain.replace("&rsaquo;", QChar(0x203A));
    plain = plain.replace("&rsquo;", QChar(0x2019));
    plain = plain.replace("&sbquo;", QChar(0x201A));
    plain = plain.replace("&Scaron;", QChar(0x0160));
    plain = plain.replace("&scaron;", QChar(0x0161));
    plain = plain.replace("&sdot;", QChar(0x22C5));
    plain = plain.replace("&sect;", QChar(0x00A7));
    plain = plain.replace("&shy;", QChar(0x00AD));
    plain = plain.replace("&Sigma;", QChar(0x03A3));
    plain = plain.replace("&sigma;", QChar(0x03C3));
    plain = plain.replace("&sigmaf;", QChar(0x03C2));
    plain = plain.replace("&sim;", QChar(0x223C));
    plain = plain.replace("&spades;", QChar(0x2660));
    plain = plain.replace("&sub;", QChar(0x2282));
    plain = plain.replace("&sube;", QChar(0x2286));
    plain = plain.replace("&sum;", QChar(0x2211));
    plain = plain.replace("&sup;", QChar(0x2283));
    plain = plain.replace("&sup1;", QChar(0x00B9));
    plain = plain.replace("&sup2;", QChar(0x00B2));
    plain = plain.replace("&sup3;", QChar(0x00B3));
    plain = plain.replace("&supe;", QChar(0x2287));
    plain = plain.replace("&szlig;", QChar(0x00DF));
    plain = plain.replace("&Tau;", QChar(0x03A4));
    plain = plain.replace("&tau;", QChar(0x03C4));
    plain = plain.replace("&there4;", QChar(0x2234));
    plain = plain.replace("&Theta;", QChar(0x0398));
    plain = plain.replace("&theta;", QChar(0x03B8));
    plain = plain.replace("&thetasym;", QChar(0x03D1));
    plain = plain.replace("&thinsp;", QChar(0x2009));
    plain = plain.replace("&THORN;", QChar(0x00DE));
    plain = plain.replace("&thorn;", QChar(0x00FE));
    plain = plain.replace("&tilde;", QChar(0x02DC));
    plain = plain.replace("&times;", QChar(0x00D7));
    plain = plain.replace("&trade;", QChar(0x2122));
    plain = plain.replace("&Uacute;", QChar(0x00DA));
    plain = plain.replace("&uacute;", QChar(0x00FA));
    plain = plain.replace("&uArr;", QChar(0x21D1));
    plain = plain.replace("&uarr;", QChar(0x2191));
    plain = plain.replace("&Ucirc;", QChar(0x00DB));
    plain = plain.replace("&ucirc;", QChar(0x00FB));
    plain = plain.replace("&Ugrave;", QChar(0x00D9));
    plain = plain.replace("&ugrave;", QChar(0x00F9));
    plain = plain.replace("&uml;", QChar(0x00A8));
    plain = plain.replace("&upsih;", QChar(0x03D2));
    plain = plain.replace("&Upsilon;", QChar(0x03A5));
    plain = plain.replace("&upsilon;", QChar(0x03C5));
    plain = plain.replace("&Uuml;", QChar(0x00DC));
    plain = plain.replace("&uuml;", QChar(0x00FC));
    plain = plain.replace("&weierp;", QChar(0x2118));
    plain = plain.replace("&Xi;", QChar(0x039E));
    plain = plain.replace("&xi;", QChar(0x03BE));
    plain = plain.replace("&Yacute;", QChar(0x00DD));
    plain = plain.replace("&yacute;", QChar(0x00FD));
    plain = plain.replace("&yen;", QChar(0x00A5));
    plain = plain.replace("&Yuml;", QChar(0x0178));
    plain = plain.replace("&yuml;", QChar(0x00FF));
    plain = plain.replace("&Zeta;", QChar(0x0396));
    plain = plain.replace("&zeta;", QChar(0x03B6));
    plain = plain.replace("&zwj;", QChar(0x200D));
    plain = plain.replace("&zwnj;", QChar(0x200C));

    QRegularExpressionMatch match = unicode.match(plain);
    while (match.hasMatch()) {
        plain = plain.replace(match.capturedStart(), match.capturedLength(), QChar(match.captured(1).toInt()));
        match = unicode.match(plain);
    }
    return plain;
}

QString RssSupportPrivate::fromHtmlEscaped(const QString &rich)
{
    QString plain = rich;
    plain = plain.replace("&lt;", "<");
    plain = plain.replace("&gt;", ">");
    plain = plain.replace("&amp;", "&");
    plain = plain.replace("&quot;", "\"");
    return plain;
}

QString RssSupportPrivate::stripQuotes(const QString &string)
{
    QString newString = string;
    if (newString.startsWith("\"") || newString.startsWith("'")) {
        newString.remove(0, 1);
    }
    if (newString.endsWith("\"") || newString.endsWith("'")) {
        newString.remove(newString.size() - 1, 1);
    }
    return newString;
}

QString RssSupportPrivate::manageTag(const QString &tag, const QString &tagArguments,
                                     const QString &text)
{
    QString newText = fromHtmlEscaped(text).trimmed();
    if (tag == "a") {
        QRegularExpressionMatchIterator iterator = arguments.globalMatch(tagArguments);
        while (iterator.hasNext()) {
            QRegularExpressionMatch match = iterator.next();
            if (match.captured(1) == HREF_KEY) {
                return QString("<a href=\"%1\">%2</a>").arg(stripQuotes(match.captured(2)), newText);
            }
        }
        return newText;
    } else if (tag == "abbr") {
        return newText;
    } else if (tag == "acronym") {
        return newText;
    } else if (tag == "article") { // Skipped address, applet, area
        return taggedText("p", newText);
    } else if (tag == "b") { // Skipped aside, audio
        return taggedText("b", newText);
    } else if (tag == "bdi") { // Skipped base, basefont
        return newText;
    } else if (tag == "bdo") {
        return newText;
    } else if (tag == "blockquote") {
        return taggedText("p", newText); // TODO: format better ?
    } else if (tag == "br") { // Skipped body
        return "<br/>";
    } else if (tag == "center") { // Skipped button, canvas, caption,
        return newText;
    } else if (tag == "cite") {
        return newText; // TODO: maybe new paragraph ?
    } else if (tag == "code") {
        return newText;
    } else if (tag == "del") { // Skipped col
        return newText;
    } else if (tag == "em") { // Skipped del, details, dfn, dialog
        return taggedText("i", newText);
    } else if (tag == "div") { // Skipped dir, div, dl, dt TODO: really skip dl, dt ?
        return taggedText("p", newText);
    } else if (tag == "font") { // Skipped embed, fieldlist, figcaption, figure
        return newText;
    } else if (title.match(tag).hasMatch()) { // Skipped footer, form, frame, frameset
        return taggedText(tag, newText);
    } else if (tag == "i") { // Skipped head, header, hgroup, hr, html TODO: what to do with hr
        return taggedText("i", newText);
    } else if (tag == "li") { // Skipped iframe
        return taggedText("li", newText);
    } else if (tag == "img") {
        QRegularExpressionMatchIterator iterator = arguments.globalMatch(tagArguments);
        while (iterator.hasNext()) {
            QRegularExpressionMatch match = iterator.next();
            if (match.captured(1) == SRC_KEY) {
                QString src = stripQuotes(match.captured(2));
                images.append(src);
                return QString("<img src=\"%1\" />").arg(src);
            }
        }
        return QString();
    } else if (tag == "main") { // Skipping input, ins, kbd, keygen, label, legend link,
        return newText;
    } else if (tag == "mark") { // Skipped map,
        return taggedText("b", newText); // TODO: really use bold ?
    } else if (tag == "ol") { // Skipped menu, meta, meter, nav, noframes, noscript, object,
        return taggedText("ol", newText);
    } else if (tag == "p") { // Skipped optgroup, option, output
        return taggedText("p", newText);
    } else if (tag == "pre") { // Skipped param
        return taggedText("pre", newText);
    } else if (tag == "q") { // Skipped progress,
        return newText;
    } else if (tag == "span") { // Skipped rp, rt, ruby, s, samp, script, section, select, small, source TODO: really skip samp, section ?
        return newText;
    } else if (tag == "strike") {
        return newText;
    } else if (tag == "strong") {
        return taggedText("b", newText);
    } else if (tag == "table") { // Skipped style, sub, summary, sup
        return newText;
    } else if (tag == "tbody") {
        return newText;
    } else if (tag == "td") {
        return newText;
    } else if (tag == "tr") { // Skipped newTextarea, tfoot, th, thread, time, title
        return taggedText("p", newText);
    } else if (tag == "tt") { // Skipped track
        return newText;
    } else if (tag == "u") {
        return taggedText("u", newText);
    } else if (tag == "ul") {
        return taggedText("ul", newText);
    } // Skipped var, video, wbr

    return QString();
}

RssSupport::RssSupport()
    : d_ptr(new RssSupportPrivate)
{
}

RssSupport::~RssSupport()
{
}

void RssSupport::setHtml(const QString &html)
{
    Q_D(RssSupport);
    d->images.clear();
    d->processedHtml = html;
    d->processedHtml = d->processedHtml.replace(d->comment, "");

    QRegularExpression tagOpen ("<(\\w+)([^>]*)>");
    QRegularExpression tagClose ("</(\\w+)[^>]*>");
    QRegularExpressionMatch match = tagOpen.match(d->processedHtml);
    while (match.hasMatch()) {
        // We should find the end of the tag
        int end = match.capturedEnd();
        QRegularExpressionMatch nextOpen = tagOpen.match(d->processedHtml, end);
        QRegularExpressionMatch nextClose = tagClose.match(d->processedHtml, end);

        bool openMatch = nextOpen.hasMatch();
        int openStart = nextOpen.capturedStart();

        bool closeMatch = nextClose.hasMatch();
        int closeStart = nextClose.capturedStart();
        if (openMatch && ((closeMatch && openStart < closeStart) || !closeMatch)) {
            match = nextOpen;
        } else if (closeMatch && ((openMatch && closeStart < openStart) || !openMatch)) {
            QString openTag = match.captured(1);
            QString closeTag = nextClose.captured(1);

            if (openTag != closeTag) {
                QString managed = d->manageTag(openTag, match.captured(2));
                // Replacing the text with the managed one
                d->processedHtml = d->processedHtml.replace(match.capturedStart(),
                                                            match.capturedLength(),
                                                            managed.toHtmlEscaped());
            } else {
                int textLength = nextClose.capturedStart() - match.capturedEnd();
                int totalLength = nextClose.capturedEnd() - match.capturedStart();
                QString managed = d->manageTag(openTag, match.captured(2),
                                               d->processedHtml.mid(match.capturedEnd(),
                                                                    textLength));
                // Replacing the text with the managed one
                d->processedHtml = d->processedHtml.replace(match.capturedStart(), totalLength,
                                                            managed.toHtmlEscaped());
            }
            match = tagOpen.match(d->processedHtml);
        } else {
            // Neither open nor close match
            // Means that we have a self closing tag at the end: we parse it
            QString managed = d->manageTag(match.captured(1), match.captured(2));
            // Replacing the text with the managed one
            d->processedHtml = d->processedHtml.replace(match.capturedStart(),
                                                        match.capturedLength(),
                                                        managed.toHtmlEscaped());
            match = tagOpen.match(d->processedHtml);
        }
    }

    d->processedHtml = d->fromHtmlCleaned(RssSupportPrivate::fromHtmlEscaped(d->processedHtml));
}

QString RssSupport::processedHtml() const
{
    Q_D(const RssSupport);
    return d->processedHtml;
}

QList<QString> RssSupport::images() const
{
    Q_D(const RssSupport);
    return d->images;
}
