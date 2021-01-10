#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);

    // Set the id's for the buttons 'backward' and 'forward'
    ui->backwardForwardGroup->setId(ui->backwardBtn, 0);
    ui->backwardForwardGroup->setId(ui->forwardBtn, 1);
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_findBtn_clicked()
{
    QString word = ui->findLine->text();

    emit sendWordToFind(word,
                        ui->caseSensitiveCB->isChecked(),
                        ui->wholeWordsCB->isChecked(),
                        ui->backwardForwardGroup->checkedId());
}

void ReplaceDialog::on_replaceBtn_clicked()
{
    QString oldWord = ui->findLine->text();
    QString newWord = ui->replaceLine->text();

    emit sendWordsToReplace(oldWord,
                            newWord,
                            ui->caseSensitiveCB->isChecked(),
                            ui->wholeWordsCB->isChecked(),
                            ui->backwardForwardGroup->checkedId());
}

void ReplaceDialog::on_replaceAllBtn_clicked()
{
    QString oldWord = ui->findLine->text();
    QString newWord = ui->replaceLine->text();

    emit sendWordsToReplaceAll(oldWord,
                               newWord,
                               ui->caseSensitiveCB->isChecked(),
                               ui->wholeWordsCB->isChecked());
}
