#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QStringList>
#include <QMap>

namespace Coquillo {
	/**
	 * Application version and release date. Defined in main.cpp.
	 **/
	extern const QString releaseDate;
	extern const QString appVersion;

	/**
	 * RowModifiedRole indicates whether the item (some cell of the row) has been
	 * modified. Only available for column 0 in the model.
	 **/
	enum MetaDataRoles { OriginalDataRole = Qt::UserRole+1, RowModifiedRole };

	enum FieldEncoding { UTF_8=1, ISO_8859_15 };

	extern int encoding;

	/**
	 * Indicates how long numbers will be padded with leading zeros
	 **/
	extern int numberPadWidth;

	/**
	 * 0: Don't touch; 1: Lower case; 2: Upper case
	 **/
	extern int fileExtensionCase;

	extern bool intelligentCapitalization;

	/**
	 * FAT-compatible filenames
	 **/
	extern bool safeFileNames;

	/**
	 * Truncate extra whitespace
	 **/
	extern bool trimWhiteSpace;

	/**
	 * Search for strings "(Disc %d)" and "(CD %d)" and removes them from album names
	 **/
	extern bool removeDiscFromAlbumName;

	/**
	 * If enabled, the tracks table will show darker bg for tracks in subdirs.
	 **/
	extern bool dimSubdirectoriesBackground;

	namespace Flac {
		/**
		 * Enable ID3v2 tags with FLAC files.
		 **/
		extern bool id3v2;
	}

	namespace Mpeg {
		/**
		 * Enable ID3v1 tags with MP3s and such.
		 **/
		extern bool id3v1;
	}

	namespace OggVorbis {
		/**
		 * Remove coverart that is in the legacy format in Ogg/Vorbis files.
		 **/
		extern bool stripLegacyCovers;
	}

	/**
	 * List of words to not capitalize when intelligent capitalization enabled.
	 **/
	extern QStringList ignoreWords;

	/**
	 * Directory currently under edit
	 **/
	extern QString currentDirectory;
};

/**
 * Convert a C string to QString
 **/
QString toQString(const char * str, int encoding=-1);

#endif
