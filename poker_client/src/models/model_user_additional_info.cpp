#include <precompiled.h>
#include "model_user_additional_info.h"
#include <basewidgets/util.h>
#include <QApplication>
#include <api_models/visitors.hpp>
#include <api_models/qdatastream_io.hpp>
#include <utils/crypt.h>
#include <hostinfo/hostinfo.h>
#include <PokerApplication.h>
#include "model_user_additional_info_items.hpp"

template <typename T>
void print_debug(const T& arg) {
    std::cout << arg << ", ";
}
template <>
void print_debug(const QVariant& arg) {
    QString str;
    QDebug dbg(&str);
    dbg << arg;
    std::cout << str.toStdString() << ", ";
}

namespace {
    QTime g_time = QTime::currentTime();
    QString rolename;
} //namespace

void debug_out() {
    std::cout << std::endl;
}

template <typename T, typename ...Args>
void debug_out(const T& arg, const Args&... args) {
    print_debug(arg);
    debug_out(args...);
}

//template<typename T>
//void debug_out(const T& arg) {
//    qDebug() << arg;
//}



#define PRINT_INFO(role, ...) \
rolename = ((role <= Qt::UserRole) ? ns_util::enumToString(Qt::ItemDataRole(role)) : ns_util::enumToString(::UserType::ERoles(role))); \
qDebug("   Loc: [%s:%i], in %s(),  %i ms", __FILE__, __LINE__, __FUNCTION__, g_time.msecsTo(QTime::currentTime())); \
debug_out(rolename.toStdString(), ##__VA_ARGS__); \
g_time = QTime::currentTime();


CategoryItem::CategoryItem(const QString &name, int rows)
    : QStandardItem(rows)
    , m_name(name)
{
}

CategoryItem::~CategoryItem() {}

void CategoryItem::load() {
    QFile file(ns_model::IOModel::getPath() + name() + ".bin");
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        read(stream);
        file.close();
        return;
    }
}

void CategoryItem::save() const {
    QFile file(ns_model::IOModel::getPath() + name() + ".bin");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        write(stream);
        file.close();
        return;
    }
    qDebug() << "can not open the device";
}

QVariant CategoryItem::fieldData(const QString &name, int role) const {
    if (QStandardItem* f = field(name)) {
        return f->data(role);
    }
    return QVariant();
}

void CategoryItem::setField(const QString &name, const QVariant &value, int role) {
    if (QStandardItem* f = field(name)) {
        f->setData(value, role);
    }
}

bool CategoryItem::contains(QStandardItem *child) const {
    return !fieldName(child).isNull();
}

const QString &CategoryItem::name() const {
    return m_name;
}

QVariant CategoryItem::data(int role) const {
    if (role == Qt::UserRole + 1) {
        return name();
    } else if (role <= Qt::UserRole) {
        return QStandardItem::data(role);
    }
    return QVariant();
}

void CategoryItem::setData(const QVariant &value, int role) {
    if (role == Qt::UserRole + 1) {
        return;
    }
    QStandardItem::setData(value, role);
}


struct PasswordItem : QStandardItem
{
    int len = 0;
    QByteArray hash;

    virtual void setData(const QVariant &value, int role) override
    {
        g_time = QTime::currentTime();

        QVariant oldValue = data(role);
        if (oldValue != value)
        {
            switch (role) {
            case ::UserType::HashRole:
            {
                if (value.canConvert<QPair<QByteArray, int>>()) {
                    QPair<QByteArray, int> val = qvariant_cast<QPair<QByteArray, int>>(value);
                    hash = val.first;
                    len = val.second;
                } else if (value.type() == QVariant::ByteArray) {
                    hash = value.toByteArray();
                } else if (value.type() == QVariant::String){
                    auto str = value.toString();
                    hash = ns_util::toSha1(str);
                    len  = str.length();
                } else {
                    return;
                }
                emitDataChanged();
                return;
            }
            case Qt::EditRole:
            case Qt::DisplayRole:
            {
                hash.clear();
                len = 0;
                QStandardItem::setData(value, role);
                emitDataChanged();
                return;
            }
            default:
                return;
//                QStandardItem::setData(value, role);
            }
        }
    }


    virtual QVariant data(int role) const
    {
        g_time = QTime::currentTime();

        switch (role) {
        case Qt::DisplayRole:
        {
            QString txt = QStandardItem::data(role).toString();

            if (txt.isEmpty() && len > 0) {
                QString str;
                for (; str.length() < len; str.append("*"));
                return str;
            }
            return txt;
        }
        case ::UserType::HashRole:
        {
            return !hash.isEmpty() ? hash : ns_util::toSha1(text());
        }
        default:
            if (role <= Qt::UserRole) {
                return QStandardItem::data(role);
            } else {
                return QVariant();
            }
        }



        return QVariant();
    }
};

