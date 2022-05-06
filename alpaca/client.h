#include <string>
#include <mongocxx/client.hpp>

using namespace std;

struct Bar {
    float open, close, low, high;
    unsigned short day, month, year, hour, minute, second;
    Bar(unsigned short day, unsigned short month, unsigned short year, unsigned short hour, unsigned short minute);
};

struct Client {
    Bar get_bars(string ticker, unsigned short day, unsigned short month, unsigned short year, unsigned short hour, unsigned short minute);
    private:
        void query_database(string query);
};
