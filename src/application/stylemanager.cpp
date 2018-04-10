#include "stylemanager.h"

StyleManager::StyleManager(QObject *parent) : QObject(parent)
{
}

QStringList StyleManager::getStyles()
{
    QString appPath = QApplication::applicationDirPath();
    QDir dir(appPath + "/style");
    QStringList dirs = dir.entryList();
    QStringList end;
    foreach(QString entry, dirs) {
        if(!entry.contains('.')) {
            end.append(entry);
        }
    }
    return end;
}

bool StyleManager::setStyle(QString style)
{
    QString appPath = QApplication::applicationDirPath();
    QString defaultPath = appPath + "/style/default/style.qss";
    QString stylePath = appPath + "/style/" + style + "/style.qss";
    QFile defaultFile(defaultPath);
    qDebug() << "here";
    QFile file(stylePath);
    if(!file.open(QFile::ReadOnly)) return false;
    if(!defaultFile.open(QFile::ReadOnly)) return false;
    QString styleSheet = defaultFile.readAll() + " ";
    styleSheet += file.readAll();
    qApp->setStyleSheet(styleSheet);
    qDebug() << "done";
    return true;
}
