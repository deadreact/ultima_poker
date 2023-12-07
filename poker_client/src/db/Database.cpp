#include <precompiled.h>
#include "Database.h"
#include <QApplication>
#include <basewidgets/util.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <PokerApplication.h>
#include <utils/Utils.h>

#include "db_utils.h"
#ifndef Q_MOC_RUN
    #include <api/tools/sha1.h>
#include <api_models/model_log.h>
    #include <rpc/network_client.h>
#endif //Q_MOC_RUN

#include "utils/logger.h"
#include <qregularexpression.h>
#include "SharedDataService.h"
#include <core/colortransform.h>
#include <qurl.h>
#include "modeldatabase.h"


namespace
{
    static const QByteArray empty = "";
    static const QPixmap& nullPixmap()
    {
        static const QPixmap nullpix;
        return nullpix;
    }
}



void Database::initDB(const QString &path, const QString &db_name)
{
    //        Type db;
    if (!this->isInitialized())
    {
        this->m_sdb = QSqlDatabase::addDatabase("QSQLITE");
        if (!path.isEmpty())
        {
            if (!QDir(path).exists()) {
                QDir::current().mkpath(path);
            }
            this->m_path = path + "/";
        }
        this->m_path += db_name;
        this->m_sdb.setDatabaseName(this->m_path);

        Logger::msg(QString("Database inited succsessfull, PATH: %0 %1 %2").arg(this->m_path).arg(m_sdb.connectionName()).arg(m_sdb.databaseName()).toLatin1(), NO_FILELINE);

        this->m_isInitialized = true;
    }
    else
    {
        Logger::warning("Trying to initialize database, but it's already initialized", __FILELINE__);
    }
}

struct DbPrivateData
{
    SharedDataService m_sharedDataService;
    ModelDatabase m_modelsDb;


    DbPrivateData(SharedDB* db)
    : m_sharedDataService(db)
    , m_modelsDb()
    {

    }
};


SharedDB::SharedDB(QObject *parent)
    : QObject(parent)
    , Database()
    , m_dPtr(new DbPrivateData(this))
    , m_query(nullptr)
    , m_lastResult(true)
{}

//SharedDB::SharedDB(SharedDB &&moved)
//    : QObject(moved.parent())
//    , m_sharedDataService(std::move(moved.m_sharedDataService))
//{
//    m_pathCache = std::move(moved.m_pathCache);
//    m_pixmapCache = std::move(moved.m_pixmapCache);
//    m_query = moved.m_query;
//    moved.m_query = nullptr;
//    m_sqlRecord = std::move(moved.m_sqlRecord);
//    m_lastResult = moved.m_lastResult;
//}

SharedDB::~SharedDB()
{
    //qDebug() << "~SharedDB()";
    delete m_dPtr;
}

QSqlQuery SharedDB::query(const QString& q) const
{
    return (q.isNull()) ? QSqlQuery(m_sdb) : QSqlQuery(q, m_sdb);
}

bool SharedDB::onLastActionResult(bool result, const QStringList& args)
{
    if (!result)
    {

        ns_model::LogModel& log = modelsDB().logModel()->newMsg(m_query->lastError().text(), ns_model::LogMsgItem::MsgCritical);
        for (const QString& arg: args) {
            log << arg;
        }
        if (m_sdb.isOpen()) {
            m_sdb.close();
        }
    }
    return result;
}

SharedDataService &SharedDB::sharedDataService()
{
    return m_dPtr->m_sharedDataService;
}

ModelDatabase &SharedDB::modelDB() {
    return m_dPtr->m_modelsDb;
}

bool SharedDB::getCountryInfo(int countryId, CountryInfo& info)
{
    if (!this->m_isInitialized)
    {
        Logger::error("Trying to get country from db before it initialized", __FILELINE__);
        return false;
    }

    if (!this->m_sdb.open())
    {
        Logger::error(m_sdb.lastError().text(), __FILELINE__);
        return false;
    }

    if (m_query != nullptr) delete m_query;
    m_query = new QSqlQuery;
    QString str = QString("SELECT * FROM country_phone_code WHERE ID = %0").arg(countryId);

    info.id = countryId;
    if (m_query->exec(str))
    {
        QSqlRecord rec = m_query->record();

        while (m_query->next())
        {
            info.name = m_query->value(rec.indexOf("name")).toString();
            info.code = m_query->value(rec.indexOf("code")).toString();
            info.icon = QString(":textures/Common/icons/ico/%0.ico").arg(info.name);
        }
    }

    return true;
}

