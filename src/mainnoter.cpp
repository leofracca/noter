#include "mainnoter.h"
#include "ui_mainnoter.h"
#include "finddialog.h"
#include "replacedialog.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QBoxLayout>
#include <QTextCursor>
#include <QProcess>
#include <QFontDialog>

MainNoter::MainNoter(QWidget *parent)
    : QMainWindow(parent)
    , settings("NoterCompany", QCoreApplication::applicationName())
    , ui(new Ui::MainNoter)
{
	ui->setupUi(this);

	loadSettings();

	showFilesInDir(QDir(directoryPath));
	lastOpenedFile();

	ui->textNote->setFocus();
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
	setWindowTitle(tr("Unnamed[*] - Main Noter"));
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
	if (maybeSave())
		openFile();
	else
		return;
}

void MainNoter::openFile()
{
	QString filters = "Text File (*.txt) ;;"
	                  "XML File (*.xml)";

	QString fileName = QFileDialog::getOpenFileName(this, "Open", directoryPath, filters);
	QFile file(fileName);
	name = fileName;

	openOperations(file);
}

void MainNoter::lastOpenedFile()
{
	QFile file(name);

	openOperations(file);
}

void MainNoter::openOperations(QFile &file)
{
	// Check if it is possible to open the file correctly
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return;
	    //QMessageBox::warning(this, "Error!", "Error opening " + fileName);

	// Read the content of the file
	QTextStream in(&file);
	QString text = in.readAll();

	setWindowTitle(tr("%1[*] - Main Noter").arg(QFileInfo(file).fileName()));

	// Copy the text in the text editor
	ui->textNote->setPlainText(text);
	file.close();
}

// CTRL + S or
// File -> Save or
// Save toolbar icon
void MainNoter::on_actionSave_triggered()
{
	// If it is the first time, create the file and open the saving dialog
	if (name == "")
		saveAs();
	// If it is not the first time, don't create the file
	else
		save();
}

// CTRL + Alt + S or
// File -> Save As...
void MainNoter::on_actionSave_As_triggered()
{
	saveAs();
}

void MainNoter::save()
{
	QFile file;
	file.setFileName(name);

	saveOperations(file);
}

void MainNoter::saveAs()
{
	QFile file;
	QString filters = "All File (*.*) ;;"
	                  "Text File (*.txt) ;;"
	                  "XML File (*.xml)";
	QString selectedFilter = "Text File (*.txt)";

	QString fileName = QFileDialog::getSaveFileName(this, "Save", directoryPath, filters, &selectedFilter);
	name = fileName;
	file.setFileName(fileName);

	// Update the list
	if (QFileInfo(file).absolutePath().compare(directoryPath) == 0)
		ui->listNotes->addItem(file.fileName().split("/").last());

	saveOperations(file);
}

void MainNoter::saveOperations(QFile &file)
{
	// Check if it is possible to open the file correctly
	// If it is, then open it
	if (!file.open(QFile::WriteOnly | QFile::Text))
		return;
	    //QMessageBox::warning(this, "Error!", "Error saving " + name);

	setWindowTitle(tr("%1[*] - Main Noter").arg(QFileInfo(file).fileName()));
	ui->textNote->document()->setModified(false);

	QTextStream out(&file);
	QString text = ui->textNote->toPlainText();
	out << text;

	file.close();
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
	connect(findDialog, SIGNAL(sendWordToFind(const QString &, bool, bool, int)), this, SLOT(receiveWordToFind(const QString &, bool, bool, int)));
	findDialog->setModal(false);
	findDialog->show();
}

// CTRL + R or
// Edit -> Replace
void MainNoter::on_actionReplace_triggered()
{
	ReplaceDialog *replaceDialog = new ReplaceDialog(this);

	connect(replaceDialog, SIGNAL(sendWordToFind(const QString &, bool, bool, int)), this, SLOT(receiveWordToFind(const QString &, bool, bool, int)));
	connect(replaceDialog, SIGNAL(sendWordsToReplace(const QString &, const QString &, bool, bool, int)), this, SLOT(receiveWordsToReplace(const QString &, const QString &, bool, bool, int)));
	connect(replaceDialog, SIGNAL(sendWordsToReplaceAll(const QString &, const QString &, bool, bool)), this, SLOT(receiveWordsToReplaceAll(const QString &, const QString &, bool, bool)));

	replaceDialog->setModal(false);
	replaceDialog->show();
}

// Replace the word with a new one
// The word comes from replaceDialog
void MainNoter::receiveWordsToReplace(const QString &oldWord, const QString &newWord, bool caseSensitive, bool wholeWords, int backwardOrForward)
{
	if (!replaceWord(oldWord, newWord, caseSensitive, wholeWords, backwardOrForward))
		QMessageBox::information(this,
		                         "Warning!",
		                         "\"" + oldWord + "\""  + " is not present " + (backwardOrForward == 0 ? " backward" : " forward"));
}

// Replace all the occurrences of the word with a new one
// The word comes from replaceDialog
void MainNoter::receiveWordsToReplaceAll(const QString &oldWord, const QString &newWord, bool caseSensitive, bool wholeWords)
{
	ui->textNote->moveCursor(QTextCursor::Start);
	bool repeat = false;

	while (replaceWord(oldWord, newWord, caseSensitive, wholeWords, 1))
		repeat = true;

	// The word is not in the file
	if (!repeat)
		QMessageBox::information(this,
		                         "Warning!",
		                         "\"" + oldWord + "\""  + " is not present ");
}

