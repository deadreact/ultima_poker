#pragma once

#include <basewidgets/entitywidget.h>
#include <widgets/widget_ProfilePicture.h>
#include <models/model_AvatarsList.h>
#include <QAbstractButton>
#include <qgraphicseffect.h>
#include <qpropertyanimation.h>
#include <utils/Utils.h>
#include <const.h>

namespace Ui {
class SelectAvatar;
}

//---------------------------------------------------------
    struct Element {
        ns_widget::ProfilePicture* m_element;
        Element* m_prev;
        Element* m_next;
        Element(ns_widget::ProfilePicture* el): m_element(el), m_prev(nullptr), m_next(nullptr) {}

        Element* next(unsigned int count = 1) {
            if (count == 1) {
                return m_next;
            }
            return m_next->next(count-1);
        }
        Element* prev(unsigned int count = 1) {
            if (count == 1) {
                return m_prev;
            }
            return m_prev->next(count-1);
        }
    };
//---------------------------------------------------------
    struct List {
        Element* m_first;
        Element* m_last;
        ns_model::AvatarsList* m_model;
        int m_modelOffset;

        List(ns_model::AvatarsList* model);
        Element* indexElement(int index) const;
        QPropertyAnimation* adjustPositions(int xPos = 0, int gridXSize = 120, int msec = 300);

        inline void cycleLeft();
        inline void cycleRight();
    };

    inline void List::cycleLeft() {
        m_modelOffset--;
        Element* target = m_first;
        m_first = target->next();
        target->m_next = nullptr;
        target->m_prev = m_last;
        m_last->m_next = target;
        m_last = target;
        m_first->m_prev = nullptr;
        target->m_element->setIcon(m_model->icon(m_modelOffset+6));
    }

    inline void List::cycleRight() {
        m_modelOffset++;
        Element* lastToFirst = m_last;
        Element* newLast = m_last->m_prev;
        Element* firstToSecond = m_first;

        lastToFirst->m_prev = nullptr;
        newLast->m_next = nullptr;
        firstToSecond->m_prev = lastToFirst;
        lastToFirst->m_next = firstToSecond;
        m_first = lastToFirst;
        m_last = newLast;
        lastToFirst->m_element->setIcon(m_model->icon(m_modelOffset));
    }

//---------------------------------------------------------
//---------------------------------------------------------
    class AvatarsListWidget : public EntityWidget
    {
        Q_OBJECT
    private:
        //    Ui::AvatarsList* m_ui;
        List* m_lst;
        ns_widget::ProfilePicture* m_pressedItem;
    public:
        explicit AvatarsListWidget(QWidget *parent = 0);
        ~AvatarsListWidget();
        void init(Ui::SelectAvatar *ui);

        constexpr inline static int gridWidth() {
            return ns_const::ns_avatar_size::base.width();
        }

        inline QRect bigRect() const;
        inline QRect smallRect() const;
        inline QRect microRect() const;

        static QRect quadInCenterOf(int radius, int w, int h);


        ns_widget::ProfilePicture* avatarAt(const QPoint& pos) const;
        inline ns_widget::ProfilePicture* avatarAt(int x, int y) const;
    signals:
        void avatarClicked(const Avatar&);
    public slots:

    protected:
        void setShadowEffect(QWidget* w);
        virtual void mousePressEvent(QMouseEvent *e) override;
        virtual void mouseReleaseEvent(QMouseEvent *e) override;
        virtual void mouseMoveEvent(QMouseEvent *e) override;
        virtual void wheelEvent(QWheelEvent *e) override;
        virtual void enterEvent(QEvent *e) override;
        virtual void leaveEvent(QEvent *e) override;

        void onItemClicked(ns_widget::ProfilePicture* item, bool emitSignal = true);

        // QWidget interface
    protected:
        virtual void showEvent(QShowEvent *e) override;
    };

    inline ns_widget::ProfilePicture* AvatarsListWidget::avatarAt(int x, int y) const {
        return avatarAt(QPoint(x, y));
    }

    inline QRect AvatarsListWidget::bigRect() const {
        return quadInCenterOf(ns_const::ns_avatar_size::baseRadius, gridWidth(), height());
    }
    inline QRect AvatarsListWidget::smallRect() const {
        return quadInCenterOf(ns_const::ns_avatar_size::smallRadius, gridWidth(), height());
    }
    inline QRect AvatarsListWidget::microRect() const {
        return quadInCenterOf(ns_const::ns_avatar_size::microRadius, gridWidth(), height());
    }
