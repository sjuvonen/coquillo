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

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QListWidget>
#include <QPushButton>
#include <QStackedWidget>

#include "SettingsDialog.h"
#include "SettingsDialogPage.h"

SettingsDialog::SettingsDialog(QWidget * parent) : QDialog(parent) {
	_navigation = new QListWidget(this);
	_navigation->setFixedWidth(120);
	_navigation->setViewMode(QListView::IconMode);
	_navigation->setVisible(false);
	_navigation->setResizeMode(QListView::Adjust);

	_stack = new QStackedWidget(this);

	QPushButton * ok = new QPushButton(tr("OK"));
	QPushButton * cancel = new QPushButton(tr("Cancel"));

	QDialogButtonBox * buttons = new QDialogButtonBox(this);
	buttons->addButton(ok, QDialogButtonBox::AcceptRole);
	buttons->addButton(cancel, QDialogButtonBox::RejectRole);

	QGridLayout * layout = new QGridLayout;
	layout->addWidget(_navigation, 0, 0);
	layout->addWidget(_stack, 0, 1);
	layout->addWidget(buttons, 1, 1);

	setLayout(layout);
	setWindowTitle(tr("Configure..."));

	resize(460, 300);

	connect(cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
	connect(_navigation, SIGNAL(currentRowChanged(int)), _stack, SLOT(setCurrentIndex(int)));
}

void SettingsDialog::addPage(SettingsDialogPage * page) {
	QListWidgetItem * item = new QListWidgetItem(page->icon(), page->name(), _navigation);
	item->setSizeHint(QSize(114, 56));

	_stack->addWidget(page);

	if (_stack->count() == 1)
		_navigation->setCurrentRow(0);

	if (_stack->count() > 1)
		_navigation->setVisible(true);

	connect(this, SIGNAL(accepted()), page, SLOT(saveSettings()));
}

void SettingsDialog::setNavigationVisible(bool state) {
	_navigation->setVisible(state);
}

QMap<QString, QVariant> SettingsDialog::settingsData() const {
	QMap<QString, QVariant> data;

	for (int i = 0; i < _stack->count(); i++) {
		SettingsDialogPage * p = qobject_cast<SettingsDialogPage*>(_stack->widget(i));
		data.unite(p->settings());
	}

	return data;
}
