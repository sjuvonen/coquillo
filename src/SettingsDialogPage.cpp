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
#include <QMap>
#include <QSettings>

#include "SettingsDialogPage.h"

#include "ui_SettingsPage1.h"
#include "ui_SettingsPage2.h"
#include "ui_SettingsPage3.h"
#include "ui_SettingsPage4.h"

#include "globals.h"

#include <QDebug>

SettingsDialogPage::SettingsDialogPage(const QString & name, const QIcon & icon, QWidget * parent)
	: QWidget(parent), _name(name), _icon(icon) {

}

/**
 * SettingsPage1
 **/


SettingsPage1::SettingsPage1(QWidget * parent)
: SettingsDialogPage(tr("General"), QIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon)), parent) {

	_ui = new Ui::SettingsPage1;
	_ui->setupUi(this);

	QSettings settings;

	const QString defCddb = "http://freedb.freedb.org/~cddb/cddb.cgi";

	const QString defDir = QDesktopServices::storageLocation(QDesktopServices::MusicLocation);
	const QString homeDir = settings.value("Scanning/HomeDirectory", defDir).toString();
	bool recursive = settings.value("Scanning/ScanRecursive").toBool();

	const QString cddbUrl = settings.value("CDDB/URL", defCddb).toString();

	_ui->defaultDirectory->setText(homeDir);
	_ui->recursiveScanning->setChecked(recursive);
	_ui->cddbUrl->setText(cddbUrl);
	_ui->checkDimSubdirectories->setChecked(Coquillo::dimSubdirectoriesBackground);
}

void SettingsPage1::saveSettings() {
	QSettings settings;
	QMap<QString, QVariant> s = this->settings();

	for (QMap<QString, QVariant>::const_iterator i = s.constBegin(); i != s.constEnd(); i++)
		settings.setValue(i.key(), i.value());

	settings.sync();

	Coquillo::dimSubdirectoriesBackground = settings.value("MainWindow/DimSubDirectoriesRows").toBool();
}

QMap<QString, QVariant> SettingsPage1::settings() const {
	QMap<QString, QVariant> data;

	const QString cddb = _ui->cddbUrl->text().trimmed();
	const QString homeDir = _ui->defaultDirectory->text().trimmed();
	bool recursive = _ui->recursiveScanning->isChecked();

	data.insert("Scanning/HomeDirectory", homeDir);
	data.insert("Scanning/ScanRecursive", recursive);
	data.insert("CDDB/URL", cddb);
	data.insert("MainWindow/DimSubDirectoriesRows", _ui->checkDimSubdirectories->isChecked());

	return data;
}





/**
 * SettingsPage2
 **/

SettingsPage2::SettingsPage2(QWidget * parent)
: SettingsDialogPage(tr("Tags"), QIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon)), parent) {

	_ui = new Ui::SettingsPage2;
	_ui->setupUi(this);

	QSettings settings;

	const QSize size = settings.value("Pictures/ScaleSize", QSize(100, 100)).toSize();
	bool scale = settings.value("Pictures/ScalePictures", true).toBool();

	bool mp3v1 = settings.value("FileTypes/MPEG/WriteID3v1", true).toBool();
	bool flacv2 = settings.value("FileTypes/FLAC/WriteID3v2").toBool();
	bool vlc = settings.value("FileTypes/OggVorbis/StripLegacyCovers").toBool();

	_ui->scalePicturesW->setValue( size.width() );
	_ui->scalePicturesH->setValue( size.height() );

	_ui->scalePictures->setChecked(scale);
	_ui->mpegV1->setChecked(mp3v1);
	_ui->flacV2->setChecked(flacv2);
	_ui->vorbisLegacyCovers->setChecked(vlc);

	int i = _ui->tagsEncoding->findText( settings.value("Application/Encoding", "UTF-8").toString() );

	_ui->tagsEncoding->setCurrentIndex(i);
}

void SettingsPage2::saveSettings() {
	QSettings settings;
	QMap<QString, QVariant> s = this->settings();

	for (QMap<QString, QVariant>::const_iterator i = s.constBegin(); i != s.constEnd(); i++)
		settings.setValue(i.key(), i.value());

	settings.sync();

	Coquillo::encoding = s.value("Application/Encoding").toString() == "UTF-8" ? Coquillo::UTF_8 : Coquillo::ISO_8859_15;

}


