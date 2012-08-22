#include <string>
#include <iostream>
#include <vector>
#include "client/dbclient.h"
#include "client/dbclient_rs.h"
#include "util/net/hostandport.h"

#include "Database.h"

using namespace mongo;
using namespace std;

class Connection {
	public:
		Connection(string host = "", int port = 27017, string username = "", string password = "");
		~Connection();
		void connect();
		vector<string> database_names();
		Database *get_database(string name);
		Connection *get_master(string dbname, string username, string password, bool is_digest = false);
		
		void w(string write_concern);
		string w();
		void wtimeout(int write_timeout);
		int wtimeout();
		void query_timeout(int qtimeout);
		int query_timeout();
		
	private:
		DBClientConnection *_conn;
		DBClientReplicaSet *_replica_conn;
		HostAndPort *_hostport;
		string 	_w;
		int 	_wtimeout;
		int 	_timeout;
		string	_username;
		string 	_password;
		int 	_query_timeout;
};