#include "../database/client.h"

ostream &operator<<(ostream &os, Bar const &bar) {
  return os << "Ticker: " << bar.ticker << endl << "Hour: " << bar.hour << endl
            << "Minute: " << bar.minute << endl << "Open: " << bar.open << endl << "Close: "
            << bar.close << endl << "Low: " << bar.low << endl << "High: " << bar.high << endl;
}

ostream &operator<<(ostream &os, Time* const &time) {
  return os << time->_time[0] << ":" << time->_time[1] << ":" << time->_time[2] << endl;
}

int main(int argc, char* argv[]) {
  Client client;

  Client::Queue queue = client.sma_bars["AAPL"];
  // Bar* bar = client.get_bar("AAPL", 9, 32);
  // queue.enqueue(bar);
  // bar = client.get_bar("AAPL", 9, 33);
  // queue.enqueue(bar);
  // bar = client.get_bar("AAPL", 9, 34);
  // queue.enqueue(bar);

  // cout << *(queue.dequeue()) << endl;
  for (auto i = queue.begin(); i != queue.end(); i++) {
    cout << *(i.value()) << endl;
  }

  return 0;
}
