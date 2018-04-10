#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QThread>
#include "databasemanager.h"
#include "downloadfeed.h"
#define MIN 1000*60


class DownloadManager : public QObject
{
    Q_OBJECT
    QNetworkAccessManager *m_networkManager;
    QTimer *m_updateCheckInterval;
    DatabaseManager *m_db;
public:
    explicit DownloadManager(QObject *parent = nullptr);


signals:

public slots:
    void init();
    void checkForUpdates();
};

#endif // DOWNLOADMANAGER_H
