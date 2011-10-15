#ifndef METADATAMODEL_H
#define METADATAMODEL_H

#include <QStandardItemModel>

#include "MetaDataImage.h"

#include "../globals.h"

class QThread;

class MediaScanner;

int modelColumn(const QString & name);

class MetaDataModel: public QStandardItemModel {
	Q_OBJECT

	public:
		static MetaDataModel * instance();
	
		enum Roles {
			OriginalDataRole = Qt::UserRole+1, RowModifiedRole,
			PictureTypeRole, PictureDescriptionRole
		};
		
		MetaDataModel(QObject * parent=0);

		QString columnName(int i) const { return _columnNames.value(i); }
		int column(const QString & name) const { return _columnNames.key(name); }

		QVariant data(const QModelIndex & idx, int role=Qt::DisplayRole) const;

		bool setData(const QModelIndex & idx,
			const QVariant & value, int role=Qt::EditRole);

		int pictureCount(const QModelIndex & idx);
		
		QList<MetaDataImage> pictures(const QModelIndex & idx) const;
		bool addPicture(const QModelIndex & idx, const QVariantMap & data);
		bool removePicture(const QModelIndex & idx, int pos);

		// Edit picture that is found in idx.
		bool editPicture(const QModelIndex & idx,
			const QVariant & value, int role=PictureDescriptionRole);

		// Edit picture on row 'pos' of 'parent' (track).
		bool editPicture(const QModelIndex & parent, int pos,
			const QVariant & value, int role=PictureDescriptionRole);

	signals:
		void metaDataStateChanged(bool isModified);
		
	public slots:
		void setDirectory(const QString & dir) { _directory = dir; }
		void setRecursive(bool state) { _recurse = state;}

		void scan(const QString & path=QString());

		void saveChanges();
		void undoChanges();


	private slots:
		void addItem(const MetaData &);
	

	private:
		static MetaDataModel * s_instance;
		
		QMap<int, QString> _columnNames;
		QList<MetaData> _metaData;

		QString _directory;

		// Recursive scan.
		bool _recurse;

		// If enabled, will dim bg color for subdirs.
		bool _showDepth;

		MediaScanner * _scanner;
		QThread * _utilThread;
};

#endif
