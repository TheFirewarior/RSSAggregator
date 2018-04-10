#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

#include "databasemanager.h"

class Settings : public QObject
{
    Q_OBJECT
    DatabaseManager *m_db;
public:
    explicit Settings(DatabaseManager *db, QObject *parent = nullptr);
    QString operator [](QString name);
    QString valueAt(QString name);
    bool setSetting(QString name, QString value);
signals:

public slots:
};

#endif // SETTINGS_H
