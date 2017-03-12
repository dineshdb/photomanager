#ifndef FILESDATABASE_HH
#define FILESDATABASE_HH

#include <vector>
#include <mutex>
#include <thread>
#include <iostream>

using namespace std;
#include "ImageDetails.hh"

namespace FilesDatabase {
class FilesDatabase {
	std::vector<ImageDetails> files;
	std::mutex lock;
	
	public:
	FilesDatabase(){}

	void addFile(ImageDetails d){
		lock_guard<mutex> l(lock);
		files.push_back(d);
		cout << "Total :" << files.size()<<endl;
	}
	
	auto getFiles(){
		lock_guard<mutex> l(lock);
		return files;
	}
	
	size_t size(){
		lock_guard<mutex> l(lock);
		return files.size();	
	}
};
}


#endif
