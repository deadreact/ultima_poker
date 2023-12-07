TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += ../tools/tools \
           ../basewidgets/basewidgets \
           ../animation/animation \
           plugins_base \
           basewidgetsplugin \
           Mappers

INCLUDEPATH += $$PWD/plugins_base
DEPENDPATH += $$PWD/plugins_base
