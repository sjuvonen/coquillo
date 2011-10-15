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

#include <QCompleter>
#include <QDir>
#include <QFileInfo>
#include <QPushButton>
#include <QSettings>
#include <QStackedWidget>

#include <metadata/MetaDataModel.h>

#include "ProcessorWidget.h"

#include "ui_Processor.h"

#include "globals.h"

#include <QDebug>

ProcessorWidget::ProcessorWidget(QWidget * parent)
: QWidget(parent), _model(0) {

	QSettings settings;

	const QSize size = settings.value("Processor/WindowSize").toSize();

	if (!size.isNull())
		resize(size);

	_symbols.insert("%a", "Artist");
	_symbols.insert("%b", "Album");
	_symbols.insert("%c", "Comment");
	_symbols.insert("%d", "Disc");
	_symbols.insert("%m", "MaxNumber");
	_symbols.insert("%n", "Number");
	_symbols.insert("%o", "OriginalArtist");
	_symbols.insert("%p", "Composer");
	_symbols.insert("%t", "Title");
	_symbols.insert("%y", "Year");
	_symbols.insert("%i", "Ignore");

	_ui = new Ui::ProcessorWidget;
	_ui->setupUi(this);

	readPatternHistory(&_resolveHistory, 0);
	readPatternHistory(&_renameHistory, 1);

	/**
	 * We create a copy of the history because we want to preserve the order
	 * of patterns in history so that we can easily drop the least frequently
	 * used pattern when the list is full.
	 **/

	QStringList resolveHist(_resolveHistory);
	QStringList renameHist(_renameHistory);

	resolveHist.sort();
	renameHist.sort();

	_ui->comboResolvePattern->completer()->setCaseSensitivity(Qt::CaseSensitive);
	_ui->comboResolvePattern->setDuplicatesEnabled(false);
	_ui->comboResolvePattern->insertItems(0, resolveHist);

	_ui->comboRenamePattern->completer()->setCaseSensitivity(Qt::CaseSensitive);
	_ui->comboRenamePattern->setDuplicatesEnabled(false);
	_ui->comboRenamePattern->insertItems(0, renameHist);

	_ui->comboRenamePattern->setEditText(QString());
	_ui->comboResolvePattern->setEditText(QString());

	int i = 0;
	foreach (QAbstractButton * b, _ui->groupCapitalization->buttons())
		_ui->groupCapitalization->setId(b, i++);

	connect(_ui->buttonsDialog, SIGNAL(rejected()), SLOT(close()));
	connect(_ui->buttonsDialog->button(QDialogButtonBox::Apply), SIGNAL(clicked()), SLOT(apply()));

	connect(_ui->comboResolvePattern, SIGNAL(editTextChanged(const QString &)), SLOT(updateResolvePreview()));
	connect(_ui->comboRenamePattern, SIGNAL(editTextChanged(const QString &)), SLOT(updateRenamePreview()));
}

ProcessorWidget::~ProcessorWidget() {
	delete _ui;
}





void ProcessorWidget::setDataIndexes(const QModelIndexList & indexes) {
	_indexes.clear();

	foreach (QPersistentModelIndex i, indexes)
		_indexes << i;

	updateRenamePreview();
	updateResolvePreview();
}

void ProcessorWidget::setFloatingLayout(bool setFloating) {
	QDialogButtonBox * box = _ui->buttonsDialog;

	if (setFloating) {
		if (!box->button(QDialogButtonBox::Close))
			box->addButton(QDialogButtonBox::Close);

		box->setCenterButtons(false);
	} else {
		box->removeButton( box->button(QDialogButtonBox::Close) );
		box->setCenterButtons(true);
	}
}

void ProcessorWidget::readPatternHistory(QStringList * history, int type) const {
	QSettings settings;

	if (type == 0)
		settings.beginGroup("Patterns/Resolve");
	else
		settings.beginGroup("Patterns/Rename");

	QStringList keys = settings.childKeys();

	if (!keys.isEmpty()) {
		foreach (QString k, keys)
			history->append(settings.value(k).toString());
	} else {
		history->append("%a - %t");
		history->append("%b/%n - %t");
		history->append("%n. %a - %t");
		history->append("%n - %t");
	}
}

