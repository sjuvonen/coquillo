/***********************************************************************
* Copyright (c) 2011 Samu Juvonen <samu.juvonen@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#include <QApplication>
#include <QDesktopServices>
#include <QPixmap>
#include <QUrl>

#include "globals.h"
#include "AboutDialog.h"

#include "uih/ui_AboutApp.h"

AboutDialog::AboutDialog(QWidget * parent) : QDialog(parent) {
	_ui = new Ui::AboutApp;
	_ui->setupUi(this);

	const QImage icon(":/resources/Coquillo.png");
	const QSize size(128, 128);

	_ui->labelIcon->setFixedSize(size);
	_ui->labelIcon->setPixmap( QPixmap::fromImage(icon.scaled(size, Qt::KeepAspectRatio)) );
	_ui->labelAppName->setText( QApplication::applicationName() );
	_ui->labelAppVersion->setText( tr("Version: ") + QApplication::applicationVersion() );

	_ui->labelAppReleased->setText( tr("Released: " ) + Coquillo::releaseDate);

	_ui->labelDescription->setText(
		tr("Coquillo is a metadata editor for audio files.")
	);

	_ui->labelCopyright->setText(
		tr("Copyright: ") +
		"2010-2011 Samu Juvonen"
	);

	_ui->labelWebsite->setText(
		tr("Website: ") +
		"<a href='http://qt-apps.org/content/show.php/Coquillo?content=141896'>Qt-Apps.org</a>"
	);

	_ui->labelLicense->setText(
		tr("License: ") +
		"<a href='http://www.gnu.org/licenses/gpl-2.0.html/'>GNU GPLv2</a>"
	);
}

AboutDialog::~AboutDialog() {
	delete _ui;
}
