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

#include <QFileDialog>
#include <frmOptions.h>

FrmOptions::FrmOptions(QWidget *parent, unsigned int columns, unsigned int dataSize, Endianness endianness)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.spColumns->setValue(columns);
	switch (dataSize) {
		case 1: ui.rb8bits->setChecked(true); break;
		case 2: ui.rb16bits->setChecked(true); break;
		case 4: ui.rb32bits->setChecked(true); break;
		default:
			qt_assert_x(__FUNCTION__, "Oops, Invalid data size", __FILE__, __LINE__);
	}
	switch(endianness) {
		case DATA_BIG_ENDIAN: ui.rbBigEndian->setChecked(true); break;
		case DATA_LITTLE_ENDIAN: ui.rbLittleEndian->setChecked(true); break;
		default:
			qt_assert_x(__FUNCTION__, "Oops, Invalid endianness", __FILE__, __LINE__);
	}
}

FrmOptions::~FrmOptions()
{
}

unsigned int FrmOptions::getDataSize(void)
{
	unsigned int dataSize = 0;
	
	if (ui.rb8bits->isChecked())
		dataSize = 1;
	else if (ui.rb16bits->isChecked())
		dataSize = 2;
	else if (ui.rb32bits->isChecked())
		dataSize = 4;
	else {
		qt_assert_x(__FUNCTION__, "Oops, Invalid data size", __FILE__, __LINE__);
	}

	return dataSize;
}

Endianness FrmOptions::getEndianness(void)
{
	Endianness endianness;
	
	if (ui.rbBigEndian->isChecked())
		endianness = DATA_BIG_ENDIAN;
	else if (ui.rbLittleEndian->isChecked())
		endianness = DATA_LITTLE_ENDIAN;
	else{
		qt_assert_x(__FUNCTION__, "Oops, Invalid endianness", __FILE__, __LINE__);
	}
	
	return endianness;
}

unsigned int FrmOptions::getColumns(void)
{
	return ui.spColumns->value();
}
