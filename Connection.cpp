#include "Connection.h"

// Todo: replica set connection
Connection::Connection(bool auto_connect, string host, int port, string username, string password)
{
	// Save in private varibles
	_username = username;
	_password = password;
	
	// Create out objects for the connecetion
	_hostport = new HostAndPort(host, port);
	_conn = new DBClientConnection(true);		//True is for 'auto_reconnect'...Maybe should be an option.
	
	if(auto_connect){
		connect();
		
		// If the user passed in a user and password, lets try to authenicate them.
		if(_username != "" && _password != ""){
			authenticate("admin", _username, _password);
		}
	}
}

void Connection::connect()
{
	// Try to connect. If this fails a `UserException` is thrown.
	_conn->connect(_hostport->toString());
}

bool Connection::authenticate(string dbname, string username, string password)
{
	string auth_error = "";
	if(!_conn->auth(dbname, username, password, auth_error))
		throw "Could not authenicate: "+ auth_error;
}

Connection::~Connection()
{
	if(_conn != NULL)
		delete _conn;
		
	if(_replica_conn)
		delete _replica_conn;
		
	delete _hostport;
}