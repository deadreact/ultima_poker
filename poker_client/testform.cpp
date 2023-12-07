#include "testform.h"
#include "ui_testform.h"

#include <qevent.h>
#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/api_models.hpp>
#include <api_models/api_util.hpp>
#include <db/modeldatabase.h>
#include <PokerApplication.h>
#include <widgets/MainWindow.h>
#include <qsortfilterproxymodel.h>
#include <qstringlistmodel.h>
#include <models/proxy/model_singlecolumn.h>
#include <models/proxy/model_modelcontainer.h>
#include <models/proxy/model_commongametable.h>
#include <api_models/model_cash_stat_t.hpp>
#include <api_models/model_tournament_stat_t.hpp>
#include <api_models/model_log.h>
#include <api_models/model_table_state_t.hpp>
#include <qfiledialog.h>


using QStringSet = QSet<QString>;
using ModelInfo = QMap<QAbstractItemModel*, QStringSet>;

class MsgFilterModel: public QSortFilterProxyModel
{
public:
    enum Filter : uint
    {
        Nothing  = 0,
        Function = 1 << ns_model::LogMsgItem::MsgFunction,
        Info     = 1 << ns_model::LogMsgItem::MsgInfo,
        Warning  = 1 << ns_model::LogMsgItem::MsgWarning,
        Critical = 1 << ns_model::LogMsgItem::MsgCritical,
        Fatal    = 1 << ns_model::LogMsgItem::MsgFatal,
        All      = Function | Info | Warning | Critical | Fatal
    };
    uint m_filter;
public:
    MsgFilterModel(QObject* parent = nullptr)
        : QSortFilterProxyModel(parent)
        , m_filter(All)
    {}

    void resetMsgFilter(uint filter) {
        if (filter != m_filter) {
            invalidateFilter();
            m_filter = filter;
        }
    }

    int filter() const {
        return m_filter;
    }

protected:
//    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
//        if (ns_model::LogModel* logModel = qobject_cast<ns_model::LogModel*>(sourceModel())) {
//            if (const ns_model::LogMsgItem* item = logModel->indexToItem(logModel->index(source_row, 0, source_parent))) {
//                return ((1 << item->type()) & m_filter);
//            }
//        }
//        return false;
//    }
};


class ProxyModel : public QIdentityProxyModel
{
public:
    ProxyModel(QObject* parent = nullptr)
        : QIdentityProxyModel(parent)
    {}
    virtual ~ProxyModel() {}

    virtual QVariant data(const QModelIndex &proxyIndex, int role) const override {
        if (role == Qt::DecorationRole)
        {
            QVariant decoration = QIdentityProxyModel::data(proxyIndex, role);
            if (hasChildren(proxyIndex) && rowCount(proxyIndex) > 0 && columnCount(proxyIndex) > 0 && !decoration.isValid()) {
                decoration = QVariant::fromValue(qApp->style()->standardIcon(QStyle::SP_ArrowUp));
            }
            return decoration;
        }
        return QIdentityProxyModel::data(proxyIndex, role);
    }

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override {
        return QIdentityProxyModel::flags(index) | Qt::ItemIsEnabled;
    }

};

