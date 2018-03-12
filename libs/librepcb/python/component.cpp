#include <boost/python.hpp>

#include "../library/cmp/component.h"

#include "serializablelist.h"
#include "propertywrapper.h"

#include "component.h"

namespace librepcb {
namespace python {

namespace cmp {

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
    // SignalRole

    auto signalRoleClass = class_<SignalRole, shared_ptr<SignalRole> >(
            "SignalRole"
            )
        .def("passive",   make_function(&SignalRole::passive  , return_value_policy<copy_const_reference>())).staticMethod("passive")
        .def("power",     make_function(&SignalRole::power    , return_value_policy<copy_const_reference>())).staticMethod("power")
        .def("input",     make_function(&SignalRole::input    , return_value_policy<copy_const_reference>())).staticMethod("input")
        .def("output",    make_function(&SignalRole::output   , return_value_policy<copy_const_reference>())).staticMethod("output")
        .def("inout",     make_function(&SignalRole::inout    , return_value_policy<copy_const_reference>())).staticMethod("inout")
        .def("opendrain", make_function(&SignalRole::opendrain, return_value_policy<copy_const_reference>())).staticMethod("opendrain")
        .def(self == other<SignalRole>)
        //.def("getAllRoles", &SignalRole::getAllRoles) // TODO
        ;

    // ComponentSignal
    auto componentSignalClass = class_<ComponentSignal, shared_ptr<ComponentSignal> >(
            "ComponentSignal",
            init<const Uuid&, const QString&>()
            )
        .add_property("uuid", make_function(&ComponentSignal::getUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(componentSignalClass, ComponentSignal, QString, Name, "name");
    ADD_WRAPPED_PROPERTY(componentSignalClass, ComponentSignal, SignalRole, Role, "role");
    ADD_WRAPPED_PROPERTY(componentSignalClass, ComponentSignal, QString, ForcedNetName, "forcedNetName");
    ADD_WRAPPED_BOOL_PROPERTY(componentSignalClass, ComponentSignal, bool, Required, "required");
    ADD_WRAPPED_BOOL_PROPERTY(componentSignalClass, ComponentSignal, bool, Negated, "negated");
    ADD_WRAPPED_BOOL_PROPERTY(componentSignalClass, ComponentSignal, bool, Clock, "clock");
    ADD_WRAPPED_BOOL_PROPERTY(componentSignalClass, ComponentSignal, bool, NetSignalNameForced, "netSignalNameForced");

    DECLARE_SERIALIZABLE_LIST(ComponentSignal);

    // CmpSigPinDisplayType
    // TODO

    // ComponentPinSignalMapItem
    auto componentPinSignalMapItemClass = class_<ComponentPinSignalMapItem, shared_ptr<ComponentPinSignalMapItem> >(
            "ComponentPinSignalMapItem",
            init<const Uuid&, const Uuid&, const CmpSigPinDisplayType& displayType>()
            )
        .add_property("pinUuid", make_function(&ComponentPinSignalMapItem::getPinUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(componentPinSignalMapItemClass, ComponentPinSignalMapItem, Uuid, SignalUuid, "signalUuid");
    DECLARE_SERIALIZABLE_LIST(ComponentPinSignalMapItem);

    // ComponentSymbolVariantItem
    auto componentSymbolVariantItemClass = class_<ComponentSymbolVariantItem, shared_ptr<ComponentSymbolVariantItem> >(
            "ComponentSymbolVariantItem",
            init<const Uuid&, const Uuid&, bool, const QString&>()
            )
        .add_property("uuid", make_function(&ComponentSymbolVariantItem::getUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(componentSymbolVariantItemClass, ComponentSymbolVariantItem, Uuid, SymbolUuid, "symbolUuid");
    ADD_WRAPPED_PROPERTY(componentSymbolVariantItemClass, ComponentSymbolVariantItem, Point, SymbolPosition, "symbolPosition");
    ADD_WRAPPED_PROPERTY(componentSymbolVariantItemClass, ComponentSymbolVariantItem, Angle, SymbolRotation, "symbolRotation");
    ADD_WRAPPED_BOOL_PROPERTY(componentSymbolVariantItemClass, ComponentSymbolVariantItem, bool, Required, "required");
    ADD_WRAPPED_PROPERTY(componentSymbolVariantItemClass, ComponentSymbolVariantItem, QString, Suffix, "suffix");
    ADD_LIST_PROPERTY(componentSymbolVariantItemClass, ComponentSymbolVariantItem, ComponentPinSignalMapItem, PinSignalMap, "pinSignalMap");

    DECLARE_SERIALIZABLE_LIST(ComponentSymbolVariantItem);

    // ComponentSymbolVariant
    auto componentSymbolVariantClass = class_<ComponentSymbolVariant, shared_ptr<ComponentSymbolVariant> >(
            "ComponentSymbolVariant",
            init<const Uuid&, const QString&, const QString&, const QString&>()
            )
        .add_property("uuid", make_function(&ComponentSymbolVariant::getUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_LIST_PROPERTY(componentSymbolVariantClass, ComponentSymbolVariant, ComponentSymbolVariantItem, SymbolItems, "symbolItems");
    ADD_WRAPPED_PROPERTY(componentSymbolVariantClass, ComponentSymbolVariant, QString, Norm, "norm");

    DECLARE_SERIALIZABLE_LIST(ComponentSymbolVariant);

    // Component
    auto componentClass = class_<Component, shared_ptr<Component> >(
            "Component",
            init<FilePath, bool>()
            )
        .add_property("uuid", make_function(&Component::getUuid, return_value_policy<copy_const_reference>()))
        ;

    ADD_LIST_PROPERTY(componentClass, Component, ComponentSignal, Signals, "signals");
    ADD_LIST_PROPERTY(componentClass, Component, ComponentSymbolVariant, SymbolVariants, "symbolVariants");
}

}
}
}
