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
	void on_FindBtn_clicked();

signals:
	void sendWordToFind(const QString word);

private:
	Ui::FindDialog *ui;
};

