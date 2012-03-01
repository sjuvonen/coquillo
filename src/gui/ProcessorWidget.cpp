
#include <QCompleter>
#include <QDir>
#include <QFileInfo>
#include <QKeyEvent>
#include <QSettings>

#include "ProcessorWidget.h"
#include "ui_ProcessorWidget.h"

ProcessorWidget::ProcessorWidget(QWidget * parent)
: DataWidget(parent) {

	_ui = new Ui::ProcessorWidget();
	_ui->setupUi(this);

	_ui->renamePattern->installEventFilter(this);
	_ui->resolvePattern->installEventFilter(this);

	_symbols.insert("%a", MetaData::ArtistField);
	_symbols.insert("%b", MetaData::AlbumField);
	_symbols.insert("%c", MetaData::CommentField);
	_symbols.insert("%d", MetaData::DiscNumberField);
	_symbols.insert("%m", MetaData::MaxNumberField);
	_symbols.insert("%n", MetaData::NumberField);
	_symbols.insert("%o", MetaData::OriginalArtistField);
	_symbols.insert("%p", MetaData::ComposerField);
	_symbols.insert("%t", MetaData::TitleField);
	_symbols.insert("%y", MetaData::YearField);
	_symbols.insert("%i", -1);

	readPatternHistory(&_renameHistory, RenameHistory);
	readPatternHistory(&_resolveHistory, ResolveHistory);

	setHistory(_renameHistory, _ui->renamePattern);
	setHistory(_resolveHistory, _ui->resolvePattern);

	QSettings s;

	if (s.contains("Widgets/ProcessorSize")) {
		resize(s.value("Widgets/ProcessorSize").toSize());
		move(s.value("Widgets/ProcessorPosition").toPoint());
	}

	int i = 0;
	
	foreach (QAbstractButton * b, _ui->groupCapitalization->buttons())
		_ui->groupCapitalization->setId(b, i++);
}

ProcessorWidget::~ProcessorWidget() {
	delete _ui;
}

bool ProcessorWidget::eventFilter(QObject * object, QEvent * event) {
	if (object == _ui->renamePattern || object == _ui->resolvePattern) {
		if (event->type() == QEvent::KeyRelease) {
			int key = dynamic_cast<QKeyEvent*>(event)->key();
			if (key == Qt::Key_Return || key == Qt::Key_Enter)
				apply();
		}
			
		
	}

	return false;
}





/**
 * PUBLIC SLOTS
 **/

void ProcessorWidget::setSelection(const QItemSelection & selection) {
	DataWidget::setSelection(selection);

	updatePreviews();
}





/**
 * PROTECTED
 **/

void ProcessorWidget::hideEvent(QHideEvent * event) {
	savePatternHistory(_renameHistory, RenameHistory);
	savePatternHistory(_resolveHistory, ResolveHistory);
	saveSettings();
	
	QWidget::hideEvent(event);
}

void ProcessorWidget::keyReleaseEvent(QKeyEvent * event) {
	if (event->key() == Qt::Key_Escape)
		close();
}





/**
 * PRIVATE SLOTS
 **/

void ProcessorWidget::apply() {
	if (!model() || rows().isEmpty())
		return;

	const int action = _ui->tabs->currentIndex();

	foreach (const QModelIndex idx, rows()) {
		switch (action) {

			case RenameFiles: {
				const QString pattern = _ui->renamePattern->currentText();
				const QString fileName = pathForPattern(pattern, idx.row());

				model()->setData(idx.sibling(idx.row(), MetaData::PathField), fileName);

				// Move the selected pattern to top of the list and save patterns.
				if (!pattern.isEmpty()) {
					_renameHistory.removeOne(pattern);
					_renameHistory.insert(0, pattern);
					savePatternHistory(_renameHistory, RenameHistory);
				}

				break;
			}
			
			case ResolveTags: {
				const QString pattern = _ui->resolvePattern->currentText();
				const MetaData newData = tagsForPattern(pattern, idx.row());

				for (int i = MetaData::TitleField; i <= MetaData::PicturesField; i++) {
					if (i == MetaData::PicturesField || !newData.has((MetaData::Field)i))
						continue;

					model()->setData(idx.sibling(idx.row(), i), newData.get((MetaData::Field)i));
				}

				// Move the selected pattern to top of the list and save patterns.
				if (!pattern.isEmpty()) {
					_resolveHistory.removeOne(pattern);
					_resolveHistory.insert(0, pattern);
					savePatternHistory(_resolveHistory, ResolveHistory);
				}

				break;
			}

			case ProcessFields:
				processFields();

				break;
		}
	}
	
}

