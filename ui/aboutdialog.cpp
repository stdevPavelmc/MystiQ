/*  MystiQ - a C++/Qt5 gui frontend for ffmpeg
 *  Copyright (C) 2011-2019 Maikel Llamaret Heredia <llamaret@webmisolutions.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGlobal>
#include <QLocale>
#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "version.h"
#include "services/constants.h"
#ifdef USE_LIBNOTIFY
 #include "services/notificationservice-libnotify.h"
#endif

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QTextBrowser *info = ui->txtInfo;
    QTextBrowser *translators = ui->txtTranslators;

    QString versionId = " %1";
#ifdef VERSION_ID_STRING
    versionId = versionId.arg(QString(VERSION_ID_STRING));
#endif

    QString libnotifyVersion;
#ifdef USE_LIBNOTIFY
    libnotifyVersion = QString(" and libnotify %1").arg(NotiticationService_libnotify::getVersion());
#endif

    QString aboutText = info->toHtml();
    aboutText = aboutText
            .arg(VERSION_STRING)
            .arg(versionId)
            .arg(Constants::getBool("Portable") ? "Portable" : "")
            .arg(QT_VERSION_STR)
            .arg(libnotifyVersion);
    info->setText(aboutText);

    translators->setHtml(getTranslators());

    // Set the background color of the textbox to the color of the window.
    QPalette p = info->palette();
    p.setColor(QPalette::Base, this->palette().color(QPalette::Window));

    info->setPalette(p);
    info->setFrameShape(QTextBrowser::NoFrame);  // Hide textbox border.

    translators->setPalette(p);
    translators->setFrameShape(QTextBrowser::NoFrame);

    // Make the window size fixed.
    this->adjustSize();
    this->setMinimumWidth(this->width());
    this->setMinimumHeight(this->height());
    this->setMaximumWidth(this->width());
    this->setMaximumHeight(this->height());

    ui->tabInfo->setAutoFillBackground(true);
    ui->tabTranslators->setAutoFillBackground(true);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

QString AboutDialog::getTranslators()
{
    struct {QString locale; QString translator;} table[] = {
        //: Spanish Language (Spain)
        {"es_ES", trad(tr("Spanish (Spain)")
                    , "Maikel Llamaret <llamaret@webmisolutions.com>")},
    };
    const int size = sizeof(table) / sizeof(table[0]);

    QStringList translators;
    QString current_locale = QLocale::system().name();
    for (int i=0; i<size; i++) {
        // Put the translator of the current language at the top.
        if (current_locale.startsWith(table[i].locale))
            translators.push_front(table[i].translator);
        else
            translators.push_back(table[i].translator);
    }

    return translators.join("");
}

QString AboutDialog::trad(const QString& lang, const QString& author)
{
    return trad(lang, QStringList() << author);
}

QString AboutDialog::trad(const QString& lang, const QStringList& authors)
{
    QString s = "<ul>";
    for (int n = 0; n < authors.count(); n++) {
        QString author = authors[n];
        s += "<li>"+ author.replace("<", "&lt;").replace(">", "&gt;")
                + "</li>";
    }
    s+= "</ul>";

    return QString("<b>%1</b>: %2").arg(lang).arg(s);
}
