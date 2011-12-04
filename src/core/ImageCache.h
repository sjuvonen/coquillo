#ifndef IMAGECACHE
#define IMAGECACHE

#include <QMap>
#include <QObject>
#include <QSize>

class QImage;

class ImageCache : public QObject {
	Q_OBJECT

	public:
		ImageCache(QObject * parent);

		static QImage image(qint16 id);
		static qint16 addImage(const QImage & image);
		static void setScaleSize(const QSize & size);
		static QSize scaleSize() { return QSize(_w, _h); }

	public slots:
		static void clear();

	private:
		static QMap<qint16, const QImage *> _images;
		static QMap<qint16, const QImage *> _scaled;

		static int _w;
		static int _h;
};

#endif
