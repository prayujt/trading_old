#include <iostream>
#include <stdlib.h>
#include <string>

#include "client.h"

int main(int argc, char* argv[]) {
  Bar bar("SPY", 5, 5);
  std::cout << bar.ticker << std::endl;
  Client client;
  client.query_database("test");
  return 0;
}