sha1_t SharedDB::addAvatar(const base64_t& data64)
{
    if (!open("Trying to add avatar to db before it initialized", "Database isn't opened"))
        return empty;

    QSqlQuery q;
    QString str = QStringLiteral( "CREATE TABLE IF NOT EXISTS avatars ("
                                  "hash VARCHAR(40) NOT NULL UNIQUE, "
                                  "image BLOB"
                                  ");");
    m_lastResult = q.exec(str);

    QString str_insert = QStringLiteral("INSERT INTO avatars(hash, image) VALUES ('%1', '%2');");
    sha1_t hashkey = sha1_t::fromStdString(gbp::ns_crypt::to_sha1(data64.toStdString()));
    str = str_insert.arg(QString(hashkey)).arg(QString(data64));

    m_lastResult = q.exec(str);
    this->m_sdb.close();
    return hashkey;
}

sha1_t SharedDB::addAvatar(const QPixmap* pixmap)
{
    return addAvatar(/*std::move*/(ns_util::toBase64(*pixmap)));
}

base64_t SharedDB::getAvatar(sha1_t hashkey)
{
    QString filename = QString("%0.png").arg(hashkey.data());
    if (QDir(":textures/avatars/").entryList().contains(filename)) {
        QPixmap pixmap(filename);
        //qDebug() << "Pixmap taken from filesystem";
        return ns_util::toBase64(pixmap);
    }

    if (!open("Trying to get avatar from db before it initialized", "Database isn't opened"))
        return empty;

    QString str = QString("SELECT image FROM avatars WHERE hash = '%0'").arg(QString(hashkey));
    QSqlQuery q(str, m_sdb);

    if (q.exec(str) && q.next()) {
        return q.value("image").toByteArray();
    }

    return empty;
}

QPixmap SharedDB::getAvatarPixmap(sha1_t hashkey)
{
    return ns_util::fromBase64(getAvatar(hashkey));
}

void SharedDB::selectFromAvatars(HashList& lst, const QString& column, const QString& condition)
{
    if (!open("Trying to select from avatars before db has been initialized", "Database isn't opened"))
        return;

//    QStringList lst;
    QString str = QString("SELECT %0 FROM avatars").arg(column);
    if (!condition.isEmpty())
    {
        str += QString("WHERE %0").arg(condition);
    }
    QSqlQuery q(str, m_sdb);

    if (q.exec())
    {
        QSqlRecord rec = q.record();

        while (q.next())
        {
//            std::string str = q.value(rec.indexOf(column)).toString().toStdString();
            lst.push_back(q.value(rec.indexOf(column)).toByteArray());
        }
    }

    //    return lst;
}


void SharedDB::getAllHashKeys(HashList& lst)
{
    selectFromAvatars(lst, "hash");
}

void SharedDB::getAllAvatars(Data64List& lst)
{
    selectFromAvatars(lst, "image");
}

bool SharedDB::exec(const QString &request)
{
    if (!open("Trying to exec before db has been initialized", "Database isn't opened"))
        return false;

    if (m_query != nullptr) delete m_query;
    m_query = new QSqlQuery;

    if (m_query->exec(request))
    {
        m_sqlRecord = m_query->record();

        return true;
    }    

    return false;
}

QVariant SharedDB::yield()
{
    static int index = 0;

    QVariant result;

    m_query->first();
    if (!m_query->value(index).isNull() && m_query->value(index).isValid())
    {
        result = m_query->value(index);
        ++index;
        return result;
    }

    index = 0;
    if (m_query->next()) {
        return yield();
    }
    return QVariant();
}

