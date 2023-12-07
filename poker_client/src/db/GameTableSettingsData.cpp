#include <precompiled.h>
#include "GameTableSettingsData.h"
#include <basewidgets/util.h>
#include <QDebug>
#include <utils/logger.h>

/*static */int GameTableSettingsData::sm_currentObjectCount = 0;

GameTableSettingsData::GameTableSettingsData()
    : m_isLoaded(false)
{
    sm_currentObjectCount++;

//    qDebug() << "GameTableSettingsData objects in memory:" << sm_currentObjectCount << "Total memory used:" << sizeof(GameTableSettingsData)*sm_currentObjectCount << "bytes";
}

GameTableSettingsData::~GameTableSettingsData()
{
    sm_currentObjectCount--;

//    qDebug() << "GameTableSettingsData objects in memory:" << sm_currentObjectCount << "Total memory used:" << sizeof(GameTableSettingsData)*sm_currentObjectCount << "bytes";

}

const QColorList& GameTableSettingsData::getFeltColors() const {
    return m_feltColors;
}

const QColorList& GameTableSettingsData::getBackgrounds() const {
    return m_backgrounds;
}

const QByteArrayList& GameTableSettingsData::getDecks() const {
    return m_decks;
}

const QByteArrayList& GameTableSettingsData::getShirts() const {
    return m_shirts;
}

const QStringList &GameTableSettingsData::getThemes() const {
    return m_themes;
}

QColor GameTableSettingsData::feltColor(int index) const {
    if (index < m_feltColors.size()) {
        return m_feltColors[index];
    }
    return QColor();
}

QColor GameTableSettingsData::background(int index) const {
    if (index < m_backgrounds.size()) {
        return m_backgrounds[index];
    }
    return QColor();
}

const QByteArray& GameTableSettingsData::deck(int index) const {
    if (index < m_decks.size()) {
        return m_decks[index];
    }
    static const QByteArray invalid;
    return invalid;
}

const QByteArray& GameTableSettingsData::shirt(int index) const {
    if (index < m_shirts.size()) {
        return m_shirts[index];
    }
    static const QByteArray invalid;
    return invalid;
}

const QString &GameTableSettingsData::theme(int index) const
{
    static const QString invalid;
    if (index < m_themes.size()) {
        return m_themes.at(index);
    }
    return invalid;
}

void GameTableSettingsData::addFeltColor(const QColor &color) {
    m_feltColors.push_back(color);
}

void GameTableSettingsData::addBackground(const QColor &color) {
    m_backgrounds.push_back(color);
}

void GameTableSettingsData::addDeck(const QByteArray &svgSource) {
    m_decks << svgSource;
}

void GameTableSettingsData::addShirt(const QByteArray &svgSource) {
    m_shirts << svgSource;
}

void GameTableSettingsData::addTheme(const QString &name) {
    m_themes << name;
}

GameTableSettingsData::Item GameTableSettingsData::selectItem(const Selection &selection) const
{
//    if (!m_isLoaded) {
//        load();
//    }
    Item item;
    item.feltColor       = feltColor(selection.index[0]);
    item.background      = background(selection.index[1]);
    item.deck            = deck(selection.index[2]);
    item.shirt           = shirt(selection.index[3]);
    item.theme           = theme(selection.index[4]);
    item.appliedTo       = selection.appliedTo;
    item.concreteTableId = selection.concreteTableId;
    item.flags           = selection.flags;

    return item;
}

