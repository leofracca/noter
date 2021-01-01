#include "mainnoter.h"
#include "ui_mainnoter.h"
#include "finddialog.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QBoxLayout>
#include <QTextCursor>

MainNoter::MainNoter(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainNoter)
{
	ui->setupUi(this);

	directoryPath = "C:/";
	showFilesInDir(QDir("C:/"));
}

MainNoter::~MainNoter()
{
	delete ui;
}

// CTRL + N or
// File -> New File or
// New toolbar icon
void MainNoter::on_actionNew_File_triggered()
{
	// Reset the name of the file
	name = "";
	// Clear the editor
	ui->textNote->clear();
}

// CTRL + O or
// File -> Open File or
// Open toolbar icon
void MainNoter::on_actionOpen_File_triggered()
{
	QString filters = "Text File (*.txt) ;;"
					  "XML File (*.xml)";

	// TODO: made the directory general
	QString fileName = QFileDialog::getOpenFileName(this, "Open", directoryPath, filters);
	QFile file(fileName);
	name = fileName;

	// Check if it is possible to open the file correctly
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return;
		//QMessageBox::warning(this, "Error!", "Error opening " + fileName);

	// Read the content of the file
	QTextStream in(&file);
	QString text = in.readAll();

	// Copy the text in the text editor
	ui->textNote->setText(text);
	file.close();
}

// CTRL + S or
// File -> Save or
// Save toolbar icon
void MainNoter::on_actionSave_triggered()
{
	QFile file;

	// If it is the first time, create the file and open the saving dialog
	if (name == "")
	{
		QString filters = "All File (*.*) ;;"
						  "Text File (*.txt)";
		QString selectedFilter = "Text File (*.txt)";

		// TODO: made the directory general
		QString fileName = QFileDialog::getSaveFileName(this, "Save", directoryPath, filters, &selectedFilter);
		name = fileName;
		file.setFileName(fileName);

		// Update the list
		ui->listNotes->addItem(file.fileName().split("/").last());
	}
	// If it is not the first time, don't create the file
	else
		file.setFileName(name);

	// Check if it is possible to open the file correctly
	// If it is, then open it
	if (!file.open(QFile::WriteOnly | QFile::Text))
		return;
		//QMessageBox::warning(this, "Error!", "Error saving " + name);

	QTextStream out(&file);
	QString text = ui->textNote->toPlainText();
	out << text;

	file.close();
}

// CTRL + Alt + S or
// File -> Save As...
void MainNoter::on_actionSave_As_triggered()
{
	QFile file;

	QString filters = "All File (*.*) ;;"
					  "Text File (*.txt) ;;"
					  "XML File (*.xml)";
	QString selectedFilter = "Text File (*.txt)";

	// TODO: made the directory general
	QString fileName = QFileDialog::getSaveFileName(this, "Save", directoryPath, filters, &selectedFilter);
	name = fileName;
	file.setFileName(fileName);

	// Update the list
	ui->listNotes->addItem(file.fileName().split("/").last());

	// Check if it is possible to open the file correctly
	// If it is, then open it
	if (!file.open(QFile::WriteOnly | QFile::Text))
		return;
		//QMessageBox::warning(this, "Error!", "Error saving " + name);

	QTextStream out(&file);
	QString text = ui->textNote->toPlainText();
	out << text;

	file.close();
}

// CTRL + Shift + S or
// File -> Save All
void MainNoter::on_actionSave_All_triggered()
{
	// TODO
}