bool SharedDB::yield(sha1_t &hashkey, base64_t &avatarData)
{
    if (m_query->next())
    {
        hashkey = m_query->value(m_sqlRecord.indexOf("hash")).toByteArray();
        avatarData = m_query->value(m_sqlRecord.indexOf("image")).toByteArray();

        return true;
    }
    return false;
}

bool SharedDB::yield(sha1_t &hashkey, QPixmap &avatar)
{
    base64_t avatarData = "";
    if ( yield(hashkey, avatarData) )
    {
        avatar = ns_util::fromBase64(avatarData);
        return true;
    }
    return false;
}

bool SharedDB::clearAvatars()
{
    if (!open("Trying to get avatar to db before it initialized", "Database isn't opened"))
            return false;
    return QSqlQuery("DROP table avatars", m_sdb).exec();
}

void SharedDB::printToDebug()
{
    if (!open())
        return;

    QSqlQuery q("SELECT * FROM avatars", m_sdb);
    if (q.exec())
    {
        QSqlRecord rec = q.record();

        while (q.next())
        {
            QByteArray hash = q.value(rec.indexOf("hash")).toByteArray();
            QByteArray byteArray = q.value(rec.indexOf("image")).toByteArray();
        }
    }

}

bool SharedDB::addState(const std::string &name, const std::string &state, const std::string &/*tag*/)
{
    if (!open("Trying AddState to db before it initialized", "Database isn't opened"))
        return false;

    if (m_query != nullptr) delete m_query;
    m_query = new QSqlQuery;
    QString str = QStringLiteral( "CREATE TABLE IF NOT EXISTS SaveState ("
                                  "name  VARCHAR(40) NOT NULL UNIQUE, "
                                  "state VARCHAR(40),"
                                  "tag   VARCHAR(40)"
                                  ");");
    QString str_insert = QStringLiteral("INSERT INTO SaveState(name, state, tag) VALUES ('%1', '%2', '%3');");
//    bool b = m_query->exec(str);
    if (!onLastActionResult(m_query->exec(str), QStringList() << name.c_str() << state.c_str())) {
        return false;
    }
    str = str_insert.arg(name.c_str()).arg(state.c_str()).arg(state.c_str());

//    b = m_query->exec(str);
    if (!onLastActionResult(m_query->exec(str), QStringList() << name.c_str() << state.c_str())) {
        return false;
    }
    m_sdb.close();

    return true;
}

bool SharedDB::restoreState(const std::string &name, std::string &state, const std::string &/*tag*/)
{

    if (!open("Trying RestoreState to db before it initialized", "Database isn't opened"))
        return false;

    if (m_query != nullptr) delete m_query;


    m_query = new QSqlQuery;
    QString str = QString("SELECT state FROM SaveState WHERE name = '%0'").arg(name.c_str());


    if (m_query->exec(str))
    {
        QSqlRecord rec = m_query->record();

        while (m_query->next())
        {
            state = m_query->value(rec.indexOf("state")).toString().toStdString();
            return true;
        }
    }

    return false;
}