void ProcessorWidget::savePatternHistory(const QStringList & history, int type) const {
	const int maxItems = 10;

	QSettings settings;

	if (type == 0) {
		settings.remove("Patterns/Resolve");
		settings.beginGroup("Patterns/Resolve");
	} else {
		settings.remove("Patterns/Rename");
		settings.beginGroup("Patterns/Rename");
	}

	for (int i = 0; i < qMin(history.size(), maxItems); i++)
		settings.setValue(QString("Pattern%1").arg(i), history.at(i));

	settings.endGroup();
}





void ProcessorWidget::apply() {
	if (!_model)
		return;

	int action = _ui->comboPage->currentIndex();

	foreach (QModelIndex idx, _indexes) {
		switch (action) {
			case ResolveTagsFromFilenames: {
				const QString pattern = _ui->comboResolvePattern->currentText();
				const QString filename = idx.sibling(idx.row(), modelColumn("Path")).data(Qt::EditRole).toString();
				const MetaData newData = tagsFromFileName(pattern, filename);

				foreach (const QString k, newData.keys()) {
					int col = modelColumn(k);

					if (col == -1)
						continue;

					_model->setData( idx.sibling(idx.row(), col), newData.value(k), Qt::EditRole );
				}

				// Move the selected pattern to top of the list and save patterns.
				if (!pattern.isEmpty()) {
					_resolveHistory.removeOne(pattern);
					_resolveHistory.insert(0, pattern);
					savePatternHistory(_resolveHistory, 0);
				}

				break;
			}

			case RenameFilesAndFolders: {
				// MetaData metaData = _model->metaData(idx);
				MetaData metaData = this->metaData(idx);
				const QString pattern = _ui->comboRenamePattern->currentText();
				const QString fileName = fileNameForPattern(pattern, metaData);



				_model->setData( idx.sibling(idx.row(), modelColumn("Path")), fileName, Qt::EditRole );

				// Move the selected pattern to top of the list and save patterns.
				if (!pattern.isEmpty()) {
					_renameHistory.removeOne(pattern);
					_renameHistory.insert(0, pattern);
					savePatternHistory(_renameHistory, 1);
				}

				break;
			}

			case ProcessFields:
				processFields();
				break;
		}
	}
}

void ProcessorWidget::updateRenamePreview() {
	if (_indexes.isEmpty()) {
		_ui->labelRenamePreview->setText(tr("(Preview)"));
		return;
	}

	const QString pattern = _ui->comboRenamePattern->currentText();
	const QStringList syms = extractSymbols(pattern);
	// const MetaData data = _model->metaData(_indexes[0].row());
	const MetaData data = metaData(_indexes[0]);

	const QString path = fileNameForPattern(pattern, data, false);

	_ui->labelRenamePreview->setText(path);
}

MetaData ProcessorWidget::metaData(const QModelIndex & idx) const {
	MetaData data;
	MetaDataModel * mdm = MetaDataModel::instance();

	for (int i = 0; i < mdm->columnCount(); i++) {
		data.insert( mdm->columnName(i), idx.sibling(idx.row(), i).data(Qt::EditRole) );
	}

	return data;
}

void ProcessorWidget::updateResolvePreview() {
	if (_indexes.isEmpty()) {
		_ui->labelResolvePreview->setText(tr("(Preview)"));
		return;
	}

	const QString pattern = _ui->comboResolvePattern->currentText();
	const QString path = _indexes[0].sibling(_indexes[0].row(), modelColumn("Path")).data(Qt::EditRole).toString();
	const QStringList syms = extractSymbols(pattern);
	const MetaData data = tagsFromFileName(pattern, path);

	QString text;

	foreach (QString s, syms)
		if (s != "%i")
			text += QString("<b>%1:</b> %2; ").arg(s, data.value(_symbols.value(s), "-").toString());

	text.chop(2);

	_ui->labelResolvePreview->setText(text);
}




MetaData ProcessorWidget::tagsFromFileName(const QString & pattern, const QString & fileName) const {
	MetaData data;

	int depth = QDir::fromNativeSeparators(pattern).count('/');

	QFileInfo info(fileName);

	// Convert path to have forward slashes as directory separators.
	QString fname = QDir::fromNativeSeparators(fileName);

	fname = fname.section('/', -(depth+1));
	fname = fname.left( fname.length() - info.suffix().length()-1 );

	const QStringList nums = QStringList() << "Disc" << "MaxNumber" << "Number" << "Year";
	const QStringList syms = extractSymbols(pattern);
	QString p = QRegExp::escape(pattern);

	foreach (QString s, syms) {
		QString t = nums.contains(_symbols.value(s)) ? "(\\d+)" : "(.+)";
		p.replace(s, t);
	}

	p.prepend('^');

	QRegExp rx(p);
	rx.indexIn(fname);

	for (int i = 0; i < syms.count(); i++) {
		QString v = rx.cap(i+1);

		// Strip leading zeros from numbers. They wouldn't be saved to tags anyways.
		if (nums.contains(_symbols.value(syms[i])))
			v = QString::number(v.toInt());

		data.insert(_symbols.value(syms.at(i)), v);
	}

	return data;
}

