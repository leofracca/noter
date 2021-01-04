#pragma once

#include <QDialog>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FindDialog(QWidget *parent = nullptr);
	~FindDialog();

private slots:
	void on_findBtn_clicked();

private:
	Ui::FindDialog *ui;

signals:
	void sendWordToFind(const QString &word, bool caseSensitive, bool wholeWords, int backwardOrForward);

};
