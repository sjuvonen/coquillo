#ifndef METADATAIMAGE_H
#define METADATAIMAGE_H

#include <QHash>
#include <QImage>
#include <QString>

class MetaDataImage {

	public:
		enum Roles {
			ImageRole = Qt::DecorationRole,
			DescriptionRole = Qt::DisplayRole,
			MetaTypeRole = Qt::UserRole+32
		};

		enum Fields { ImageField, DescriptionField, TypeField };
		
		MetaDataImage();
		MetaDataImage(const QImage & image);
		MetaDataImage(const QImage & image, int type, const QString & description=QString());

		qint16 id() const { return _id; }

		void setImage(const QImage & image);
		QImage image() const;
		QImage small() const;

		bool null() const;

		void setDescription(const QString & descr) { _description = descr; }
		QString description() const { return _description; }

		void setType(int type) { _type = type; }
		int type() const { return _type; }

	private:
		QString _description;

		int _type;
		qint16 _id;

		static QHash<qint16, QImage> s_cache;
		static QHash<qint16, QImage> s_small;
};

#endif