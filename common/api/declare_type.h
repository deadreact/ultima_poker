
#ifndef _gbp__api__declare_type
#define _gbp__api__declare_type

#ifndef GBP_DECLARE_TYPE_DISABLE_DECLARE_TYPE

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/tuple/enum.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/comparison/less.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

#include <boost/type_traits/function_traits.hpp>

#include <api/declare_type/decorators.hpp>
#include <api/declare_type/list.hpp>
#include <api/declare_type/map.hpp>
#include <api/declare_type/pair.hpp>
#include <api/declare_type/quoting.hpp>
#include <api/declare_type/set.hpp>
#include <api/declare_type/tuple.hpp>
#include <api/declare_type/unordered_map.hpp>
#include <api/declare_type/unordered_set.hpp>
#include <api/declare_type/vector.hpp>

#include <functional>

/***************************************************************************/

#define GBP_DECLARE_TYPE_WRAP_X(...) ((__VA_ARGS__)) GBP_DECLARE_TYPE_WRAP_Y
#define GBP_DECLARE_TYPE_WRAP_Y(...) ((__VA_ARGS__)) GBP_DECLARE_TYPE_WRAP_X
#define GBP_DECLARE_TYPE_WRAP_X0
#define GBP_DECLARE_TYPE_WRAP_Y0

#define GBP_DECLARE_TYPE_DECLARE_MEMBER_WITH_INIT(elem) \
    boost::function_traits< \
        void BOOST_PP_TUPLE_ELEM(1, elem) \
    >::arg1_type BOOST_PP_TUPLE_ELEM(0, elem) \
        { BOOST_PP_TUPLE_ENUM(BOOST_PP_TUPLE_ELEM(2, elem)) };

#define GBP_DECLARE_TYPE_DECLARE_MEMBER_WITHOUT_INIT(elem) \
    boost::function_traits< \
        void BOOST_PP_TUPLE_ELEM(1, elem) \
    >::arg1_type BOOST_PP_TUPLE_ELEM(0, elem){};

#define GBP_DECLARE_TYPE_DECLARE_MEMBER(unused0, unused1, idx, elem) \
    BOOST_PP_IF( \
         BOOST_PP_EQUAL(BOOST_PP_TUPLE_SIZE(elem), 3) \
        ,GBP_DECLARE_TYPE_DECLARE_MEMBER_WITH_INIT \
        ,GBP_DECLARE_TYPE_DECLARE_MEMBER_WITHOUT_INIT \
    )(elem)

#define GBP_DECLARE_TYPE_ENUMERATE_SERIALIZED(unused0, unused1, idx, elem) \
    & BOOST_PP_TUPLE_ELEM(0, elem)

#define GBP_DECLARE_TYPE_ENUMERATE_OUTPUTED(unused0, unused1, idx, elem) \
    s << BOOST_PP_IF(BOOST_PP_EQUAL(idx, 0), , ",") \
        "\"" BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem)) "\":"; \
    quoting(s, o.BOOST_PP_TUPLE_ELEM(0, elem));

#define GBP_DECLARE_TYPE_ENUM_CTOR_ARGS_CB(unused0, unused1, idx, elem) \
    BOOST_PP_COMMA_IF(idx) \
        const boost::function_traits< \
            void BOOST_PP_TUPLE_ELEM(1, elem) \
        >::arg1_type & arg##idx

#define GBP_DECLARE_TYPE_ENUM_CTOR_ARGS(seq) \
    BOOST_PP_SEQ_FOR_EACH_I( \
            GBP_DECLARE_TYPE_ENUM_CTOR_ARGS_CB \
        ,~ \
        ,seq \
    )

