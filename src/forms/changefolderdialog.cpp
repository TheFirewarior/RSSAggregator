#include "changefolderdialog.h"

void ChangeFolderDialog::getKeywords()
{
    QSqlQuery keywords = m_db->getOwnFolderFilter(m_folderID);
    while(keywords.next()) {
        globalKeywords.append(keywords.value("global_filter").toString().split("\n"));
        titleKeywords.append(keywords.value("title_filter").toString().split("\n"));
        descriptionKeywords.append(keywords.value("description_filter").toString().split("\n"));
        categoryKeywords.append(keywords.value("category_filter").toString().split("\n"));
    }

}
QString ChangeFolderDialog::makeFilter(QStringList filterData)
{
    return filterData.join('\n');
}

ChangeFolderDialog::ChangeFolderDialog(qint32 folderID, DatabaseManager *db, QWidget *parent) : QWidget(parent)
{
    qDebug() << "Got all the keywords?";
    m_folderID = folderID;
    m_db = db;
    createLayout();
    getCurrentData();
}

void ChangeFolderDialog::createLayout()
{
    setLayout(new QVBoxLayout);
    // create general settings
    m_settingTabs = new QTabWidget();
    layout()->addWidget(m_settingTabs);
    m_formLayout = new QFormLayout();

    lb_title = new QLabel(tr("Title"));
    le_title = new QLineEdit;

    lb_updatePeriod = new QLabel(tr("Update period:"));
    sb_updatePeriod = new QSpinBox;
    sb_updatePeriod->setMinimum(1);

    m_formLayout->addRow(lb_title, le_title);
    m_formLayout->addRow(lb_updatePeriod, sb_updatePeriod);

    QWidget *generalSettings = new QWidget();
    generalSettings->setLayout(m_formLayout);
    m_settingTabs->addTab(generalSettings, tr("General"));
    // create filters
    filterLayout = new QVBoxLayout();

    filter = new QLabel(tr("Edit filters: "));
    pb_globalFilters = new QPushButton(tr("Global filters"));
    connect(pb_globalFilters, SIGNAL(clicked()), this, SLOT(updateGlobalFilters()));
    pb_titleFilters = new QPushButton(tr("Title filters"));
    connect(pb_titleFilters, SIGNAL(clicked()), this, SLOT(updateTitleFilters()));
    pb_descriptionFilters = new QPushButton(tr("Description filters"));
    connect(pb_descriptionFilters, SIGNAL(clicked()), this, SLOT(updateDescriptionFilters()));
    pb_categoryFilters = new QPushButton(tr("Category filters"));
    connect(pb_categoryFilters, SIGNAL(clicked()), this, SLOT(updateCategoryFilters()));

    filterLayout->addWidget(filter);
    filterLayout->addWidget(pb_globalFilters);
    filterLayout->addWidget(pb_titleFilters);
    filterLayout->addWidget(pb_descriptionFilters);
    filterLayout->addWidget(pb_categoryFilters);
    QWidget *filterTab = new QWidget();
    filterTab->setLayout(filterLayout);
    m_settingTabs->addTab(filterTab, "Filter settings");

    pb_ok = new QPushButton(tr("Ok"));
    connect(pb_ok, SIGNAL(clicked()), this, SLOT(setFolderData()));
    layout()->addWidget(pb_ok);
}

void ChangeFolderDialog::getCurrentData()
{
    QSqlQuery query = m_db->getFolderData(m_folderID);
    if(!query.next()) {
        qDebug() << "No such folder!";
        return;
    }
    le_title->setText(query.value("title").toString());
    sb_updatePeriod->setValue(query.value("update_period").toInt());
    getKeywords();
}

void ChangeFolderDialog::setFolderData()
{
    m_db->updateFolder(m_folderID, le_title->text(), sb_updatePeriod->value());
    m_db->setFilterFolder(m_folderID, makeFilter(globalKeywords), makeFilter(titleKeywords), makeFilter(descriptionKeywords), makeFilter(categoryKeywords));
    close();
}

void ChangeFolderDialog::updateGlobalFilters()
{
    qDebug() << "here";
    ChangeFilters *dialog = new ChangeFilters(globalKeywords);
    connect(dialog, SIGNAL(updateKeywords(QStringList)), this, SLOT(setGlobalFilters(QStringList)));
    dialog->show();
}

void ChangeFolderDialog::updateTitleFilters()
{
    ChangeFilters *dialog = new ChangeFilters(titleKeywords);
    connect(dialog, SIGNAL(updateKeywords(QStringList)), this, SLOT(setTitleFilters(QStringList)));
    dialog->show();
}

void ChangeFolderDialog::updateDescriptionFilters()
{
    ChangeFilters *dialog = new ChangeFilters(descriptionKeywords);
    connect(dialog, SIGNAL(updateKeywords(QStringList)), this, SLOT(setDescriptionFilters(QStringList)));
    dialog->show();
}

void ChangeFolderDialog::updateCategoryFilters()
{
    ChangeFilters *dialog = new ChangeFilters(categoryKeywords);
    connect(dialog, SIGNAL(updateKeywords(QStringList)), this, SLOT(setCategoryFilters(QStringList)));
    dialog->show();
}

void ChangeFolderDialog::setGlobalFilters(QStringList data)
{
    globalKeywords = data;
}

void ChangeFolderDialog::setTitleFilters(QStringList data)
{
    titleKeywords = data;
}

void ChangeFolderDialog::setDescriptionFilters(QStringList data)
{
    descriptionKeywords = data;
}

void ChangeFolderDialog::setCategoryFilters(QStringList data)
{
    categoryKeywords = data;
}

