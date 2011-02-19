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

#include <QMessageBox>
#include "frhexFileHandler.h"
#include "frhexModel.h"

FrhexFileHandler::FrhexFileHandler(const QString filename)
{
	mName = filename;
	mSize = 0;
}

FrhexFileHandler::~FrhexFileHandler()
{
}

bool FrhexFileHandler::open(void)
{
	mFile.setFileName(mName);

	if (!mFile.exists())
		return false;

	if (!mFile.open(QIODevice::ReadWrite))
		return false;

	mSize = mFile.size();

	return true;
}

bool FrhexFileHandler::read(qint64 address, quint8 *value, int size, Endianness endianness)
{
	int sizeToRead;
	
	if (address >= mSize)
		return false;
	
	memset(value, 0, 4);
	
	if ((mSize - address) < size)
		sizeToRead = mSize - address;
	else
		sizeToRead = size;
	
	if (!mReadFromFile(value, address, sizeToRead))
		return false;
	
	/* 
	 * Apply byte order
	 */
	if (endianness == DATA_LITTLE_ENDIAN) {
		quint8 tmp;
		
		switch(size) {
			case 2:
				tmp = value[0];
				value[0] = value[1];
				value[1] = tmp;
				break;
				
			case 4:
				tmp = value[0];
				value[0] = value[3];
				value[3] = tmp;
				tmp = value[1];
				value[1] = value[2];
				value[2] = tmp;
				break;
				
			default:
				return false;
		}		
	}
	
	return true;
}

bool FrhexFileHandler::write(qint64 address, quint8 value)
{
	if (address < mSize) {
		QMessageBox msgBox(QMessageBox::Critical, "Save to file", "Not implemented");
		msgBox.exec();
		return true;
	}
	else
		return false;
}

bool FrhexFileHandler::mReadFromFile(quint8 *data, qint64 address, qint64 count)
{
	qint64 totalDataRead = 0;
	
	if (!mFile.seek(address))
		return false;
	
	while( totalDataRead < count ) {
		qint64 dataRead = 0;

		dataRead = mFile.read( reinterpret_cast<char*>(&data[totalDataRead]), count - totalDataRead);
		if ( dataRead == -1 )
			return false;

		totalDataRead += dataRead;
	}
	Q_ASSERT_X(totalDataRead == count, __FUNCTION__, "Reading file failed.");
	return true;
}

qint64 FrhexFileHandler::getSize(void)
{
	return mSize;
}

