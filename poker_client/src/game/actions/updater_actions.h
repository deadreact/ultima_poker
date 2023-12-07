#pragma once

#include <tools/sequentalactionqueue.h>
#include <functional>
#include <tools/types_fwd.hpp>

namespace ns_action {
class UpdateWaiters;
} // namespace ns_action


template <int paramNo>
inline gbp_u16 get(ns_action::UpdateWaiters* action);


namespace ns_action
{

    class UpdateWaiters : public Action
    {
        Q_OBJECT

        gbp_i64 m_cashID;
        gbp_u16 m_actualWaitersCount;
        gbp_u16 m_actualUserPosInQueue;
    public:
        enum class Type : int {
            WaitersCount = 0,
            UserPosInQueue = 1
        };

    protected:
        bool updateStatistic();
        bool updateUser();
        bool updateTable();
        bool updateViews();
    public:
        UpdateWaiters(gbp_i64 cashID, QObject* parent = nullptr);
        virtual ~UpdateWaiters();

        void setWaitersCount(gbp_u16 count);
        void setUserPosInQueue(gbp_u16 pos);
        inline gbp_u16 waitersCount() const { return m_actualWaitersCount; }
        inline gbp_u16 userPosInQueue() const { return m_actualUserPosInQueue; }

        template <typename Obj>
        QMetaObject::Connection connectView(Obj* view, typename QtPrivate::FunctionPointer<void(Obj::*)()>::Function fn) {
            return connect(this, &UpdateWaiters::done, view, fn);
        }
        template <typename Obj>
        QMetaObject::Connection connectView(Obj* view, typename QtPrivate::FunctionPointer<void(Obj::*)(bool)>::Function fn) {
            return connect(this, &UpdateWaiters::done, view, fn);
        }
        template <typename Obj, Type param = Type::WaitersCount>
        QMetaObject::Connection connectView(Obj* view, typename QtPrivate::FunctionPointer<void(Obj::*)(int)>::Function fn) {
            return connect(this, &UpdateWaiters::done, view, [view, this, &fn](bool){
                std::bind(fn, view, get<int(param)>(this))();
            });
        }
        template <typename Obj, Type param = Type::WaitersCount>
        QMetaObject::Connection connectView(Obj* view, typename QtPrivate::FunctionPointer<void(Obj::*)(bool, int)>::Function fn) {
            return connect(this, &UpdateWaiters::done, view, [&](bool res){
                std::bind(fn, view, res, get<int(param)>(this))();
            });
        }

        virtual void execute() override;
    };


} // namespace ns_action
