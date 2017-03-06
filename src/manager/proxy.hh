
#ifndef SERVER_PROXY_H
#define SERVER_PROXY_H

#include <giomm.h>
#include <glibmm.h>

class ServerProxy {
  Glib::RefPtr<Gio::DBus::Connection> connection;
  Glib::RefPtr<Gio::Cancellable> cancellable;
  Glib::RefPtr<Gio::DBus::Proxy> proxy;

public:
  ServerProxy(){
    connection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
    proxy = Gio::DBus::Proxy::create_sync(
      connection, 
      "com.dinnux.PhotoManager",                        // service name
      "/com/dinnux/PhotoManager/Photo",                 // object path
      "com.dinnux.PhotoManager.Photo",  				// interface name
      cancellable);
  }

  Glib::ustring getTime(){
    Glib::VariantContainerBase r = proxy->call_sync("GetTime", cancellable);
    Glib::Variant< Glib::ustring > introspection_str;
    r.get_child(introspection_str);
    return introspection_str.get();
  }
  
  std::vector<Glib::ustring> getPhotos(){
  	Glib::VariantContainerBase photos = proxy->call_sync("GetPhotos", cancellable);
  	Glib::Variant<std::vector<Glib::ustring>> introspection_str;
  	photos.get_child(introspection_str);
  	return introspection_str.get();
  }
};

#endif
