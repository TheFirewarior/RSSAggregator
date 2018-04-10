#ifndef ADDFOLDERDIALOG_H
#define ADDFOLDERDIALOG_H

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

class AddFolderDialog : public QWidget
{
    Q_OBJECT
    DatabaseManager *m_db;

    struct Folder {
        qint32 id;
        qint32 parent;
        QString title;
    };
    QVBoxLayout *formLayout;
    qint32 m_selected;
    QLineEdit *le_enterTitle;
    QTreeWidget *folderStruct;
    QPushButton *pb_finish;
public:
    explicit AddFolderDialog(DatabaseManager *db, QWidget *parent = nullptr);

signals:
    void createFolder(QString title, qint32 folderID);
public slots:
    void setEnterData();
    void setDefaultSelected(qint32 id);
    void handleFinishPressed();
};

#endif // ADDFOLDERDIALOG_H
