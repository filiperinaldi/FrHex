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
#include <frmOpen.h>

FrmOpen::FrmOpen(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.btSelectFile, SIGNAL(clicked()), this, SLOT(selectFile()));
}

FrmOpen::~FrmOpen()
{
}

int FrmOpen::getDataSize(void)
{
	int dataSize = 0;
	
	if (ui.rb8bits->isChecked())
		dataSize = 1;
	else if (ui.rb16bits->isChecked())
		dataSize = 2;
	else if (ui.rb32bits->isChecked())
		dataSize = 4;
	else
		exit(1);
	
	return dataSize;
}

Endianness FrmOpen::getEndianness(void)
{
	Endianness endianness;
	
	if (ui.rbBigEndian->isChecked())
		endianness = DATA_BIG_ENDIAN;
	else if (ui.rbLittleEndian->isChecked())
		endianness = DATA_LITTLE_ENDIAN;
	else
		exit(1);
	
	return endianness;
}

QString FrmOpen::getFilename(void)
{
	return ui.edFilename->text();
}

void FrmOpen::selectFile(void)
{
	const QString fileName = QFileDialog::getOpenFileName(this,
					tr("Select file"),
					"./",
					tr("Binary files (*.hex *.bin *.raw);;All (*.*)"));
	
	ui.edFilename->setText(fileName);
}