TestForm::TestForm(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::TestForm)
    , m_model(new ProxyModel)
    , m_containerModel(new ns_model::ModelContainer)
    , m_proxyModel(new MsgFilterModel(this))
    , m_proxyTableModel(new ns_model::CommonGameTable)
    , m_choosingNow(false)
{
    m_ui->setupUi(this);

    m_ui->proxyView->setModel(m_proxyTableModel);

    m_ui->listView->setModel(new QStringListModel);
    m_ui->tableView->setModel(m_model.data());
    m_ui->treeView->setModel(m_model.data());

    QAction* a = new QAction("back", this);
    a->setShortcut(QKeySequence("Backspace"));
    addAction(a);

    connect(a, &QAction::triggered, this, [this]{
        if (m_model->sourceModel() != nullptr && m_ui->tableView->rootIndex().isValid()) {
            m_ui->tableView->setRootIndex(m_ui->tableView->rootIndex().parent());
        }
    });

//    m_ui->containerView->setModel(m_containerModel.data());
    QSortFilterProxyModel* secondProxy = new QSortFilterProxyModel(this);
    secondProxy->setSourceModel(modelsDB().logModel());
    m_proxyModel->setSourceModel(secondProxy);
    m_proxyModel->setSortRole(UserType::SortRole);
    m_ui->log->setModel(m_proxyModel);
    modelsDB().logModel()->insertRow(0);
    modelsDB().logModel()->setData(modelsDB().logModel()->index(0, 0), "!!!!!!!!!!!!!!!", Qt::DisplayRole);

    connect(m_ui->bttn_view_table, &QCheckBox::toggled, modelsDB().logModel(), &ns_model::LogModel::setUseTableRepresentation);

    m_ui->bttnGrp_filters->setId(m_ui->bttn_function, MsgFilterModel::Function);
    m_ui->bttnGrp_filters->setId(m_ui->bttn_info, MsgFilterModel::Info);
    m_ui->bttnGrp_filters->setId(m_ui->bttn_warnings, MsgFilterModel::Warning);
    m_ui->bttnGrp_filters->setId(m_ui->bttn_critical, MsgFilterModel::Critical);
    m_ui->bttnGrp_filters->setId(m_ui->bttn_fatal, MsgFilterModel::Fatal);

    connect(m_ui->bttnGrp_filters, SIGNAL(buttonToggled(int,bool)), this, SLOT(on_msgTypeFilterChanged(int,bool)));

    m_ui->log->header()->resizeSection(0, 900);
}

TestForm::~TestForm()
{
    delete m_ui;
}

void TestForm::on_bttn_go_clicked()
{
    m_choosingNow = true;
}

//void TestForm::on_bttn_push_back_clicked()
//{
//    if (QListWidgetItem* item = m_ui->listWidget->currentItem()) {
//        m_containerModel->push_back(item->data(Qt::UserRole).value<QAbstractItemModel*>());
//    }
//}

//void TestForm::on_bttn_pop_back_clicked() {
//    m_containerModel->pop_back();
//}


void TestForm::mousePressEvent(QMouseEvent *e)
{
    if (m_choosingNow) {
        if (QWidget* child = childAt(e->pos())) {
            m_choosingNow = false;
            m_ui->bttn_go->setText("Go");
            static ns_model::ModelMapper *mapper = new ns_model::ModelMapper;
            connect(m_ui->bttn_go, &QPushButton::clicked, this, [this, child]{
                mapper->addSubscriber(child, m_ui->edit_row->value(), m_ui->edit_col->value(), m_ui->edit_role->value(), [this, child](const QVariant& d){
                    child->setProperty(m_ui->edit_property->text().toLatin1(), d);
                });
            });
        }
    }
    m_ui->label_2->setPixmap(grab(rect()));
    QWidget::mousePressEvent(e);
}

void TestForm::mouseReleaseEvent(QMouseEvent *e)
{

    QWidget::mouseReleaseEvent(e);
}

void TestForm::mouseMoveEvent(QMouseEvent *e)
{
    m_ui->value_x->setNum(e->x());
    m_ui->value_y->setNum(e->y());
    if (QWidget* child = childAt(e->x(), e->y()))
    {
        m_ui->value_classname->setText(child->metaObject()->className());
        m_ui->value_objectname->setText(child->objectName());
    }
    else
    {
        m_ui->value_classname->clear();
        m_ui->value_objectname->clear();
    }
    QWidget::mouseMoveEvent(e);
}


void addModelInfo(QAbstractItemModel* m, ModelInfo& modelsInfo)
{
    if (modelsInfo.find(m) == modelsInfo.end()) {
        modelsInfo.insert(m, QStringSet() << QString("class: %0").arg(m->metaObject()->className()));
    }
}

