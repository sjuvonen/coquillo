
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
	
	const QRegExp pref("^pref");
	
	foreach (QLineEdit * line, findChildren<QLineEdit*>(pref)) {
		QString name = line->objectName().remove(pref);
		line->setText(_settings->value(name).toString());
	}

	foreach (QAbstractButton * button, findChildren<QAbstractButton*>(pref)) {
		if (!button->inherits("QRadioButton") && !button->inherits("QCheckBox")) {
			continue;
		}

		QString name = button->objectName().remove(pref);
		button->setChecked(_settings->value(name).toBool());
	}

	foreach (QTextEdit * edit, findChildren<QTextEdit*>(pref)) {
		QString name = edit->objectName().remove(pref);
		edit->setPlainText(_settings->value(name).toString());
	}

	foreach (QComboBox * combo, findChildren<QComboBox*>(pref)) {
		QString name = combo->objectName().remove(pref);
		combo->setCurrentIndex(_settings->value(name).toInt());
	}

	foreach (QSpinBox * box, findChildren<QSpinBox*>(pref)) {
		QString name = box->objectName().remove(pref);
		box->setValue(_settings->value(name).toInt());
	}
}

void SettingsDialog::saveSettings() {
	if (!backend())
		return;

	const QRegExp pref("^pref");

	foreach (QLineEdit * line, findChildren<QLineEdit*>(pref)) {
		QString name = line->objectName().remove(pref);
		_settings->setValue(name, line->text());
	}

	foreach (QAbstractButton * button, findChildren<QAbstractButton*>(pref)) {
		if (!button->inherits("QRadioButton") && !button->inherits("QCheckBox")) {
			continue;
		}

		QString name = button->objectName().remove(pref);
		_settings->setValue(name, button->isChecked());
	}

	foreach (QTextEdit * edit, findChildren<QTextEdit*>(pref)) {
		QString name = edit->objectName().remove(pref);
		_settings->setValue(name, edit->toPlainText());
	}

	foreach (QComboBox * combo, findChildren<QComboBox*>(pref)) {
		QString name = combo->objectName().remove(pref);
		_settings->setValue(name, combo->currentIndex());
	}

	foreach (QSpinBox * box, findChildren<QSpinBox*>(pref)) {
		QString name = box->objectName().remove(pref);
		_settings->setValue(name, box->value());
	}
}