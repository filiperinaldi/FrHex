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

#include "frhexModel.h"
#include "frhexFileHandler.h"

FrhexModel::FrhexModel(QObject *parent, 
					   FrhexFileHandler *file, 
					   Endianness endianness,
					   int dataSize,
					   int columns)
		: QAbstractTableModel(parent)
{
	mFile = file;
	mEndianness = endianness;
	mDataSize = dataSize;
	mColumns = columns;
	
	mSetupGridLayout();
}

FrhexModel::~FrhexModel()
{
}

struct GridLayout *FrhexModel::getGridLayout(void)
{
	return &mGridLayout;
}

int FrhexModel::rowCount ( const QModelIndex & /*parent*/ ) const
{
	return (mFile->getSize() / mGridLayout.dataCharFieldCount)+1;
}

int FrhexModel::columnCount ( const QModelIndex & /*parent*/ ) const
{
	
	return mGridLayout.totalColumns;
}

void FrhexModel::mSetupGridLayout(void)
{
	/*
	 *  Setup Grid Layout
	 */
	mGridLayout.addressColumn = 0;
	mGridLayout.addressFieldSize = 100;
	
	mGridLayout.separator1Column = 1;
	mGridLayout.separator1FieldSize = 10;
	
	mGridLayout.dataHexaColumn = 2;
	mGridLayout.dataHexaFieldSize = mDataSize * 25;
	mGridLayout.dataHexaFieldCount = mColumns / mDataSize;
	
	mGridLayout.separator2Column = mGridLayout.dataHexaColumn +\
								   mGridLayout.dataHexaFieldCount;
	mGridLayout.separator2FieldSize = 10;
	
	mGridLayout.dataCharColumn = mGridLayout.separator2Column + 1;
	mGridLayout.dataCharFieldSize = 13;
	mGridLayout.dataCharFieldCount = mColumns;
	
	mGridLayout.totalColumns = mGridLayout.dataCharColumn +\
							   mGridLayout.dataCharFieldCount;
	
}

QVariant FrhexModel::mGetData(const QModelIndex & index) const
{
	quint8 value[4];
	qint64 address;
	
	switch (mGetColumType(index.column()))
	{
	case COL_ADDRESS:
		{
			return QString("%1").arg((index.row() * mGridLayout.dataCharFieldCount), 0, 16).rightJustified(10,'0').toUpper();
		}
	case COL_DATA_HEX:
		{
			QString strValue;
			int i;			
			
			address = mDataOffset(index);			
			
			Q_ASSERT_X(address != -1,__FUNCTION__, "Oops, unexpected value for a valid column!");
			
			if (address >= mFile->getSize())
				return QVariant();
			
			mFile->read(address, value, mDataSize, mEndianness);
			
			for(i=0; i<mDataSize; i++)
				strValue.append( QString("%1").arg(static_cast<unsigned char>(value[i]), 0, 16).rightJustified(2,'0').toUpper());
			
			return strValue;
		}
	case COL_DATA_CHAR:
		{
			address = mDataOffset(index);
			
			Q_ASSERT_X(address != -1,__FUNCTION__, "Oops, unexpected value for a valid column!");
			if (address > mFile->getSize())
				return QVariant();
			mFile->read(address, value, 1, mEndianness);
			QChar data(value[0]);
			if (data.isPrint())
				return data;
			else
				return QChar('.');
		}
	default:
		return QVariant();
	}
}

QVariant FrhexModel::mGetBackground(const QModelIndex & index) const
{
	if (mGetColumType(index.column()) == COL_SEPARATOR)
		return QColor(Qt::white);

	return QVariant();
}

QVariant FrhexModel::mGetForeground(const QModelIndex & index) const
{
	qint64 dataOffset = mDataOffset(index);
	if ( dataOffset != -1)
	{
		if (mChangedValues.find(dataOffset) != mChangedValues.end())
			return QColor(Qt::red);
	}

	return QVariant();
}