void SharedDB::testDataStream()
{
    if (!open("NO ACCESS", "Database isn't opened"))
        return;

    QSqlQuery q(m_sdb);

    ColorTransform sepia(0.393f, 0.769f, 0.189f, 0.f, 0.f
                       , 0.349f, 0.686f, 0.168f, 0.f, 0.f
                       , 0.272f, 0.534f, 0.131f, 0.f, 0.f
                       , 0.f   , 0.f   , 0.f   , 1.f, 0.f);

    ColorTransform polaroid( 1.438f, -0.122f, -0.016f, 0.f, -0.03f
                          , -0.062f,  1.378f, -0.016f, 0.f, -0.05f
                          , -0.062f, -0.122f,  1.483f, 0.f, -0.02f
                          ,  0.f   ,  0.f   ,  0.f   , 0.f,  1.f);

    ColorTransform swapped( 0.f, 0.f, 1.f, 0.f, 0.f
                          , 1.f, 0.f, 0.f, 0.f, 0.f
                          , 0.f, 1.f, 0.f, 0.f, 0.f
                          , 0.f, 0.f, 0.f, 0.f, 1.f);
    sepia.print();
    QByteArray data;
    QDataStream os(&data, QIODevice::WriteOnly);
    os << ColorTransform();
    bool ok = q.prepare(QString("CREATE TABLE IF NOT EXISTS colorMat("
                                "    name VARCHAR UNIQUE NOT NULL,"
                                "    data BLOB    DEFAULT (0) NOT NULL"
                                ");").arg(QString(data)));
    if (ok) {
        ok = q.exec();
    } else {
        Logger::error(q.lastError().text(), __FILELINE__);
    }

    if (ok)
    {
        {
            q.prepare("INSERT INTO colorMat VALUES(?, ?);");
            q.addBindValue("sepia");
            QByteArray data;
            QDataStream os(&data, QIODevice::WriteOnly);
            os << sepia;
            q.addBindValue(data);
            q.exec();
        }
        {
            q.prepare("INSERT INTO colorMat VALUES(?, ?);");
            q.addBindValue("polaroid");
            QByteArray data;
            QDataStream os(&data, QIODevice::WriteOnly);
            os << polaroid;
            q.addBindValue(data);
            q.exec();
        }
        {
            q.prepare("INSERT INTO colorMat VALUES(?, ?);");
            q.addBindValue("swapped");
            QByteArray data;
            QDataStream os(&data, QIODevice::WriteOnly);
            os << swapped;
            q.addBindValue(data);
            q.exec();
        }
    } else {
        Logger::error(q.lastError().text(), __FILELINE__);
    }
}

void SharedDB::restoreMat(const QString& name, ColorTransform& mat)
{
    if (!open("NO ACCESS", "Database isn't opened"))
        return;

    QSqlQuery q(QString("SELECT data FROM colorMat WHERE name = '%0'").arg(name), m_sdb);

    if (q.exec() && q.next())
    {
        QByteArray data = q.value("data").toByteArray();
        QDataStream ios(&data, QIODevice::ReadOnly);
        ios >> mat;
    }
    mat.print();
}

bool SharedDB::setState(const std::string &name, const std::string &state, const std::string &tag)
{
    if (!open("Trying SetState to db before it initialized", "Database isn't opened"))
        return false;

    if (m_query != nullptr) delete m_query;
    m_query = new QSqlQuery;

    QString str = QString("UPDATE SaveState SET state = '%0' WHERE name = '%1'").arg(state.c_str()).arg(name.c_str());
    bool b = m_query->exec(str);
    if (b)
    {
        str = QString("UPDATE SaveState SET tag = '%0' WHERE name = '%1'").arg(tag.c_str()).arg(name.c_str());
        b = m_query->exec(str);
    }

    this->m_sdb.close();

    return (b);
}


void SharedDB::initUserInfo()
{
    if (!open()) return;

    QSqlQuery q(m_sdb);
    bool ok = q.exec(
        "CREATE TABLE IF NOT EXISTS user_info("
            "id INT DEFAULT 0,"
            "name VARCHAR DEFAULT NULL,"
            "pass VARCHAR DEFAULT NULL,"
            "logged_in INT DEFAULT 0"
        ");"
    );
    if ( !ok )
        Logger::error(q.lastError().text(), __FILELINE__);

    ok = q.exec("SELECT id FROM user_info;");
    if ( !ok )
        Logger::error(q.lastError().text(), __FILELINE__);

    if ( !q.next() ) {
        ok = q.exec("INSERT INTO user_info VALUES(0, NULL, NULL, 0);");
        if ( !ok )
             Logger::error(q.lastError().text(), __FILELINE__);
    }
    m_sdb.close();
}

bool SharedDB::open(const QString& errInitMsg, const QString& errOpenMsg) const
{
    if (!this->m_isInitialized)
    {
        Logger::error(errInitMsg, __FILELINE__);
        return false;
    }

    if (!m_sdb.open())
    {
        Logger::error(errOpenMsg + m_sdb.lastError().text(), __FILELINE__);
        return false;
    }
    return true;
}

bool SharedDB::close(bool needCommit)
{
    bool result = false;
    if (m_sdb.isOpen()) {
        result = !needCommit;
        if (needCommit) {
            result = m_sdb.commit();
        }
        m_sdb.close();
    }
    return result;
}

