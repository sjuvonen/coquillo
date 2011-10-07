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

#ifndef SETTINGSDIALOGPAGE_H
#define SETTINGSDIALOGPAGE_H

#include <QIcon>
#include <QWidget>

namespace Ui {
	class SettingsPage1;
	class SettingsPage2;
	class SettingsPage3;
	class SettingsPage4;
}

class SettingsDialogPage : public QWidget {
	Q_OBJECT

	public:
		SettingsDialogPage(const QString & name, const QIcon & icon, QWidget * parent=0);

		QString name() const { return _name; }
		void setName(const QString & name) { _name = name; }

		QIcon icon() const { return _icon; }
		void setIcon(const QIcon & icon) { _icon = icon; }

		virtual QMap<QString, QVariant> settings() const = 0;

	public slots:
		virtual void saveSettings() = 0;

	private:
		QString _name;
		QIcon _icon;
};

class SettingsPage1 : public SettingsDialogPage {
	Q_OBJECT

	public:
		SettingsPage1(QWidget * parent=0);
		QMap<QString, QVariant> settings() const;

	public slots:
		void saveSettings();

	private:
		Ui::SettingsPage1 * _ui;
};

class SettingsPage2 : public SettingsDialogPage {
	Q_OBJECT

	public:
		SettingsPage2(QWidget * parent=0);
		QMap<QString, QVariant> settings() const;

	public slots:
		void saveSettings();

	private:
		Ui::SettingsPage2 * _ui;
};

class SettingsPage3 : public SettingsDialogPage {
	Q_OBJECT

	public:
		SettingsPage3(QWidget * parent=0);
		QMap<QString, QVariant> settings() const;

	public slots:
		void saveSettings();

	private:
		Ui::SettingsPage3 * _ui;
};

class SettingsPage4 : public SettingsDialogPage {
	Q_OBJECT

	public:
		SettingsPage4(QWidget * parent=0);
		QMap<QString, QVariant> settings() const;

	public slots:
		void saveSettings();

	private:
		Ui::SettingsPage4 * _ui;
};

#endif
