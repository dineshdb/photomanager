#ifndef DIRECTORY_SCANNER
#define DIRECTORY_SCANNER

#include <stack>
#include <vector>
#include <glibmm.h>

class DirectoryScanner {
  std::vector<Glib::ustring> directories;
  std::vector<Glib::ustring> files;

public:
  DirectoryScanner(){
  
  }
  DirectoryScanner(Glib::ustring &folder){
    directories.push_back(folder);
  }
  
  DirectoryScanner(std::vector<Glib::ustring> &folders){
    directories = folders;
  }
  
  void addFolder(Glib::ustring dir){
    directories.push_back(dir);
  }
  
  void start(){
    while(directories.size() != 0){
      Glib::ustring dirname = directories.back();
      directories.pop_back();
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
