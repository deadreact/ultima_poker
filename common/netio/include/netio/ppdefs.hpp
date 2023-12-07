
#ifndef __gbp__net__ppdefs_hpp
#define __gbp__net__ppdefs_hpp

/****************************************************************************/

#define __NETIO_STRINGIZE_I(x) #x
#define __NETIO_STRINGIZE(x) __NETIO_STRINGIZE_I(x)

#define __NETIO_MAKE_FILELINE() __FILE__ "(" __NETIO_STRINGIZE(__LINE__) ")"

/****************************************************************************/

#define __NETIO_INIT_ERROR(err, ec, emsg) \
    const ::gbp::net::error err(__NETIO_MAKE_FILELINE(), ec, emsg)

#define __NETIO_PUSH_ERROR(err, fl, ec, emsg) \
    const_cast<::gbp::net::error&>(err).push(fl, ec, emsg)

#define __NETIO_POP_ERROR(err) \
    const_cast<::gbp::net::error&>(err).pop()

#define __NETIO_CLEAR_ERROR(err) \
    const_cast<::gbp::net::error&>(err).clear()

/****************************************************************************/

#define __NETIO_CATCH_AND_REPORT_EXCEPTIONS(...) \
    try { \
        __VA_ARGS__; \
    } catch (const std::exception &ex) { \
        std::fprintf(stderr, "%s: std::exception was thrown: %s\n", __NETIO_MAKE_FILELINE(), ex.what()); \
        std::fflush(stderr); \
    } catch (...) { \
        std::fprintf(stderr, "%s: unknown exception was thrown\n", __NETIO_MAKE_FILELINE()); \
        std::fflush(stderr); \
    }

/****************************************************************************/

#endif // __gbp__net__ppdefs_hpp
