#include "client.h"

Client::Client() {
  std::string uri = getenv("MONGO_DB_URI");
  std::string database = getenv("MONGO_DB_DATABASE");

  client_ = mongocxx::client{
    mongocxx::uri{uri}
  };
  database_ = client_[database];
}

std::vector<bsoncxx::document::view> Client::query_database(std::string collection_name, std::unordered_map<std::string, std::any> query) {
  std::unordered_map<std::string, const std::string> empty;
  return complex_query_database(collection_name, query, empty);
}

std::vector<bsoncxx::document::view> Client::complex_query_database(std::string collection_name, std::unordered_map<std::string, std::any> query, std::unordered_map<unsigned int, const std::string> operators) {
  std::vector<bsoncxx::document::view> documents;
  mongocxx::collection collection = database_[collection_name];
  auto doc = document{};
  unsigned int counter = 0;
  for (auto i = query.begin(); i != query.end(); i++) {
    std::any temp = i->second;
    for (auto i = )
    try {
      unsigned short value = std::any_cast<unsigned short>(temp);
      if (operators.count(counter) != 0) {
        doc.append(kvp(i->first, make_document(kvp(operators.find(counter)->second, value))));
      }
      else doc.append(kvp(i->first, value));
    } catch (const std::bad_any_cast& e) {
      // std::cout << "bad_any_cast unsigned short" << std::endl;
    }

    try {
      int value = std::any_cast<int>(temp);
      if (operators.count(i->first) != 0) {
        doc.append(kvp(i->first, make_document(kvp(operators.find(counter)->second, value))));
      }
      else doc.append(kvp(i->first, value));
    } catch (const std::bad_any_cast& e) {
      // std::cout << "bad_any_cast int" << std::endl;
    }

    try {
      double value = std::any_cast<double>(temp);
      if (operators.count(i->first) != 0) {
        doc.append(kvp(i->first, make_document(kvp(operators.find(counter)->second, value))));
      }
      else doc.append(kvp(i->first, value));
    } catch (const std::bad_any_cast& e) {
      // std::cout << "bad_any_cast double" << std::endl;
    }

    try {
      std::string value = std::any_cast<std::string>(temp);
      if (operators.count(i->first) != 0) {
        doc.append(kvp(i->first, make_document(kvp(operators.find(counter)->second, value))));
      }
      else doc.append(kvp(i->first, value));
    } catch (const std::bad_any_cast& e) {
      // std::cout << "bad_any_cast string" << std::endl;
    }
    counter++;
  }

  mongocxx::cursor cursor = collection.find(
      doc.extract()
  );
  for (bsoncxx::document::view doc_ : cursor) {
    documents.push_back(doc_);
  }
  return documents;
}

Bar* Client::get_bar(std::string ticker, unsigned short hour, unsigned short minute) {
  std::unordered_map<std::string, std::any> query{
    {"HOUR", hour},
    {"MINUTE", minute}
  };
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
  if (hour_start == hour_end) {
    std::unordered_map<std::string, std::any> query{
      {"HOUR", hour_start},
      {"MINUTE", minute}
    };
    std::unordered_map<int, const std::string> operators{
    {"MINUTE", LESS_THAN} //TODO: Allow for range (eg. x <= MINUTE <= y)
    };
  }
  std::unordered_map<std::string, std::any> query{
    {"HOUR", hour},
    {"MINUTE", minute}
  };
  std::unordered_map<std::string, const std::string> operators{
  {"HOUR", LESS_THAN}
  };
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