struct CommonCategoryItemSaveData
{
    bool autologin = false;
    QString nickname = "";
    int passwordLenght = 0;
    QByteArray passwordHash = "";
};

QDataStream& operator<<(QDataStream& stream, const CommonCategoryItemSaveData& data) {
    stream << data.autologin;
    if (data.autologin) {
        stream << data.nickname << data.passwordLenght << data.passwordHash;
    }
    return stream;
}
QDataStream& operator>>(QDataStream& stream, CommonCategoryItemSaveData& data) {
    stream >> data.autologin;
    if (data.autologin) {
        stream >> data.nickname >> data.passwordLenght >> data.passwordHash;
    }
    return stream;
}

//! Категория для инфы по юзеру, если не была совершена авторизация
struct CommonCategoryItem : CategoryItem
{
    QMap<QString, QStandardItem*> itemByName;
    CommonCategoryItem(): CategoryItem("common", 3) {}

    virtual void read(QDataStream &stream) override {

        uint key;
        stream >> key;
        if (key != ns_model::IOModel::serialize_key) {
            qDebug() << "keys mismatch" << key << ns_model::IOModel::serialize_key;
            return;
        }
        QString version;
        stream >> version;
        if (version != QApplication::applicationVersion()) {
            qDebug() << "application version mismatch" << version << QApplication::applicationVersion();
            return;
        }

        CommonCategoryItemSaveData data;
        stream >> data;
        data.passwordHash = ns_crypt::string_decrypt(data.passwordHash, pApp->_host_info.to_string());

        itemByName["autologin"]->setCheckState(data.autologin ? Qt::Checked : Qt::Unchecked);
        if (data.autologin) {
            itemByName["nickname"]->setText(data.nickname);
            PasswordItem* passItem = static_cast<PasswordItem*>(itemByName["password"]);
            passItem->setData(QVariant::fromValue(qMakePair(data.passwordHash, data.passwordLenght)), ::UserType::HashRole);
        } else {
            itemByName["nickname"]->setText("");
            itemByName["password"]->setData(QVariant::fromValue(qMakePair(QByteArray(), 0)), ::UserType::HashRole);
        }

    }


    virtual void write(QDataStream &stream) const override {
        stream << ns_model::IOModel::serialize_key << QApplication::applicationVersion();
        CommonCategoryItemSaveData data;
        data.autologin = itemByName["autologin"]->checkState() == Qt::Checked;
        if (data.autologin) {
            data.nickname = itemByName["nickname"]->text();
            data.passwordHash = itemByName["password"]->data(::UserType::HashRole).toByteArray();
            data.passwordLenght = itemByName["password"]->text().length();

            //QTime t = QTime::currentTime();
            //qDebug() << "before encrypt" << data.passwordHash;
            data.passwordHash = ns_crypt::string_encrypt(data.passwordHash, pApp->_host_info.to_string());
            //qDebug() << data.passwordHash << "time spent:" << t.msecsTo(QTime::currentTime());
        }
        stream << data;
    }
    virtual CategoryItem* init(QStandardItem *parent) override {
        for (int i = 0; i < parent->rowCount(); i++) {
            auto* child = parent->child(i);
            if (child->data().toString() == name()) {
                if (CommonCategoryItem* me = dynamic_cast<CommonCategoryItem*>(child)) {
                    return me;
                }
                parent->removeRow(i);
                break;
            }
        }

        parent->appendRow(this);
        setChild(0, new QStandardItem);
        setChild(1, new QStandardItem);
        setChild(2, new PasswordItem);
        QStandardItem* item = child(0);
        itemByName.insert("autologin", item);
        item->setData("autologin");
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);

        item = child(1);
        itemByName.insert("nickname", item);
        item->setData("nickname");
        item = child(2);
        itemByName.insert("password", item);
        item->setData("password");
        return this;
    }
    virtual QStandardItem* field(const QString& name) const override {
        return itemByName.value(name);
    }

    virtual QString fieldName(QStandardItem *child) const override {
        if (child == nullptr) {
            return QString();
        }
        for (const QString& key: itemByName.uniqueKeys()) {
            if (itemByName.value(key) == child) {
                return key;
            }
        }
        return QString();
    }

    virtual void load() override {
        QFile file(ns_model::IOModel::getPath() + "../" + name() + ".bin");
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file);
            read(stream);
            file.close();
            return;
        }
        qDebug() << "can't open device";
    }

    virtual void save() const override {
        QFile file(ns_model::IOModel::getPath() + "../" + name() + ".bin");
        if (file.open(QIODevice::WriteOnly)) {
            QDataStream stream(&file);
            write(stream);
            file.close();
            return;
        }
        qDebug() << "can not open the device";
    }
};

