#pragma once

#include <QColor>
#include <QPixmap>
#include <vector>
#include "Database.h"

using QColorList = QList<QColor>;

class GameTableSettingsData //: public QObject
{
//    Q_OBJECT
public:
    constexpr static const char* createDataTableSql = "CREATE TABLE IF NOT EXISTS gameTableSettings ("
                                                       "feltColors BIGINT UNIQUE,"
                                                       "background BIGINT UNIQUE,"
                                                       "deck  BLOB   UNIQUE,"
                                                       "shirt      BLOB   UNIQUE,"
                                                       "themeName  STRING UNIQUE,"
                                                       "theme      TEXT"
                                                   ");";
    constexpr static const char* createUserSettingSql = "CREATE TABLE IF NOT EXISTS @GAME_TYPE@GameTableSetting ("
                                                        "id BIGINT NOT NULL UNIQUE,"
                                                        "index_0 INT DEFAULT (0),"
                                                        "index_1 INT DEFAULT (0),"
                                                        "index_2 INT DEFAULT (0),"
                                                        "index_3 INT DEFAULT (0),"
                                                        "themeName  STRING,"
                                                        "flags INT DEFAULT(0)"
                                                        ");";

    constexpr static const char* insertUserIdSql      = "INSERT INTO @GAME_TYPE@GameTableSetting(id) VALUES (@id);";
    constexpr static const char* updateUserSettingSql = "UPDATE @GAME_TYPE@GameTableSetting SET "
                                                        "index_0 = @index_0,"
                                                        "index_1 = @index_1,"
                                                        "index_2 = @index_2,"
                                                        "index_3 = @index_3,"
                                                        "themeName = '@themeName',"
                                                        "flags = @flags "
                                                        "WHERE (id = @id);";

public:
    enum class type
    {
        feltColor = 0,
        background,
        deck,
        shirt,
        theme,
        flags,

        count
    };

    enum AppliedToTableType : gbp_u8
    {
        Nothing     = 0x00,
        Cashes      = 0x01,
        Tournaments = 0x02,
        AllTables   = Cashes | Tournaments,
        Concrete    = 0x04
    };

    enum Flags : gbp_u8
    {
        AllUnchecked        = 0x00,
        AllChecked          = 0xff,
        ShowDealerMsg       = 0x01,
        ShowPlayersMsg      = 0x02,
        DisplayEmoticone    = 0x04,
        DropLooserHand      = 0x08,
        DontShowWinningHand = 0x10,
        DontShowLastFold    = 0x20
    };

    struct Item {
        QColor feltColor;
        QColor background;
        QByteArray deck;
        QByteArray shirt;
        QString theme; // name, styleSheet
        AppliedToTableType appliedTo;
        gbp_i64 concreteTableId;
        Flags flags;

        Item()
          : feltColor(15, 100, 255, 255)
          , background(Qt::white)
          , deck()
          , shirt()
          , theme("default")
          , appliedTo(Nothing)
          , concreteTableId(-1)
          , flags(AllUnchecked)
        {}
    };

    struct Selection
	{

        size_t index[5];
		QString key;
		AppliedToTableType appliedTo;
        gbp_i64 concreteTableId;
        Flags flags;

        Selection(): index{0,0,0,0}, key("default"), appliedTo(Concrete), concreteTableId(-1), flags(AllUnchecked) {}

        Selection(size_t _0, size_t _1, size_t _2, size_t _3, const QString& key, AppliedToTableType appliedTo = Concrete, gbp_i64 id = -1, Flags flags = AllUnchecked)
            : index{_0,_1,_2,_3}, key(key), appliedTo(appliedTo), concreteTableId(id), flags(flags) {}

        QString toSql(gbp_i64 userId) const
        {
            QString request(updateUserSettingSql);
            request.replace("@id", QString::number(userId));
            request.replace("@index_0", QString::number(index[0]));
            request.replace("@index_1", QString::number(index[1]));
            request.replace("@index_2", QString::number(index[2]));
            request.replace("@index_3", QString::number(index[3]));
            request.replace("@themeName", key);
            request.replace("@flags", QString::number(flags));

            return request;
        }
	};
private:
    // все возможные настройки, подгруженные в память из БД
    QColorList     m_feltColors;
    QColorList     m_backgrounds;
    QByteArrayList m_decks;
    QByteArrayList m_shirts;
    QStringList    m_themes;
    bool m_isLoaded;

    // конкретные настройки для даного юзера
    std::pair<Selection, Selection> m_cashAndTournamentSettings; // <cash, tourn>
    std::map<gbp_i64, Selection> m_concreteCashSettings;
    std::map<gbp_i64, Selection> m_concreteTournamentSettings;

    static int sm_currentObjectCount;
protected:
    static QString createSelectRequest(const QString& name);
public:
    GameTableSettingsData();
    /*virtual */~GameTableSettingsData();

    bool isLoaded() const { return m_isLoaded; }

    void reset();
    void load();
    void reload();
    bool commit();

    int elementCount(type whichType) const;

    const QColorList& getFeltColors() const;
    const QColorList& getBackgrounds() const;
    const QByteArrayList& getDecks() const;
    const QByteArrayList& getShirts() const;
    const QStringList& getThemes() const;

    QColor feltColor(int index) const;
    QColor background(int index) const;
    const QByteArray& deck(int index) const;
    const QByteArray& shirt(int index) const;
    const QString &theme(int index) const;

    void addFeltColor(const QColor& color);
    void addBackground(const QColor& color);
    void addDeck(const QByteArray& svgSource);
    void addShirt(const QByteArray& svgSource);

    void addTheme(const QString &name);

    Item selectItem(const Selection &selection) const;
    void saveSelectionState(gbp_i64 userId, const Selection &selection);
    void restoreSelectionState(gbp_i64 userId, AppliedToTableType appliedTo = AllTables);

    Selection cashSetting(gbp_i64 concreteId = -1) const {
        if (concreteId != -1 ) {
            auto it = m_concreteCashSettings.find(concreteId);
            if (it != m_concreteCashSettings.end()) {
                return it->second;
            }
        }
        return m_cashAndTournamentSettings.first;
    }
    Selection tournamentSetting(gbp_i64 concreteId = -1) const {
        if (concreteId != -1 ) {
            auto it = m_concreteCashSettings.find(concreteId);
            if (it != m_concreteCashSettings.end()) {
                return it->second;
            }
        }
        return m_cashAndTournamentSettings.second;
    }

    QString toString() const;
};

Q_DECLARE_METATYPE(GameTableSettingsData::Item)
