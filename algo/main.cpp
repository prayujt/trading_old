#include "../database/database.h"
#include "../exec/client.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

ostream &operator<<(ostream &os, Bar const &bar)
{
  return os << "Ticker: " << bar.ticker << endl << "Hour: " << bar.hour << endl
            << "Minute: " << bar.minute << endl << "Open: " << bar.open << endl << "Close: "
            << bar.close << endl << "Low: " << bar.low << endl << "High: " << bar.high << endl;
}

ostream &operator<<(ostream &os, Time* const &time)
{
  return os << time->_time[0] << ":" << time->_time[1] << ":" << time->_time[2] << endl;
}

struct memory_struct{
  char* buffer;
  size_t size;
};

// void algo(string ticker, Database* database)
// {
//   // Bar* bar = database->get_bar(ticker, 9, 32);
//   // cout << *bar << endl;
//   while (1)
//   {
//     database->update_bars(ticker);
//     cout << *(database->sma_bars.begin_from_end().value());
//   }
// }

int main(int argc, char* argv[])
{

  string ticker = argv[1];
  Database database(ticker);

  while (database.sma_bars.size != database.sma_bars.max_size)
  {
    cout << "loading..." << endl;
    database.update_bars(ticker);
  }
  bool bought = false;
  while (1)
  {
    double rsi = database.calculate_rsi(ticker);
    double macd = database.calculate_macd(ticker);
    if (rsi < 30 && macd < -0.025 && !bought)
    {
      string msg = "curl -X POST -H \"Content-Type: application/json\" -d '{\"value1\":\"Bought " + ticker + " at " + to_string(database.sma_bars.peek()->close) + "\"}' https://maker.ifttt.com/trigger/notify/with/key/co2pjtO4l9SKFTkaXMYz6H -o /dev/null -s";
      system(msg.c_str());
      cout << "bought" << endl;
      bought = true;
    }
    if (rsi > 40) bought = false;
    cout << "RSI: " << rsi << endl;
    cout << "MACD: " << macd << endl << endl;
    // cout << "Last Bar: " << endl << *(database.sma_bars.peek()) << endl;
  }

  // auto env = alpaca::Environment();
  // if (auto status = env.parse(); !status.ok())
  // {
  //   cerr << "Error parsing config from environment: " << status.getMessage() << endl;
  //   return status.getCode();
  // }
  // auto client = alpaca::Client(env);

  // auto buy_response =
  //     client.submit_order("AAPL", 1, alpaca::OrderSide::Buy, alpaca::OrderType::Limit, alpaca::OrderTimeInForce::Day, "3.00");

  // if (auto status = buy_response.first; !status.ok())
  // {
  //   cerr << "Error submitting purchase order: " << status.getMessage() << endl;
  //   cerr << status.getCode();
  // }
  // else
  // {
  //   cout << client.get_order_by_client_id(buy_response.second.client_order_id).second.symbol << endl;
  // }

  return 0;
}
