
#ifndef __gbp__asio_client_skeleton__api_hpp
#define __gbp__asio_client_skeleton__api_hpp

namespace api {

struct myrequest {
    virtual ~myrequest() {}

    virtual void req0() = 0;
    virtual void req1(int) = 0;
};

struct myreplies {
    virtual ~myreplies() {}

    virtual void rep0(int, int) = 0;
    virtual void rep1(int) = 0;
};

} // ns api

#endif // __gbp__asio_client_skeleton__api_hpp
