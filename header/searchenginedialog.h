#pragma once

#include <QDialog>
#include <QSettings>

namespace Ui {
class SearchEngineDialog;
}

class SearchEngineDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SearchEngineDialog(QWidget *parent = nullptr);
	~SearchEngineDialog();

	enum SearchEngine
	{
		Bing,
		Ddg,
		Ecosia,
		Google,
		Wikipedia,
		Yahoo,
		Other,
	};

	QStringList searchEnginePrefixes =
	{
	    "https://bing.com/search?q=",
	    "https://duckduckgo.com/?q=",
	    "https://ecosia.com/search?q=",
	    "https://google.com/search?q=",
	    "https://wikipedia.com/search?q=",
	    "https://yahoo.com/search?q=",
	};

	QSettings settings;

	QString getEnginePrefix();

private slots:
	void on_closeBtn_clicked();
	void on_applyBtn_clicked();
	void on_otherRadioButton_toggled(bool checked);

private:
	Ui::SearchEngineDialog *ui;

	QString enginePrefix;

signals:
	void sendEnginePrefix(const QString &enginePrefix, int engineId);
};
