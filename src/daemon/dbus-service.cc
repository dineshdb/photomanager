#include "dbus-service.hh"
#include "DirectoryScanner.hh"
#include "recognizer.hh"
#include "FileDatabase.hh"

extern FilesDatabase::FilesDatabase scannedFiles;

static Glib::TimeVal curr_alarm;

void on_method_call(
  const Glib::RefPtr<Gio::DBus::Connection>& /* connection */,
  const Glib::ustring& /* sender */, 
  const Glib::ustring& /* object_path */,
  const Glib::ustring& /* interface_name */,
  const Glib::ustring& method_name,
  const Glib::VariantContainerBase& parameters,
  const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation)
{
  if (method_name == "GetTime") {
    Glib::TimeVal curr_time;
    curr_time.assign_current_time();

    const Glib::ustring time_str = curr_time.as_iso8601();
    const auto time_var = Glib::Variant<Glib::ustring>::create(time_str);

    // Create the tuple.
    Glib::VariantContainerBase response = Glib::VariantContainerBase::create_tuple(time_var);
    invocation->return_value(response);
  } else if (method_name == "SetAlarm") {
    // Get the parameter tuple.
    // Glib::VariantContainerBase parameters;
    // invocation->get_parameters(parameters);

    // Get the variant string.
    Glib::Variant<Glib::ustring> param;
    parameters.get_child(param);

    // Get the time string.
    const Glib::ustring time_str = param.get();

    if (!curr_alarm.assign_from_iso8601(time_str)) {
      Gio::DBus::Error error(
        Gio::DBus::Error::INVALID_ARGS, "Alarm string is not in ISO8601 format.");
      invocation->return_error(error);
    }
    
  } else if( method_name == "GetPhotos") { 		
		auto files = scannedFiles.getFiles();
		
		// TODO Serialize this vector and send it to client.
  		//const auto photos = Glib::Variant<std::vector<Glib::ustring>>::create(file_names);
  		//auto response = Glib::VariantContainerBase::create_tuple(photos);
  		//invocation->return_value(response);
  		
  		// files is of type std::vector<ImageDetails>
  		cout << "Got here" << files.size() << endl;
  		for (auto file : files){
  			cout << file.path << endl;
  		}
  		std::vector<Glib::ustring> crap_container;
  		for (auto file : files)
  		{
  			// file is of type ImageDetails
  			crap_container.push_back(file.getString());
  		}
  		
  		const auto crap_variant = Glib::Variant<std::vector<Glib::ustring>>::create(crap_container);
  		Glib::VariantContainerBase crap_variant_base = Glib::VariantContainerBase::create_tuple(crap_variant);
  		invocation->return_value(crap_variant_base);
  		
  } else if( method_name == "GetPeople") {
  
  } else if( method_name == "GetEvents") {
  } else if( method_name == "GetTags") {
  } else if( method_name == "GetPhotoDetail") {
  } else if( method_name == "GetEvents") {
  } else {
    Gio::DBus::Error error(Gio::DBus::Error::UNKNOWN_METHOD, "Method does not exist.");
    invocation->return_error(error);
  }
}

const Gio::DBus::InterfaceVTable interface_vtable(sigc::ptr_fun(&on_method_call));

void on_name_acquired(const Glib::RefPtr<Gio::DBus::Connection>&/*connection*/,const Glib::ustring&/*name*/){}
/**
 * @brief Called right after the bus is acquired. Exports an object to the bus.
 * @param connection DBus connection object
 * @param name Acquired dbus name
 * @return void
 * @see https://bugzilla.gnome.org/show_bug.cgi?id=646417 about avoiding the
 * 	repitition of the interface name.
 */
void on_bus_acquired(const Glib::RefPtr<Gio::DBus::Connection>& connection,
			const Glib::ustring& /* name */)
{
  try {
	registered_id = connection->register_object(
		"/com/dinnux/PhotoManager/Photo",
		introspection_data->lookup_interface(),
		interface_vtable
	);
  } catch (const Glib::Error& ex) {
    std::cerr << "Registration of object failed." << std::endl;
  }
  return;
}

/**
 * @brief Function called upon the loss of name, or failure to acquire name
 */
void on_name_lost(const Glib::RefPtr<Gio::DBus::Connection>& connection, const Glib::ustring& /* name */){
	connection->unregister_object(registered_id);
}

int dbus_init(){
	Gio::init();
	try{
		introspection_data = Gio::DBus::NodeInfo::create_for_xml(introspection_xml);
	} catch (const Glib::Error& ex) {
		std::cerr << "Unable to create introspection data: "
			<< ex.what() << "." << std::endl;
		return 1;
	}

  	const auto id = Gio::DBus::own_name(
  	    Gio::DBus::BUS_TYPE_SESSION,
		BUS_Address,
		sigc::ptr_fun(&on_bus_acquired),
		sigc::ptr_fun(&on_name_acquired),
		sigc::ptr_fun(&on_name_lost)
	);

  	auto loop = Glib::MainLoop::create();
  	cout << "Initializing daemon" << endl;
  	loop->run();

  	Gio::DBus::unown_name(id);
  	return 0;
}
