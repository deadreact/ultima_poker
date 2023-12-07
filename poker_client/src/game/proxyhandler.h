#pragma once

#include <qlist.h>

template <typename HandlerType>
class ProxyHandler : public HandlerType
{
protected:
    QList<HandlerType*> m_handlers;
public:
    ProxyHandler(const GameId& id)
        : HandlerType(id)
    {}

    virtual ~ProxyHandler() {
        qDeleteAll(m_handlers);
    }

public:
    void push_back(HandlerType* handler) {
        m_handlers.push_back(handler);
    }
    void pop_back() {
        m_handlers.pop_back();
    }
    void push_front(HandlerType* handler) {
        m_handlers.push_front(handler);
    }
    void pop_front() {
        m_handlers.pop_front();
    }
    HandlerType* front() const {
        return m_handlers.front();
    }
    HandlerType* back() const {
        return m_handlers.back();
    }
    HandlerType* at(int idx) const {
        return m_handlers.at(idx);
    }
    int size() const {
        return m_handlers.size();
    }
    bool empty() const {
        return m_handlers.empty();
    }

    template <typename T>
    typename std::enable_if<std::is_base_of<HandlerType, T>::value, T>::type*
    handler()
    {
        for (HandlerType* h: m_handlers) {
            if (T* typedHandler = dynamic_cast<T*>(h)) {
                return typedHandler;
            }
        }
        return nullptr;
    }
    
};
