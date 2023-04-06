#ifndef HEXAGRAMQIAN_STATICCONTROLLER_H
#define HEXAGRAMQIAN_STATICCONTROLLER_H

#include <restbed>

/**
 * 提供静态资源的控制器
*/
class StaticController
{
    static void indexHandler(const std::shared_ptr<restbed::Session> session);

    static void staticHandler(const std::shared_ptr<restbed::Session> session);

public:
    static void publish(restbed::Service &service);
}

#endif