QString ProcessorWidget::fileNameForPattern(const QString & pattern, const MetaData & metaData, bool fullPath) const {
	QStringList syms = extractSymbols(pattern, true);

	const QString suffix = metaData.value("Path").toString().section('.', -1);
	QString path = metaData.value("Path").toString().section('/', 0, -2);

	if (path.isEmpty())
		path = ".";

	QString fileName = pattern;

	foreach (QString s, syms)
		fileName.replace(s, metaData.value(_symbols.value(s)).toString());

	// "Extended symbols" that will pad numbers with zeros to defined length.
	fileName.replace("%N", metaData.value("Number").toString().rightJustified(Coquillo::numberPadWidth, '0'));
	fileName.replace("%D", metaData.value("Disc").toString().rightJustified(Coquillo::numberPadWidth, '0'));
	fileName.replace("%M", metaData.value("MaxNumber").toString().rightJustified(Coquillo::numberPadWidth, '0'));

	fileName += metaData.value("Path").toString().right(suffix.length()+1);

	if (fullPath && pattern[0] != '/')
		fileName.prepend(path + "/");

	return fileName;
}

QStringList ProcessorWidget::extractSymbols(const QString & pattern, bool unique) const {
	QStringList syms;
	QRegExp rx("(%[abcdgmnoptyi])");

	int pos = 0;

	while ((pos = rx.indexIn(pattern, pos)) != -1) {
		syms << rx.cap(1);
		pos += rx.matchedLength();
	}

	if (unique)
		syms.removeDuplicates();

	return syms;
}

void ProcessorWidget::processFields() const {

	bool convertChars = _ui->convertCharacters->isChecked();
	bool stringMode = _ui->stringReplaceMode->isChecked();
	int casing = _ui->groupCapitalization->checkedId();

	foreach (const QModelIndex & idx, _indexes) {
		// MetaData metaData = _model->metaData(idx);
		MetaData metaData = this->metaData(idx);

		foreach (QAbstractButton * b, _ui->groupButtons->buttons()) {
			if (!b->isChecked())
				continue;

			const QString field = b->objectName().remove(QRegExp("^Button"));
			QString value = metaData.value(field).toString();

			switch (casing) {
				// Capitalize each word
				case 1: {
					bool hyphenated = _ui->capitalizeHyphenated->isChecked();

					QStringList words = value.split(QRegExp("\\s+"));

					for (QStringList::iterator i = words.begin(); i != words.end(); i++) {
						// Capitalize only if intelligent disabled OR word not in ignore list OR
						// word length > 1 and word is not 'I'.
						if ((Coquillo::intelligentCapitalization && Coquillo::ignoreWords.contains(*i)) ||
							(i->length() == 1 && i->toLower() != "i"))
							continue;

						i->replace(0, 1, i->at(0).toUpper());
					}

					value = words.join(" ");

					if (hyphenated) {
						QStringList words = value.split("-");

						for (QStringList::iterator i = words.begin(); i != words.end(); i++)
							i->replace(0, 1, i->at(0).toUpper());

						value = words.join("-");
					}

					break;
				}

				// Capitalize only the first letter of first word
				case 2:
					value = value.toLower();
					value.replace(0, 1, value[0].toUpper());
					break;

				// Lower-case everything
				case 3:
					value = value.toLower();
					break;
			};

			if (convertChars) {
				QString from = _ui->charactersFrom->text();
				QString to = _ui->charactersTo->text();

				if (stringMode)
					value.replace(from, to);
				else {
					for (int i = 0; i < from.length(); i++) {
						if (i < to.length())
							value.replace(from.at(i), to.at(i));
						else
							value.remove(from.at(i));
					}
				}
			}

			_model->setData( idx.sibling(idx.row(), modelColumn(field)), value );
		}
	}
}
