#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <iostream>
#include <sqlite3.h>

using namespace std;


class Database {
  sqlite3 *db = NULL;
  char *errMsg = 0;
  
public:
  Database(string name){
    create(name);
  }
  void create(string &name){  // Create database
    if(int err = sqlite3_open(name.c_str(), &db)){
      cout << "error opening database" << name << err;
      // throw new Error('Error opening database')
    } else {
      cout << "Opened database successfully";
    }
  }
  
  int exec(string sql, int (*callback)(void*,int,char**,char**)){
    return sqlite3_exec(db, sql.c_str(), callback, 0, &errMsg);
  }
  
  void destroy(){ // Delete database
    sqlite3_close(db);
  }
};

#endif
