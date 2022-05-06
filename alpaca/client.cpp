#include "client.h"

void Client::query_database(string query) {

}

Bar Client::get_bars(string ticker, unsigned short day, unsigned short month, unsigned short year, unsigned short hour, unsigned short minute) {
    return Bar(day, month, year, hour, minute);
}
