#ifndef ADDFEEDDIALOG_H
#define ADDFEEDDIALOG_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QPushButton>
#include <QButtonGroup>
#include <QHostInfo>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QSqlQuery>
#include <QQueue>
#include <QUrl>

#include "databasemanager.h"

class AddFeedDialog : public QWidget
{
    Q_OBJECT
    DatabaseManager *m_db;
    QString m_url;
    struct Folder {
        qint32 id;
        qint32 parent;
        QString title;
    };
    qint32 m_selected;
    // enter url dialog
    QLabel *lb_enterUrl;
    QLineEdit *le_enterUrl;
    QPushButton *pb_submitUrl;
    QVBoxLayout *formLayout;

    // enter data dialog
    QLineEdit *le_enterTitle;
    QTreeWidget *folderStruct;
    QPushButton *pb_finish;
public:
    explicit AddFeedDialog(DatabaseManager *db, QWidget *parent = nullptr);

signals:
    void createFeed(QString title, QString url, qint32 folderID);
public slots:
    void setEnterUrlDialog();
    void setEnterFeedData();
    void clearEnterUrl();
    void validateUrl();
    void checkUrlResult(const QHostInfo &info);
    void handleFinishPressed();
    void setDefaultSelected(qint32 id);
};

#endif // ADDFEEDDIALOG_H
