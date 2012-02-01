#ifndef METADATAMODEL_H
#define METADATAMODEL_H

#include <QStandardItemModel>

#include "MetaData.h"

class QThread;

class MediaScanner;

extern volatile bool abortAction;

class MetaDataModel : public QStandardItemModel {
	Q_OBJECT

	public:
		enum DataRole {
			MetaDataRole=Qt::UserRole+1,
			OriginalDataRole,
			ChangedStatusRole,
			OriginalImageRole,
		};
		
		MetaDataModel(QObject * parent=0);

		MetaData metaData(const QModelIndex & idx) const;
		MetaData metaData(int row) const;

		QList<MetaData> all() const;
		
		QString directory() const { return _directory; }
		bool recursive() const { return _recursive; }

		QVariant data(const QModelIndex & idx, int role=Qt::DisplayRole) const;
		bool setData(const QModelIndex & idx, const QVariant & value, int role=Qt::EditRole);

	signals:
		// These are emitted during a media scan or tag writing.
		void actionFinished();
		void actionMaximumChanged(int);
		void actionProgress(int);
		void actionStarted();

		/**
		 * Emitted when any item is modified. The boolean flag indicates if the model
		 * has any unsaved changes. (Will be false when restore() is called)
		 **/
		void metaDataStateChanged(bool modified);

	public slots:
		bool add(const MetaData & data);
		
		void setDirectory(const QString & dir) { _directory = dir; }
		void setRecursive(bool state) { _recursive = state; }

		void abortAction() { ::abortAction = true; }
		
		void readDirectory(const QString & dir);
		void reload();
		
		void save();
		void restore();

	private slots:
		void lock() { _locked = true; };
		void unlock() { _locked = false; }

	private:
		QString _directory;
		QMap<int, QString> _fieldNames;
		QList<int> _checked;

		QThread * _slave;
		
		bool _recursive;
		bool _locked;
};

#endif