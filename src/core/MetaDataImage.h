#ifndef METADATAIMAGE_H
#define METADATAIMAGE_H

#include <QImage>
#include <QMetaType> // Needed for Q_DECLARE_METATYPE()
#include <QString>

class MetaDataImage {
	public:
		MetaDataImage();
		MetaDataImage(const QImage & image);
		MetaDataImage(const QImage & image, int type, const QString & description);
		MetaDataImage(qint16 id, int type, const QString & description);
		MetaDataImage(qint16 id, const QString & typeString, const QString & description);
		MetaDataImage(const QImage & image, const QString & typeString, const QString & description);

		int width() const { return image().width(); }
		int height() const { return image().height(); }
		QSize size() const { return image().size(); }

		QImage image() const;

		void setDescription(const QString & d) { _description = d; }
		QString description() const { return _description; }

		void setTypeString(const QString & t) { _typeString = t; _type = -1; }
		QString typeString() const { return _typeString; }

		void setType(int t) { _type = t; }
		int type() const { return _type; }

		qint16 id() const { return _id; }

	private:
		qint16 _id;
		int _type;
		QString _typeString;
		QString _description;
};

Q_DECLARE_METATYPE(MetaDataImage)

#endif
