#include "Database.hh"
#include <iostream>

using namespace std;


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(){
  Database db("mydb.sqlite3");
  db.exec("CREATE TABLE company(ID INT NOT NULL, NAME TEXT)", callback);
  db.exec("INSERT INTO company (id, name) VALUES (0, 'Dinesh')", callback);
  
}
