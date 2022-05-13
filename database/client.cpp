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
  auto doc = document{};
  for (unsigned int i = 0; i < query.size(); i++) {
    QueryBase* _query = query[i];
    unsigned int type = _query->type;
    switch (type) {
      case 0:
        if (static_cast<int>(_query->getValue<int>()) == 0) {
          doc.append(kvp(_query->key,
            [_query](bsoncxx::builder::basic::sub_document subdoc) {
              if (_query->eq) subdoc.append(kvp(GREATER_THAN_EQ, static_cast<int>(_query->getLow<int>())), kvp(LESS_THAN_EQ, static_cast<int>(_query->getHigh<int>())));
              else subdoc.append(kvp(GREATER_THAN, static_cast<int>(_query->getLow<int>())), kvp(LESS_THAN, static_cast<int>(_query->getHigh<int>())));
            }
          ));
        }
        else {
          if (_query->_operator == "") {
            doc.append(kvp(_query->key, static_cast<int>(_query->getValue<int>())));
          }
          else {
            doc.append(kvp(_query->key, make_document(kvp(_query->_operator, static_cast<int>(_query->getValue<int>())))));
          }
        }
        break;
      case 1:
        if (static_cast<unsigned short>(_query->getValue<unsigned short>()) == 0) {
          doc.append(kvp(_query->key,
            [_query](bsoncxx::builder::basic::sub_document subdoc) {
              if (_query->eq) subdoc.append(kvp(GREATER_THAN_EQ, static_cast<unsigned short>(_query->getLow<unsigned short>())), kvp(LESS_THAN_EQ, static_cast<unsigned short>(_query->getHigh<unsigned short>())));
              else subdoc.append(kvp(GREATER_THAN, static_cast<unsigned short>(_query->getLow<unsigned short>())), kvp(LESS_THAN, static_cast<unsigned short>(_query->getHigh<unsigned short>())));
            }
          ));
        }
        else {
          if (_query->_operator == "") {
            doc.append(kvp(_query->key, static_cast<unsigned short>(_query->getValue<unsigned short>())));
          }
          else {
            doc.append(kvp(_query->key, make_document(kvp(_query->_operator, static_cast<unsigned short>(_query->getValue<unsigned short>())))));
          }
        }

        break;
      case 2:
        if (static_cast<double>(_query->getValue<double>()) == 0) {
          doc.append(kvp(_query->key,
            [_query](bsoncxx::builder::basic::sub_document subdoc) {
              if (_query->eq) subdoc.append(kvp(GREATER_THAN_EQ, static_cast<double>(_query->getLow<double>())), kvp(LESS_THAN_EQ, static_cast<double>(_query->getHigh<double>())));
              else subdoc.append(kvp(GREATER_THAN, static_cast<double>(_query->getLow<double>())), kvp(LESS_THAN, static_cast<double>(_query->getHigh<double>())));
            }
          ));
        }
        else {
          if (_query->_operator == "") {
            doc.append(kvp(_query->key, static_cast<double>(_query->getValue<double>())));
          }
          else {
            doc.append(kvp(_query->key, make_document(kvp(_query->_operator, static_cast<double>(_query->getValue<double>())))));
          }
        }
        break;
    }
  }

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
  Query<unsigned short> hour_query("HOUR", hour);
  Query<unsigned short> minute_query("MINUTE", minute);
  query.push_back(&hour_query);
  query.push_back(&minute_query);
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

// template <typename T>
// Query::Query(std::string key_, T value_) {
//   key = key_;
//   value = value_;
// }

// template <typename T>
// Query::Query(std::string key_, const std::string operator_, T value_) {
//   key = key_;
//   value = value_;
//   _operator = operator_;
// }

// template <typename T>
// Query::Query(std::string key_, T low_, T high_, bool eq_) {
//   key = key_;
//   low = low_;
//   high = high_;
//   eq = eq_;
// }
