#include "RpcCore.hpp"
#include "Test.h"
#include "log.h"

namespace RpcCoreTest {

void RpcTest() {
    using namespace RpcCore;

    // 回环的连接 用于测试 实际使用应为具体传输协议实现的Connection
    auto connection = std::make_shared<LoopbackConnection>();

    // 创建Rpc 收发消息 注意只能使用智能指针 不要在栈上创建
    auto rpc = std::make_shared<Rpc>(connection);
    rpc->setTimerImpl([](uint32_t ms, const Rpc::TimeoutCb& cb) {
        // 定时器实现 应当配合当前应用的事件循环 确保消息收发和超时在同一个线程
        // 此示例为回环的连接 不需要具体实现
    });

    // 设备A 注册消息等待接收
    rpc->subscribe<Json>("cmd_xxx", [&](const Json & msg) {
        LOG("%s", msg.dump().c_str());
        {
            auto name = msg["name"].get<std::string>();
            auto age  = msg["age"].get<int>();
            LOG("name:%s, age:%d", name.c_str(), age);
        }
        {
            std::string name = msg["name"];
            int age  = msg["age"];
            LOG("name:%s, age:%d", name.c_str(), age);
        }
    });

    // 设备B 发送消息
    Json msg;
    msg["name"] = "小明";
    msg["age"]  = 18;
    msg["msg"]  = "hello";
    rpc->createRequest()
            ->cmd("cmd_xxx")
            ->msg(msg)
            ->call();
}

}
