#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QSettings;

namespace Ui {
	class SettingsDialog;
}

class SettingsDialog : public QDialog {
	Q_OBJECT

	public:
		SettingsDialog(QWidget * parent=0);
		~SettingsDialog();

		void setBackend(QSettings * s);
		QSettings * backend() const { return _settings; }

	public slots:
		void accept();

	private:
		void loadSettings();
		void saveSettings();
		
		Ui::SettingsDialog * _ui;
		QSettings * _settings;
};

#endif
