#include <precompiled.h>
#include "localizer.h"
#include <qlibraryinfo.h>
#include <QActionGroup>
#include <QLocale>
#include <QDir>
#include <db/Database.h>
#include <PokerApplication.h>
#include <QDebug>

#ifndef Q_MOC_RUN
    #include <rpc/network_client.h>
    #include <rpc/rpc_lobby.h>
#endif // !Q_MOC_RUN

namespace
{
    QString capitalize(QString&& str) {
        str[0] = str.at(0).toUpper();
        return str;
    }
}

Localizer::Localizer()
    : QObject(nullptr)
    , m_translator(this)
    , m_translatorQt(this)
    , m_currLang("")
    , m_languages(new QActionGroup(this))
{}

void Localizer::init(QWidget *mainWidget)
{
    setParent(mainWidget);

    m_languages->setExclusive(true);

    QString currentLocale = QLocale::system().name();

    currentLocale.truncate(currentLocale.lastIndexOf('_'));
    ns_utildb::restoreState("Language", currentLocale);
    QDir dir(":/translations/");
    QStringList fileNames = dir.entryList(QStringList("lang_*.qm"));
#ifdef LANG_ENGLISH_ONLY
    fileNames = QStringList("lang_en.qm");
#endif //LANG_ENGLISH_ONLY

    for (auto locale : fileNames) {

        locale.truncate(locale.lastIndexOf('.'));
        locale.remove(0, locale.indexOf('_') + 1);
        QString lang = capitalize(QLocale(locale).nativeLanguageName());
        QString iconPath = QString(dir.path() + "/icon_%0.ico").arg(locale);

        // костыль "English" / "American English"

        if (locale == "en") {
            lang = "English";
        }

        QAction *action = new QAction(lang, this);
        action->setCheckable(true);
        action->setData(locale);

        if (QFileInfo(iconPath).exists()) {
            QIcon ico(iconPath);
            action->setIcon(ico);
        }
        m_languages->addAction(action);

        action->setChecked(currentLocale == locale);
    }
    mainWidget->setLocale(QLocale(currentLocale));
    loadLanguage(currentLocale);
}

void Localizer::changeLanguage(QAction* action)
{
    if(action != nullptr) {
        QString lang = action->data().toString();
        ns_utildb::saveState("Language", lang);
        loadLanguage(lang);
    }
}

bool Localizer::switchTranslator(QTranslator& translator, const QString& filename)
{
    qApp->removeTranslator(&translator);
    if(translator.load(filename)) {
        return qApp->installTranslator(&translator);
    }
    return false;
}
bool Localizer::switchTranslator(QTranslator& translator, const QString& filename, const QLocale& locale)
{
    qApp->removeTranslator(&translator);
    if(translator.load(locale, filename, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        return qApp->installTranslator(&translator);
    }
    return false;
}

void Localizer::loadLanguage(const QString& rLanguage)
{
    if(m_currLang != rLanguage) {
        m_currLang = rLanguage;
        QLocale locale = QLocale(m_currLang);
        QLocale::setDefault(locale);

//        bool loaded2 = m_translatorQt.load("qt_" + rLanguage,
//                            QLibraryInfo::location(QLibraryInfo::TranslationsPath));
        // TODO: не ясно почему не загружаются переводы qt
        bool loaded = switchTranslator(m_translator, QString(":/translations/lang_%1.qm").arg(rLanguage));

        if (loaded)
        {
            rpc_lobby* rpc = netClient().rpc<rpc_lobby>();
            rpc->set_user_lang(rLanguage, [](gbp::ns_api::ns_lobby::ns_reply::e_lobby status){ std::cout << "Lang change status: " << status << std::endl;});
        }
    }
}

QList<QAction *> Localizer::languageList() const
{
    return m_languages->actions();
}

const QActionGroup *Localizer::languageGroup() const
{
    return m_languages;
}

QAction *Localizer::currentLanguage() const
{
    return m_languages->checkedAction();
}

QAction *Localizer::language(const QString &lang) const
{
    for (QAction* a: m_languages->actions()) {
        if (lang == a->data().toString()) {
            return a;
        }
    }
    return nullptr;
}



