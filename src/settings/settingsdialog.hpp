#ifndef COQUILLO_SETTINGS_SETTINGSDIALOG_H
#define COQUILLO_SETTINGS_SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

namespace Coquillo {
    namespace Settings {
        class SettingsDialog : public QDialog {
            Q_OBJECT

            public:
                SettingsDialog(QWidget * parent=0);
                ~SettingsDialog();

            public slots:
                void accept() { saveSettings(); QDialog::accept(); }

            private slots:
                void selectHomeDirectory();

            private:
                QList<QWidget*> findInputs() const;
                void restoreSettings();
                void saveSettings();

                Ui::SettingsDialog * _ui;

        };
    }
}

#endif
