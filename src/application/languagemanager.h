#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QApplication>
#include <QTranslator>

class LanguageManager : public QObject
{
    Q_OBJECT
    QTranslator *m_translator;
public:
    explicit LanguageManager(QObject *parent = nullptr);

signals:

public slots:
    void setLanguage(QString language);
};

#endif // LANGUAGEMANAGER_H
