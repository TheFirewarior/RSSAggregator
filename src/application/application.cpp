#include "application.h"

Application::Application(int &argc, char **argv): QApplication(argc, argv)
{
    // Init application values
    loadAttributes();
    createDatabase();
    loadLanguage();
    loadStyle();
    createMainWindow();

}

void Application::createMainWindow()
{
    m_window = new MainWindow(m_db);
}

void Application::loadAttributes()
{
    setAttribute(Qt::AA_EnableHighDpiScaling, true);
}

void Application::createDatabase()
{
    m_db = new DatabaseManager("MainThread");
}

void Application::loadStyle()
{
    StyleManager manager(this);
    Settings s(m_db, this);
    QString style = s.valueAt(STYLE_OPTION);
    if(style == "") style = "default";
    manager.setStyle(style);
}

void Application::loadLanguage()
{
    LanguageManager manager(this);
    Settings s(m_db, this);
    QString language = s.valueAt(LANGUAGE_OPTION);
    if(language == "") language = "English";
    manager.setLanguage(language);
}
