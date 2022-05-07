#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <any>
#include <vector>

#include "client.h"

std::ostream &operator<<(std::ostream &os, Bar const &bar) {
  return os << "Ticker: " << bar.ticker << std::endl << "Hour: " << bar.hour << std::endl
            << "Minute: " << bar.minute << std::endl << "Open: " << bar.open << std::endl << "Close: "
            << bar.close << std::endl << "Low: " << bar.low << std::endl << "High: " << bar.high << std::endl;
}

int main(int argc, char* argv[]) {
  Client client;
  std::vector<Bar> bars = client.get_bars("AAPL", 18, 18, 0, 31);
  for (Bar bar : bars) {
    std::cout << bar << std::endl;
  }
  return 0;
}
