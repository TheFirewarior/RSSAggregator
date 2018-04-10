#include "languagemanager.h"

LanguageManager::LanguageManager(QObject *parent) : QObject(parent)
{
    m_translator = new QTranslator;
}

void LanguageManager::setLanguage(QString language)
{
    if(language == "English") {
    }
    else if(language == "Slovenian") {
        m_translator->load("language/RSSAggregator_sl");
    }
    qApp->installTranslator(m_translator);
}
