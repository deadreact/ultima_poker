# Не забываем смотреть экзамплы!

## Архитектура сервисов

Сервисы - это некоторый функционал, изолированный в некотором классе.

Сервисы можно добавлять/удалять во время выполнения программы.

В пределах одного `netio` каждый конкретный сервис может существовать
только в единственном экземпляре.

Сервисы могут взаимодействовать с другими сервисами.

Базовый интерфейс сервисов выглядит следующим образом:
```cpp
namespace gbp {
namespace net {

/**************************************************************************/

struct service_base {
    virtual ~service_base() {}

    virtual bool start(impl_holder holder = impl_holder()) = 0;
    virtual void stop() = 0;

    virtual bool started() const = 0;

    virtual std::string name() const = 0;
};

/**************************************************************************/

} // ns net
} // ns gbp
```
Наследник должен реализовать все эти методы.

Метод `start()` - старт сервиса. (зовется из `netio::start()`)

Метод `stop()` - стоп сервиса. (зовется из `netio::stop()`)

Метод `started()` - геттер состояния сервиса. Может быть просто заглушкой. (зовется из `netio`)

Метод `name()` - должен возвращать имя сервиса. (зовется из `netio`)

## Требования к сервисам

Сервисы работают "внутри" `netio`, и первым аргументом в свой конструктор
должны принимать ссылку на `netio`.

Сервисы добавляются в `netio` использованием метода `add_service`:
```cpp
gbp::net::netio netio(...);
netio.add_service<gbp::net::ping_service>(
     1 // ping interval
    ,[](std::uint64_t time_us){std::cout<<"ping time="<<time_us<<"us"<<std::endl;}
);

```
Шаблонная функция `netio::add_service` имеет следующую сигнатуру:
```cpp
template<typename T, typename ...Args>
service_handle add_service(Args&& ...args) {}
```
Таким образом мы можем передавать в конструктор сервиса произвольные аргументы.

Самый простой способ добавить сервисы в `netio` - это добавлять их сразу после
создания объекта `netio`. В таком случае, сервисы будут запущены(для них будет 
вызван метод `start()`) при старте `netio`(метод `netio::start()`).

Сервисы не должны сохранять в качестве мембера `gbp::net::impl_holder`, полученный
в их метод `start(gbp::net::impl_holder holder)`, потому что если `netio` используется
на стороне сервера - сохраненный `gbp::net::impl_holder` будет "держать" сессию,
не позволяя ей деструктится.
Вообще, если все же возникает желание сохранить `gbp::net::impl_holder` - нужно
позаботиться о том, чтоб в методе `stop()` сервиса происходил сброс этого указателя.

Сервисы стартуют в том порядке, в котором были добавлены в `netio`.
Останавливаются в обратном порядке при вызове `netio::stop()`.

Т.к. сервисы могут взаимодействовать между собой, `netio` предоставляет метод
`get_service` имеющий следующую сигнатуру:
```cpp
template<typename T>
T* get_service();
```
Т.к. этот метод возвращает указатель - необходимо проверять его валидность.

Готовые примеры сервисов можно посмотреть в экзамплах:
* ping_service ([ping_service.hpp](http://109.236.85.111/gitlab/tasheehoo/netio/blob/master/examples/services/ping_service.hpp)/[ping_service.cpp](http://109.236.85.111/gitlab/tasheehoo/netio/blob/master/examples/services/ping_service.cpp))
* remote_time_service ([remote_time_service.hpp](http://109.236.85.111/gitlab/tasheehoo/netio/blob/master/examples/services/remote_time_service.hpp)/[remote_time_service.cpp](http://109.236.85.111/gitlab/tasheehoo/netio/blob/master/examples/services/remote_time_service.cpp))
* disconnect_service ([disconnect_service.hpp](http://109.236.85.111/gitlab/tasheehoo/netio/blob/master/examples/services/disconnect_service.hpp)/[disconnect_service.cpp](http://109.236.85.111/gitlab/tasheehoo/netio/blob/master/examples/services/disconnect_service.cpp))
