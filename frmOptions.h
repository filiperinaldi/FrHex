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

#ifndef FRMOPTIONS_H
#define FRMOPTIONS_H

#include <QtGui/QDialog>
#include <ui_frmOptions.h>
#include <frhexEditor.h>

class FrmOptions : public QDialog
{
    Q_OBJECT;

private:
    Ui::FrmOptions_ui ui;

public:
    FrmOptions(QWidget *parent, unsigned int columns, unsigned int dataSize, Endianness endianness);
    ~FrmOptions();
	unsigned int getColumns(void);
	unsigned int getDataSize(void);
	Endianness getEndianness(void);
};

#endif // FRMOPTIONS_H
