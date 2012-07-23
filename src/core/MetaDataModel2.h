#ifndef METADATAMODEL2_H
#define METADATAMODEL2_H

#include <QAbstractItemModel>
#include <QStringList>

#include "MetaData.h"

class QImage;
class QNetworkAccessManager;
class QMimeData;

class MediaScanner;

extern volatile bool abortAction;

class MetaDataModel2 : public QAbstractItemModel {
	Q_OBJECT

	public:
		enum DataRole { FieldTypeRole = Qt::UserRole + 1, FieldChangedRole, OriginalDataRole };
		enum ActionType { FindMediaAction, WriteTagsAction };

		MetaDataModel2(QObject * parent=0);

		void setNetworkManager(QNetworkAccessManager * manager);

		bool recursive() const { return _recursive; }
		QString directory() const { return _directory; }

		MetaData metaData(int row) const;

		bool addImage(int row, const MetaDataImage & image);
		bool addImage(const QModelIndex & idx, const MetaDataImage & image);
		void setImages(const QModelIndex & idx, const QList<MetaDataImage> & images);

		QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;

		QVariant data(const QModelIndex & idx, int role=Qt::DisplayRole) const;
		bool setData(const QModelIndex & idx, const QVariant & value, int role=Qt::EditRole);

		bool removeRows(int start, int count, const QModelIndex & parent=QModelIndex());

		QModelIndex index(int row, int column, const QModelIndex & parent=QModelIndex()) const;
		QModelIndex parent(const QModelIndex & idx) const;

		int columnCount(const QModelIndex & p=QModelIndex()) const;
		int rowCount(const QModelIndex & p=QModelIndex()) const;

		Qt::ItemFlags flags(const QModelIndex & idx) const;

		QStringList mimeTypes() const;

		bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row,
			int column, const QModelIndex & parent);

	signals:
		// These are emitted during a media scan or tag writing.
		void actionError(const QString & message);
		void actionFinished();
		void actionMaximumChanged(int total);
		void actionProgress(int progress);
		void actionStarted();

		/**
		 * Emitted when any item is modified. The boolean flag indicates if the model
		 * has any unsaved changes. (Will be false when restore() is called)
		 **/
		void metaDataStateChanged(bool modified);

	public slots:
		void add(MetaData data);
		void clear() { removeRows(0, rowCount()); }
		void readDirectory(const QString & dir);
		void reload();
		void restore();
		void save();

		void setDirectory(const QString & dir) { _directory = dir; }
		void setRecursive(bool state) { _recursive = state; }

		void abortAction() { ::abortAction = true; }

	private slots:
		void lock() { _locked = true; }
		void unlock() { _locked = false; }

		void networkActionProgressChanged(qint64 progress, qint64 total) {
			emit actionProgress(int(progress / 1000));
			emit actionMaximumChanged(int(total / 1000));
		}

	private:
		void backup(int row);
		void connectMappers(QObject * object, int actionType);

		QImage downloadImage(const QUrl & url);

		QList<MetaData> _data;
		QList<MetaData> _original;
		QMap<int, QString> _fields;

		QNetworkAccessManager * _networkManager;
		QString _directory;

		MediaScanner * _scanner;

		bool _recursive;
		bool _locked;

		volatile bool _abortAction;
};

#endif
