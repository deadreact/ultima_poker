#include <precompiled.h>
#include "page_SettingsCommon.h"
#include "ui_page_SettingsCommon.h"
#include <utils/Utils.h>
#include <PokerApplication.h>
#include <basewidgets/models/model_timezone.h>
#include <widgets/MainWindow.h>
#include <styles/shadowedtextstyle.h>
#include <qstyleditemdelegate.h>

namespace ns_page
{
QStringList SettingsCommon::m_sounds =  { "Fold"
                                        , "Check"
                                        , "Sound call"
                                        , "Bet"
                                        , "Rise"
                                        , "Round end"
                                        , "Win Pot"
                                        , "Dealing Cards"
                                        , "Shuffle Deck"
                                        , "Your turn"
                                        , "Wake up"};


    SettingsCommon::SettingsCommon(QWidget *parent)
        : SettingsBase(parent)
        , m_ui(new Ui::SettingsCommon)
        , m_groupPlaySound(new QButtonGroup)
        , m_localizer(nullptr)
    {
        m_ui->setupUi(this);

        m_ui->listWidget->addItems(m_sounds);
        for (int i = 0; i < m_sounds.size(); i++) {
            QPushButton* bttn = new QPushButton("", m_ui->listWidget);
            bttn->setCheckable(true);
            bttn->setFixedSize(28, 28);
//            m_ui->listWidget->setIndexWidget(m_ui->listWidget->model()->index(i, 0), bttn);
            m_ui->listWidget->item(i)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
            m_ui->listWidget->item(i)->setCheckState(Qt::Checked);
//            bttn->;
            bttn->show();
            bttn->raise();
            bttn->move(383 + 0, i*30 + 1);
            m_groupPlaySound->addButton(bttn, i);
        }
        FramedButton* bttn = new FramedButton(m_ui->listWidget);
        m_groupPlaySound->addButton(bttn, -1);
        bttn->show();
        bttn->setBackgroundColor(QColor(123, 123, 123, 12));
        bttn->setCheckable(true);
    }

    SettingsCommon::~SettingsCommon() {}


    void SettingsCommon::init()
    {
        if (isInitialized()) {
            return;
        }

        connect(m_groupPlaySound, (void(QButtonGroup::*)(QAbstractButton*))&QButtonGroup::buttonClicked, this, [this](QAbstractButton* clickedBttn) {
            QAbstractButton* cap = m_groupPlaySound->button(-1);

            if (clickedBttn == cap) {
                cap->hide();
            } else {
                cap->setGeometry(clickedBttn->geometry());
                cap->show();
                cap->raise();
            }
        });

        createTimezoneMenu(pApp->mainWindow()->getModelTimeZone());
        createLanguageMenu(pApp->mainWindow()->localizer());

        QFile f("tmp_file.txt");
        SettingsCommonPageData d;
        QDataStream os(&f);

        if (f.exists()) {
            if (f.open(QIODevice::ReadOnly)) {
                os >> d;
                f.close();
            }
        } else {
            if (f.open(QIODevice::WriteOnly)) {
                os << d;
                f.close();
            }
        }

        static const QRegularExpression re("(^(msg_|filter_|title_|text_|bttn_)\\w+$)");
        QWidgetList lst = m_ui->rightPart->findChildren<QWidget*>(re);
        lst << m_ui->subpage_sound->findChildren<QWidget*>(re);

        for (QWidget* w: lst) {
            setTextDropShadowStyle(w, QPoint(0, -1), QColor(0x191919), E::Blending::Normal);
        }
        setTextDropShadowStyle(m_ui->bttn_apply, QPoint(0, 1), QColor(0, 0, 0, 255*0.4), E::Blending::Normal);
        updateTexts();
        importData(d);

        onInitialized();
    }

    void SettingsCommon::importData(const SettingsCommonPageData &d)
    {
        m_ui->bttn_enableSound->setChecked(d.soundEnabled);
        m_ui->slider_volume->setValue(d.volume);
        for (int i = 0; i < m_ui->listWidget->count(); i++) {
            m_ui->listWidget->item(i)->setCheckState((d.sfxEnable & (1 << i)) ? Qt::Checked : Qt::Unchecked);
        }
        m_ui->bttn_rememberLogin->setChecked(d.memLogin);
        m_ui->bttn_rememberPassword->setChecked(d.memPassword);
        m_ui->bttn_turnOffWhenInactive->setChecked(d.useTimeout);
        m_ui->spinBox_turnOffWhenInactive->setEnabled(d.useTimeout);
        m_ui->spinBox_turnOffWhenInactive->setValue(d.timeoutMinutes);
        m_ui->comboBox_timezone->setCurrentText(QTimeZone(d.timezoneID).displayName(QTimeZone::GenericTime, QTimeZone::DefaultName, locale()));
        int idx = m_ui->comboBox_language->findData(d.lang);

        m_ui->comboBox_language->setCurrentIndex(idx);
    }