struct UserCategoryItem : CategoryItem
{
    ns_model::user_info_t* m_usrModel;
    QMap<QString, QStandardItem*> itemByName;

    UserCategoryItem(ns_model::user_info_t* model)
        : CategoryItem(QString("user_%0").arg(ns_model::IOModel::userId()), model->rowCount())
        , m_usrModel(model)
    {}

    // QStandardItem interface
public:
    virtual void read(QDataStream &in) override {
        gbp::ns_api::ns_lobby::user_info_t info;
        in >> info;
        m_usrModel->resetData(new gbp::ns_api::ns_lobby::user_info_t(info));
    }
    virtual void write(QDataStream &out) const override {
        if (m_usrModel->nativeData()) {
            out << *m_usrModel->nativeData();
        }
    }

public:
    virtual CategoryItem* init(QStandardItem *parent) override
    {
        auto local_init = [&]{
            static const PropertyIndex<gbp::ns_api::ns_lobby::user_info_t> propIdx;

            setRowCount(propIdx.count());
            for (int i = 0; i < rowCount(); i++) {
                QStandardItem* item = new QStandardItem;
                item->setData(propIdx.keyAt(i));
                setChild(i, item);
                auto field = ns_model::user_info_t::field(i);
                item->setData(m_usrModel->valueOf(field), ::UserType::OriginDataRole);
                model()->setItemData(item->index(), m_usrModel->itemData(m_usrModel->indexOf(field)));
            }
        };

        for (int i = 0; i < parent->rowCount(); i++) {
            auto* child = parent->child(i);
            if (!child) {
                parent->setChild(i, this);
                local_init();
                return this;
            }
            if (child->data().toString() == name()) {
                if (UserCategoryItem* me = dynamic_cast<UserCategoryItem*>(child)) {
                    return me;
                }
                parent->removeRow(i);
                break;
            }
        }
        parent->appendRow(this);

        local_init();
        return this;
    }
    virtual QStandardItem *field(const QString &name) const override {
        if (name.isNull()) {
            return nullptr;
        }
        return itemByName[name];
    }
    virtual QString fieldName(QStandardItem *child) const override {
        if (child == nullptr) {
            return QString();
        }
        for (const QString& key: itemByName.uniqueKeys()) {
            if (itemByName.value(key) == child) {
                return key;
            }
        }
        return QString();
    }
};


struct GameInfoUserData : CategoryItem
{
    struct Layout
    {
        QMap<gbp_i64, int> m_tableIdsAsColumn;
        QMap<QString, int> m_propertiesAsRow;
        QVector<QVariant> m_data;
        QVariant& getData(int row, int col) {
            return m_data[row*m_tableIdsAsColumn.size() + col];
        }
    };
    Layout m_layout;

    QMap<QString, QStandardItem*> itemByName;

    GameInfoUserData(const QStringList& fields)
        : CategoryItem("gameinfo", fields.size())
    {
        for (const QString& name: fields)
        {
            itemByName.insert(name, new QStandardItem(name));
        }
    }
    virtual ~GameInfoUserData()
    {
        qDeleteAll(itemByName);
    }

    // QStandardItem interface
public:
    virtual void read(QDataStream &in) override
    {
    }
    virtual void write(QDataStream &out) const override
    {
    }

    // CategoryItem interface
public:
    virtual CategoryItem *init(QStandardItem *parent) override
    {
        for (int i = 0; i < parent->rowCount(); i++) {
            auto* child = parent->child(i);
            if (child->data().toString() == name()) {
                if (UserCategoryItem* me = dynamic_cast<UserCategoryItem*>(child)) {
                    return me;
                }
                parent->removeRow(i);
                break;
            }
        }
        parent->appendRow(this);

        return this;
    }
    virtual QStandardItem *field(const QString &name) const override {
        return itemByName.value(name);
    }
    virtual QString fieldName(QStandardItem *child) const override {
        return itemByName.key(child);
    }

    void setField(const QString &name, const QVariant& value) {
        if (!itemByName.contains(name)) {
            QStandardItem* item = new QStandardItem(name);
            itemByName.insert(name, item);
            appendRow(item);
        }
        itemByName.value(name)->setData(value);
    }
};

namespace ns_model
{
    struct UserInfo::PrivateData {
    private:
        UserInfo* m_owner;
    public:
        bool initialized;
        QScopedPointer<CommonCategoryItem> common;
        QScopedPointer<GameInfoUserData> gamedata;
        QScopedPointer<UserCategoryItem> user;

        QMap<QString, CategoryItem*> categoryByName;

