#ifndef MACRO_TABLE_STATE_HPP
#define MACRO_TABLE_STATE_HPP

#include <boost/type_traits/has_not_equal_to.hpp>

#define FOR_EACH_ELEMENT_OF_TABLE_PLAYER(F, ...) \
    F(_id, __VA_ARGS__);                         \
    F(_is_user, __VA_ARGS__);                    \
    F(_pos, __VA_ARGS__);                        \
    F(_nickname, __VA_ARGS__);                   \
    F(_country, __VA_ARGS__);                    \
    F(_money, __VA_ARGS__);                      \
    F(_start_stack, __VA_ARGS__);                \
    F(_bounty_progressive_money, __VA_ARGS__);   \
    F(_hash_avatar, __VA_ARGS__);                \
    F(_is_dealer, __VA_ARGS__);                  \
    F(_cards, __VA_ARGS__);                      \
    F(_round_bet, __VA_ARGS__);                  \
    F(_is_current, __VA_ARGS__);                 \
    F(_state, __VA_ARGS__)

#define FOR_EACH_ELEMENT(F, ...) \
    F(_tournament_id, __VA_ARGS__);\
    F(_game_id, __VA_ARGS__);\
    F(_big_blind, __VA_ARGS__);\
    F(_seats_count, __VA_ARGS__);\
    F(_players, __VA_ARGS__);\
    F(_current_user_pos, __VA_ARGS__);\
    F(_table_cards, __VA_ARGS__);\
    F(_all_pots, __VA_ARGS__);\
    F(_waiters_count, __VA_ARGS__);\
    F(_time_short, __VA_ARGS__);\
    F(_time_main, __VA_ARGS__);\
    F(_time_action_passed, __VA_ARGS__);\
    F(_time_action_left, __VA_ARGS__);\
    F(_distribution_number, __VA_ARGS__)

#define FOR_EACH_ELEMENT_COMMA(F, ...) \
    F(_tournament_id, __VA_ARGS__),\
    F(_game_id, __VA_ARGS__),\
    F(_big_blind, __VA_ARGS__),\
    F(_seats_count, __VA_ARGS__),\
    F(_players, __VA_ARGS__),\
    F(_current_user_pos, __VA_ARGS__),\
    F(_table_cards, __VA_ARGS__),\
    F(_all_pots, __VA_ARGS__),\
    F(_waiters_count, __VA_ARGS__),\
    F(_time_short, __VA_ARGS__),\
    F(_time_main, __VA_ARGS__),\
    F(_time_action_passed, __VA_ARGS__),\
    F(_time_action_left, __VA_ARGS__),\
    F(_distribution_number, __VA_ARGS__)

#define INSERT_DELEGATE(elem, scope, container, func) container.insert(scope::elem, func<scope::elem>)
#define DECLARE_ELEMENT_PTR(name, owner_type) decltype(owner_type::name)* name
#define DECLARE_ELEMENT_REF(name, owner_type) decltype(owner_type::name)& name
#define DECLARE_ELEMENT(name, owner_type) decltype(owner_type::name) name
#define DECLARE_ELEMENT_CONST_REF(name, owner_type) const decltype(owner_type::name)& name
#define DECLARE_GETTER(name, ...) const decltype(name)& get##name() const { return name; }
#define DECLARE_SETTER(name, ...) bool set##name(const decltype(name)& other) { if (!comparator<decltype(name)>::cmp(other, name)) { name = other; return true;} return false; }
#define COPY_REF_TO_PTR(name, source_of_ref, source_of_ptr) source_of_ptr.name = &source_of_ref.name
#define INIT_ELEMENT(name, source) name(source.name)

#define DECLARE_QPROPERTY(_name, source_type_name) \
Q_PROPERTY(decltype(source_type_name::_name) _name MEMBER _name WRITE set##_name NOTIFY _name##Changed) \
public:\
decltype(source_type_name::_name) _name;\
Q_SIGNAL void _name##Changed(const decltype(source_type_name::_name)&); \
Q_SLOT void set##_name(const decltype(source_type_name::_name)& v) { if (v != _name) {_name = v; emit _name##Changed(v);} }

namespace
{

    template <typename T, bool is = boost::is_fundamental<T>::value>
    struct comparator
    {
        static bool cmp(const T& obj1, const T& obj2) {
            return (obj1 == obj2);
        }
    };

    template <typename T>
    struct comparator<T, false>
    {
        static bool cmp(const T&, const T&) {
            return false;
        }
    };
} //namespace



#define DECLARE_ACCESSOR_STRUCT(macro_declare_type, struct_type_name, source_type_name) \
struct struct_type_name { \
    Q_GADGET \
    using source_type = source_type_name; \
    FOR_EACH_ELEMENT(DECLARE_QPROPERTY, source_type_name) \
    struct_type_name(source_type_name& source) \
    : FOR_EACH_ELEMENT_COMMA(INIT_ELEMENT, source) \
    {} \
    FOR_EACH_ELEMENT(DECLARE_GETTER) \
}



#endif // MACRO_TABLE_STATE_HPP
