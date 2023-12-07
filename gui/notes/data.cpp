#include "data.h"
#include "model_io.h"
#include "model_markers.h"
#include "model_notes.h"

namespace ns_model
{
/* --------------------------------------------------------------------------- */
namespace ns_data
{
    template <>
    struct Item<Markers>
    {
        static uint sm_idIncrementer;
        uint id;
        QColor m_color;
        QString name;

        Item(): id(sm_idIncrementer++), m_color(Qt::transparent), name("No marker")
        {}
        Item(uint id): id(id), m_color(Qt::transparent), name("No marker")
        {
            if (sm_idIncrementer <= id) {
                sm_idIncrementer = id + 1;
            }
        }
        Item(const Item<Markers>& item)
            : id(item.id), m_color(item.m_color), name(item.name)
        {}
        Item(const QColor& color, const QString& name)
            : id(sm_idIncrementer++), m_color(color), name(name)
        {}

        inline QColor color() const { return m_color; }
        void setColor(const QColor& col) {
            m_color = col;
        }

        static const Item<Markers>& invalid()
        {
            int val = sm_idIncrementer;
            static const Item<Markers> invalid(-1);
            sm_idIncrementer = val;
            return invalid;
        }
    };
    uint Item<Markers>::sm_idIncrementer = 0;


    template <>
    struct Item<Notes>
    {
        uint markerId;
        QString nickname;
        QString note;
        QDateTime modified;
    };





} // namespace ns_data
/* --------------------------------------------------------------------------- */
} // namespace ns_model
