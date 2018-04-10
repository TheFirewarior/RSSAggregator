#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QFile>
#define CONNECTION_NAME "feeds_database"
#define DATABASE_NAME   "feeds_database.db"

class DatabaseHandler : public QObject, public QSqlDatabase
{
    Q_OBJECT
public:
    explicit DatabaseHandler(QObject *parent = nullptr);
    static getDatabaseInstance();

signals:

public slots:
};

#endif // DATABASEHANDLER_H
