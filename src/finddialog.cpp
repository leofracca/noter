#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FindDialog)
{
	ui->setupUi(this);

	// Set the id's for the buttons 'backward' and 'forward'
	ui->backwardForwardGroup->setId(ui->backwardBtn, 0);
	ui->backwardForwardGroup->setId(ui->forwardBtn, 1);
}

FindDialog::~FindDialog()
{
	delete ui;
}

// Send to the main window the selected word/phrase
void FindDialog::on_findBtn_clicked()
{
	QString word = ui->findLine->text();
	emit sendWordToFind(word,
						ui->caseSensitiveCB->isChecked(),
						ui->wholeWordsCB->isChecked(),
						ui->backwardForwardGroup->checkedId());
}
