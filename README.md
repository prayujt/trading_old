# Alpaca Trading
Requires the following environment variables to be set
* `TDA_CLIENT_KEY` -> application_key registered with your TD Ameritrade Developer Account
* `TDA_ACCOUNT_ID` -> TD Ameritrade Brokerage ID
* `TOKEN_PATH` -> path in which the token containing the access code is stored, must be generated using tda-api
* `MONGO_DB_HOST` -> MongoDB location
* `MONGO_DB_USERNAME` -> MongoDB username
* `MONGO_DB_PASSWORD` -> MongoDB password
* `TICKER_PATH` -> path to tickers that are being streamed
* `APCA_API_KEY_ID` -> client key from Alpaca brokerage account
* `APCA_API_SECRET_KEY` -> secret key from Alpaca brokerage account
* `APCA_API_BASE_URL` -> endpoint for access to Alpaca brokerage
* `APCA_API_DATA_URL` -> url to stream market data from (optional, unless using Alpaca for data streaming)
## Build
### C++
Requires make
## Dependencies
### C++
* mongoc-driver
* mongocxx
* bsoncxx
* libssl
* libcrypto
* rapidjson
* glog
### Python
* asyncio
* pymongo
* tda-api
