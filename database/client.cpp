#include "client.h"
#include <ctime>

Client::Client() {
  std::string uri = getenv("MONGO_DB_URI");
  std::string database = getenv("MONGO_DB_DATABASE");

  client_ = mongocxx::client{
    mongocxx::uri{uri}
  };
  database_ = client_[database];
}

mongocxx::cursor Client::query_database(std::string collection_name, std::vector<QueryBase*> query) {
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

  mongocxx::cursor cursor = collection.find(
      doc.extract()
  );

  return cursor;
}

Bar* Client::get_bar(std::string ticker, unsigned short hour, unsigned short minute) {
  std::vector<QueryBase*> query;
  Query<unsigned short>* hour_query = new Query<unsigned short>("HOUR", hour);
  Query<unsigned short>* minute_query = new Query<unsigned short>("MINUTE", minute);
  query.push_back(hour_query);
  query.push_back(minute_query);
  mongocxx::cursor result = query_database(ticker, query);

  double min = std::numeric_limits<double>::max();
  double max = std::numeric_limits<double>::min();
  double open, close;
  double temp = 0;
  bool first = 1;
  for (mongocxx::cursor::iterator iter = result.begin(); iter != result.end(); iter++) {
    double last_price = 0;
    try {
      last_price = (*iter)["LAST_PRICE"].get_double().value;
    }
    catch (...) {
      last_price = (double) (*iter)["LAST_PRICE"].get_int32().value;
    }
    if (first) {
      first = !first;
      open = last_price;
    }
    if (last_price < min) min = last_price;
    if (last_price > max) max = last_price;
    temp = last_price;
  }
  close = temp;
  if (!(min == std::numeric_limits<double>::max() || max == std::numeric_limits<double>::min()))
    return new Bar(ticker, hour, minute, open, close, min, max);
  else return NULL;
}

std::vector<Bar*> Client::get_bars(std::string ticker, unsigned short hour_start, unsigned short hour_end, unsigned short minute_start, unsigned short minute_end) {
  std::vector<Bar*> bars;
  // if (hour_start == hour_end) {
  //   std::vector<QueryBase*> query;
  //   Query<unsigned short>* query1 = new Query<unsigned short>("MINUTE", minute_start, minute_end, 1);
  //   Query<unsigned short>* query2 = new Query<unsigned short>("HOUR", hour_start);
  //   query.push_back(query1);
  //   query.push_back(query2);
  //   mongocxx::cursor result = query_database(ticker, query);
  // }
  unsigned short hour = hour_start;
  for (unsigned short i = minute_start; i <= ((hour_end - hour_start) * 60) + minute_end; i++) {
    if (i != minute_start && i % 60 == 0) hour++;
    Bar* bar = get_bar(ticker, hour, i % 60);
    if (bar != NULL) bars.push_back(bar);
  }
  return bars;
}

double Client::get_sma(std::string ticker, unsigned short offset, unsigned short hour, unsigned short minute) {
  Time timeObj(hour, minute);
  unsigned short* time = timeObj._time;
  double sum = 0;
  for (unsigned short i = 0; i < offset; i++) {
    Bar* bar = get_bar(ticker, time[0], time[1]);
    sum += bar->close;
    timeObj--;
  }
  return sum / offset;
}

double Client::get_sma(std::string ticker, unsigned short offset) {
  Time timeObj;
  return get_sma(ticker, offset, timeObj._time[0], timeObj._time[1]);
}

Time::Time() {
  std::time_t now = std::time(0);
  std::tm* ltm = std::localtime(&now);
  _time[0] = ltm->tm_hour;
  _time[1] = ltm->tm_min;
  _time[2] = ltm->tm_sec;
}

Time::Time(unsigned short hour, unsigned short minute) {
  _time[0] = hour;
  _time[1] = minute;
  _time[2] = 0;
}

Time::Time(unsigned short hour, unsigned short minute, unsigned short second) {
  _time[0] = hour;
  _time[1] = minute;
  _time[2] = second;
}

void Time::operator++(int value) {
  if (_time[1] == 59) {
    _time[0] = _time[0] + 1;
    _time[1] = 0;
  }
  else _time[1] = _time[1] + 1;
}

void Time::operator--(int value) {
  if (_time[1] == 0) {
    _time[0] = _time[0] - 1;
    _time[1] = 59;
  }
  else _time[1] = _time[1] - 1;
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
