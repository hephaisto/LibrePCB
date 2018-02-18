#include <boost/python.hpp>

#include "../library/sym/symbol.h"

#include "serializablelist.h"

#include "library.h"

namespace librepcb {
namespace python {

using std::shared_ptr;
using boost::python::class_;
using boost::python::init;
using boost::python::no_init;
using boost::python::return_internal_reference;
using boost::python::return_value_policy;
using boost::python::copy_const_reference;
using boost::python::bases;


using namespace librepcb::library;

void init_library()
{
    class_<SymbolPin, shared_ptr<SymbolPin> >(
            "SymbolPin",
            init<const Uuid&, const QString&, const Point&, const Length&, const Angle&>()
            )
        .add_property("uuid", make_function(&SymbolPin::getUuid, return_value_policy<copy_const_reference>()))
        .add_property("name", make_function(&SymbolPin::getName, return_value_policy<copy_const_reference>()), &SymbolPin::setName)
        .add_property("position", make_function(&SymbolPin::getPosition, return_internal_reference<1>()), &SymbolPin::setPosition)
        .add_property("length", make_function(&SymbolPin::getLength, return_internal_reference<1>()), &SymbolPin::setLength)
        .add_property("rotation", make_function(&SymbolPin::getRotation, return_internal_reference<1>()), &SymbolPin::setRotation)
        .def("getName", &SymbolPin::getName, return_value_policy<copy_const_reference>())
        .def("getPosition", &SymbolPin::getPosition, return_internal_reference<1>())
        .def("getLength", &SymbolPin::getLength, return_internal_reference<1>())
        .def("getRotation", &SymbolPin::getRotation, return_internal_reference<1>())
        ;

    DECLARE_SERIALIZABLE_LIST(UuidName, SymbolPin);

    class_<LibraryBaseElement, boost::noncopyable>(
            "LibraryBaseElement",
            no_init
            )
        .add_property("author", make_function(&LibraryBaseElement::getAuthor, return_value_policy<copy_const_reference>()), &LibraryBaseElement::setAuthor)
        .add_property("uuid", make_function(&LibraryBaseElement::getUuid, return_value_policy<copy_const_reference>()))
        //TODO const Version& getVersion() const noexcept {return mVersion;}
        //TODO: const QDateTime& getCreated() const noexcept {return mCreated;}
        //TODO: const QDateTime& getLastModified() const noexcept {return mLastModified;}
        .add_property("deprecated", &LibraryBaseElement::isDeprecated, &LibraryBaseElement::setDeprecated)
        ;

    class_<Symbol, bases<LibraryBaseElement>, boost::noncopyable>(
            "Symbol",
            init<FilePath, bool>()
            )
        .add_property("pins", make_function((SymbolPinList& (Symbol::*)() noexcept) &Symbol::getPins, return_internal_reference<1>()))



        .def("save", &Symbol::save)
        ;
}

}
}
