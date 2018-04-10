#-------------------------------------------------
#
# Project created by QtCreator 2017-10-31T15:34:46
#
#-------------------------------------------------

QT       += core gui sql network xml webengine webenginewidgets multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RSSAggregator
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/ui/feeddisplay.cpp \
    src/database/databasemanager.cpp \
    src/ui/mainwindow.cpp \
    src/database/feedproxymodel.cpp \
    src/database/feedmodel.cpp \
    src/network/downloadmanager.cpp \
    src/network/downloadfeed.cpp \
    src/database/articlelistmodel.cpp \
    src/ui/articlelistdisplay.cpp \
    src/ui/articledisplay.cpp \
    src/forms/addfeeddialog.cpp \
    src/forms/addfolderdialog.cpp \
    src/forms/changefeeddialog.cpp \
    src/forms/changefolderdialog.cpp \
    src/network/articlefilter.cpp \
    src/forms/changefilters.cpp \
    src/database/filtersmodel.cpp \
    src/database/articlelistproxymodel.cpp \
    src/ui/articlelistdelegate.cpp \
    src/ui/favouritestar.cpp \
    src/ui/mediadisplay.cpp \
    src/network/downloadmedia.cpp \
    src/widgets/audioplayer.cpp \
    src/widgets/imagedisplay.cpp \
    src/application/application.cpp \
    src/database/settings.cpp \
    src/application/stylemanager.cpp \
    src/forms/settingsdialog.cpp \
    src/widgets/listselection.cpp \
    src/application/languagemanager.cpp

HEADERS += \
        src/ui/feeddisplay.h \
    src/database/databasemanager.h \
    src/ui/mainwindow.h \
    src/database/feedproxymodel.h \
    src/database/feedmodel.h \
    src/network/downloadmanager.h \
    src/network/downloadfeed.h \
    src/database/articlelistmodel.h \
    src/ui/articlelistdisplay.h \
    src/ui/articledisplay.h \
    src/forms/addfeeddialog.h \
    src/forms/addfolderdialog.h \
    src/forms/changefeeddialog.h \
    src/forms/changefolderdialog.h \
    src/network/articlefilter.h \
    src/forms/changefilters.h \
    src/database/filtersmodel.h \
    src/database/articlelistproxymodel.h \
    src/ui/articlelistdelegate.h \
    src/ui/favouritestar.h \
    src/common/articlestruct.h \
    src/common/feedsstruct.h \
    src/common/mediastruct.h \
    src/common/mediagroup.h \
    src/ui/mediadisplay.h \
    src/network/downloadmedia.h \
    src/widgets/audioplayer.h \
    src/widgets/imagedisplay.h \
    src/application/application.h \
    src/database/settings.h \
    src/application/stylemanager.h \
    src/common/commonstrings.h \
    src/forms/settingsdialog.h \
    src/widgets/listselection.h \
    src/application/languagemanager.h

INCLUDEPATH += \
        src/ui/ \
        src/database/ \
        src/network/ \
        src/forms/ \
        src/common/ \
        src/widgets/ \
        src/application/

bundle_target.path = $$OUT_PWD
bundle_target.files += $$PWD/style
language_target.files += $$PWD/language/*.qm
language_target.path = $$OUT_PWD/language

INSTALLS += bundle_target language_target

TRANSLATIONS = language/RSSAggregator_sl.ts
