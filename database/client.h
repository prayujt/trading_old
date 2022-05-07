#include <stdlib.h>
#include <string>
#include <iostream>
#include <unordered_map>
#include <any>
#include <vector>
#include <limits>

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/types.hpp>

using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;

struct Bar {
    std::string ticker;
    double open, close, low, high;
    unsigned short hour, minute;
    Bar(std::string ticker, unsigned short hour, unsigned short minute, double open, double close, double low, double high);
};

struct Client {
    mongocxx::instance instance_ = mongocxx::instance{};
    mongocxx::client client_;
    mongocxx::database database_;
    Bar get_bar(std::string ticker, unsigned short hour, unsigned short minute);
    std::vector<Bar> get_bars(std::string ticker, unsigned short hour_start, unsigned short hour_end, unsigned short minute_start, unsigned short minute_end);
    Client();

    private:
        std::vector<bsoncxx::document::view> query_database(std::string collection_name, std::unordered_map<std::string, std::any>);
};
