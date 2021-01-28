#include "searchenginedialog.h"
#include "ui_searchenginedialog.h"

SearchEngineDialog::SearchEngineDialog(QWidget *parent) :
    QDialog(parent),
    settings("NoterCompany", QCoreApplication::applicationName()),
    ui(new Ui::SearchEngineDialog)
{
	ui->setupUi(this);

	// Set the id's for the buttons
	ui->searchEngineGroup->setId(ui->bingRadioButton, SearchEngine::Bing);
	ui->searchEngineGroup->setId(ui->ddgRadioButton, SearchEngine::Ddg);
	ui->searchEngineGroup->setId(ui->ecosiaRadioButton, SearchEngine::Ecosia);
	ui->searchEngineGroup->setId(ui->googleRadioButton, SearchEngine::Google);
	ui->searchEngineGroup->setId(ui->wikipediaRadioButton, SearchEngine::Wikipedia);
	ui->searchEngineGroup->setId(ui->yahooRadioButton, SearchEngine::Yahoo);
	ui->searchEngineGroup->setId(ui->otherRadioButton, SearchEngine::Other);

	// Set the previosly selected engine checked
	ui->searchEngineGroup->button(settings.value("engineId").toInt())->setChecked(true);

	// Fill the line edit with the user specified search engine
	ui->otherLineEdit->setText(settings.value("userEngine").toString());
}

SearchEngineDialog::~SearchEngineDialog()
{
	delete ui;
}

QString SearchEngineDialog::getEnginePrefix()
{
	return enginePrefix;
}

// Close the dialog
void SearchEngineDialog::on_closeBtn_clicked()
{
	close();
}

// Save the changes
void SearchEngineDialog::on_applyBtn_clicked()
{
	// Get the selection
	int searchEngine = ui->searchEngineGroup->checkedId();

	// Set the default browser
	switch (searchEngine)
	{
		case SearchEngine::Bing:
			enginePrefix = searchEnginePrefixes[SearchEngine::Bing];
			break;
		case SearchEngine::Ddg:
			enginePrefix = searchEnginePrefixes[SearchEngine::Ddg];
			break;
		case SearchEngine::Ecosia:
			enginePrefix = searchEnginePrefixes[SearchEngine::Ecosia];
			break;
		case SearchEngine::Google:
			enginePrefix = searchEnginePrefixes[SearchEngine::Google];
			break;
		case SearchEngine::Wikipedia:
			enginePrefix = searchEnginePrefixes[SearchEngine::Wikipedia];
			break;
		case SearchEngine::Yahoo:
			enginePrefix = searchEnginePrefixes[SearchEngine::Yahoo];
			break;
		default: // Other
			enginePrefix = ui->otherLineEdit->text();
			settings.setValue("userEngine", enginePrefix);
	}

	emit sendEnginePrefix(enginePrefix, searchEngine);
}

// (De)activate the line edit for the input for others search engines
void SearchEngineDialog::on_otherRadioButton_toggled(bool checked)
{
	ui->otherLineEdit->setEnabled(checked);
}