void TestForm::on_bttn_reparse_clicked()
{
    QList<QAbstractItemView*> lst = pApp->mainWindow()->findChildren<QAbstractItemView*>();
    ModelInfo modelsInfo;
    m_ui->listWidget->clear();

    for (QAbstractItemView *view: lst)
    {
        if (QAbstractItemModel* m = view->model()) {
            addModelInfo(m, modelsInfo);
            modelsInfo[m] << QString("view(class:%0, name:%1)").arg(view->metaObject()->className()).arg(view->objectName());
            while (QAbstractProxyModel* pm = qobject_cast<QAbstractProxyModel*>(m)) {
                m = pm->sourceModel();
                if (m) {
                    addModelInfo(m, modelsInfo);
                    modelsInfo[pm] << QString("source model: %0").arg(m->metaObject()->className());
                }
            }
        }
    }
    for (QAbstractItemModel* m : modelsDB().getModels()) {
        addModelInfo(m, modelsInfo);
        modelsInfo[m] << QString("from database");
        while (QAbstractProxyModel* pm = qobject_cast<QAbstractProxyModel*>(m)) {
            m = pm->sourceModel();
            if (m) {
                addModelInfo(m, modelsInfo);
                modelsInfo[pm] << QString("source model: %0").arg(m->metaObject()->className());
            }
        }
    }
    modelsInfo.remove(m_model.data());
    for (QAbstractItemModel* m : modelsInfo.keys())
    {
        QListWidgetItem* item = new QListWidgetItem(QString(m->metaObject()->className()) + " : " + m->objectName(), m_ui->listWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(m));

        QStringList lst = modelsInfo.value(m).toList();
        item->setData(Qt::UserRole+1, QVariant::fromValue(lst));
    }
}

void TestForm::on_bttn_setModel_toggled(bool toggle)
{
    if (!toggle) {
        return;
    }
    switchSourceModel();
}
void TestForm::on_bttn_singleColumn_toggled(bool toggle)
{
//    m_model->enableSingleColumn(toggle);
}

void TestForm::on_bttn_applyStyle_clicked()
{
    m_ui->tabWidget->setStyleSheet(m_ui->textBrowser->toPlainText());
}

void TestForm::on_listWidget_currentRowChanged(int)
{
    if (QListWidgetItem* item = m_ui->listWidget->currentItem()) {
        if (m_ui->bttn_setModel->isChecked()) {
            switchSourceModel();
            m_ui->tableView->horizontalHeader()->adjustSize();
            m_ui->tableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
        }
//        qDebug() << item->data(Qt::UserRole+1);
//        qDebug() << item->data(Qt::UserRole+1).toStringList();
        static_cast<QStringListModel*>(m_ui->listView->model())->setStringList(item->data(Qt::UserRole+1).toStringList());
    }
}

void TestForm::on_tableView_activated(const QModelIndex &index) {
    if (auto* m = m_ui->tableView->model())
    {
        if (m->hasChildren(index)) {
            m_ui->tableView->setRootIndex(index);
        }
    }
}

void TestForm::on_bttn_bg_toggled(bool /*checked*/)
{
    QString text;
    if (m_ui->bttn_bg->isChecked()) {
        text = QString("\nQTableView { background-color:%0 }").arg(m_ui->input_bg->text());
    }
    if (m_ui->bttn_alternate->isChecked()) {
        text += QString("\nQTableView { alternate-background-color:%0 }").arg(m_ui->input_alternate->text());
    }
    m_ui->tableView->setStyleSheet(text);
}
void TestForm::on_bttn_alternate_toggled(bool /*checked*/)
{
    QString text;
    if (m_ui->bttn_bg->isChecked()) {
        text = QString("\nQTableView { background-color:%0 }").arg(m_ui->input_bg->text());
    }
    if (m_ui->bttn_alternate->isChecked()) {
        text += QString("\nQTableView { alternate-background-color:%0 }").arg(m_ui->input_alternate->text());
    }
    m_ui->tableView->setStyleSheet(text);
}

void TestForm::on_bttn_back_clicked()
{
    m_ui->stackedWidget->setCurrentIndex(0);
}
void TestForm::on_bttn_next_clicked()
{
    m_ui->stackedWidget->setCurrentIndex(1);
}

