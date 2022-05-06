#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "alpaca/client.h"
#include "alpaca/alpaca.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

int main(int argc, char* argv[]) {
	for (int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}
	system("./main2");
	
  auto env = alpaca::Environment();
  if (auto status = env.parse(); !status.ok()) {
    std::cerr << "Error parsing config from environment: " << status.getMessage() << std::endl;
    return status.getCode();
  }
  auto client = alpaca::Client(env);

  auto buy_response =
      client.submitOrder("AAPL", 1, alpaca::OrderSide::Buy, alpaca::OrderType::Limit, alpaca::OrderTimeInForce::Day, "3.00");

  if (auto status = buy_response.first; !status.ok()) {
    std::cerr << "Error submitting purchase order: " << status.getMessage() << std::endl;
    return status.getCode();
  }


  return 0;
}
