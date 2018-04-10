#ifndef CHANGEFOLDERDIALOG_H
#define CHANGEFOLDERDIALOG_H

#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSqlQuery>
#include <QWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QHostInfo>
#include <QUrl>
#include <QTabWidget>

#include "databasemanager.h"
#include "changefilters.h"

class ChangeFolderDialog : public QWidget
{
    Q_OBJECT
    qint32 m_folderID;
    DatabaseManager *m_db;

    QFormLayout *m_formLayout;

    QLabel *lb_title;
    QLineEdit *le_title;

    QLabel *lb_updatePeriod;
    QSpinBox *sb_updatePeriod;

    QPushButton *pb_ok;

    // filter settings
    QVBoxLayout *filterLayout;
    QLabel *filter;
    QPushButton *pb_globalFilters;
    QPushButton *pb_titleFilters;
    QPushButton *pb_descriptionFilters;
    QPushButton *pb_categoryFilters;


    QStringList globalKeywords;
    QStringList titleKeywords;
    QStringList descriptionKeywords;
    QStringList categoryKeywords;

    void getKeywords();

    QTabWidget *m_settingTabs;
    QString makeFilter(QStringList filterData);

public:
    explicit ChangeFolderDialog(qint32 folderID, DatabaseManager *db, QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void createLayout();
    void getCurrentData();
    void setFolderData();

    void updateGlobalFilters();
    void updateTitleFilters();
    void updateDescriptionFilters();
    void updateCategoryFilters();

    void setGlobalFilters(QStringList data);
    void setTitleFilters(QStringList data);
    void setDescriptionFilters(QStringList data);
    void setCategoryFilters(QStringList data);

};

#endif // CHANGEFOLDERDIALOG_H
