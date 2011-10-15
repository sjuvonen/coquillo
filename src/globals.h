#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QStringList>
#include <QMap>

#include <QVariantMap>

typedef QVariantMap MetaData;

namespace Coquillo {
	// These two are defined in main.cpp
	extern const QString releaseDate;
	extern const QString appVersion;

	// RowModifiedRole indicates whether the item (some cell of the row) has been modified. Only checked for column 0.
	enum MetaDataRoles { OriginalDataRole = Qt::UserRole+1, RowModifiedRole };

	enum FieldEncoding { UTF_8=1, ISO_8859_15 };
	extern int encoding;

	// Indicates how long numbers will be padded with leading zeros
	extern int numberPadWidth;

	// 0: Don't touch; 1: Lower case; 2: Upper case
	extern int fileExtensionCase;

	extern bool intelligentCapitalization;

	// FAT-compatible filenames
	extern bool safeFileNames;

	// Truncate extra whitespace
	extern bool trimWhiteSpace;

	// Search for strings "(Disc %d)" and "(CD %d)" and removes them from album names
	extern bool removeDiscFromAlbumName;

	// If enabled, the tracks table will show darker bg for tracks in subdirs.
	extern bool dimSubdirectoriesBackground;

	namespace Flac {
		extern bool id3v2;
	}

	namespace Mpeg {
		extern bool id3v1;
	}

	namespace OggVorbis {
		extern bool stripLegacyCovers;
	}

	// Map of field names to columns for the item model.
	extern QMap<int, QString> fieldNames;

	// List of words to not capitalize when intelligent capitalization enabled.
	extern QStringList ignoreWords;
};

QString toQString(const char * str, int encoding=-1);
int modelColumn(const QString & name);

#endif
