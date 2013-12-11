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

#ifndef FRHEXEDITOR_H
#define FRHEXEDITOR_H

#include <QWidget>
#include <QTableView>
#include <QString>
#include "frhexFileHandler.h"
#include "frhexModel.h"

class FrhexEditor : QObject
{
	Q_OBJECT

private:
	QTableView *mView;
	FrhexModel *mModel;
	FrhexFileHandler *mFileHandler;
	void mSetupColumns(void);
	void mSendAddressValue(qint64 address, quint8 value);

public:
	FrhexEditor(QObject *parent, QString filename, Endianness endianness=DATA_BIG_ENDIAN, int dataSize=1, int columns=16);
	~FrhexEditor();
	QWidget *getWidget(void);
	void setOptions(unsigned int columns, unsigned int dataSize, Endianness endianness);

	unsigned int getDataSize(void) {return mModel->getDataSize();};
	unsigned int getColumns(void) {return mModel->getColumns();};
	Endianness getEndianness(void) {return mModel->getEndianness();};
	QString getFilename(void);

signals:
	void selectionUpdated(qint64 address, quint8 value);
};


#endif // FRHEXEDITOR_H