void GameTableSettingsData::saveSelectionState(gbp_i64 userId, const Selection &selection)
{
    SharedDB& db = pApp->db();
    if (!db.open()) {
        return;
    }
    if (selection.appliedTo & AppliedToTableType::Concrete) {
        m_concreteCashSettings[selection.concreteTableId] = Selection(selection);
    } else {
        QSqlQuery q = std::move(db.query());
        if (selection.appliedTo & AppliedToTableType::Cashes) {
            m_cashAndTournamentSettings.first = selection;
            q.exec(QString(createUserSettingSql).replace("@GAME_TYPE@", "cash"));
            q.exec(QString(insertUserIdSql).replace("@GAME_TYPE@", "cash").replace("@id", QString::number(userId)));
            q.exec(selection.toSql(userId).replace("@GAME_TYPE@", "cash"));
        }
        if (selection.appliedTo & AppliedToTableType::Tournaments) {
            m_cashAndTournamentSettings.second = selection;
            q.exec(QString(createUserSettingSql).replace("@GAME_TYPE@", "tournament"));
            q.exec(QString(insertUserIdSql).replace("@GAME_TYPE@", "tournament").replace("@id", QString::number(userId)));
            q.exec(selection.toSql(userId).replace("@GAME_TYPE@", "tournament"));
        }

        // qDebug() << selection.toSql(userId);
        db.close();
    }

}

void GameTableSettingsData::restoreSelectionState(gbp_i64 userId, AppliedToTableType appliedTo)
{
    SharedDB& db = pApp->db();
    if (!isLoaded()) {
        load();
    }
    if (db.open())
    {
        if (appliedTo & Cashes)
        {
            QSqlQuery q = db.query(QString("SELECT * FROM cashGameTableSetting WHERE (id = %0)").arg(userId));
            if (q.exec() && q.next())
            {
                Selection& selection = m_cashAndTournamentSettings.first;
                selection.index[0]        = q.value("index_0").toInt();
                selection.index[1]        = q.value("index_1").toInt();
                selection.index[2]        = q.value("index_2").toInt();
                selection.index[3]        = q.value("index_3").toInt();
                selection.key             = q.value("themeName").toString();
                selection.flags           = Flags(q.value("flags").toInt());
            }
        }
        if (appliedTo & Tournaments)
        {
            QSqlQuery q = db.query(QString("SELECT * FROM tournamentGameTableSetting WHERE (id = %0)").arg(userId));
            if (q.exec() && q.next())
            {
                Selection& selection = m_cashAndTournamentSettings.second;
                selection.index[0]        = q.value("index_0").toInt();
                selection.index[1]        = q.value("index_1").toInt();
                selection.index[2]        = q.value("index_2").toInt();
                selection.index[3]        = q.value("index_3").toInt();
                selection.key             = q.value("themeName").toString();
                selection.flags           = Flags(q.value("flags").toInt());
            }
        }
    }
}

QString GameTableSettingsData::toString() const
{
    QString str("feltColors: %0, backgrounds: %1, shirts: %2, decks: %3, themes: %4");

    return str.arg(m_feltColors.size()).arg(m_backgrounds.size()).arg(m_shirts.size()).arg(m_decks.size()).arg(m_themes.size());
}


QString GameTableSettingsData::createSelectRequest(const QString &name)
{
    static const QString pattern("SELECT %0 FROM gameTableSettings WHERE (%0 not NULL);");
    return pattern.arg(name);
}


