#ifndef DBUS_SERVICE_HH
#define DBUS_SERVICE_HH

#include <giomm.h>
#include <glibmm.h>
#include <iostream>

using namespace std; 

namespace {
	std::string introspection_xml=
	"<node>"
	"  <interface name='com.dinnux.PhotoManager.Photo'>"
	"    <method name='GetTime'>"
    "      <arg type='s' name='iso8601' direction='out'/>"
	"    </method>"
    "    <method name='SetAlarm'>"
	"      <arg type='s' name='iso8601' direction='in'/>"
    "    </method>"
	"  </interface>"
    "</node>";
    guint registered_id;
    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data ;
    const std::string BUS_Address = "com.dinnux.PhotoManager";
}
void on_method_call(
	const Glib::RefPtr<Gio::DBus::Connection>& connection,
	const Glib::ustring& sender, const Glib::ustring& object_path,
	const Glib::ustring& interface_name, const Glib::ustring& method_name,
	const Glib::VariantContainerBase& parameters,
	const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation
);

int dbus_init();
void on_name_lost(const Glib::RefPtr<Gio::DBus::Connection>& connection, const Glib::ustring& name);
void on_bus_acquired(const Glib::RefPtr<Gio::DBus::Connection>& connection, const Glib::ustring& name);
void on_name_acquired(const Glib::RefPtr<Gio::DBus::Connection>& connection, const Glib::ustring& name);
#endif
