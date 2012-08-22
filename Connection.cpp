#include "Connection.h"

// Todo: replica set connection
Connection::Connection(string host, int port, string username, string password)
{
	// Save in private varibles
	_username = username;
	_password = password;
	
	// Create out objects for the connecetion
	_hostport = new HostAndPort(host, port);
	_conn = new DBClientConnection(true);
	
	// Try to connect. If this fails a `UserException` is thrown.
	_conn->connect(_hostport->toString());
	
	// If the user passed in a user and password, lets try to authenicate them.
	if(username != "" && password != ""){
		string auth_error = "";
		if(!_conn->auth("admin", _username, _password, auth_error))
			throw "Could not authenicate: "+ auth_error;
	}
}

Connection::~Connection()
{
	if(_conn != NULL)
		delete _conn;
		
	if(_replica_conn)
		delete _replica_conn;
		
	delete _hostport;
}