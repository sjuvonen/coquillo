
#include <QSettings>

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget * parent)
: QDialog(parent), _settings(0) {

	_ui = new Ui::SettingsDialog();
	_ui->setupUi(this);
}

SettingsDialog::~SettingsDialog() {
	delete _ui;
}

void SettingsDialog::setBackend(QSettings * settings) {
	bool load = (_settings == 0);

	_settings = settings;

	if (load) {
		loadSettings();
	}
}

void SettingsDialog::accept() {
	saveSettings();
	close();
}

void SettingsDialog::loadSettings() {
	if (!backend())
		return;
	
	foreach (QLineEdit * line, findChildren<QLineEdit*>()) {
		QString name = line->objectName();
		name[0] = name[0].toUpper();

		line->setText(_settings->value(name).toString());
	}

	foreach (QAbstractButton * button, findChildren<QAbstractButton*>()) {
		if (!button->inherits("QRadioButton") && !button->inherits("QCheckBox")) {
			continue;
		}

		QString name = button->objectName();
		name[0] = name[0].toUpper();

		button->setChecked(_settings->value(name).toBool());
	}

	foreach (QTextEdit * edit, findChildren<QTextEdit*>()) {
		QString name = edit->objectName();
		name[0] = name[0].toUpper();

		edit->setPlainText(_settings->value(name).toString());
	}

	foreach (QComboBox * combo, findChildren<QComboBox*>()) {
		QString name = combo->objectName();
		name[0] = name[0].toUpper();

		combo->setCurrentIndex(_settings->value(name).toInt());
	}
}

void SettingsDialog::saveSettings() {
	if (!backend())
		return;

	foreach (QLineEdit * line, findChildren<QLineEdit*>()) {
		QString name = line->objectName();
		name[0] = name[0].toUpper();

		_settings->setValue(name, line->text());
	}

	foreach (QAbstractButton * button, findChildren<QAbstractButton*>()) {
		if (!button->inherits("QRadioButton") && !button->inherits("QCheckBox")) {
			continue;
		}

		QString name = button->objectName();
		name[0] = name[0].toUpper();

		_settings->setValue(name, button->isChecked());
	}

	foreach (QTextEdit * edit, findChildren<QTextEdit*>()) {
		QString name = edit->objectName();
		name[0] = name[0].toUpper();

		_settings->setValue(name, edit->toPlainText());
	}

	foreach (QComboBox * combo, findChildren<QComboBox*>()) {
		QString name = combo->objectName();
		name[0] = name[0].toUpper();

		_settings->setValue(name, combo->currentIndex());
	}
}