// It shows the files in the selected directory that the editor can open
// CTRL + D or
// File -> Open Directory
void MainNoter::on_actionOpen_Directory_triggered()
{
	QString selectedDirectory = QFileDialog::getExistingDirectory(this, "Open Directory", directoryPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	directoryPath = selectedDirectory;
	QDir directory(directoryPath);

	ui->listNotes->clear();

	showFilesInDir(directory);

	ui->textNote->clear();
}

// CTRL + C or
// Edit -> Copy or
// Copy toolbar icon
void MainNoter::on_actionCopy_triggered()
{
	ui->textNote->copy();
}

// CTRL + X or
// Edit -> Cut or
// Cut toolbar icon
void MainNoter::on_actionCut_triggered()
{
	ui->textNote->cut();
}

// CTRL + V or
// Edit -> Paste or
// Paste toolbar icon
void MainNoter::on_actionPaste_triggered()
{
	ui->textNote->paste();
}

// CTRL + Z or
// Edit -> Undo
void MainNoter::on_actionUndo_triggered()
{
	ui->textNote->undo();
}

// CTRL + Y or
// Edit -> Redo
void MainNoter::on_actionRedo_triggered()
{
	ui->textNote->redo();
}

// CTRL + F or
// Edit -> Find
void MainNoter::on_actionFind_triggered()
{
	FindDialog *findDialog = new FindDialog(this);
	connect(findDialog, SIGNAL(sendWordToFind(QString, bool, bool, int)), this, SLOT(receiveWordToFind(QString, bool, bool, int)));
	findDialog->setModal(false);
	findDialog->show();
}

// Highlight the word in the editor
// The word comes from findDialog
void MainNoter::receiveWordToFind(const QString &word, bool caseSensitive, bool wholeWords, int backwardOrForward)
{
	QTextCursor cursor = ui->textNote->textCursor();
	bool found = false;
	QTextDocument::FindFlags flags;

	// Set the flags
	if (caseSensitive)
		flags |= QTextDocument::FindCaseSensitively;
	if (wholeWords)
		flags |= QTextDocument::FindWholeWords;
	if (backwardOrForward == 0) // 0 means backward, 1 means forward
		flags |= QTextDocument::FindBackward;

	// Search
	if (ui->textNote->find(word, flags))
		found = true;

	// The word is not in the file
	if (!found)
		QMessageBox::information(this, "Warning!", "\"" + word + "\"" + " is not present in this file");
}

// CTRL + B or
// Format -> Bold or
// Bold toolbar icon
void MainNoter::on_actionBold_triggered()
{
	QTextCharFormat format;

	if (ui->actionBold->isChecked())
		format.setFontWeight(QFont::Bold);
	else
		format.setFontWeight(QFont::Normal);

	ui->textNote->mergeCurrentCharFormat(format);
}

// CTRL + I or
// Format -> Italic or
// Italic toolbar icon
void MainNoter::on_actionItalic_triggered()
{
	QTextCharFormat format;

	if (ui->actionItalic->isChecked())
		format.setFontItalic(true);
	else
		format.setFontItalic(false);

	ui->textNote->mergeCurrentCharFormat(format);
}

// CTRL + U or
// Format -> Underlined or
// Underlined toolbar icon
void MainNoter::on_actionUnderlined_triggered()
{
	QTextCharFormat format;

	if (ui->actionUnderlined->isChecked())
		format.setFontUnderline(true);
	else
		format.setFontUnderline(false);

	ui->textNote->mergeCurrentCharFormat(format);
}

// Double click an item on the list
void MainNoter::on_listNotes_itemDoubleClicked(QListWidgetItem *item)
{
	// Open the double-clicked item
	name = directoryPath + "/" + item->text();
	QFile file(name);

	// Check if it is possible to open the file correctly
	if (!file.open(QFile::ReadOnly | QFile::Text))
		QMessageBox::warning(this, "Error!", "Error opening " + name);

	// Read the content of the file
	QTextStream in(&file);
	QString text = in.readAll();

	// Copy the text in the text editor
	ui->textNote->setText(text);
	file.close();
}

// Fill the side list with the files in the directory
void MainNoter::showFilesInDir(QDir directory)
{
	for (QFileInfo var : directory.entryInfoList())
	{
		if (var.isFile() &&
				(var.fileName().endsWith(".txt") ||
				 var.fileName().endsWith(".xml")))
		{
			ui->listNotes->addItem(var.fileName());
		}
	}
}

// Zoom
void MainNoter::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() == Qt::ControlModifier)
	{
		// Zoom in
		if (event->angleDelta().y() > 0)
		{
			ui->textNote->zoomIn();
			zoomValue++;
		}
		// Zoom out
		else
		{
			if (zoomValue > -8) // -8 is the minimun zoom
			{
				ui->textNote->zoomOut();
				zoomValue--;
			}
		}
		qDebug() << zoomValue;
	}
}

// CTRL + + or
// View -> Zoom in
void MainNoter::on_actionZoom_in_triggered()
{
	ui->textNote->zoomIn();
	zoomValue++;
}

// CTRL + - or
// View -> Zoom out
void MainNoter::on_actionZoom_out_triggered()
{
	ui->textNote->zoomOut();
	zoomValue--;
}

// CTRL + 0 or
// View -> Default zoom
void MainNoter::on_actionDefault_zoom_triggered()
{
	if (zoomValue >= 0)
		ui->textNote->zoomOut(zoomValue);
	else
		ui->textNote->zoomIn(zoomValue);

	zoomValue = 0;
}


