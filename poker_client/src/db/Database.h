#pragma once

#include <QSqlError>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <basewidgets/types.h>

class SharedDataService;
class ModelDatabase;
struct DbPrivateData;

class QPixmap;
class ColorTransform;
//template <class T>
class Database
{
protected:
    QString m_path;
    bool m_isInitialized;

    mutable QSqlDatabase m_sdb;

    Database(): m_path(""), m_isInitialized(false) {}
public:
//    typedef T Type;
    virtual ~Database() {}
    bool isInitialized() const { return m_isInitialized; }
//protected:
    void initDB(const QString& path, const QString &db_name);
};

class SharedDB : public QObject, public Database
{
private:
    DbPrivateData* m_dPtr;
//    SharedDataService m_sharedDataService;
    QSqlQuery* m_query;
    QSqlRecord m_sqlRecord;
    bool m_lastResult;

public:
    std::map<sha1_t, QPixmap> m_pixmapCache;
    std::map<QString, sha1_t> m_pathCache;

    SharedDB(QObject* parent = nullptr);
//    SharedDB(SharedDB&& moved);
    virtual ~SharedDB();

    QSqlQuery query(const QString& q = QString()) const;
protected:
    bool onLastActionResult(bool result, const QStringList &args = QStringList());
public:
    SharedDataService& sharedDataService();
    ModelDatabase& modelDB();
    bool getCountryInfo(int countryId, CountryInfo& info);

    //! @returns "hashkey"/"empty string" if successfull/failed
    //! @note use: my_key = std::move(addAvatar(my_image));
    sha1_t addAvatar(const base64_t& data64);
    sha1_t addAvatar(const QPixmap *pixmap);
    //! @returns "base64 image data"/"empty string" if successfull/failed
    //! @note use: my_image = std::move(getAvatar(my_key));
    base64_t getAvatar(sha1_t hashkey);
    QPixmap getAvatarPixmap(sha1_t hashkey);
    void getAllHashKeys(HashList& lst);
    void getAllAvatars(Data64List& lst);

    //! @brief executes custom query and nothing more
    bool exec(const QString& request);
    //! @brief  calls QSqlQuery::next once per call. Requires to call exec first

    QVariant yield();
    bool yield(sha1_t& hashkey, base64_t& avatarData);
    bool yield(sha1_t &hashkey, QPixmap& avatar);
    bool clearAvatars();

    void printToDebug();
    void selectFromAvatars(HashList &lst, const QString &column, const QString &condition="");

    //---------------------------------------------
    bool addState(const std::string& name, const std::string& state, const std::string& tag="GAME");
    bool setState(const std::string& name, const std::string& state, const std::string& tag="GAME");
    //bool SaveState(const std::string& name, const std::string& state, const std::string& tag="GAME");
    bool restoreState(const std::string& name, std::string& state, const std::string& tag="GAME");

    //---------------------------------------------
    void testDataStream();
private:
    void initUserInfo();
public:
    bool open(const QString& errInitMsg=QString("Database isn't initialized"), const QString& errOpenMsg=QString()) const;
    bool close(bool needCommit = true);
    bool autoLogin() const;
    void setAutoLogin(bool auto_login);
    void getLoginUserId(gbp_i64 &userID) const;
    void getLogin(QString &name, QString &pass, int& passlen) const;
    void setLogin(const std::string &name, const std::string &hash, int passlen);
    void setLogin(const QString &name, const QString &pass);

    //---------------------------------------------
    bool isInCache() const;
    const QPixmap* fromCache(const sha1_t& hash) const;
    QPixmap* fromCache(const sha1_t& hash);
    sha1_t fromCache(const QPixmap& pix);
    const QPixmap* loadToCache(const QString& path);
    //---------------------------------------------
    const QPixmap& placeholder(E::Placeholder type) const;
//    sha1_t fromCache(const QString& path);
    //    sha1_t fromCache(const QUrl& url);
    void restoreMat(const QString &name, ColorTransform &mat);
};

namespace ns_utildb
{
    bool saveState(const QString& name, const QByteArray& state);
    bool saveState(const QString& name, const QString& state);
    bool saveState(const QString& name, int state);
    bool restoreState(const QString& name, QByteArray& state);
    bool restoreState(const QString& name, QString& state);
    bool restoreState(const QString& name, int& state);

    QDateTime utcToLocalDateTime(const QDateTime& utcDateTime);

    bool applySavedStyle(const QString& name, QWidget* w = nullptr);
    bool appendSavedStyle(const QString& name, QWidget* w = nullptr);

    template <typename T>
    const T& cached(const T&);
    template <typename T>
    const T& cached(const QUrl& url);
    template <typename T>
    const T& cached(const QString& path);

    const QPixmap& cached(const char* path);

}
