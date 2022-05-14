#include "client.h"

Client::Client() {
  std::string uri = getenv("MONGO_DB_URI");
  std::string database = getenv("MONGO_DB_DATABASE");

  client_ = mongocxx::client{
    mongocxx::uri{uri}
  };
  database_ = client_[database];
}

std::vector<bsoncxx::document::view> Client::query_database(std::string collection_name, std::vector<QueryBase*> query) {
  std::vector<bsoncxx::document::view> documents;
  mongocxx::collection collection = database_[collection_name];
  bsoncxx::builder::basic::document doc = document{};
  for (unsigned int i = 0; i < query.size(); i++) {
    QueryBase* _query = query[i];
    unsigned int type = _query->query_type;
    switch (type) {
      case 0:
        parse_query<unsigned short>(_query, &doc);
        break;
      case 1:
        parse_query<int>(_query, &doc);
        break;
      case 2:
        parse_query<double>(_query, &doc);
        break;
    }
  }

  // std::cout << bsoncxx::to_json(doc) << std::endl;
  mongocxx::cursor cursor = collection.find(
      doc.extract()
  );
  for (bsoncxx::document::view doc_ : cursor) {
    std::cout << bsoncxx::to_json(doc_) << std::endl;
    documents.push_back(doc_);
  }
  return documents;
}

Bar* Client::get_bar(std::string ticker, unsigned short hour, unsigned short minute) {
  std::vector<QueryBase*> query;
  Query<unsigned short>* hour_query = new Query<unsigned short>("HOUR", hour);
  Query<unsigned short>* minute_query = new Query<unsigned short>("MINUTE", minute);
  query.push_back(hour_query);
  query.push_back(minute_query);
  std::vector<bsoncxx::document::view> result = query_database(ticker, query);

  double min = std::numeric_limits<double>::max();
  double max = std::numeric_limits<double>::min();
  double open, close;
  for (unsigned int i = 0; i < result.size(); i++) {
    double last_price = 0;
    try {
      last_price = result[i]["LAST_PRICE"].get_double().value;
    }
    catch (...) {
      last_price = (double) result[i]["LAST_PRICE"].get_int32().value;
    }
    if (i == 0) open = last_price;
    if (i == result.size() - 1) close = last_price;
    if (last_price < min) min = last_price;
    if (last_price > max) max = last_price;
  }
  if (!(min == std::numeric_limits<double>::max() || max == std::numeric_limits<double>::min())) return new Bar(ticker, hour, minute, open, close, min, max);
  else return NULL;
}

std::vector<Bar*> Client::get_bars(std::string ticker, unsigned short hour_start, unsigned short hour_end, unsigned short minute_start, unsigned short minute_end) {
  std::vector<Bar*> bars;
  std::vector<bsoncxx::document::view> result;
  // if (hour_start == hour_end) {
  // }
  for (unsigned short i = hour_start; i <= hour_end; i++) {
    for (unsigned short j = minute_start; j <= ((hour_end - hour_start) * 60) + minute_end; j++) {
      Bar* bar = get_bar(ticker, i, j % 60);
      if (bar != NULL) bars.push_back(bar);
    }
  }
  return bars;
}

Bar::Bar(std::string ticker_, unsigned short hour_, unsigned short minute_, double open_, double close_, double low_, double high_) {
  ticker = ticker_;
  hour = hour_;
  minute = minute_;
  open = open_;
  close = close_;
  low = low_;
  high = high_;
}
