#include <boost/python.hpp>

#include "../library/sym/symbol.h"
#include "../library/sym/cmd/cmdsymbolpinedit.h"

#include "serializablelist.h"
#include "propertywrapper.h"

#include "library.h"

namespace librepcb {
namespace python {

namespace sym {

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
    // SymbolPin
    auto symbolPinClass = class_<SymbolPin, shared_ptr<SymbolPin> >(
            "SymbolPin",
            init<const Uuid&, const QString&, const Point&, const Length&, const Angle&>()
            )
        .add_property("uuid", make_function(&SymbolPin::getUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(symbolPinClass, SymbolPin, QString, Name, "name");
    ADD_WRAPPED_PROPERTY(symbolPinClass, SymbolPin, Point, Position, "position");
    ADD_WRAPPED_PROPERTY(symbolPinClass, SymbolPin, Length, Length, "length");
    ADD_WRAPPED_PROPERTY(symbolPinClass, SymbolPin, Angle, Rotation, "rotation");

    DECLARE_SERIALIZABLE_LIST(SymbolPin);

    // LibraryBaseElement
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

    // Symbol
    auto symbolClass = class_<Symbol, bases<LibraryBaseElement>, boost::noncopyable>(
            "Symbol",
            init<FilePath, bool>()
            )
        //.add_property("polygons", make_function((PolygonList& (Symbol::*)() noexcept) &Symbol::getPolygons, return_internal_reference<1>()))
        //.add_property("ellipses", make_function((EllipseList& (Symbol::*)() noexcept) &Symbol::getEllipses, return_internal_reference<1>()))



        .def("save", &Symbol::save)
        ;
    ADD_LIST_PROPERTY(symbolClass, Symbol, SymbolPin, Pins, "pins");
    ADD_LIST_PROPERTY(symbolClass, Symbol, Text, Texts, "texts");
    ADD_LIST_PROPERTY(symbolClass, Symbol, Polygon, Polygons, "polygons");
    ADD_LIST_PROPERTY(symbolClass, Symbol, Ellipse, Ellipses, "ellipses");
    //addListProperty<SymbolPin, SymbolPinListNameProvider, Symbol, decltype(symbolClass), (SymbolPinList& (Symbol::*)() noexcept) (&Symbol::getPins)>("pins", symbolClass);
}

}

}
}
