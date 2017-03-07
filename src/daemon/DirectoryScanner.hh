#ifndef DIRECTORY_SCANNER
#define DIRECTORY_SCANNER

#include <stack>
#include <vector>
#include <glibmm.h>

class DirectoryScanner {
  std::stack<Glib::ustring> directories;
  std::vector<Glib::ustring> files;

public:
  DirectoryScanner(){}
  
  void addFolder(Glib::ustring dir){
    directories.push(dir);
  }
  
  void start(){
    while(directories.size() != 0){
      Glib::ustring dirname = directories.top();
      directories.pop();
      Glib::Dir dir(dirname);
      
      Glib::ustring entry;
      while(( entry = dir.read_name()) != ""){
        Glib::ustring fullURL = dirname + "/" + entry;
        if (Glib::file_test(fullURL, Glib::FILE_TEST_IS_DIR)){
          addFolder(fullURL);
        } else {
          // TODO Filter out image files only
          files.push_back(fullURL);
        }
      }
    }
  }
  
  auto getFiles(){
     return files;
  }
};
#endif