    void SettingsCommon::exportData(SettingsCommonPageData &d)
    {
        d.soundEnabled  = m_ui->bttn_enableSound->isChecked();
        d.volume        = m_ui->slider_volume->value();
        d.sfxEnable = 0;
        for (int i = 0; i < m_ui->listWidget->count(); i++) {
            d.sfxEnable |= (m_ui->listWidget->item(i)->checkState() == Qt::Checked ? 1 : 0) << i;
        }
        d.memLogin       = m_ui->bttn_rememberLogin->isChecked();
        d.memPassword    = m_ui->bttn_rememberPassword->isChecked();
        d.useTimeout     = m_ui->bttn_turnOffWhenInactive->isChecked();
        d.timeoutMinutes = m_ui->spinBox_turnOffWhenInactive->value();
        d.timezoneID     = m_ui->comboBox_timezone->currentTimeZoneId();
        d.lang           = m_ui->comboBox_language->currentData().toString();
    }

    void SettingsCommon::load()
    {
        QFile f("common.txt");

        if (f.exists())
        {
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream is(&f);
                is >> m_data;
                f.close();
            }
        }
        else
        {
            if (f.open(QIODevice::WriteOnly)) {
                QDataStream os(&f);
                os << m_data;
                f.close();
            }
        }
        importData(m_data);

        ns_utildb::saveState("Language", m_data.lang);
        ns_utildb::saveState("CurrentTimeZone", m_data.timezoneID);
        m_localizer->language(m_data.lang)->setChecked(true);
        m_ui->comboBox_timezone->acceptCurrentTimezone();

        m_modified = false;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

    int SettingsCommon::indexOfLang(const QString &lang) const
    {
        for (int i = 0; i < m_ui->comboBox_language->count(); i++) {
            if (m_ui->comboBox_language->itemData(i).toString() == lang) {
                return i;
            }
        }
        return -1;
    }

    int SettingsCommon::indexOfLang(QAction *lang) const {
        return (lang) ? indexOfLang(lang->data().toString()) : -1;
    }

    QAction *SettingsCommon::languageAction(const QString &lang) const {
        return m_localizer->language(lang);
    }

    QString SettingsCommon::restoreLanguage() const
    {
        QString lang;
        ns_utildb::restoreState("Language", lang);
        return lang;
    }


    //    }

    void SettingsCommon::createTimezoneMenu(ns_model::Timezone* model)
    {
        m_ui->comboBox_timezone->setModel(model);
        m_ui->comboBox_timezone->view()->setItemDelegate(new QStyledItemDelegate);

        m_ui->comboBox_timezone->view()->setCurrentIndex(model->current());

        connect(model, &ns_model::Timezone::dataChanged, this, [this, model]{
             m_ui->comboBox_timezone->view()->setCurrentIndex(model->current());
             m_ui->comboBox_timezone->setCurrentIndex(model->current().row());
        });
    }
    void SettingsCommon::createLanguageMenu(Localizer *localizer)
    {
        m_localizer = localizer;

        for (QAction* lang: m_localizer->languageList()) {
             m_ui->comboBox_language->addItem(lang->icon(), lang->text(), lang->data());
             if (lang->isChecked()) {
                 m_ui->comboBox_language->setCurrentText(lang->text());
             }
        }

        connect(localizer->languageGroup(), &QActionGroup::triggered, this, [this](QAction* a){
            int index  = m_ui->comboBox_language->findText(a->text());
            m_ui->comboBox_language->setCurrentIndex(index);
//            m_localizer->changeLanguage(a);
        });

    }

    void SettingsCommon::updateTexts()
    {
        m_ui->retranslateUi(this);


    }

    void SettingsCommon::updateIsModified()
    {
        if (!isInitialized()) {
            return;
        }
        SettingsCommonPageData d;
        exportData(d);

        m_modified = m_data != d;
        m_ui->footer->setEnabled(m_modified);
        setWindowModified(m_modified);
    }

    void SettingsCommon::apply()
    {
        ns_utildb::saveState("CurrentTimeZone", m_ui->comboBox_timezone->currentData().toByteArray());
        m_ui->comboBox_timezone->acceptCurrentTimezone();

        for (QAction* lang: m_localizer->languageList()) {
            if (lang->text() == m_ui->comboBox_language->currentText()) {
                lang->toggle();
                lang->trigger();
                //qDebug() << lang << "TRIGGERED";
                break;
            }
        }

        QFile f("tmp_file.txt");
        if (f.open(QIODevice::WriteOnly)) {
            SettingsCommonPageData d;
            exportData(d);
            QDataStream os(&f);
            os << d;
            f.close();

            emit applied();
        }
    }

    SettingsCommonPageData::SettingsCommonPageData()
    {
        // default values
        soundEnabled   = true;
        volume         = 500;
        sfxEnable      = 0b11111111111;
        memLogin       = true;
        memPassword    = true;
        useTimeout     = false;
        timeoutMinutes = 60;

        timezoneID = QTimeZone::systemTimeZoneId();
        lang       = "en";
    }

    bool SettingsCommonPageData::operator==(const SettingsCommonPageData &d) const
    {
        return commonFlags == d.commonFlags && timezoneID == d.timezoneID && lang == d.lang;
    }

    
} //namespace ns_page


QDataStream &operator<<(QDataStream &os, const ns_page::SettingsCommonPageData &data)
{
    os << data.lang << data.timezoneID << data.commonFlags;
    return os;
}

QDataStream &operator>>(QDataStream &is, ns_page::SettingsCommonPageData &data)
{
    is >> data.lang >> data.timezoneID >> data.commonFlags;
    return is;
}
