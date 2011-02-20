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

#ifndef FRHEXMODEL_H
#define FRHEXMODEL_H

#include <QAbstractTableModel>
#include <QTableView>
#include "frhexFileHandler.h"

struct GridLayout {
	int addressColumn;
	int addressFieldSize;
	int separator1Column;
	int separator1FieldSize;
	int dataHexaColumn;
	int dataHexaFieldSize;
	int dataHexaFieldCount;
	int separator2Column;
	int separator2FieldSize;
	int dataCharColumn;
	int dataCharFieldSize;
	int dataCharFieldCount;
	int totalColumns;
};

enum ColumnType {
	COL_ADDRESS,
	COL_SEPARATOR,
	COL_DATA_HEX,
	COL_DATA_CHAR,
};

class FrhexModel : public QAbstractTableModel
{
	Q_OBJECT

private:
	FrhexFileHandler *mFile;
	Endianness mEndianness;
	unsigned int mDataSize;
	unsigned int mColumns;
	struct GridLayout mGridLayout;	
	QSet<qint64> mChangedValues;
	QVariant mGetData(const QModelIndex & index) const;
	QVariant mGetBackground(const QModelIndex & index) const;
	QVariant mGetForeground(const QModelIndex & index) const;
	ColumnType mGetColumType(int column) const;
	qint64 mDataOffset(const QModelIndex & index) const;
	void mSetupGridLayout(void);

public:
	FrhexModel(QObject *parent, FrhexFileHandler *file);
	~FrhexModel();
	struct GridLayout *getGridLayout(void);
	int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
	int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	QVariant headerData (int section, Qt::Orientation orientation, int role) const;
	Qt::ItemFlags flags ( const QModelIndex & index ) const;
	bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

	unsigned int getColumns(void) {return mColumns;};
	unsigned int getDataSize(void) {return mDataSize;};
	Endianness getEndianness(void) {return mEndianness;};

	void setOptions(unsigned int columns, unsigned int dataSize, Endianness endianness);

signals:
	void cursorChanged(qint64 address, unsigned char value);
};

#endif // FRHEXMODEL_H
