#pragma once

#include <QDialog>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog();

private slots:
    void on_findBtn_clicked();
    void on_replaceBtn_clicked();
    void on_replaceAllBtn_clicked();

private:
    Ui::ReplaceDialog *ui;

signals:
    void sendWordToFind(const QString &word, bool caseSensitive, bool wholeWords, int backwardOrForward);
    void sendWordsToReplace(const QString &oldWord, const QString &newWord, bool caseSensitive, bool wholeWords, int backwardOrForward);
    void sendWordsToReplaceAll(const QString &oldWord, const QString &newWord, bool caseSensitive, bool wholeWords);
};