#define GBP_DECLARE_TYPE_ENUM_CTOR_INIT_LIST_CB_COLON(idx, elem) \
    :BOOST_PP_TUPLE_ELEM(0, elem){arg##idx}

#define GBP_DECLARE_TYPE_ENUM_CTOR_INIT_LIST_CB_COMMA(idx, elem) \
    ,BOOST_PP_TUPLE_ELEM(0, elem){arg##idx}

#define GBP_DECLARE_TYPE_ENUM_CTOR_INIT_LIST_CB(unused0, unused1, idx, elem) \
    BOOST_PP_IF( \
         idx \
        ,GBP_DECLARE_TYPE_ENUM_CTOR_INIT_LIST_CB_COMMA \
        ,GBP_DECLARE_TYPE_ENUM_CTOR_INIT_LIST_CB_COLON \
    )(idx, elem)

#define GBP_DECLARE_TYPE_ENUM_CTOR_INIT_LIST(seq) \
    BOOST_PP_SEQ_FOR_EACH_I( \
         GBP_DECLARE_TYPE_ENUM_CTOR_INIT_LIST_CB \
        ,~ \
        ,seq \
    )

/***************************************************************************/

#ifndef GBP_DECLARE_TYPE_GEN_ADDITIONALS
#   define GBP_DECLARE_TYPE_GEN_TUPLE_ELEMS(tuplename, seq)
#else
#   define GBP_DECLARE_TYPE_GEN_TUPLE_ELEMS_CB(unused0, unused1, idx, elem) \
        BOOST_PP_COMMA_IF(idx) \
            typename boost::function_traits<void BOOST_PP_TUPLE_ELEM(1, elem)>::arg1_type

#   define GBP_DECLARE_TYPE_GEN_TUPLE_ELEMS_IMPL(seq) \
        BOOST_PP_SEQ_FOR_EACH_I( \
            GBP_DECLARE_TYPE_GEN_TUPLE_ELEMS_CB \
            ,~ \
            ,seq \
        )

#   define GBP_DECLARE_TYPE_GEN_TUPLE_ELEMS(tuplename, seq) \
        using tuplename = std::tuple<GBP_DECLARE_TYPE_GEN_TUPLE_ELEMS_IMPL(seq)>;

#endif // GBP_DECLARE_TYPE_NO_GEN_ADDITIONALS_FLAG()

/***************************************************************************/

#ifndef GBP_DECLARE_TYPE_GEN_ADDITIONALS
#   define GBP_DECLARE_TYPE_GEN_APPLY_FUNCTIONS(seq)
#else
#   define GBP_DECLARE_TYPE_ENUMERATE_APPLIED(unused0, unused1, unused2, elem) \
        f(BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem)), BOOST_PP_TUPLE_ELEM(0, elem));

#   define GBP_DECLARE_TYPE_GEN_APPLY_FUNCTIONS(seq) \
        template<typename F> /* void(const char *memname, const T &v) */ \
        void apply(F &&f) const { \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 GBP_DECLARE_TYPE_ENUMERATE_APPLIED \
                ,~ \
                ,seq \
            ) \
        } \
        template<typename F> /* void(const char *memname, T &v) */ \
        void apply(F &&f) { \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 GBP_DECLARE_TYPE_ENUMERATE_APPLIED \
                ,~ \
                ,seq \
            ) \
        }

#endif // GBP_DECLARE_TYPE_NO_GEN_ADDITIONALS_FLAG()

/***************************************************************************/

#ifndef GBP_DECLARE_TYPE_GEN_ADDITIONALS
#   define GBP_DECLARE_TYPE_GEN_OPERATOR_EQUAL_NOTEQUAL(tname, seq)
#else
#   define GBP_DECLARE_TYPE_ENUMERATE_EQUAL(unused0, unused1, idx, elem) \
        BOOST_PP_IF(idx, &&, ) \
            BOOST_PP_TUPLE_ELEM(0, elem) == r.BOOST_PP_TUPLE_ELEM(0, elem)

#   define GBP_DECLARE_TYPE_GEN_OPERATOR_EQUAL_NOTEQUAL(tname, seq) \
        bool operator== (const tname &r) const { \
            return \
                BOOST_PP_SEQ_FOR_EACH_I( \
                     GBP_DECLARE_TYPE_ENUMERATE_EQUAL \
                    ,~ \
                    ,seq \
                ) \
            ; \
        } \
        bool operator!= (const tname &r) const { \
            return !this->operator==(r); \
        }

#endif // GBP_DECLARE_TYPE_NO_GEN_ADDITIONALS_FLAG()

/***************************************************************************/

