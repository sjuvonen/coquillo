#ifndef PICTURESTAB_H
#define PICTURESTAB_H

#include <QMap>
#include <QWidget>

#include <metadata/MetaDataImage.h>

class QModelIndex;
class QStandardItemModel;

class MetaDataModel;

namespace Ui {
	class TagEditorPictures;
}

class PicturesTab : public QWidget {
	Q_OBJECT

	public:
		enum { PictureTypeRole = Qt::UserRole+1, PictureTypeStringRole, PictureDimensionsRole, PictureIdRole };

		PicturesTab(QWidget * parent=0);

		QList<MetaDataImage> pictures() const;

	signals:
		void applyPicturesToAll();
		void pictureAdded(const MetaDataImage &);
		void pictureChanged(const MetaDataImage &);
		void pictureRemoved(qint16 id);

	public slots:
		void clearPictures();
		void setPictures(const QList<MetaDataImage> & pics);

	private slots:
		void addPictures();
		void changePictureDescription(const QString & text);
		void changePictureType(int index);
		void changePictureType(const QString & customText);
		void deleteCurrentPicture();
		void savePicture();
		void selectedPictureChanged(const QModelIndex & idx);

	private:
		void addPictureToList(const MetaDataImage & img);

		Ui::TagEditorPictures * _ui;

		QStandardItemModel * _pictureModel;
		QMap<int, QString> _typeStrings;
};

#endif