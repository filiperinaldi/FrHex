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
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QProgressDialog>
#include "frmMain.h"
#include "frmOpen.h"
#include "frhexEditor.h"

FrmMain::FrmMain(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tabs->removeTab(0); // Remove default tab

	QStatusBar *bar = statusBar();
	QLabel *lbAddressTitle = new QLabel("Address:");
	QLabel *lbValueTitle = new QLabel("Value:");
	mLabelAddress.setText("");
	mLabelValue.setText("");
	bar->addWidget(lbAddressTitle, 0);
	bar->addWidget(&mLabelAddress, 0);
	bar->addWidget(lbValueTitle, 0);
	bar->addWidget(&mLabelValue, 0);

	connect(ui.actionOpen,		SIGNAL(triggered()),	this,	SLOT(openFile()) );
	connect(ui.actionCloseTab,	SIGNAL(triggered()),	this,	SLOT(closeTab()) );
	connect(ui.actionSave,		SIGNAL(triggered()),	this,	SLOT(saveFile()) );
}

FrmMain::~FrmMain()
{
}

void FrmMain::openFile(void)
{
	QString fileName;
	int dataSize;
	Endianness endianness;
	
	FrmOpen dialog(this);	
	dialog.exec();
	
	if (dialog.result() == QDialog::Rejected)
		return;

	dataSize = dialog.getDataSize();
	endianness = dialog.getEndianness();
	fileName = dialog.getFilename();
	
	if (fileName.isEmpty()) {
		QMessageBox msgBox(QMessageBox::Critical, "Open File", "Empty file name");
		msgBox.exec();
		return;
	}
	
	/* 
	 * Add new tab 
	 */
	FrhexEditor *editor = new FrhexEditor(0, fileName, endianness, dataSize, 16);
	QWidget *widget = editor->getWidget();
	QFileInfo fileInfo(fileName); // Used to extract the filename without the path
	int index = ui.tabs->addTab(widget, fileInfo.fileName());
	ui.tabs->setCurrentIndex(index);
}

void FrmMain::closeTab(void)
{
	int index = ui.tabs->currentIndex();
	ui.tabs->removeTab(index);
}

void FrmMain::saveFile(void)
{
}

void FrmMain::saveFileAs(void)
{
}

void FrmMain::selectionUpdated(qint64 address, quint8 value)
{
	QString addressStr = QString("%1").arg(address, 0, 16).rightJustified(10,'0').toUpper();
	QString valueStr = QString("%1").arg(value, 0, 16).rightJustified(2,'0').toUpper();
	mLabelAddress.setText(addressStr);
	mLabelValue.setText(valueStr);
}
