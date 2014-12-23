
#include <QDebug>
#include <QFileDialog>
#include <QRegExp>
#include <QSettings>

#include "ui_settingsdialog.h"
#include "settingsdialog.h"

namespace Coquillo {
    namespace Settings {
        SettingsDialog::SettingsDialog(QWidget * parent)
        : QDialog(parent) {
            _ui = new Ui::SettingsDialog;
            _ui->setupUi(this);

            restoreSettings();
        }

        SettingsDialog::~SettingsDialog() {
            delete _ui;
        }

        QList<QWidget*> SettingsDialog::findInputs() const {
            return findChildren<QWidget*>(QRegExp("conf_(.+)"));
        }

        void SettingsDialog::restoreSettings() {
            QSettings settings;
            QList<QWidget*> inputs = findInputs();

            foreach (QWidget * input, inputs) {
                const QString key = input->objectName().mid(5);

                if (input->inherits("QLineEdit")) {
                    qobject_cast<QLineEdit*>(input)->setText(settings.value(key).toString());
                } else if (input->inherits("QCheckBox")) {
                    qobject_cast<QCheckBox*>(input)->setChecked(settings.value(key).toBool());
                }
            }
        }

        void SettingsDialog::saveSettings() {
            QSettings settings;
            QList<QWidget*> inputs = findInputs();

            foreach (QWidget * input, inputs) {
                const QString key = input->objectName().mid(5);

                if (input->inherits("QLineEdit")) {
                    settings.setValue(key, qobject_cast<QLineEdit*>(input)->text());
                } else if (input->inherits("QCheckBox")) {
                    settings.setValue(key, qobject_cast<QCheckBox*>(input)->isChecked());
                }
            }
        }

        void SettingsDialog::selectHomeDirectory() {
            const QString current_home = QSettings().value("DefaultLocation").toString();
            QString new_home = QFileDialog::getExistingDirectory(this, tr("Select directory"), current_home);

            if (!new_home.isNull()) {
                _ui->conf_DefaultLocation->setText(new_home);
            }
        }
    }
}