void TestForm::on_input_filter_textChanged(const QString &text)
{
    QSortFilterProxyModel* secondProxy = qobject_cast<QSortFilterProxyModel*>(m_proxyModel->sourceModel());
    if (m_ui->bttn_enableRegExp->isChecked()) {
        secondProxy->setFilterRegExp(text);
    } else {
        secondProxy->setFilterFixedString(text);
    }
}

void TestForm::on_bttn_enableRegExp_toggled(bool toggled)
{
    if (toggled) {
        m_proxyModel->setFilterRegExp(m_ui->input_filter->text());
    } else {
        m_proxyModel->setFilterFixedString(m_ui->input_filter->text());
    }
}


void TestForm::on_bttn_save_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if (!filename.isNull() && !filename.isEmpty()) {
        QFile f(filename);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            modelsDB().logModel()->write(&f);
        }
    }
}

void TestForm::on_msgTypeFilterChanged(int mask, bool set)
{
    QStringList lst;
    if (m_ui->bttn_function->isChecked()) {
        lst << "Function";
    }
    if (m_ui->bttn_info->isChecked()) {
        lst << "Info";
    }
    if (m_ui->bttn_warnings->isChecked()) {
        lst << "Warning";
    }
    if (m_ui->bttn_critical->isChecked()) {
        lst << "Critical";
    }
    if (m_ui->bttn_fatal->isChecked()) {
        lst << "Fatal";
    }
    m_proxyModel->setFilterRole(UserType::FilterRole);
    m_proxyModel->setFilterKeyColumn(0);
    m_proxyModel->setFilterRegExp(QString("Msg(%0)").arg(lst.join("|")));
}

void TestForm::showEvent(QShowEvent *e)
{
    QDialog::showEvent(e);
    if (e->isAccepted()) {
        emit shown();
    }
}

void TestForm::on_TestForm_shown()
{
    m_ui->frame->hide();
}

void TestForm::on_bttn_l1_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if (!filename.isEmpty()) {
        int id = QFontDatabase::addApplicationFont(filename);
        if (id != -1) {
            m_ui->l1->setText(filename + "has been loaded");
            QFont fnt = QFont(QFontDatabase::applicationFontFamilies(id).last());
            fnt.setPixelSize(m_ui->input_fontSize->value());
            fnt.setWeight(m_ui->input_fontWeight->value());
            m_ui->l1->setFont(fnt);
        }
    }
}

void TestForm::on_input_fontSize_valueChanged(int arg1)
{
    QFont fnt = m_ui->l1->font();
    fnt.setPixelSize(arg1);
    m_ui->l1->setFont(fnt);
}

void TestForm::on_input_fontWeight_valueChanged(int arg1)
{
    QFont fnt = m_ui->l1->font();
    fnt.setWeight(arg1);
    m_ui->l1->setFont(fnt);
}

void TestForm::on_input_gameID_valueChanged(int arg1)
{
    GameId newGameID(arg1, m_ui->input_tournamentID->value());
    if (newGameID.isCash()) {
        QModelIndex index = modelsDB().getCashStatistic()->indexOf(newGameID.m_tableID);
        if (index.isValid()) {
            m_proxyTableModel->setGameID(newGameID);
        }
    }
}

void TestForm::on_input_tournamentID_valueChanged(int arg1)
{
    GameId newGameID(m_ui->input_gameID->value(), arg1);
    if (newGameID.isCash()) {
        QModelIndex index = modelsDB().getCashStatistic()->indexOf(newGameID.m_tableID);
        if (index.isValid()) {
            m_proxyTableModel->setGameID(newGameID);
        }
    }
}

void TestForm::on_bttn_load_clicked()
{

}

void TestForm::switchSourceModel()
{
    if (QListWidgetItem* item = m_ui->listWidget->currentItem()) {
        auto* m = item->data(Qt::UserRole).value<QAbstractItemModel*>();
        if (ns_model::table_state_t* tableModel = qobject_cast<ns_model::table_state_t*>(m)) {
            ns_model::CommonGameTable* commonGameTable = new ns_model::CommonGameTable;
            commonGameTable->setGameID(GameId(tableModel->origin()._game_id, tableModel->origin()._tournament_id));
            m = commonGameTable;
        }
        m_model->setSourceModel(m);
    }
}
