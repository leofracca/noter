#pragma once

#include <QMainWindow>
#include <QSettings>
#include <QListWidgetItem>
#include <QDir>
#include <QWheelEvent>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainNoter; }
QT_END_NAMESPACE

class MainNoter : public QMainWindow
{
	Q_OBJECT

public:
	MainNoter(QWidget *parent = nullptr);
	~MainNoter();

	//QSettings *settings = new QSettings("NoterCompany", QCoreApplication::applicationName());
	QSettings settings;
	bool darkThemeEnabled = false;

private slots:
	void on_actionNew_File_triggered();
	void on_actionOpen_File_triggered();
	void on_actionSave_triggered();
	void on_actionPaste_triggered();
	void on_actionCopy_triggered();
	void on_actionCut_triggered();
	void on_actionUndo_triggered();
	void on_actionRedo_triggered();
	void on_actionBold_triggered();
	void on_actionItalic_triggered();
	void on_actionUnderlined_triggered();
	void on_actionSave_As_triggered();
	void on_actionOpen_Directory_triggered();
	void on_listNotes_itemDoubleClicked(QListWidgetItem *item);
	void on_actionZoom_in_triggered();
	void on_actionZoom_out_triggered();
	void on_actionDefault_zoom_triggered();
	void on_actionFind_triggered();
	void receiveWordToFind(const QString &word, bool caseSensitive, bool wholeWords, int backwardOrForward);
	void on_actionReplace_triggered();
	void receiveWordsToReplace(const QString &oldWord, const QString &newWord, bool caseSensitive, bool wholeWords, int backwardOrForward);
	void receiveWordsToReplaceAll(const QString &oldWord, const QString &newWord, bool caseSensitive, bool wholeWords);
	void on_actionAbout_Noter_triggered();
	void on_actionChange_Theme_triggered();

private:
	Ui::MainNoter *ui;
	QString name; // Of the file
	QString directoryPath; // The current directory
	int zoomValue = 0; // If the text is bigger it is a positive number, otherwise negative
	bool isMaximized = false;

	void showFilesInDir(QDir directory);
	void openFile();
	void lastOpenedFile();
	void openOperations(QFile &file);
	void save();
	void saveAs();
	void saveOperations(QFile &file);
	bool findWord(const QString &word, bool caseSensitive, bool wholeWords, int backwardOrForward);
	bool replaceWord(const QString &oldWord, const QString &newWord, bool caseSensitive, bool wholeWords, int backwardOrForward);
	bool maybeSave();
	void writeSettings();
	void loadSettings();

protected:
	void wheelEvent(QWheelEvent *event);
	void closeEvent(QCloseEvent *event);

};
