#include "client.h"

std::ostream &operator<<(std::ostream &os, Bar const &bar) {
  return os << "Ticker: " << bar.ticker << std::endl << "Hour: " << bar.hour << std::endl
            << "Minute: " << bar.minute << std::endl << "Open: " << bar.open << std::endl << "Close: "
            << bar.close << std::endl << "Low: " << bar.low << std::endl << "High: " << bar.high << std::endl;
}

int main(int argc, char* argv[]) {
  Client client;
  // std::vector<Bar*> bars = client.get_bars("AAPL", 18, 18, 0, 30);
  // for (Bar* bar : bars) {
  //   std::cout << *bar << std::endl;
  // }
  // Bar* bar = client.get_bar("AAPL", 9, 30);
  // std::cout << *bar << std::endl;

  std::vector<QueryBase*> query;
  Query<unsigned short>* query1 = new Query<unsigned short>("MINUTE", 30);
  Query<unsigned short>* query2 = new Query<unsigned short>("HOUR", GREATER_THAN, 10);
  query.push_back(query1);
  query.push_back(query2);
  std::vector<bsoncxx::document::view> result = client.query_database("AAPL", query);
  // for (auto i = result->begin(); i != result->end(); i++) {
    // std::cout << bsoncxx::to_json(*i) << std::endl;
  // }

  for (bsoncxx::document::view doc : result) {
    std::cout << bsoncxx::to_json(doc) << std::endl;
  // for (unsigned int i = 0; i < result.size(); i++) {
    // std::cout << "i: " << i << std::endl << bsoncxx::to_json(result[i]) << std::endl;
    // std::cout << result.size() << std::endl;
  // }
  }
  return 0;
}