#ifndef GBP_DECLARE_TYPE_GEN_ADDITIONALS
#   define GBP_DECLARE_TYPE_GEN_GET_MEMBERS(seq)
#else
#   define GBP_DECLARE_TYPE_ENUMERATE_MEMBERS_CB(unused0, unused1, idx, elem) \
        BOOST_PP_COMMA_IF(idx) \
            std::ref(BOOST_PP_TUPLE_ELEM(0, elem))

#   define GBP_DECLARE_TYPE_ENUMERATE_MEMBERS(seq) \
        BOOST_PP_SEQ_FOR_EACH_I( \
            GBP_DECLARE_TYPE_ENUMERATE_MEMBERS_CB \
            ,~ \
            ,seq \
        )

#   define GBP_DECLARE_TYPE_GEN_GET_MEMBERS(seq) \
        template< \
             std::size_t I \
            ,typename T = typename std::tuple_element<I, types_as_tuple>::type> \
        T& get_member() { \
            auto t = std::make_tuple(GBP_DECLARE_TYPE_ENUMERATE_MEMBERS(seq)); \
            return std::get<I>(t); \
        } \
        template< \
             std::size_t I \
            ,typename T = typename std::tuple_element<I, types_as_tuple>::type> \
        const T& get_member() const { \
            auto t = std::make_tuple(GBP_DECLARE_TYPE_ENUMERATE_MEMBERS(seq)); \
            return std::get<I>(t); \
        }

#endif // GBP_DECLARE_TYPE_NO_GEN_ADDITIONALS_FLAG()

/***************************************************************************/

#ifndef GBP_DECLARE_TYPE_GEN_ADDITIONALS
#   define GBP_DECLARE_TYPE_GEN_COMPARE(tname, seq)
#else
#   define GBP_DECLARE_TYPE_ENUMERATE_COMPARE(unused0, unused1, unused2, elem) \
        if ( BOOST_PP_TUPLE_ELEM(0, elem) != r.BOOST_PP_TUPLE_ELEM(0, elem) ) { \
            res = true; \
            f( \
                BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, elem)) \
                ,BOOST_PP_TUPLE_ELEM(0, elem) \
                ,r.BOOST_PP_TUPLE_ELEM(0, elem) \
            ); \
        }

#   define GBP_DECLARE_TYPE_GEN_COMPARE(tname, seq) \
        template<typename F> /* void(const char *memname, const T &l, const T &r) */ \
        bool compare(const tname &r, F &&f) const { \
            bool res = false; \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 GBP_DECLARE_TYPE_ENUMERATE_COMPARE \
                ,~ \
                ,seq \
            ) \
            return res; \
        } \
        template<typename F> /* void(const char *memname, T &l, const T &r) */ \
        bool compare(const tname &r, F &&f) { \
            bool res = false; \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 GBP_DECLARE_TYPE_ENUMERATE_COMPARE \
                ,~ \
                ,seq \
            ) \
            return res; \
        }

#endif // GBP_DECLARE_TYPE_NO_GEN_ADDITIONALS_FLAG()

/***************************************************************************/

#define GBP_DECLARE_TYPE_IMPL(tname, seq, ...) \
    struct tname { \
        /* user code expanded here */ \
        __VA_ARGS__ \
        \
        tname() = default; \
        \
        tname(GBP_DECLARE_TYPE_ENUM_CTOR_ARGS(seq)) \
            GBP_DECLARE_TYPE_ENUM_CTOR_INIT_LIST(seq) \
        {} \
        \
        BOOST_PP_SEQ_FOR_EACH_I( \
             GBP_DECLARE_TYPE_DECLARE_MEMBER \
            ,~ \
            ,seq \
        ) \
        \
        GBP_DECLARE_TYPE_GEN_TUPLE_ELEMS(types_as_tuple, seq); \
        \
        GBP_DECLARE_TYPE_GEN_APPLY_FUNCTIONS(seq) \
        \
        GBP_DECLARE_TYPE_GEN_OPERATOR_EQUAL_NOTEQUAL(tname, seq) \
        \
        GBP_DECLARE_TYPE_GEN_GET_MEMBERS(seq) \
        \
        GBP_DECLARE_TYPE_GEN_COMPARE(tname, seq) \
        \
        template<typename Archive> \
        void serialize(Archive &ar) { \
            ar \
                BOOST_PP_SEQ_FOR_EACH_I( \
                     GBP_DECLARE_TYPE_ENUMERATE_SERIALIZED \
                    ,~ \
                    ,seq \
                ) \
            ; \
        } \
        \
        friend std::ostream& operator<< (std::ostream &s, const tname &o) { \
            s << '{'; \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 GBP_DECLARE_TYPE_ENUMERATE_OUTPUTED \
                ,~ \
                ,seq \
            ) \
            s << '}'; \
            return s; \
        } \
        \
        std::string to_string() const { \
            std::ostringstream os; \
            os << *this; \
            return os.str(); \
        } \
    };

