#include "changefeeddialog.h"

void ChangeFeedDialog::getKeywords()
{
    QSqlQuery keywords = m_db->getOwnFeedFilter(m_feedID);
    while(keywords.next()) {
        globalKeywords.append(keywords.value("global_filter").toString().split("\n"));
        titleKeywords.append(keywords.value("title_filter").toString().split("\n"));
        descriptionKeywords.append(keywords.value("description_filter").toString().split("\n"));
        categoryKeywords.append(keywords.value("category_filter").toString().split("\n"));
    }
}

QString ChangeFeedDialog::makeFilter(QStringList filterData)
{
    return filterData.join('\n');
}

ChangeFeedDialog::ChangeFeedDialog(qint32 feedID, DatabaseManager *db, QWidget *parent): QWidget(parent)
{
    m_feedID = feedID;
    m_db = db;
    createLayout();
    getCurrentData();
}

void ChangeFeedDialog::createLayout()
{
    setLayout(new QVBoxLayout);
    // create general settings
    m_settingTabs = new QTabWidget();
    layout()->addWidget(m_settingTabs);
    m_layout = new QFormLayout();

    lb_title = new QLabel(tr("Title:"));
    lb_url = new QLabel(tr("Url:"));
    lb_updatePeriod = new QLabel(tr("Update period:"));

    le_title = new QLineEdit;
    le_url = new QLineEdit;
    sb_updatePeriod = new QSpinBox;
    sb_updatePeriod->setMinimum(1);

    m_layout->addRow(lb_title, le_title);
    m_layout->addRow(lb_url, le_url);
    m_layout->addRow(lb_updatePeriod, sb_updatePeriod);

    QWidget *generalSettings = new QWidget();
    generalSettings->setLayout(m_layout);
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
    connect(pb_ok, SIGNAL(clicked()), this, SLOT(validateData()));
    layout()->addWidget(pb_ok);
}

void ChangeFeedDialog::getCurrentData()
{
    QSqlQuery query = m_db->getFeedData(m_feedID);
    if(!query.next())  {
        qDebug() << "No such feed: ";
        return;
    }
    le_title->setText(query.value("display_title").toString());
    le_url->setText(query.value("url").toString());
    sb_updatePeriod->setValue(query.value("update_period").toInt());
    getKeywords();
}

void ChangeFeedDialog::validateData()
{
    QHostInfo::lookupHost(QUrl(le_url->text()).host(), this, SLOT(handleUrlResult(QHostInfo)));
}

void ChangeFeedDialog::handleUrlResult(const QHostInfo &host)
{
    if(host.error() != QHostInfo::NoError) return;
    else {
        m_db->updateFeed(m_feedID, le_title->text(), le_url->text(), sb_updatePeriod->value());
        m_db->setFilterFeed(m_feedID, makeFilter(globalKeywords), makeFilter(titleKeywords), makeFilter(descriptionKeywords), makeFilter(categoryKeywords));
        close();
    }
}

void ChangeFeedDialog::updateGlobalFilters()
{
    qDebug() << "here";
    ChangeFilters *dialog = new ChangeFilters(globalKeywords);
    connect(dialog, SIGNAL(updateKeywords(QStringList)), this, SLOT(setGlobalFilters(QStringList)));
    dialog->show();
}

void ChangeFeedDialog::updateTitleFilters()
{
    ChangeFilters *dialog = new ChangeFilters(titleKeywords);
    connect(dialog, SIGNAL(updateKeywords(QStringList)), this, SLOT(setTitleFilters(QStringList)));
    dialog->show();
}

void ChangeFeedDialog::updateDescriptionFilters()
{
    ChangeFilters *dialog = new ChangeFilters(descriptionKeywords);
    connect(dialog, SIGNAL(updateKeywords(QStringList)), this, SLOT(setDescriptionFilters(QStringList)));
    dialog->show();
}

void ChangeFeedDialog::updateCategoryFilters()
{
    ChangeFilters *dialog = new ChangeFilters(categoryKeywords);
    connect(dialog, SIGNAL(updateKeywords(QStringList)), this, SLOT(setCategoryFilters(QStringList)));
    dialog->show();
}

void ChangeFeedDialog::setGlobalFilters(QStringList data)
{
    globalKeywords = data;
}

void ChangeFeedDialog::setTitleFilters(QStringList data)
{
    titleKeywords = data;
}

void ChangeFeedDialog::setDescriptionFilters(QStringList data)
{
    descriptionKeywords = data;
}

void ChangeFeedDialog::setCategoryFilters(QStringList data)
{
    categoryKeywords = data;
}
