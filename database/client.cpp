#include "client.h"

Client::Client() {
  std::string uri = getenv("MONGO_DB_URI");
  std::string database = getenv("MONGO_DB_DATABASE");

  client_ = mongocxx::client{
    mongocxx::uri{uri}
  };
  database_ = client_[database];
}

void Client::query_database(std::string collection_name, std::unordered_map<std::string, std::any> query) {
  mongocxx::collection collection = database_[collection_name];
  auto doc = document{};
  for (auto i = query.begin(); i != query.end(); i++) {
    std::any temp = i->second;
    try {
      int value = std::any_cast<int>(temp);
      doc.append(kvp(i->first, value));
    }
    catch (const std::bad_any_cast& e) {
    }

    try {
      double value = std::any_cast<double>(temp);
      doc.append(kvp(i->first, value));
    }
    catch (const std::bad_any_cast& e) {
    }

    try {
      std::string value = std::any_cast<std::string>(temp);
      doc.append(kvp(i->first, value));
    }
    catch (const std::bad_any_cast& e) {
    }

  }
  auto v = doc.view();

  mongocxx::cursor cursor = collection.find(
      doc.extract()
  );
  for (auto doc : cursor) {
    std::cout << bsoncxx::to_json(doc) << "\n";
  }
}

Bar Client::get_bars(std::string ticker, unsigned short hour, unsigned short minute) {
    Bar bar(ticker, hour, minute);
    return bar;
}

Bar::Bar(std::string ticker_, unsigned short hour_, unsigned short minute_) {
    ticker = ticker_;
    hour = hour_;
    minute = minute_;
}
