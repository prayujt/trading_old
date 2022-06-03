#include <stdlib.h>
#include <string>
#include <iostream>
#include <unordered_map>
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

using namespace std;

// constants for database queries
const string GREATER_THAN = "$gt";
const string LESS_THAN = "$lt";
const string GREATER_THAN_EQ = "$gte";
const string LESS_THAN_EQ = "$lte";
const string EQ = "$eq";

template <typename T>
unsigned int checkType(T check) {
  if (typeid(check) == typeid(unsigned short)) return 0;
  else if (typeid(check) == typeid(int)) return 1;
  else if (typeid(check) == typeid(double)) return 2;
}

struct QueryBase {
  bool eq;
  string key;
  unsigned int query_type, constructor_type;
  const string _operator;
  template <typename T> T getValue();
  template <typename T> T getLow();
  template <typename T> T getHigh();
  QueryBase(string key_) : key(key_) {}
  QueryBase(string key_, const string operator_) : key(key_), _operator(operator_) {}
  QueryBase(string key_, bool eq_) : key(key_), eq(eq_) {}
  virtual ~QueryBase() = default;
};

template <typename T>
struct Query : public QueryBase {
  T value, low, high;
  T getValue() { return value; }
  T getLow() { return low; }
  T getHigh() { return high; }
  Query(string key_, T value_) : QueryBase(key_), value(value_) {
    query_type = checkType<T>(value_);
    constructor_type = 0;
  }
  Query(string key_, const string operator_, T value_) : QueryBase(key_, operator_), value(value_) {
    query_type = checkType<T>(value_);
    constructor_type = 1;
  }
  Query(string key_, T low_, T high_, bool eq_) : QueryBase(key_, eq_), low(low_), high(high_) {
    query_type = checkType<T>(low_);
    unsigned int temp = checkType<T>(high_);
    if (temp > query_type) query_type = temp;
    constructor_type = 2;
  }
};

struct Bar {
  string ticker;
  double open, close, low, high;
  unsigned short hour, minute;
  Bar(string ticker, unsigned short hour, unsigned short minute, double open, double close, double low, double high);
};

struct Client {
  struct Queue {
    struct Node {
      Node* prev;
      Node* next;
      Bar* value;
      Node(Node* _prev, Bar* _value) : prev(_prev), value(_value) {}
      Node(Bar* _value, Node* _next) : value(_value), next(_next) {}
      Node(Bar* _value) : value(_value) {}
      Node() {}
    };
    struct iterator {
      void operator++(int);
      void operator--(int);
      bool operator==(iterator);
      bool operator!=(iterator);

      Bar* value();

      iterator(Node* _node) : node(_node) {}

      private:
        Node* node;
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    unsigned short max_size;
    unsigned short size = 0;
    unsigned short last_min = 0;
    unsigned short last_hour = 0;

    Bar* dequeue();
    void enqueueHead(Bar* _bar);
    void enqueue(Bar* _bar);
    Bar* peek();
    bool isEmpty();
    bool isFull();

    iterator begin();
    iterator beginFromEnd();
    iterator end();

    Queue(unsigned short max_size);
    Queue() {}
  };

  mongocxx::instance instance_ = mongocxx::instance{};
  mongocxx::client client_;
  mongocxx::database database_;

  unordered_map<string, Queue> sma_bars;
  // Queue sma_bars{128};

  Bar* get_bar(string ticker, unsigned short hour, unsigned short minute);
  vector<Bar*> get_bars(string ticker, unsigned short hour_start, unsigned short hour_end, unsigned short minute_start, unsigned short minute_end);

  double get_sma(string ticker, unsigned short offset, unsigned short hour, unsigned short minute);
  double get_sma(string ticker, unsigned short offset);

  void update_bars(string ticker);

  mongocxx::cursor query_database(string collection_name, vector<QueryBase*> query);

  Client();
};

struct Time {
  unsigned short _time[3];
  void operator++(int);
  void operator--(int);
  Time();
  Time(unsigned short hour, unsigned short minute);
  Time(unsigned short hour, unsigned short minute, unsigned short second);
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
