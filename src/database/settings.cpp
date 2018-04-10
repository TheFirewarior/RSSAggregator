#include "settings.h"

Settings::Settings(DatabaseManager *db, QObject *parent) : QObject(parent)
{
    m_db = db;
}

QString Settings::operator [](QString name)
{
    return valueAt(name);
}

QString Settings::valueAt(QString name)
{
    return m_db->getSetting(name);
}

bool Settings::setSetting(QString name, QString value)
{
    return m_db->setSetting(name, value);
}
