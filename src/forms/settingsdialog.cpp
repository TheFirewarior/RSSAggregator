#include "settingsdialog.h"

SettingsDialog::SettingsDialog(DatabaseManager *db, QWidget *parent) : QWidget(parent), m_db(db)
{
    m_settings = new Settings(m_db, this);
    m_language = m_settings->valueAt(LANGUAGE_OPTION);
    m_style = m_settings->valueAt(STYLE_OPTION);
    initSettings();
    createLayout();
}

void SettingsDialog::createLayout()
{
    resize(800,600);
    setLayout(new QVBoxLayout);
    QSplitter *splitter = new QSplitter();

    splitter->addWidget(m_settingsList);
    splitter->addWidget(m_displayContainer);
    layout()->addWidget(splitter);
}

void SettingsDialog::initSettings()
{
    // Init list of setting tabs
    m_settingsList = new QListWidget;
    // Add settings
    QListWidgetItem *style = new QListWidgetItem;
    style->setText(tr("Style"));
    style->setData(Qt::UserRole, static_cast<int>(SettingsType::STYLE));
    m_settingsList->addItem(style);

    QListWidgetItem *language = new QListWidgetItem;
    language->setText(tr("Language"));
    language->setData(Qt::UserRole, static_cast<int>(SettingsType::LANGUAGE));
    m_settingsList->addItem(language);
    connect(m_settingsList, SIGNAL(clicked(QModelIndex)), this, SLOT(selectionChanged(QModelIndex)));


    // Init settings display
    m_settingsWidget = new QWidget;
    m_settingsWidget->setLayout(new QHBoxLayout);

    m_displayContainer = new QWidget;
    QVBoxLayout *widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(m_settingsWidget);

    QHBoxLayout *options = new QHBoxLayout;
    QPushButton *ok = new QPushButton(tr("Ok"));
    QPushButton *apply = new QPushButton(tr("Apply"));
    QPushButton *cancel = new QPushButton(tr("Cancel"));
    options->addWidget(ok);
    options->addWidget(apply);
    options->addWidget(cancel);

    // Connect buttons
    connect(ok, SIGNAL(clicked()), this, SLOT(applySettings()));
    connect(apply, SIGNAL(clicked()), this, SLOT(applySettings()));
    connect(ok, SIGNAL(clicked()), this, SLOT(deleteLater()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(deleteLater()));

    QWidget *optionsContainer = new QWidget;
    optionsContainer->setLayout(options);
    widgetLayout->addWidget(optionsContainer);
    m_displayContainer->setLayout(widgetLayout);



}

void SettingsDialog::setLanguage(QString language)
{
    m_language = language;
}

void SettingsDialog::setStyle(QString style)
{
    m_style = style;
}

void SettingsDialog::selectionChanged(const QModelIndex &index)
{
    if(static_cast<SettingsType>(index.data(Qt::UserRole).toInt()) == SettingsType::STYLE) {
        displayStyles();
    }
    else if (static_cast<SettingsType>(index.data(Qt::UserRole).toInt()) == SettingsType::LANGUAGE) {
        displayLanguages();
    }
}

void SettingsDialog::displayStyles()
{
    clearSettingsWidget();
    m_style = m_settings->valueAt(STYLE_OPTION);
    if(m_style == "") {
        m_style="default";
    }
    ListSelection *selection = new ListSelection(this);
    StyleManager manager(this);
    selection->setOptionsList(manager.getStyles());
    selection->setCurrentOption(m_style);

    m_settingsWidget->layout()->addWidget(selection);
    connect(selection, SIGNAL(optionChanged(QString)), this, SLOT(setStyle(QString)));
}

void SettingsDialog::displayLanguages()
{
    clearSettingsWidget();
    m_language = m_settings->valueAt(LANGUAGE_OPTION);
    if(m_language == "") {
        m_language= "English";
    }
    ListSelection *selection = new ListSelection(this);
    selection->setOptionsList({tr("English"), tr("Slovenian")});
    selection->setCurrentOption(m_language);

    m_settingsWidget->layout()->addWidget(selection);
    connect(selection, SIGNAL(optionChanged(QString)), this, SLOT(setLanguage(QString)));
}

void SettingsDialog::clearSettingsWidget()
{
    if(m_settingsWidget == nullptr)
        return;
    else {
        QLayoutItem *item;
        while((item = m_settingsWidget->layout()->takeAt(0))) {
            delete item->widget();
            delete item;
        }
    }
}

void SettingsDialog::applySettings()
{
    // Save style
    StyleManager manager;
    manager.setStyle(m_style);
    m_settings->setSetting(STYLE_OPTION, m_style);

    // Save language
    if(m_language == tr("English"))
        m_settings->setSetting(LANGUAGE_OPTION, "English");
    else if(m_language == tr("Slovenian"))
        m_settings->setSetting(LANGUAGE_OPTION, "Slovenian");

}
