#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QObject>
#include <QFile>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QApplication>

#include "settings.h"

class StyleManager : public QObject
{
    Q_OBJECT
public:
    explicit StyleManager(QObject *parent = nullptr);
    QStringList getStyles();
signals:

public slots:
    bool setStyle(QString style);
};

#endif // STYLEMANAGER_H
