#include "../database/database.h"
#include "../exec/client.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

ostream &operator<<(ostream &os, Bar const &bar) {
  return os << "Ticker: " << bar.ticker << endl << "Hour: " << bar.hour << endl
            << "Minute: " << bar.minute << endl << "Open: " << bar.open << endl << "Close: "
            << bar.close << endl << "Low: " << bar.low << endl << "High: " << bar.high << endl;
}

ostream &operator<<(ostream &os, Time* const &time) {
  return os << time->_time[0] << ":" << time->_time[1] << ":" << time->_time[2] << endl;
}

//int main(int argc, char* argv[]) {
//  Client client;

  // Bar* bar = client.get_bar("AAPL", 9, 32);
  // queue.enqueue(bar);
  // bar = client.get_bar("AAPL", 9, 33);
  // queue.enqueue(bar);
  // bar = client.get_bar("AAPL", 9, 34);
  // queue.enqueue(bar);

  // cout << *(queue.dequeue()) << endl;
//  while (1) {
    // client.update_bars("AAPL");
    // Client::Queue queue = client.sma_bars["AAPL"];
    // for (auto i = queue.begin(); i != queue.end(); i++) {
    //   cout << *(i.value()) << endl;
    // }
    // cout << client.get_sma("AAPL", 32) << endl;
    // cout << client.get_sma("AAPL", 50) << endl;
    // cout << "----------------" << endl;
//  }

//  return 0;
//}

int main(int argc, char* argv[]) {
  Database database;
  Bar* bar = database.get_bar("AAPL", 9, 32);
  cout << *bar << endl;

  auto env = alpaca::Environment();
  if (auto status = env.parse(); !status.ok())
  {
    cerr << "Error parsing config from environment: " << status.getMessage() << endl;
    return status.getCode();
  }
  auto client = alpaca::Client(env);

  auto buy_response =
      client.submit_order("AAPL", 1, alpaca::OrderSide::Buy, alpaca::OrderType::Limit, alpaca::OrderTimeInForce::Day, "3.00");

  if (auto status = buy_response.first; !status.ok())
  {
    cerr << "Error submitting purchase order: " << status.getMessage() << endl;
    cerr << status.getCode();
  }
  else
  {
    cout << client.get_order_by_client_id(buy_response.second.client_order_id).second.symbol << endl;
  }

  return 0;
}
