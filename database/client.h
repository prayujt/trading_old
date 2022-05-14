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
using bsoncxx::builder::basic::make_document;

const std::string GREATER_THAN = "$gt";
const std::string LESS_THAN = "$lt";
const std::string GREATER_THAN_EQ = "$gte";
const std::string LESS_THAN_EQ = "$lte";
const std::string EQ = "$eq";

template <typename T>
unsigned int checkType(T check) {
  if (typeid(check) == typeid(unsigned short)) return 0;
  else if (typeid(check) == typeid(int)) return 1;
  else if (typeid(check) == typeid(double)) return 2;
}

struct QueryBase {
  bool eq;
  std::string key;
  unsigned int query_type, constructor_type;
  const std::string _operator;
  template <typename T> T getValue();
  template <typename T> T getLow();
  template <typename T> T getHigh();
  QueryBase(std::string key_) : key(key_) {}
  QueryBase(std::string key_, const std::string operator_) : key(key_), _operator(operator_) {}
  QueryBase(std::string key_, bool eq_) : key(key_), eq(eq_) {}
  virtual ~QueryBase() = default;
};

template <typename T>
struct Query : public QueryBase {
  T value, low, high;
  T getValue() { return value; }
  T getLow() { return low; }
  T getHigh() { return high; }
  Query(std::string key_, T value_) : QueryBase(key_), value(value_) {
    query_type = checkType<T>(value_);
    constructor_type = 0;
  }
  Query(std::string key_, const std::string operator_, T value_) : QueryBase(key_, operator_), value(value_) {
    query_type = checkType<T>(value_);
    constructor_type = 1;
  }
  Query(std::string key_, T low_, T high_, bool eq_) : QueryBase(key_, eq_), low(low_), high(high_) {
    query_type = checkType<T>(low_);
    unsigned int temp = checkType<T>(high_);
    if (temp > query_type) query_type = temp;
    constructor_type = 2;
  }
};

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
  Bar* get_bar(std::string ticker, unsigned short hour, unsigned short minute);
  std::vector<Bar*> get_bars(std::string ticker, unsigned short hour_start, unsigned short hour_end, unsigned short minute_start, unsigned short minute_end);
  Client();

  std::vector<bsoncxx::document::view> query_database(std::string collection_name, std::vector<QueryBase*> query);
};

template <typename T>
T QueryBase::getValue() {
    return (dynamic_cast<Query<T>&>(*this)).getValue();
  }

template <typename T>
T QueryBase::getLow() {
  return (dynamic_cast<Query<T>&>(*this)).getLow();
}

template <typename T>
T QueryBase::getHigh() {
  return (dynamic_cast<Query<T>&>(*this)).getHigh();
}

template <typename T>
void parse_query(QueryBase* query, bsoncxx::builder::basic::document* doc) {
  switch (query->constructor_type) {
    case 0:
        doc->append(kvp(query->key, static_cast<T>(query->getValue<T>())));
        break;
    case 1:
      doc->append(kvp(query->key, make_document(kvp(query->_operator, static_cast<T>(query->getValue<T>())))));
      break;
    case 2:
      doc->append(kvp(query->key,
        [query](bsoncxx::builder::basic::sub_document subdoc) {
          if (query->eq) subdoc.append(kvp(GREATER_THAN_EQ, static_cast<T>(query->getLow<T>())), kvp(LESS_THAN_EQ, static_cast<T>(query->getHigh<T>())));
          else subdoc.append(kvp(GREATER_THAN, static_cast<T>(query->getLow<T>())), kvp(LESS_THAN, static_cast<T>(query->getHigh<T>())));
        }
      ));
      break;
  }
}
