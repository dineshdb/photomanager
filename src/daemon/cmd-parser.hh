#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <iostream>
#include <boost/program_options.hpp>

#include "dbus-service.hh"
#include "recognizer.hh"

extern FilesDatabase::FilesDatabase scannedFiles;

namespace cmd= boost::program_options;

using namespace std;


void updatePrediction(Glib::ustring filename){
	Recognizer r;
	Mat frame = imread(filename, cv::IMREAD_GRAYSCALE);
	auto faces = r.getFaces(frame);
	vector<Mat> nfaces;
	vector<int> nids;
	
	cout << faces.size()<<endl;
	for(auto face : faces){
	    int id;
	    	    
		cout << "Enter face id for " << face;
		cin >> id;
		nfaces.push_back(r.cropFace(frame, face));
		nids.push_back(id);			
	}
	r.updatePhoto(nfaces, nids);
	r.save();
}
	
void predictPhoto(Glib::ustring filename){
	Recognizer r;
	auto faceDetails = r.analyzePhoto(filename);
	for(auto face : faceDetails.faces){
		cout << face.bounds << " : " << face.label << ":"<< face.confidence << endl;
	}
}
void scanFolders(){
	std::vector<Glib::ustring> folders;
	folders.push_back(Glib::get_home_dir() + "/Pictures/test");
	DirectoryScanner s(folders);
	s.start();
	// TODO Diff the result with preprocessed files
	auto files = s.getFiles(); 
	Recognizer r;
	 
	for( auto file : files){
	  auto details = r.analyzePhoto(file);
	  scannedFiles.addFile(details);
	}

	cout << "Finished scanning files" << endl;
	cout << "Scanned "<< scannedFiles.size() << endl;
}

class CommandParser {
public:
	CommandParser(){}
	int parse(int argc, char **argv){
		try {

			cmd::options_description desc = {"Options"};
			desc.add_options()
		  		("help,h", "Help screen")
		  		("daemon,d", "Run As Daemon/DBUS Server")
		  		("update,u", cmd::value<string>(), "Update Face database")
		  		("predict,p", cmd::value<string>(), "Predict Face")
		  		("userid,i", cmd::value<int>(), "User to be updated with");

		  	cmd::command_line_parser parser = {argc, argv};
			parser.options(desc).allow_unregistered().style(
		  		cmd::command_line_style::default_style |
		  		cmd::command_line_style::allow_slash_for_short
		  	);

			cmd::parsed_options parsed = parser.run();

			cmd::variables_map vm;
			cmd::store(parsed, vm);
			cmd::notify(vm);

			if (vm.count("help"))
			  std::cout << desc << '\n';
			else if (vm.count("daemon")){
				thread t(scanFolders);
				dbus_init();
				t.join();
			} else if (vm.count("update")){
				auto filename = vm["update"].as<string>();
				updatePrediction((Glib::ustring) filename);

			} else if (vm.count("predict")) {
				auto filename = vm["predict"].as<string>();
				predictPhoto((Glib::ustring) filename);
			} else {
				std::cout << desc << '\n';
			}
		} catch (...) {
			std::cerr << "Error parsing cmd options\n";
			return -1;
	  	}
	  	return 0;
	}
};
#endif
