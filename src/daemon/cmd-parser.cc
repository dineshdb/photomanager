#include "cmd-parser.hh"
void predictPhotos(string filename){
	Recognizer r;
	Mat photo = r.loadPhotoBW(filename);
	std::vector<Rect> faces = r.getFaces(photo);
	cout << "There are "<< faces.size() << " faces." << endl;
	int i =1;
	for(auto const& face: faces) {
		Mat faceImg = r.cropFace (photo, face);
		cout << i <<  " : "
			<< r.predictPhoto(faceImg) << endl;
	}
}

void updatePrediction(string filename, int userid){
	Recognizer r;
	Mat photo = r.loadPhotoBW(filename);
	std::vector<Rect> faces = r.getFaces(photo);
	cout << "There ar "<< faces.size() << " photos." << endl;
	int i=1, id;
	std::vector<Mat> nfaces;
	std::vector<int> nnum;

	for(auto const& face: faces) {
		Mat faceImg = r.cropFace (photo, face);
		cout << i <<  " : " ;
		cin >> id;
		nfaces.push_back(faceImg);
		nnum.push_back(id);
	}
	r.updatePhoto(nfaces, nnum);
}

int CommandParser::parse(int argc, char **argv){
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
//			scanFolders();
			dbus_init();
		} else if (vm.count("update")){
			int userid = vm["userid"].as<int>();
			string filename = vm["update"].as<string>();
			updatePrediction(filename, userid);

		} else if (vm.count("predict")) {
			string filename = vm["predict"].as<string>();
			predictPhotos(filename);
		} else {
			std::cout << desc << '\n';
		}
	} catch (...) {
		std::cerr << "Error parsing cmd options\n";
		return -1;
  	}
  	return 0;
}
