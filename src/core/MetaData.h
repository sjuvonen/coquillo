#ifndef METADATA_H
#define METADATA_H

#include <QHash>
#include <QVariant>

#include "MetaDataImage.h"

class MetaData {
	public:

		/**
		 * Field codes
		 *
		 * TitleField has to be first and PicturesField last, because it is being
		 * relied on around the code! The indexing also needs to be defragmented!
		 **/
		enum Field {
			TitleField, ArtistField, AlbumField, LengthField, YearField, DiscNumberField,
			NumberField, MaxNumberField, GenreField, CommentField, OriginalArtistField,
			ComposerField, UrlField, EncoderField, PathField, PicturesField
		};

		MetaData() : _pictureCount(0) { }

		QVariant get(Field field) const { return _data.value(field); }
		void set(Field field, const QVariant & value);

		bool null() const;
		bool has(Field field) const { return _data.contains(field); }
		bool valid() const { return has(MetaData::PathField); }

		QList<MetaData::Field> fields() const { return _data.keys(); }

		int imageCount() const { return _images.count(); }

		MetaDataImage image(int index) const { return _images.value(index); }
		QList<MetaDataImage> images() const { return _images; }

		void addImage(const MetaDataImage & image);
		void setImages(const QList<MetaDataImage> & images) { _images.clear(); _images = images; }

		void removeImage(int index);

	private:
		QHash<Field, QVariant> _data;
		QList<MetaDataImage> _images;

		int _pictureCount;
};

Q_DECLARE_METATYPE(MetaData)

#endif