void ProcessorWidget::updatePreviews() {
	updateRenamePreview();
	updateResolvePreview();
}





/**
 * PRIVATE
 **/

QStringList ProcessorWidget::extractSymbols(const QString & pattern, bool unique) const {
	QStringList syms;
	QRegExp rx("(%[abcdgmnoptyi])", Qt::CaseInsensitive);

	int pos = 0;

	while ((pos = rx.indexIn(pattern, pos)) != -1) {
		syms << rx.cap(1);
		pos += rx.matchedLength();
	}

	if (unique)
		syms.removeDuplicates();

	return syms;
}

QString ProcessorWidget::pathForPattern(const QString & pattern, int row, bool fullPath) const {
	if (!model())
		return QString();
	
	QString path = model()->index(row, MetaData::PathField).data(Qt::EditRole).toString();
	QString fileName = QString("%1.%2").arg(pattern, path.section('.', -1));
	
	path = path.section('/', 0, -2);

	QStringList syms = extractSymbols(pattern, true);

	const int padWidth = QSettings().value("PadFileNumbers").toInt();
	const int substrLength = QSettings().value("SubstringPatternLength").toInt();

	foreach (QString s, syms) {
		if (s.at(1).isUpper()) {
			const int field = _symbols.value(s.toLower());
			const QString value = model()->index(row, field).data().toString();
			
			if (s.contains(QRegExp("[DNM]"))) {
				// Numbers padded with zeros
				fileName.replace(s, value.rightJustified(padWidth, '0'));
			} else {
				// Substring of string fields
				fileName.replace(s, value.left(substrLength));
			}
		} else {
			// Simply replace with full field value
			fileName.replace(s, model()->index(row, _symbols.value(s)).data(Qt::EditRole).toString());
		}
	}

	if (fullPath && pattern[0] != '/' && !path.isEmpty())
		fileName.prepend(path + "/");

	return fileName;
}

MetaData ProcessorWidget::tagsForPattern(QString pattern, int row) const {
	if (!model())
		return MetaData();

	int depth = QDir::fromNativeSeparators(pattern).count('/');

	QFileInfo info(model()->index(row, MetaData::PathField).data(Qt::EditRole).toString());
	QString fileName = QDir::fromNativeSeparators(info.absoluteFilePath());

	fileName = fileName.section('/', -(depth+1));
	fileName = fileName.left( fileName.length() - info.suffix().length()-1 );

	const QList<int> nums = QList<int>() << MetaData::DiscNumberField << MetaData::MaxNumberField
		<< MetaData::NumberField << MetaData::YearField;
		
	const QStringList syms = extractSymbols(pattern);
	pattern = QRegExp::escape(pattern);

	foreach (QString s, syms)
		pattern.replace(s, nums.contains(_symbols.value(s)) ? "(\\d+)" : "(.+)");

	QRegExp rx( QString('^') + pattern );
	rx.indexIn(fileName);
	MetaData metaData;

	for (int i = 0; i < syms.count(); i++) {
		QString v = rx.cap(i+1);

		// Strip leading zeros from numbers. They wouldn't be saved to tags anyways.
		if (nums.contains(_symbols.value(syms[i])))
			v = QString::number(v.toInt());

		metaData.set((MetaData::Field)_symbols.value(syms.at(i)), v);
	}

	return metaData;
}

