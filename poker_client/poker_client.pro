 #export DYLD_LIBRARY_PATH=/usr/lib/:$DYLD_LIBRARY_PATH
#-------------------------------------------------
#
# Project created by QtCreator 2016-08-18T11:18 :22
#
#----------------------------------- --------------

#find . -type f -print -exec touch {} \;
# . - dirname

ROOT = $$PWD/..
include($$ROOT/config_qmake_all.pri)

QT += core gui widgets network sql svg opengl
#DEFINES += NO_OPENGL
contains(DEFINES, NO_OPENGL): QT -= opengl

include(platform/platform.pri)

CONFIG(release, debug|release): QMAKE_LFLAGS += -s
DEFINES += ENABLE_VISUAL_DEBUGGING
DEFINES += NO_REGISTRATION
#DEFINES += DEBUG_WINDOW
#CONFIG += build_for_customer

build_for_customer {
    DEFINES += BUILD_FOR_CUSTOMER
    DEFINES -= ENABLE_VISUAL_DEBUGGING
    DEFINES += ONLY_FULLY_COMPLETED_CONTENT
    DEFINES += LANG_ENGLISH_ONLY
}
#-fstack-protector-strong -Wformat-security


TEMPLATE = app
OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp
UI_DIR = ./tmp
RCC_DIR = ./tmp

CONFIG += precompile_header

PRECOMPILED_HEADER = src/precompiled.h

INCLUDEPATH += $$ROOT/common
DEPENDPATH += $$ROOT/common

INCLUDEPATH += $$PWD/src \
               $$PWD/lib/include \
               $$ROOT/plugins/Mappers \
               $$ROOT/basewidgets \
               $$ROOT/gui \
               $$ROOT/api_models \
               $$ROOT/animation \
               $$ROOT/tools

DEPENDPATH +=  \
               $$ROOT/plugins/Mappers \
               $$ROOT/basewidgets \
               $$ROOT/gui \
               $$ROOT/api_models \
               $$ROOT/animation \
               $$ROOT/tools


#system(python $$ROOT/utility/utils.py --resources $$ROOT/data $$DESTDIR)

include(src/src.pri)


DISTFILES += \
    config.ini



FORMS += \
    $$PWD/forms/AvatarsList.ui \
    $$PWD/forms/dialog_Account.ui \
    $$PWD/forms/dialog_authorization.ui \
    $$PWD/forms/dialog_ChangeAvatar.ui \
    $$PWD/forms/dialog_GameTable.ui \
    $$PWD/forms/dialog_OkMessage.ui \
    $$PWD/forms/dialog_OnError.ui \
    $$PWD/forms/dialog_TableSettings.ui \
    $$PWD/forms/editors/ColorMatrixEditor.ui \
    $$PWD/forms/GameSettingsDropDown.ui \
    $$PWD/forms/GameTablePreview.ui \
    $$PWD/forms/mainwindow.ui \
    $$PWD/forms/page_MainScreen.ui \
    $$PWD/forms/page_SelectAvatar.ui \
    $$PWD/forms/page_UploadAvatar.ui \
    $$PWD/forms/PasswordToolTip.ui \
    $$PWD/forms/PixmapEditor.ui \
    $$PWD/forms/tableItems/betitem.ui \
    $$PWD/forms/tableItems/panel_BetControls.ui \
    $$PWD/forms/tableItems/potitem.ui \
    $$PWD/forms/userbox.ui \
    $$PWD/forms/widget_Bet.ui \
    $$PWD/forms/widget_LanguageItem.ui \
    $$PWD/forms/widget_minitable.ui \
    $$PWD/forms/widget_minitableItem.ui \
    $$PWD/forms/widget_TableOver.ui \
    $$PWD/forms/tableLayers/tablelayer_2seats.ui \
    $$PWD/forms/tableLayers/tablelayer_6seats.ui \
    $$PWD/forms/tableLayers/tablelayer_9seats.ui \
    $$PWD/forms/buyin/dialog_CashBuyIn.ui \
    $$PWD/forms/buyin/dialog_TournamentBuyIn.ui \
    $$PWD/forms/lobbyTournament/dialog_LobbyTournament.ui \
    $$PWD/forms/lobbyTournament/page_LobbyTables.ui \
    $$PWD/forms/lobbyTournament/page_LobbyPlayers.ui \
    $$PWD/forms/lobbyTournament/page_LobbyBlinds.ui \
    $$PWD/forms/lobbyTournament/page_LobbyCommonInfo.ui \
    $$PWD/forms/lobbyTournament/page_LobbyPrizes.ui \
    $$PWD/forms/mainLobby/panel_SitNGoDescription.ui \
    $$PWD/forms/mainLobby/panel_TournDescription.ui \
    $$PWD/forms/mainLobby/StatisticTableView.ui \
    $$PWD/forms/mainLobby/page_CashTables.ui \
    $$PWD/forms/mainLobby/page_Tournaments.ui \
    $$PWD/forms/mainLobby/page_SpinNGo.ui \
    $$PWD/forms/mainLobby/page_SitNGo.ui \
    $$PWD/forms/lobbyTournament/page_Qualifiers.ui \
    $$PWD/forms/menubar/dialog_VipSystem.ui \
    $$PWD/forms/menubar/dialog_MyTournaments.ui \
    $$PWD/forms/menubar/dialog_Settings.ui \
    $$PWD/forms/menubar/page_vipSystemDescription.ui \
    $$PWD/forms/menubar/page_vipstatusitem.ui \
    $$PWD/forms/menubar/page_vipStatusDescription.ui \
    $$PWD/forms/CalendarItem.ui \
    $$PWD/forms/menubar/settingsPages/page_SettingsCommon.ui \
    $$PWD/forms/menubar/settingsPages/page_SettingsAutorebuy.ui \
    $$PWD/forms/menubar/settingsPages/page_SettingsHistory.ui \
    $$PWD/forms/menubar/settingsPages/page_SettingsNotes.ui \
    testform.ui \
    $$PWD/forms/usercontrols.ui \
    $$PWD/forms/mainLobby/dialogServerNotAvailable.ui

INCLUDEPATH += $$ROOT/EMailSender
DEPENDPATH += $$ROOT/EMailSender
INCLUDEPATH += $$ROOT/SMTPEmail
DEPENDPATH += $$ROOT/SMTPEmail

HEADERS += \
    src/defs_qmetatype.h \
    src/common.h \
    testform.h

SOURCES += \
    src/common.cpp \
    testform.cpp \
    ../common/hostinfo/hostinfo.cpp

RESOURCES += \
    ../resources/unpacked/windowicon.qrc
