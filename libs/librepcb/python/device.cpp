#include <boost/python.hpp>

#include "../library/dev/device.h"

#include "serializablelist.h"
#include "propertywrapper.h"

#include "device.h"

namespace librepcb {
namespace python {

namespace dev {

using std::shared_ptr;
using boost::python::class_;
using boost::python::init;
using boost::python::no_init;
using boost::python::return_internal_reference;
using boost::python::return_value_policy;
using boost::python::copy_const_reference;
using boost::python::bases;


using namespace librepcb::library;

void init()
{
    // DevicePadSignalMapItem
    auto devicePadSignalMapItemClass = class_<DevicePadSignalMapItem, shared_ptr<DevicePadSignalMapItem> >(
            "DevicePadSignalMapItem",
            init<const Uuid&, const Uuid&>()
            )
        .add_property("uuid", make_function(&DevicePadSignalMapItem::getUuid, return_value_policy<copy_const_reference>()))
        .add_property("padUuid", make_function(&DevicePadSignalMapItem::getPadUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(devicePadSignalMapItemClass, DevicePadSignalMapItem, Uuid, SignalUuid, "signalUuid");

    DECLARE_SERIALIZABLE_LIST(DevicePadSignalMapItem);

    // Device
    auto deviceClass = class_<Device, shared_ptr<Device> >(
            "Device"
            init<const Uuid&, const Version&, const QString&, const QString&, const QString&, const QString&>(),
            init<const FilePath&, bool>()
            )
        .add_property("uuid", make_function(&Device::getUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(deviceClass, Device, Uuid, ComponentUuid, "componentUuid");
    ADD_WRAPPED_PROPERTY(deviceClass, Device, Uuid, PackageUuid, "packageUuid");
    ADD_LIST_PROPERTY(deviceClass, Device, DevicePadSignalMapItem, PadSignalMap, "padSignalMap");
}

}
}
}
