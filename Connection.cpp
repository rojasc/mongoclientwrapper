#include "Connection.h"

// Todo: replica set connection
Connection::Connection(string host, int port, string username, string password)
{
	// Save in private varibles
	_username = username;
	_password = password;
	
	DBClientConnection *_conn;
	
	// Create out objects for the connecetion
	_hostport = new HostAndPort(host, port);
	 
	// If the user passed in user/pass; go ahead and try to authenicate
	if(username != "" && password != "")
	    authenticate("admin", username, password);
}

void Connection::connect()
{
    string host_port = _hostport->toString();
    string error_msg = "";
    _conn = ConnectionString::parse(host_port, error_msg).connect(error_msg);
        
	
	// Now lets check if we're in replica set.
	BSONObj msg;
	((DBClientConnection*)_conn)->simpleCommand("admin", &msg, "replSetGetStatus");
	if(msg.getIntField("ok") != 1)
	     return;
	else{
    	string conn_str = msg.getStringField("setName");
    	conn_str.append("/").append(host_port);
    	ConnectionString cs = ConnectionString::parse(conn_str, error_msg);
    
        if(!cs.isValid())
            throw error_msg;
    	
	    _conn = cs.connect(error_msg);
	    
	    if(error_msg != "")
	        throw error_msg;
    }
	
}

bool Connection::authenticate(string dbname, string username, string password)
{
	string auth_error = "";
	if(!_conn->auth(dbname, username, password, auth_error))
		throw "Could not authenicate: "+ auth_error;
}

    
vector<string> Connection::database_names()
{
    // Stupid mongo returns it in a std::list. I like std::vector's better. Square brackets...
    list<string> list_dbnames = _conn->getDatabaseNames();    
    vector<string> vector_dbnames;

    // copy contents of list_dbnames to vector_dbnames (using iterators if you dont know what .begin()/.end() is)    
    copy(list_dbnames.begin(), list_dbnames.end(), vector_dbnames.begin());
    
    return vector_dbnames;
}


Database* Connection::get_database(string name)
{
    return new Database(_conn);
}

Connection* Connection::get_master()
{
    bool ismaster = false;
    _conn->isMaster(ismaster);
    if(ismaster)
        return this;
    
    
    
}


Connection::~Connection()
{
	if(_conn != NULL)
		delete _conn;
		
	delete _hostport;
	
	
	_conn = NULL;
	_hostport = NULL;
	
}