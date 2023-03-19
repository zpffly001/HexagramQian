#ifndef HEXAGRAMQIAN_APP_H
#define HEXAGRAMQIAN_APP_H
#include <restbed>
#include <memory>

/**
 * 区块链APP
*/
class App
{
    int port;

    restbed::Service server;
    std::shared_ptr<restbed::Settings> settings;

    void prepareSettings();
    void registerController();

public:

    /**
     * 构造函数 explicit修饰，必须显式调用
     * : port(port) 相当于在{}中 port = port，初始化上面的变量port
     * https://blog.csdn.net/qq_41452267/article/details/105254205
    */
    explicit App(int port) : port(port) {}

    /**
     * 启动APP
    */
   void start();

   /**
    * 关闭APP
   */
  void finalize();
};

#endif // !HEXAGRAMQIAN_APP_H