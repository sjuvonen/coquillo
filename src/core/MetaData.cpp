
#include "MetaData.h"

void MetaData::set(Field field, const QVariant & value) {
	// Don't allow setting images with this function!
	if (field >= MetaData::TitleField && field < MetaData::PicturesField)
		_data.insert(field, value);
}

bool MetaData::null() const {
	return (_data.isEmpty() && _images.isEmpty());
}

void MetaData::addImage(const MetaDataImage & image) {
	_images << image;
}

void MetaData::removeImage(int index) {
	_images.removeAt(index);
}