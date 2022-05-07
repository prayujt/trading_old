#include <iostream>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <any>

#include "client.h"

int main(int argc, char* argv[]) {
  Client client;
  std::unordered_map<std::string, std::any> query{
    {"LAST_PRICE", 156.84},
    {"SECOND", 34},
  };
  client.query_database("AAPL", query);
  return 0;
}
