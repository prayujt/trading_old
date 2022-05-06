#include <stdlib.h>
#include <string>
#include <iostream>

#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

struct Bar {
    std::string ticker;
    float open, close, low, high;
    unsigned short hour, minute;
    Bar(std::string ticker, unsigned short hour, unsigned short minute);
};

struct Client {
    Bar get_bars(std::string ticker, unsigned short hour, unsigned short minute);
    void query_database(std::string query);
};
