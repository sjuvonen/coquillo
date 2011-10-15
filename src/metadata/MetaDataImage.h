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
		QString description() const;
		QString typeString() const;
		int type() const;
		qint16 id() const;

	private:
		qint16 _id;
		int _type;
		QString _typeString;
		QString _description;
};

Q_DECLARE_METATYPE(MetaDataImage)

#endif
