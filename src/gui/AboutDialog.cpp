#include <QApplication>
#include <QDesktopServices>
#include <QPixmap>
#include <QUrl>

#include "AboutDialog.h"

#include "ui_AboutApp.h"

// From main.cpp
extern const char * releaseDate;

AboutDialog::AboutDialog(QWidget * parent) : QDialog(parent) {
	_ui = new Ui::AboutApp;
	_ui->setupUi(this);

	const QImage icon(":/resources/Coquillo.png");
	const QSize size(128, 128);

	_ui->labelIcon->setFixedSize(size);
	_ui->labelIcon->setPixmap( QPixmap::fromImage(icon.scaled(size, Qt::KeepAspectRatio)) );
	_ui->labelAppName->setText( QApplication::applicationName() );
	_ui->labelAppVersion->setText( tr("Version: ") + QApplication::applicationVersion() );

	_ui->labelAppReleased->setText( tr("Released: " ) + ::releaseDate);

	_ui->labelDescription->setText(
		tr("Coquillo is a metadata editor for audio files.")
	);

	_ui->labelCopyright->setText(
		tr("Copyright: ") +
		"2010-2012 Samu Juvonen"
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
