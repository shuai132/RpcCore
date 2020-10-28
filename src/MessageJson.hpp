#pragma once

#include "Message.hpp"
#include "json.hpp"

namespace RpcCore {

struct Json : Message, public nlohmann::json {
    using nlohmann::json::basic_json;
    std::string serialize() const override {
        return dump();
    }
    bool deSerialize(const std::string& data) override {
        nlohmann::json::parse(data).swap(*this);
        return true;
    }
};

}
