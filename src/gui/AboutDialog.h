#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
	class AboutApp;
}

class AboutDialog : public QDialog {
	Q_OBJECT

	public:
		AboutDialog(QWidget * parent=0);
		~AboutDialog();

	private:
		Ui::AboutApp * _ui;

};

#endif