#define GBP_DECLARE_TYPE( \
     name /* type name */ \
    ,seq /* types sequence */ \
    ,... /* user code */ \
) \
    GBP_DECLARE_TYPE_IMPL( \
            name \
        ,BOOST_PP_CAT(GBP_DECLARE_TYPE_WRAP_X seq, 0) \
        ,__VA_ARGS__ \
    )

/***************************************************************************/

#define GBP_DECLARE_ENUM_DECLARE_ENUM_MEMBERS_WITH_INIT(idx, elem) \
    BOOST_PP_TUPLE_ELEM(0, elem) = BOOST_PP_TUPLE_ELEM(1, elem) /* member = value */

#define GBP_DECLARE_ENUM_DECLARE_ENUM_MEMBERS_WITHOUT_INIT(idx, elem) \
    BOOST_PP_TUPLE_ELEM(0, elem) /* member */

#define GBP_DECLARE_ENUM_DECLARE_ENUM_MEMBERS(unused0, unused1, idx, elem) \
    BOOST_PP_COMMA_IF(idx) BOOST_PP_IF( \
            BOOST_PP_EQUAL(2, BOOST_PP_TUPLE_SIZE(elem)) \
        ,GBP_DECLARE_ENUM_DECLARE_ENUM_MEMBERS_WITH_INIT \
        ,GBP_DECLARE_ENUM_DECLARE_ENUM_MEMBERS_WITHOUT_INIT \
    )(idx, elem)

#define GBP_DECLARE_ENUM_DECLARE_ENUM_WRITE_CASES(unused, name, idx, elem) \
    case name::BOOST_PP_TUPLE_ELEM(0, elem): return "\"" BOOST_PP_STRINGIZE(name::BOOST_PP_TUPLE_ELEM(0, elem)) "\"";

#define GBP_DECLARE_ENUM_IMPL(name_, type_, spec_, seq_) \
    enum class name_ : type_ { \
        BOOST_PP_SEQ_FOR_EACH_I( \
             GBP_DECLARE_ENUM_DECLARE_ENUM_MEMBERS \
            ,~ \
            ,seq_ \
        ) \
    }; \
    \
    spec_ const char* enum_cast(const name_ &o) { \
        switch (o) { \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 GBP_DECLARE_ENUM_DECLARE_ENUM_WRITE_CASES \
                ,name_ \
                ,seq_ \
            ) \
        } \
        return "\"" #name_ "::unknown\""; \
    } \
    \
    spec_ std::ostream& operator<< (std::ostream &s, const name_ &o) { \
        return s << enum_cast(o); \
    }

#define GBP_DECLARE_ENUM(name_, type_, spec_, seq_) \
    GBP_DECLARE_ENUM_IMPL( \
         name_ \
        ,type_ \
        ,spec_ \
        ,BOOST_PP_CAT(GBP_DECLARE_TYPE_WRAP_X seq_, 0) \
    )

#define GBP_DECLARE_ENUM_SIMPLE_IMPL(name_, spec_, seq_) \
    enum name_ { \
        BOOST_PP_SEQ_FOR_EACH_I( \
             GBP_DECLARE_ENUM_DECLARE_ENUM_MEMBERS \
            ,~ \
            ,seq_ \
        ) \
    }; \
    \
    spec_ const char* enum_cast(const name_ &o) { \
        switch (o) { \
            BOOST_PP_SEQ_FOR_EACH_I( \
                 GBP_DECLARE_ENUM_DECLARE_ENUM_WRITE_CASES \
                ,name_ \
                ,seq_\
            ) \
        } \
    }

