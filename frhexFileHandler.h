/*
 * Frhex - Hexadecimal Editor
 * Copyright (C) 2011 Filipe Rinaldi - filipe.rinaldi@gmail.com
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef FRHEXFILEHANDLER_H
#define FRHEXFILEHANDLER_H

#include <QString>
#include <QFile>

enum Endianness {
	DATA_BIG_ENDIAN,
	DATA_LITTLE_ENDIAN
};

namespace Frhex {

enum FileFormat {
	DATA_RAW
};

}

class FrhexFileHandler
{
public:
	FrhexFileHandler(const QString filename);
	~FrhexFileHandler();
	bool open(void);
	bool read(qint64 address, quint8 *value, int size, Endianness endianness);
	bool write(qint64 address, quint8 value);
	void save(void);
	void close(void);
	qint64 getSize(void);

private:
	QFile mFile;
	QString mName;
	qint64 mSize;
	Frhex::FileFormat mFormat;			
	bool mReadFromFile(quint8 *data, qint64 address, qint64 count);
};

#endif // FRHEXFILEHANDLER_H
