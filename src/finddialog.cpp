#include "finddialog.h"
#include "ui_finddialog.h"
#include "mainnoter.h"

#include <QFile>

FindDialog::FindDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FindDialog)
{
	ui->setupUi(this);

	ui->BackwardForwardGroup->setId(ui->BackwardBtn, 0);
	ui->BackwardForwardGroup->setId(ui->ForwardBtn, 1);
}

FindDialog::~FindDialog()
{
	delete ui;
}

// Send to the main window the selected word/phrase
void FindDialog::on_FindBtn_clicked()
{
	QString word = ui->FindLine->text();
	emit sendWordToFind(word, ui->CaseSensitiveCB->isChecked(), ui->WholeWordsCB->isChecked(), ui->BackwardForwardGroup->checkedId());
}
