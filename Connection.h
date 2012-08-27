#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "mongo/client/dbclient.h"
#include "mongo/util/net/hostandport.h"
#include "mongo/bson/bsonobj.h"

#include "Database.h"

using namespace mongo;
using namespace std;

class Connection {
	public:
		Connection(string host = "", int port = 27017, string username = "", string password = "");
		~Connection();
		void connect();
		bool authenticate(string dbname, string username, string password);
		vector<string> database_names();
		Database *get_database(string name);
		Connection *get_master();
		
		// Getter and Setters
		void w(string write_concern);
		string w();
		void wtimeout(int write_timeout);
		int wtimeout();
		void query_timeout(int qtimeout);
		int query_timeout();
		
	private:
		DBClientBase *_conn;
		HostAndPort  *_hostport;
		
		string 	_w;
		int 	_wtimeout;
		int 	_timeout;
		string	_username;
		string 	_password;
		int 	_query_timeout;
		
		bool is_master;
		bool is_connected;
};