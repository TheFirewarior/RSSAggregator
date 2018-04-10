#ifndef APPLICATION_H
#define APPLICATION_H
#include <QApplication>


#include "mainwindow.h"
#include "databasemanager.h"
#include "stylemanager.h"
#include "languagemanager.h"
#include "settings.h"
#include "commonstrings.h"

class Application: public QApplication
{
    MainWindow *m_window;
    DatabaseManager *m_db;
public:
    Application(int &argc, char** argv);
private:
    void createMainWindow();
    void loadAttributes();
    void createDatabase();
    void loadStyle();
    void loadLanguage();
};

#endif // APPLICATION_H
