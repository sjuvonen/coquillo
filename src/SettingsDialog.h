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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QList>

class QListWidget;
class QStackedWidget;

class SettingsDialogPage;

class SettingsDialog : public QDialog {
	Q_OBJECT

	public:
		SettingsDialog(QWidget * parent=0);
		void addPage(SettingsDialogPage * page);
		void setNavigationVisible(bool state);

		QMap<QString, QVariant> settingsData() const;

	private:
		QListWidget * _navigation;
		QStackedWidget * _stack;
};

#endif
