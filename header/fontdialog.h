#pragma once

#include <QDialog>

namespace Ui {
class FontDialog;
}

class FontDialog : public QDialog
{
	Q_OBJECT

public:
	explicit FontDialog(QWidget *parent = nullptr);
	~FontDialog();

private:
	Ui::FontDialog *ui;
};

