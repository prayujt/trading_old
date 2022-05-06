#include "client.h"


void Client::query_database(std::string query) {
  std::string uri = getenv("MONGO_DB_URI");
  std::string database = getenv("MONGO_DB_DATABASE");

  mongocxx::instance inst{};
  mongocxx::client conn{
    mongocxx::uri{uri}
  };
  mongocxx::database db = conn[database];
  mongocxx::collection collection = db["AAPL"];
  mongocxx::cursor cursor = collection.find(
      document{} << "BID_PRICE" << 155.86 << finalize
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
