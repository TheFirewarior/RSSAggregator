#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QWidget>
#include <QSplitter>
#include <QListWidget>
#include <QListWidgetItem>
#include <QModelIndex>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "databasemanager.h"
#include "stylemanager.h"
#include "settings.h"
#include "commonstrings.h"
#include "listselection.h"

class SettingsDialog : public QWidget
{
    Q_OBJECT
    enum class SettingsType {
        STYLE,
        LANGUAGE
    };

    DatabaseManager *m_db;
    Settings *m_settings;
    // options
    QString m_style;
    QString m_language;



public:
    explicit SettingsDialog(DatabaseManager *db, QWidget *parent = nullptr);
    void createLayout();
    void initSettings();
    QListWidget *m_settingsList;
    QWidget *m_settingsWidget;
    QWidget *m_displayContainer;
signals:

public slots:
private slots:
    void setLanguage(QString language);
    void setStyle(QString style);
    void selectionChanged(const QModelIndex& index);
    void displayStyles();
    void displayLanguages();
    void clearSettingsWidget();
    void applySettings();
};

#endif // SETTINGSDIALOG_H
