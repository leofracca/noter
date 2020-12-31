#include "finddialog.h"
#include "ui_finddialog.h"
#include "mainnoter.h"

#include <QFile>

FindDialog::FindDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FindDialog)
{
	ui->setupUi(this);
}

FindDialog::~FindDialog()
{
	delete ui;
}

// Send to the main window the selected word/phrase
void FindDialog::on_FindBtn_clicked()
{
	QString word = ui->FindLine->text();
	emit sendWordToFind(word);
}