bool SharedDB::autoLogin() const {
    if(!open()) return false;

    QSqlQuery q("SELECT logged_in FROM user_info;", m_sdb);
    q.exec();

    if ( !q.next() )
        return false;

    int logg_idx = q.record().indexOf("logged_in");
    bool logged_in = q.value(logg_idx).toBool();

    m_sdb.close();

    return logged_in;
}

void SharedDB::setAutoLogin(bool auto_login) {
    if(!open()) return;

    QString str = QString("UPDATE user_info SET logged_in=%0;").arg(auto_login);
    QSqlQuery q(str, m_sdb);
    q.exec();

    m_sdb.commit();
    m_sdb.close();
}

void SharedDB::getLoginUserId(gbp_i64 &userID) const
{
    if(!open()) return;

    QSqlQuery q("SELECT id FROM user_info;", m_sdb);
    q.exec();

    if ( !q.next() )
        return;

    userID = q.value(q.record().indexOf("id")).toInt();

    m_sdb.close();
}

void SharedDB::getLogin(QString &name, QString &pass, int &passlen) const {
    if(!open()) return;
    QSqlQuery q("SELECT name,pass,passlen FROM user_info;", m_sdb);
    q.exec();

    if ( !q.next() )
        return;

    int name_idx    = q.record().indexOf("name");
    int pass_idx    = q.record().indexOf("pass");
    int passlen_idx = q.record().indexOf("passlen");
    name    = qstring_decrypt(q.value(name_idx).toString());
    pass    = qstring_decrypt(q.value(pass_idx).toString());
    passlen = q.value(passlen_idx).toInt();

    m_sdb.close();
}

void SharedDB::setLogin(const std::string &name, const std::string &hash, int passlen)
{
    if(!open()) return;

    QString str = QString("UPDATE user_info SET id=%0, name='%1', pass='%2', passlen=%3;")
                         .arg(netClient().userId())
                         .arg(name.c_str())
                         .arg(hash.c_str())
                         .arg(passlen);
    QSqlQuery q(str, m_sdb);

    if (!q.exec()) {
         Logger::error(q.lastError().text(), __FILELINE__);
    }

    m_sdb.commit();
    m_sdb.close();
}

void SharedDB::setLogin(const QString &name, const QString &pass) {
    setLogin(name.toStdString(), gbp::ns_crypt::to_sha1(pass.toStdString()), pass.length());
}

QPixmap *SharedDB::fromCache(const sha1_t &hash)
{
    auto it = m_pixmapCache.find(hash);
    if (it != m_pixmapCache.end())
        return &it->second;
    return nullptr;
}

const QPixmap *SharedDB::fromCache(const sha1_t &hash) const
{
    auto it = m_pixmapCache.find(hash);
    if (it != m_pixmapCache.end())
        return &it->second;
    return nullptr;
}

sha1_t SharedDB::fromCache(const QPixmap &pix)
{
    if (pix.isNull())
        return "";
    sha1_t hash = ns_util::toSha1(pix);
    auto it = m_pixmapCache.find(hash);
    if (it == m_pixmapCache.end())
    {
        m_pixmapCache[hash] = QPixmap(pix);
    }
    return hash;
}

const QPixmap *SharedDB::loadToCache(const QString &path)
{
    if (QFileInfo(path).exists()) {
        auto it = m_pathCache.find(path);
        if (it == m_pathCache.end()) {
            sha1_t hash = fromCache(QPixmap(path));
            m_pathCache[path] = hash;
            return fromCache(hash);
        }
        return fromCache(m_pathCache.at(path));
    }
    Logger::warning(QString("Resource %0 doesn't exists").arg(path).toLatin1(), __FILELINE__);

    return &nullPixmap();
}

const QPixmap &SharedDB::placeholder(E::Placeholder type) const
{
    switch (type) {
    case E::Placeholder::Avatar:
        static const QPixmap pix(":/textures/Common/templates/avatarPlaceholder.png");
        return pix;
    }
    return nullPixmap();
}

