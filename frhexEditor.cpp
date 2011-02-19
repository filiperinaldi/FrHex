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

#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include "frhexEditor.h"

FrhexEditor::FrhexEditor(QObject *parent, QString filename, Endianness endianness, int dataSize, int columns)
		: QObject(parent)
{
	/* 
	 * Open file
	 */
	mFileHandler = new FrhexFileHandler(filename);
	if (!mFileHandler->open()) {
		QMessageBox msgBox(QMessageBox::Critical, "Open File", "Failed to open file");
		msgBox.exec();
		return;
	}

	mSetupView(0);
	mModel = new FrhexModel(this, mFileHandler, endianness, dataSize, columns);
	mView->setModel(mModel);
	mSetupColumns();
}

FrhexEditor::~FrhexEditor()
{
	delete mFileHandler;
	delete mView;
	delete mModel;
}

void FrhexEditor::mSetupView(QWidget *parent)
{
	QGridLayout *layout = new QGridLayout(parent);
	layout->setObjectName(QString::fromUtf8("layout"));

	mView = new QTableView(parent);
	mView->setObjectName(QString::fromUtf8("mView"));
	layout->addWidget(mView, 0, 0);

	mView->verticalHeader()->hide();
	mView->setShowGrid(false);
	mView->setAlternatingRowColors(true);
	mView->setSelectionBehavior(QAbstractItemView::SelectItems);
	mView->setSelectionMode(QAbstractItemView::ContiguousSelection);
}

void FrhexEditor::mSetupColumns(void)
{
	int i;
	struct GridLayout *gridLayout;

	gridLayout = mModel->getGridLayout();
		
	/*
	 * Setup View
	 */	
	// Address
	mView->setColumnWidth(gridLayout->addressColumn, 
						  gridLayout->addressFieldSize);	
	
	// Separator 1	
	mView->setColumnWidth(gridLayout->separator1Column,
						  gridLayout->separator1FieldSize);	
	
	// Data in hexa
	for(i=gridLayout->dataHexaColumn; i<gridLayout->separator2Column; i++)
		mView->setColumnWidth(i, gridLayout->dataHexaFieldSize);
	
	// Separator 2
	for (i=gridLayout->separator2Column; i<gridLayout->dataCharColumn; i++)
		mView->setColumnWidth(i, gridLayout->separator2FieldSize);
	
	// Data in char
	for(i=gridLayout->dataCharColumn; i<gridLayout->totalColumns; i++)
		mView->setColumnWidth(i, gridLayout->dataCharFieldSize);
}

QWidget *FrhexEditor::getWidget(void)
{
	return mView;
}

void FrhexEditor::mSendAddressValue(qint64 address, quint8 value)
{
	emit selectionUpdated(address, value);
}