// Highlight the word in the editor
// The word comes from findDialog or replaceDialog
void MainNoter::receiveWordToFind(const QString &word, bool caseSensitive, bool wholeWords, int backwardOrForward)
{
	if (!findWord(word, caseSensitive, wholeWords, backwardOrForward))
		QMessageBox::information(this,
		                         "Warning!",
		                         "\"" + word + "\""  + " is not present " + (backwardOrForward == 0 ? " backward" : " forward"));
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
	if (item->text().compare(name) == 0)
		return;

	if (maybeSave())
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

		setWindowTitle(tr("%1[*] - Main Noter").arg(QFileInfo(file).fileName()));

		// Copy the text in the text editor
		ui->textNote->setPlainText(text);
		file.close();
	}
	else
		return;
}

// Fill the side list with the files in the directory
void MainNoter::showFilesInDir(QDir directory)
{
	for (QFileInfo var : directory.entryInfoList())
	{
		if (var.isFile() &&
		        (var.fileName().endsWith(".txt") ||
		         var.fileName().endsWith(".xml")
		        )
		    )
		{
			ui->listNotes->addItem(var.fileName());
		}
	}
}

bool MainNoter::findWord(const QString &word, bool caseSensitive, bool wholeWords, int backwardOrForward)
{
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
		return true;

	return false;
}

bool MainNoter::replaceWord(const QString &oldWord, const QString &newWord, bool caseSensitive, bool wholeWords, int backwardOrForward)
{
	if (findWord(oldWord, caseSensitive, wholeWords, backwardOrForward))
	{
		ui->textNote->textCursor().insertText(newWord);
		return true;
	}

	return false;
}

// This function is used to know when a file has been modified
bool MainNoter::maybeSave()
{
	if (!ui->textNote->document()->isModified())
		return true;

	const QMessageBox::StandardButton ret = QMessageBox::warning(this,
	                                                             tr("Noter"),
	                                                             tr("The document has been modified.\nDo you want to save your changes?"),
	                                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

	switch (ret)
	{
		case QMessageBox::Save:
			on_actionSave_triggered();
			return true;
		case QMessageBox::Cancel:
			return false;
		default:
			break;
	}
	return true;
}

// Save the actual settings
void MainNoter::writeSettings()
{
	isMaximized = windowState().testFlag(Qt::WindowMaximized);
	settings.setValue("isMaximized", isMaximized);
	settings.setValue("geometry", this->geometry());
	settings.setValue("directory", directoryPath);
	settings.setValue("file", name);
}

// Load the previous settings
void MainNoter::loadSettings()
{
	isMaximized = settings.value("isMaximized", false).toBool();

	if (isMaximized)
		QWidget::showMaximized();
	else
		setGeometry(settings.value("geometry").toRect());

	directoryPath = settings.value("directory", QDir::homePath()).toString();
	name = settings.value("file").toString();

	QStringList fontSettings = settings.value("font").toString().split(",");
	// fontSettings[0] --> the font
	// fontSettings[1] --> the size of the font

	ui->textNote->setFont(QFont(fontSettings[0], fontSettings[1].toInt()));
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
		//qDebug() << zoomValue;
	}
}

// Handle the close input
void MainNoter::closeEvent(QCloseEvent *event)
{
	writeSettings();

	if (maybeSave())
		event->accept();
	else
		event->ignore();
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

// About dialog
void MainNoter::on_actionAbout_Noter_triggered()
{
	QString translatedTextAboutQtCaption;
	translatedTextAboutQtCaption = QMessageBox::tr(
	        "<h3>About Noter</h3>"
			"<p>This program uses Qt version %1.</p>"
			).arg(QLatin1String(QT_VERSION_STR));

	QString translatedTextAboutQtText;
	translatedTextAboutQtText = QMessageBox::tr(
	        "<p>Noter is an open source text editor written in C++, using Qt.<br />"
			"With Noter you can work on text files and do the essential operations like create, open, modify or save a file.</p>"
			"<p>Noter is available under the MIT License.</p>"
			"<p>For the source code see <a href=\"https://github.com/leofracca/noter/\">the github repository</a>.</p>"
			);

	QMessageBox *msgBox = new QMessageBox(this);
	msgBox->setAttribute(Qt::WA_DeleteOnClose);
	msgBox->setWindowTitle(tr("About Noter"));
	msgBox->setText(translatedTextAboutQtCaption);
	msgBox->setInformativeText(translatedTextAboutQtText);

	msgBox->show();
}

// Enable or disable dark theme
void MainNoter::on_actionChange_Theme_triggered()
{
	// If the user clicks it, then it changes the current theme
	darkThemeEnabled = !darkThemeEnabled;
	settings.setValue("darkThemeEnabled", darkThemeEnabled);

	const QMessageBox::StandardButton reboot = QMessageBox::warning(this,
	                                                                tr("Noter"),
	                                                                tr("To change the theme, Noter must be restarted.\nDo you want to restart Noter?"),
	                                                                QMessageBox::Ok | QMessageBox::Cancel);

	switch (reboot)
	{
		case QMessageBox::Ok:
			// restart:
			qApp->quit();
			QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
			break;
		default:
			break;
	}
}

// Open the font selection dialog
void MainNoter::on_actionFont_triggered()
{
	QStringList fontSettings = settings.value("font").toString().split(",");
	// fontSettings[0] --> the font
	// fontSettings[1] --> the size of the font

	bool ok;

	font = QFontDialog::getFont(&ok, QFont(fontSettings[0], fontSettings[1].toInt()), this);

	if (ok)
	{
		ui->textNote->setFont(font);
		settings.setValue("font", font);
	}
}