namespace ns_utildb
{
    bool saveState(const QString& name, const QByteArray& state)
    {
        static SharedDB& db = pApp->db();
        return db.addState(name.toStdString(), state.toStdString())
            || db.setState(name.toStdString(), state.toStdString());
    }

    bool saveState(const QString& name, const QString& state)
    {
        static SharedDB& db = pApp->db();
        return db.addState(name.toStdString(), state.toStdString())
            || db.setState(name.toStdString(), state.toStdString());
    }

    bool saveState(const QString& name, int state)
    {
        static SharedDB& db = pApp->db();
        std::string str_state = std::to_string(state);
        return db.addState(name.toStdString(), str_state)
            || db.setState(name.toStdString(), str_state);
    }
    bool restoreState(const QString& name, QByteArray& state)
    {
        std::string std_state;
        static SharedDB& db = pApp->db();
        if ( db.restoreState(name.toStdString(), std_state) )
        {
            state = QByteArray::fromStdString(std_state);
            return true;
        }
        return false;
    }
    bool restoreState(const QString& name, QString& state)
    {
        std::string std_state;
        static SharedDB& db = pApp->db();
        if ( db.restoreState(name.toStdString(), std_state) )
        {
            state = QString::fromStdString(std_state);
            return true;
        }
        return false;
    }
    bool restoreState(const QString& name, int& state)
    {
        std::string std_state;
        static SharedDB& db = pApp->db();
        if ( db.restoreState(name.toStdString(), std_state) )
        {
            static const QRegularExpression re("(^-?\\d+$)");
            if (re.match(std_state.c_str()).hasMatch())
            {
                state = QString::fromStdString(std_state).toInt();
                return true;
            }
        }
        return false;
    }

    bool applySavedStyle(const QString &name, QWidget *w)
    {
        SharedDB& db = pApp->db();
        QString q = QString("SELECT script, args_count FROM styles WHERE name='%1'").arg(name);

        if (db.exec(q))
        {
            QVariant style = db.yield();
            QVariant args_count = db.yield();

            if (style.isValid() && args_count.isValid())
            {
                QString styleSheet = style.toString();
                    if (args_count.toInt() > 0) {
                        styleSheet = styleSheet.arg((w != nullptr) ? w->metaObject()->className() : "*");
                    }
                    if (w != nullptr) {
                        w->setStyleSheet(styleSheet);
                    } else {
                        pApp->setStyleSheet(styleSheet);
                    }
                    return true;
            }
        }

        return false;        
    }

    bool appendSavedStyle(const QString &name, QWidget *w)
    {
        QString style = (w != nullptr) ? w->styleSheet() : pApp->styleSheet();
        if (applySavedStyle(name, w))
        {
            if (w != nullptr) {
                w->setStyleSheet(style + "\n" + w->styleSheet());
            } else {
                pApp->setStyleSheet(style + "\n" + pApp->styleSheet());
            }
            return true;
        }
        return false;
    }

    template <>
    const QPixmap& cached(const QPixmap& pix) {
        const QPixmap* pixmap = pApp->db().fromCache(pApp->db().fromCache(pix));
        return (pixmap) ? (*pixmap) : nullPixmap();
    }

    template <>
    const QPixmap& cached(const QUrl &url) {
         const QPixmap* pixmap = pApp->db().loadToCache(url.toLocalFile());
         return (pixmap) ? (*pixmap) : nullPixmap();
    }

    template <>
    const QPixmap& cached(const QString& path) {
        const QPixmap* pixmap =  pApp->db().loadToCache(path);
        return (pixmap) ? (*pixmap) : nullPixmap();
    }

    const QPixmap& cached(const char* path) {
        const QPixmap* pixmap =  pApp->db().loadToCache(QLatin1String(path));
        return (pixmap) ? (*pixmap) : nullPixmap();
    }

    QDateTime utcToLocalDateTime(const QDateTime &utcDateTime)
    {
        QByteArray timezoneId = QTimeZone::systemTimeZoneId();
        restoreState("CurrentTimeZone", timezoneId);
        return utcDateTime.toTimeZone(QTimeZone(timezoneId));
    }

}

