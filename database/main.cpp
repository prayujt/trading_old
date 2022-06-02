#include "client.h"

std::ostream &operator<<(std::ostream &os, Bar const &bar) {
  return os << "Ticker: " << bar.ticker << std::endl << "Hour: " << bar.hour << std::endl
            << "Minute: " << bar.minute << std::endl << "Open: " << bar.open << std::endl << "Close: "
            << bar.close << std::endl << "Low: " << bar.low << std::endl << "High: " << bar.high << std::endl;
}

std::ostream &operator<<(std::ostream &os, Time* const &time) {
  return os << time->_time[0] << ":" << time->_time[1] << ":" << time->_time[2] << std::endl;
}

int main(int argc, char* argv[]) {
  Client client;
  // std::vector<Bar*> bars = client.get_bars("AAPL", 9, 9, 30, 31);
  // for (Bar* bar : bars) {
  //   std::cout << *bar << std::endl;
  // }
  double test = client.get_sma("AAPL", 2);
  std::cout << test << std::endl;
  // Bar* bar = client.get_bar("AAPL", 9, 32);
  // std::cout << *bar << std::endl;

  // std::vector<QueryBase*> query;
  // Query<unsigned short>* query1 = new Query<unsigned short>("MINUTE", LESS_THAN, 33);
  // Query<unsigned short>* query2 = new Query<unsigned short>("HOUR", 9);
  // query.push_back(query1);
  // query.push_back(query2);
  // mongocxx::cursor result = client.query_database("AAPL", query);
  // delete query1;
  // delete query2;

  // for (bsoncxx::document::view doc : result) {
  //   std::cout << bsoncxx::to_json(doc) << std::endl;
  // }

  return 0;
}
