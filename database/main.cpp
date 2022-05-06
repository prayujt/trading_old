#include <iostream>
#include <stdlib.h>
#include <string>

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
using namespace std;

int main(int argc, char* argv[]) {
  string uri = getenv("MONGO_DB_URI");
  mongocxx::instance inst{};
  mongocxx::client conn{
    mongocxx::uri{uri}
  };
  mongocxx::database db = conn["stock_data"];
  mongocxx::collection collection = db["AAPL"];
  mongocxx::cursor cursor = collection.find({});
  for (auto doc : cursor) {
    cout << bsoncxx::to_json(doc) << "\n";
  }

  return 0;
}