ColumnType FrhexModel::mGetColumType(int column) const
{
	ColumnType columnType;
	
	/* Data in Hex */
	if ( (column >= mGridLayout.dataHexaColumn) &&
		 (column < (mGridLayout.dataHexaColumn+mGridLayout.dataHexaFieldCount)))
		columnType = COL_DATA_HEX;

	/* Data in char */
	else if ( (column >= mGridLayout.dataCharColumn) &&
		 (column < (mGridLayout.dataCharColumn+mGridLayout.dataCharFieldCount)))
		columnType = COL_DATA_CHAR;

	/* Separators */
	else if ( (column == mGridLayout.separator1Column) ||
		 (column == mGridLayout.separator2Column))
		columnType = COL_SEPARATOR;

	/* Address/Offset */
	else if ( column == mGridLayout.addressColumn)
		columnType = COL_ADDRESS;
	
	else {
		exit(1);
	}
	
	return columnType;
}

qint64 FrhexModel::mDataOffset(const QModelIndex & index) const
{
	qint64 offset;

	switch (mGetColumType(index.column()))
	{
		case COL_DATA_HEX:
			offset = (index.row() * mGridLayout.dataHexaFieldCount + 
					 (index.column() - mGridLayout.dataHexaColumn) ) * mDataSize;
			break;
		case COL_DATA_CHAR:
			offset = index.row() * mGridLayout.dataCharFieldCount + 
					 (index.column() - mGridLayout.dataCharColumn);
			break;
		default:
			offset = -1;
	}
	
	return offset;
}

QVariant FrhexModel::data ( const QModelIndex & index, int role ) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	switch (role)
	{
		case Qt::DisplayRole:
		case Qt::EditRole:
			return mGetData(index);
		case Qt::BackgroundRole:
			return mGetBackground(index);
		case Qt::ForegroundRole:
			return mGetForeground(index);
		default:
			return QVariant();
	}
}

 QVariant FrhexModel::headerData(int section, Qt::Orientation orientation, int role) const
 {
	 if ((role != Qt::DisplayRole) ||
		 (orientation != Qt::Horizontal))
		 return QVariant();

	 /* Address/Offset */
	 if (mGetColumType(section) == COL_ADDRESS)
		 return QString("Offset");

	 /* Data in hexa */
	 if (mGetColumType(section) == COL_DATA_HEX)
		 return QString("%1").arg(section - mGridLayout.dataHexaColumn, 0, 16).toUpper();

	 return QVariant();
 }

Qt::ItemFlags FrhexModel::flags ( const QModelIndex & index ) const
{
	if ((mGetColumType(index.column()) == COL_SEPARATOR) ||
		mDataOffset(index) >= mFile->getSize())
		return Qt::NoItemFlags;

	if (mGetColumType(index.column()) == COL_ADDRESS)
		return Qt::ItemIsEnabled;

	return (Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsEnabled);
}

bool FrhexModel::setData ( const QModelIndex & index, const QVariant & value, int role)
{
	if (!index.isValid() && role != Qt::EditRole)
		return false;

	QString _value = value.toString();
	if (_value.isEmpty())
		return false;

	qint64 dataOffset;
	// -1 means an invalid column
	if ((dataOffset = mDataOffset(index)) == -1)
		return false;

	bool ok;
	int valueInt;

	if (mGetColumType(index.column()) == COL_DATA_HEX)
	{
		// Try to convert to a byte
		valueInt = _value.toInt(&ok, 10);
		if ( !ok )
		{
			// Try again as hexa
			valueInt = _value.toInt(&ok, 16);
			if (!ok)
				return false; // Too bad...
		}
		// Check size
		if (valueInt > 0xFF)
			return false;
	}
	else /* COL_DATA_CHAR) */
	{
		if (_value.size() != 1)
			return false;
		valueInt = _value.data()[0].toAscii();
	}

	mFile->write(dataOffset, valueInt);
	mChangedValues.insert(dataOffset);
	emit dataChanged(index, index);
	return true;
}