void ProcessorWidget::processFields() const {

	bool convertChars = _ui->convertCharacters->isChecked();
	bool stringMode = _ui->stringReplaceMode->isChecked();
	int casing = _ui->groupCapitalization->checkedId();

	const QStringList ignoreWords = QSettings().value("CapitalizeNever").toString().split(QRegExp(",\\s*"));
	const QStringList forceWords = QSettings().value("CapitalizeAlways").toString().split(QRegExp(",\\s*"));

	QMap<QString, MetaData::Field> fieldMap;

	fieldMap.insert("ButtonTitle", MetaData::TitleField);
	fieldMap.insert("ButtonArtist", MetaData::ArtistField);
	fieldMap.insert("ButtonAlbum", MetaData::AlbumField);
	fieldMap.insert("ButtonGenre", MetaData::GenreField);
	fieldMap.insert("ButtonComment", MetaData::CommentField);
// 	fieldMap.insert("ButtonUrl", MetaData::UrlField);
// 	fieldMap.insert("ButtonDiscNumber", MetaData::DiscNumberField);
// 	fieldMap.insert("ButtonNumber", MetaData::NumberField);
// 	fieldMap.insert("ButtonMaxNumber", MetaData::MaxNumberField);
// 	fieldMap.insert("ButtonYear", MetaData::YearField);
	fieldMap.insert("ButtonOriginalArtist", MetaData::OriginalArtistField);
	fieldMap.insert("ButtonComposer", MetaData::ComposerField);

	foreach (const QModelIndex & idx, rows()) {

		foreach (QAbstractButton * b, _ui->groupButtons->buttons()) {
			if (!b->isChecked())
				continue;

			if (!fieldMap.contains(b->objectName()))
				continue;
			
			MetaData::Field field = fieldMap.value(b->objectName());
			QString value = idx.sibling(idx.row(), field).data().toString();

			switch (casing) {
				// Capitalize each word
				case 1: {
					bool hyphenated = _ui->capitalizeHyphenated->isChecked();

					QStringList words = value.split(QRegExp("\\s+"));

					for (QStringList::iterator i = words.begin(); i != words.end(); i++) {
						if (ignoreWords.contains(*i, Qt::CaseInsensitive))
							continue;

						i->replace(0, 1, i->at(0).toUpper());

						if (hyphenated) {
							QStringList words = value.split("-");

							for (QStringList::iterator i = words.begin(); i != words.end(); i++)
								i->replace(0, 1, i->at(0).toUpper());

							value = words.join("-");
						}
					}

					value = words.join(" ");

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

			model()->setData( idx.sibling(idx.row(), field), value );
		}
	}
}

void ProcessorWidget::readPatternHistory(QStringList * list, int type) const {
	QSettings s;

	if (type == ResolveHistory)
		s.beginGroup("Patterns/Resolve");
	else
		s.beginGroup("Patterns/Rename");

	const QStringList keys = s.childKeys();

	if (!keys.isEmpty()) {
		foreach (const QString k, keys)
			list->append(s.value(k).toString());
	} else {
		if (type == ResolveHistory) {
			list->append("%a - %t");
			list->append("%b/%n - %t");
			list->append("%n. %a - %t");
			list->append("%n - %t");
		} else {
			list->append("%a - %t");
			list->append("%b/%N - %t");
			list->append("%N. %a - %t");
			list->append("%N - %t");
		}
	}
}

void ProcessorWidget::savePatternHistory(const QStringList & history, int type) const {
	const int maxItems = 10;

	QSettings s;

	if (type == ResolveHistory) {
		s.remove("Patterns/Resolve");
		s.beginGroup("Patterns/Resolve");
	} else {
		s.remove("Patterns/Rename");
		s.beginGroup("Patterns/Rename");
	}

	for (int i = 0; i < qMin(history.size(), maxItems); i++)
		s.setValue(QString("Pattern%1").arg(i), history.at(i));

	s.endGroup();
}

void ProcessorWidget::saveSettings() {
	QSettings s;

	s.setValue("Widgets/ProcessorSize", size());
	s.setValue("Widgets/ProcessorPosition", pos());
}

void ProcessorWidget::setHistory(const QStringList & list, QComboBox * widget) {
	QStringList patterns(list);

	patterns.sort();
	patterns.insert(0, QString());

	widget->completer()->setCaseSensitivity(Qt::CaseSensitive);
	widget->insertItems(0, patterns);
	widget->setEditText(QString());
}

void ProcessorWidget::updateRenamePreview() {
	if (!model() || rows().isEmpty() || _ui->renamePattern->currentText().isEmpty()) {
		_ui->renamePreview->setText(tr("(Preview)"));
		return;
	}

	const QString pattern = _ui->renamePattern->currentText();

	const QString path = pathForPattern(_ui->renamePattern->currentText(),
		rows().at(0).row(), false);

	_ui->renamePreview->setText(path);
}

void ProcessorWidget::updateResolvePreview() {
	if (!model() || rows().isEmpty() || _ui->resolvePattern->currentText().isEmpty()) {
		_ui->resolvePreview->setText(tr("(Preview)"));
		return;
	}

	const QString pattern = _ui->resolvePattern->currentText();
	const QStringList syms = extractSymbols(pattern);
	const MetaData data = tagsForPattern(pattern, rows().at(0).row());

	QString text;

	foreach (QString s, syms) {
		if (s == "%i")
			continue;

		QString t = data.get((MetaData::Field)_symbols.value(s)).toString();

		if (t.isEmpty())
			t = '-';
		
		text += QString("<b>%1:</b> %2; ").arg(s, t);
	}

	text.chop(2);

	_ui->resolvePreview->setText(text);
}