void GameTableSettingsData::load() {
    if (isLoaded()) {
        return;
    }
    SharedDB& db = pApp->db();
    QString fieldName = "feltColors";

    if (!db.open()) {
        Logger::error("Error in loading game table settings", __FILE__, __LINE__);
        return;
    }

    QSqlQuery q = std::move(db.query());

    q.exec(createDataTableSql);

    if (q.exec(createSelectRequest(fieldName))) {
        QSqlRecord rec = q.record();

        while (q.next()) {
            m_feltColors.push_back(QColor(q.value(rec.indexOf(fieldName)).toULongLong()));
        }
    }

    fieldName = "background";
    if (q.exec(createSelectRequest(fieldName))) {
        QSqlRecord rec = q.record();

        while (q.next()) {
            m_backgrounds.push_back(QColor(q.value(rec.indexOf(fieldName)).toULongLong()));
        }
    }

    static const char* cardName = "Q_diamonds.svg";
    for (int i = 1; QDir(QString(":textures/svg/0%0/").arg(i)).exists() ; i++) {
        QFile f(QString(":textures/svg/0%0/").arg(i) + cardName);
        if (f.exists() && f.open(QIODevice::ReadOnly)) {
            m_decks << f.readAll();
            f.close();
        }
    }

    QDir dir(":textures/svg/shirts/");
    if (dir.exists()) {
        for (const QString& filename: dir.entryList(QStringList("*.svg"))) {
            QFile f(dir.path() + filename);
            if (f.exists() && f.open(QIODevice::ReadOnly)) {
                m_shirts << f.readAll();
                f.close();
            }
        }


    }


    if (q.exec("SELECT themeName, theme FROM gameTableSettings WHERE (themeName not NULL);")) {
        QSqlRecord rec = q.record();

        while (q.next()) {
            QString themeName = q.value(rec.indexOf("themeName")).toString();
//            QString theme     = q.value(rec.indexOf("theme")).toString();

            m_themes << themeName;
        }
    }

    db.close(false);
    m_isLoaded = true;
}

void GameTableSettingsData::reset()
{
    m_feltColors.clear();
    m_backgrounds.clear();
    m_decks.clear();
    m_shirts.clear();
    m_themes.clear();
    m_isLoaded = false;
}

void GameTableSettingsData::reload()
{
    reset();
    load();
}

bool GameTableSettingsData::commit()
{
    SharedDB& db = pApp->db();

    if (!db.open()) {
        Logger::error("Db isn't open", __FILELINE__);
        return false;
    }


    QSqlQuery q = std::move(db.query());
    QString requestStr = "DROP TABLE gameTableSettings;";
    bool result = q.exec(requestStr);
    result = q.exec(createDataTableSql);

    static const QString insertPattern("INSERT INTO gameTableSettings(feltColors, background, deck, shirt, themeName, theme) "
                                       "VALUES (@feltColors, @background, '@deck', '@shirt', '@themeName', '@theme');");

    size_t maxSize = std::max({m_feltColors.size(), m_backgrounds.size(), m_decks.size(), m_shirts.size(), (int)m_themes.size()});
    requestStr = insertPattern;

    for (int i = 0; i < maxSize; i++) {
        if (i < m_feltColors.size()) {
            QColor col = feltColor(i);
            requestStr = requestStr.replace("@feltColors",QString::number(qRgba(col.red(), col.green(), col.blue(), col.alpha())));
        } else {
            requestStr = requestStr.replace("@feltColors", "NULL");
        }
        if (i < m_backgrounds.size()) {
            QColor col = background(i);
            requestStr = requestStr.replace("@background",QString::number(qRgba(col.red(), col.green(), col.blue(), col.alpha())));
        } else {
            requestStr = requestStr.replace("@background", "NULL");
        }
        if (i < m_decks.size()) {
            requestStr = requestStr.replace("@deck", deck(i).data());
        } else {
            requestStr = requestStr.replace("'@deck'", "NULL");
        }
        if (i < m_shirts.size()) {
            requestStr = requestStr.replace("@shirt", shirt(i).data() );
        } else {
            requestStr = requestStr.replace("'@shirt'", "NULL");
        }
        if (i < m_themes.size()) {
            requestStr = requestStr.replace("@themeName", m_themes[i]);
        } else {
            requestStr = requestStr.replace("'@themeName'", "NULL");
        }
    }
    result = q.exec(requestStr);

    return db.close(result) && result;
}

int GameTableSettingsData::elementCount(type whichType) const
{
    switch (whichType) {
    case type::feltColor: return m_feltColors.size();
    case type::background: return m_backgrounds.size();
    case type::deck: return m_decks.size();
    case type::shirt: return m_shirts.size();
    case type::theme: return m_themes.size();
    default: return 0;
    }
}