#define GBP_DECLARE_ENUM_SIMPLE(name_, spec_, seq_) \
    GBP_DECLARE_ENUM_SIMPLE_IMPL( \
         name_ \
        ,spec_ \
        ,BOOST_PP_CAT(GBP_DECLARE_TYPE_WRAP_X seq_, 0) \
    )

#else // ifndef GBP_DECLARE_TYPE_DISABLE_DECLARE_TYPE
#   define GBP_DECLARE_TYPE(...)
#   define GBP_DECLARE_ENUM(...)
#   define GBP_DECLARE_ENUM_SIMPLE(...)
#endif // GBP_DECLARE_TYPE_DISABLE_DECLARE_TYPE

/***************************************************************************

макрос GBP_DECLARE_TYPE() принимает как минимум два аргумента:
1. имя генерируемой структуры
2. последовательность пар, где левое значение - имя генерируемого мембера,
 среднее - его тип, а правое(опционально) - инициализатор.
(среднее и правое значения всегда необходимо оборачивать в дополнительные одинарные кавычки)

пример использования:
GBP_DECLARE_TYPE(
    type // имя структуры
    ,
    (a, (int), (-1)) // перечисление мемберов
    (b, (std::string))
    (c, (std::pair<int, std::string>), (33, "33"))
)

третий аргумент этого макроса - опциональный.
он нужен для того, чтоб в него можно было вписать произвольный код, который
будет вставлен препроцессором в самом начале тела структуры.

пример использования:
GBP_DECLARE_TYPE(
    type // имя структуры
    ,
    (a, (int)) // перечисление мемберов
    (b, (std::string))
    (c, (std::pair<int, std::string>))
    ,
    const int&         f0() const { return a; }
    const std::string& f1() const { return b; }
)

так же, на место третьего аргумента можно вставить другой макрос GBP_DECLARE_TYPE().
пример:
GBP_DECLARE_TYPE(
     type // имя структуры
    ,
    (a, (int)) // перечисление мемберов
    (b, (std::string))
    (c, (std::pair<int, std::string>))
    ,
    GBP_DECLARE_TYPE(
        sometype
        ,
        (a, (int))
        (b, (std::string))
        (c, (std::pair<int, std::string>))
    )
)

таким образом, будет сгенерирована структура 'type', внутри которой будет
декларирована вложенная структура 'sometype'.

так же, вложенная структура может использовать как член-данный структуры в
которой объявлена.
пример:
GBP_DECLARE_TYPE(
     type // имя структуры
    ,
    (a, (int)) // перечисление мемберов
    (b, (std::string))
    (c, (std::pair<int, std::string>))
    (d, (sometype)) // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    ,
    GBP_DECLARE_TYPE(
        sometype
        ,
        (a, (int))
        (b, (std::string))
        (c, (std::pair<int, std::string>))
    )
)

для генерации дополнительного функционала - задефайнить GBP_DECLARE_TYPE_GEN_ADDITIONALS
для отключения макросов GBP_DECLARE_ENUM()/GBP_DECLARE_ENUM_SIMPLE()/GBP_DECLARE_TYPE() - 
  нужно задефайнить макропеременную GBP_DECLARE_TYPE_DISABLE_DECLARE_TYPE.

****************************************************************************

еще один новый макрос - GBP_DECLARE_ENUM()
макрос принимает четыре аргумента:
1. имя енума
2. тип наследуемый енумом
3. спецификатор для генерируемых функций 'const char* enum_cast(enum);'
и 'std::ostream& operator<< (std::ostream &s, const enum &o)'.
если этот макрос используется внутри классов - этот аргумент должен иметь
значение 'friend', иначе - 'inline'.
4. последовательность членом енума. элементы последовательности могут сосять
как из одноэлементных кортежей, так и из двуэлементных.
во втором случае, правые элементы кортежей это значения, которыми будут
инициализированны мемберы.
пример:
GBP_DECLARE_ENUM(
     enum1
    ,std::uint8_t
    ,inline
    ,(a)
     (b)
)

пример:
GBP_DECLARE_ENUM(
     enum1
    ,std::uint8_t
    ,inline
    ,(a, 0x02)
     (b, 0x04)
)

****************************************************************************/

#endif // _gbp__api__declare_type