        PrivateData(UserInfo* owner)
            : m_owner(owner)
            , initialized(false)
            , common(new CommonCategoryItem)
            , gamedata(new GameInfoUserData(QStringList() << ns_util::enumToString(ns_model::UserInfo::field::waiter_pos)))
            , user(nullptr)
        {
            categoryByName.insert(common->name(), common->init(owner->invisibleRootItem()));
            categoryByName.insert(gamedata->name(), gamedata->init(owner->invisibleRootItem()));
        }


    };

    UserInfo::UserInfo(QObject *parent)
        : QStandardItemModel(parent)
        , IOModel(this)
        , m_data(new PrivateData(this))
    {

    }

    UserInfo::~UserInfo() {
        delete m_data;
    }

    void UserInfo::init()
    {
        if (isInitialized()) {
            return;
        }
        static const char* file = "userdata.bin";
        if (!restore(file)) {
            backup(file);
        }

        m_data->initialized = true;
    }

    bool UserInfo::backup(const QString &filename) const
    {
        QFile f(getPath() + filename);
        return write(&f);
    }

    bool UserInfo::restore(const QString &filename)
    {
        QFile f(getPath() + filename);
        return read(&f);
    }

    bool UserInfo::saveCategory(const QString &categoryName) const
    {
        if (auto* categ = category(categoryName)) {
            categ->save();
            return true;
        }
        return false;
    }

    bool UserInfo::restoreCategory(const QString &categoryName)
    {
        if (auto* categ = category(categoryName)) {
            categ->load();
            return true;
        }
        return false;
    }

    QStandardItem *UserInfo::getItem(const QString &itemName, const QString &category)
    {
        Q_ASSERT_X(isInitialized(), "", "");
        if (CategoryItem* categ = m_data->categoryByName.value(category)) {
            return categ->field(itemName);
        }
        return nullptr;
    }

    CategoryItem *UserInfo::category(const QString& categoryName) const {
        return m_data->categoryByName.value(categoryName);
    }


    bool UserInfo::read(QIODevice *device)
    {
        if (device != nullptr && device->open(QIODevice::ReadOnly))
        {
            QDataStream stream(device);
            QByteArray className;
            stream >> className;
            if (className != metaObject()->className()) {
                device->close();
                return false;
            }
            beginResetModel();
            QStringList newCategories;
            stream >> newCategories;
            for (const QString& categName: newCategories) {
                if (CategoryItem* categ = m_data->categoryByName.value(categName)) {
                    //categ->read(stream);
                }
            }

            device->close();
            endResetModel();
            return true;
        }
        std::cout << "Read Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;

    }

    bool UserInfo::write(QIODevice *device) const
    {
        if (device != nullptr && device->open(QIODevice::WriteOnly))
        {
            QDataStream stream(device);
            stream << QByteArray(metaObject()->className()) << categories();
            for (CategoryItem* categ: m_data->categoryByName) {
                categ->write(stream);
            }
            device->close();
            return true;
        }
        std::cout << "Write Failed! " << __FILE__ << ":" << __LINE__ << std::endl;
        return false;
    }

    QStringList UserInfo::categories() const
    {
        return m_data->categoryByName.uniqueKeys();
    }

    bool UserInfo::isInitialized() const {
        return m_data->initialized;
    }

    void UserInfo::printAll()
    {
        //qDebug() << "m_data->categoryByName" << m_data->categoryByName;
        //qDebug() << "m_data->initialized" << m_data->initialized;
    }

    void UserInfo::onUserInfoAboutToBeReset()
    {
        if (user_info_t* model = qobject_cast<user_info_t*>(sender())) {
            if (model->userId() != -1) {
                if (!m_data->user.isNull()) {
                    m_data->categoryByName.remove(m_data->user->name());
                }
                m_data->user.reset();
                invisibleRootItem()->removeRow(1);
            }
        }
    }

    void UserInfo::onUserInfoReset()
    {
        if (user_info_t* model = qobject_cast<user_info_t*>(sender())) {
            if (model->userId() != -1) {
                m_data->user.reset(new UserCategoryItem(model));
                if (m_data->user->init(invisibleRootItem())) {
                    m_data->categoryByName.insert(m_data->user->name(), m_data->user.data());
                }
            }
        }
    }

    void UserInfo::onUserInfoDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
    {
        if (user_info_t* model = qobject_cast<user_info_t*>(sender())) {
            if (roles.isEmpty())
            {
                for (int i = topLeft.row(); i < bottomRight.row(); i++) {
                    setItemData(m_data->user->child(i)->index(), model->itemData(topLeft.sibling(i, 0)));
                }
            }
            else
            {
                for (int role: roles)
                {
                    for (int i = topLeft.row(); i < bottomRight.row(); i++) {
                        m_data->user->child(i)->setData(topLeft.sibling(i, 0).data(role));
                    }
                }
            }
        }
    }

}
