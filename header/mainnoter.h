#pragma once

#include <QMainWindow>
#include <QListWidgetItem>
#include <QDir>
#include <QWheelEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainNoter; }
QT_END_NAMESPACE

class MainNoter : public QMainWindow
{
	Q_OBJECT

public:
	MainNoter(QWidget *parent = nullptr);
	~MainNoter();

	QString getFileName();

private slots:
	void on_actionNew_File_triggered();
	void on_actionOpen_File_triggered();
	void on_actionSave_triggered();
	void on_actionSave_All_triggered();
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
	void receiveWordToFind(const QString &word);

private:
	Ui::MainNoter *ui;
	QString name; // Of the file
	QString directoryPath; // The current directory
	int zoomValue = 0; // If the text is bigger it is a positive number, otherwise negative

	void showFilesInDir(QDir directory);

protected:
	void wheelEvent(QWheelEvent *event);

};
