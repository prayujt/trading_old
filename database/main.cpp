#include "client.h"

std::ostream &operator<<(std::ostream &os, Bar const &bar) {
  return os << "Ticker: " << bar.ticker << std::endl << "Hour: " << bar.hour << std::endl
            << "Minute: " << bar.minute << std::endl << "Open: " << bar.open << std::endl << "Close: "
            << bar.close << std::endl << "Low: " << bar.low << std::endl << "High: " << bar.high << std::endl;
}

int main(int argc, char* argv[]) {
  // Client client;
  // std::vector<Bar*> bars = client.get_bars("AAPL", 18, 18, 0, 30);
  // for (Bar* bar : bars) {
  //   std::cout << *bar << std::endl;
  // }
  // Bar* bar = client.get_bar("AAPL", 18, 30);
  // std::cout << *bar << std::endl;

  // Query query();
  // std::vector<bsoncxx::document::view> result = client.query_database("AAPL", query);

  // std::vector<bsoncxx::document::view> result = client.complex_query_database("AAPL", query, operators);
  // for (unsigned int i = 0; i < result.size(); i++) {
  //   std::cout << bsoncxx::to_json(result[i]) << std::endl;
  // }
  return 0;
}