QMap<QString, QVariant> SettingsPage2::settings() const {
	QMap<QString, QVariant> data;

	bool scale = _ui->scalePictures->isChecked();
	int w = _ui->scalePicturesW->value();
	int h = _ui->scalePicturesH->value();

	const QSize size(w, h);

	bool mp3v1 = _ui->mpegV1->isChecked();
	bool flacv2 = _ui->flacV2->isChecked();
	bool vlc = _ui->vorbisLegacyCovers->isChecked();

	const QString encoding = _ui->tagsEncoding->currentText();

	data.insert("Pictures/ScalePictures", scale);
	data.insert("Pictures/ScaleSize", size);
	data.insert("FileTypes/MPEG/WriteID3v1", mp3v1);
	data.insert("FileTypes/FLAC/WriteID3v2", flacv2);
	data.insert("FileTypes/OggVorbis/StripLegacyCovers", vlc);
	data.insert("Application/Encoding", encoding);

	return data;
}

SettingsPage3::SettingsPage3(QWidget * parent)
: SettingsDialogPage(tr("Processor"), QIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon)), parent) {
	_ui = new Ui::SettingsPage3;
	_ui->setupUi(this);

	QSettings settings;

	bool intelligent = settings.value("Processing/IntelligentCapitalization").toBool();
	int padWidth = settings.value("Processing/PadWidth", 2).toInt();

	_ui->intelligentCapitalization->setChecked(intelligent);
	_ui->padWidth->setValue(padWidth);
}

QMap<QString, QVariant> SettingsPage3::settings() const {
	QMap<QString, QVariant> data;

	bool intelligent = _ui->intelligentCapitalization->isChecked();
	int padWidth = _ui->padWidth->text().toInt();

	data.insert("Processing/IntelligentCapitalization", intelligent);
	data.insert("Processing/PadWidth", padWidth);

	return data;
}

void SettingsPage3::saveSettings() {
	QMap<QString, QVariant> s = this->settings();
	QSettings settings;

	for (QMap<QString, QVariant>::const_iterator i = s.constBegin(); i != s.constEnd(); i++)
		settings.setValue(i.key(), i.value());

	settings.sync();

	Coquillo::numberPadWidth = s.value("Processing/PadWidth").toInt();
	Coquillo::intelligentCapitalization = s.value("Processing/IntelligentCapitalization").toBool();
}

SettingsPage4::SettingsPage4(QWidget * parent)
: SettingsDialogPage(tr("Automations"), QIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon)), parent) {
	_ui = new Ui::SettingsPage4;
	_ui->setupUi(this);

	QSettings settings;

	bool trim = settings.value("Processing/TrimWhiteSpace", true).toBool();
	bool safeNames = settings.value("Processing/SafeFileNames").toBool();
	bool convertExts = settings.value("Processing/ConvertExtensions").toBool();
	bool lowerCase = settings.value("Processing/LowerCaseExtensions", true).toBool();
	bool removeDisc = settings.value("Processing/RemoveDiscFromAlbumName").toBool();

	_ui->removeDiscFromAlbum->setChecked(removeDisc);
	_ui->trimWhiteSpace->setChecked(trim);
	_ui->safeFileNames->setChecked(safeNames);
	_ui->convertExtensions->setChecked(convertExts);
	_ui->extensionsLowerCase->setChecked(lowerCase);
	_ui->extensionsUpperCase->setChecked(!lowerCase);
}

QMap<QString, QVariant> SettingsPage4::settings() const {
	QMap<QString, QVariant> data;

	bool trim = _ui->trimWhiteSpace->isChecked();
	bool safeNames = _ui->safeFileNames->isChecked();
	bool convertExts = _ui->convertExtensions->isChecked();
	bool lowerCase = _ui->extensionsLowerCase->isChecked();
	bool removeDisc = _ui->removeDiscFromAlbum->isChecked();

	data.insert("Processing/RemoveDiscFromAlbumName", removeDisc);
	data.insert("Processing/TrimWhiteSpace", trim);
	data.insert("Processing/SafeFileNames", safeNames);
	data.insert("Processing/ConvertExtensions", convertExts);
	data.insert("Processing/LowerCaseExtensions", lowerCase);

	return data;
}

void SettingsPage4::saveSettings() {
	QSettings settings;
	QMap<QString, QVariant> s = this->settings();

	for (QMap<QString, QVariant>::const_iterator i = s.constBegin(); i != s.constEnd(); i++)
		settings.setValue(i.key(), i.value());

	settings.sync();

	bool convext = s.value("Processing/ConvertExtensions").toBool();
	bool lower = s.value("Processing/LowerCaseExtensions").toBool();

	Coquillo::fileExtensionCase = convext ? (lower ? 1 : 2) : 0;
	Coquillo::trimWhiteSpace = s.value("Processing/TrimWhiteSpace").toBool();
	Coquillo::safeFileNames = s.value("Processing/SafeFileNames").toBool();
	Coquillo::removeDiscFromAlbumName = s.value("Processing/RemoveDiscFromAlbumName").toBool();
}

