#include <boost/python.hpp>

#include "../library/pkg/package.h"

#include "serializablelist.h"
#include "propertywrapper.h"

#include "library.h"

namespace librepcb {
namespace python {

namespace pkg {

using std::shared_ptr;
using boost::python::class_;
using boost::python::init;
using boost::python::no_init;
using boost::python::return_internal_reference;
using boost::python::return_value_policy;
using boost::python::copy_const_reference;
using boost::python::bases;
using boost::python::enum_;


using namespace librepcb::library;

void init()
{
    // BoardSide
    enum_<FootprintPad::BoardSide>("BoardSide")
        .value("top", FootprintPad::BoardSide::TOP)
        .value("bottom", FootprintPad::BoardSide::BOTTOM)
        .value("tht", FootprintPad::BoardSide::THT)
        ;

    // PadShape
    enum_<FooprintPad::PadShape>("PadShape")
        .value("round", FootprintPad::PadShape::ROUND)
        .value("rect", FootprintPad::PadShape::RECT)
        .value("octagon", FootprintPad::PadShape::octagon)
        ;

    // FootprintPad
    auto footprintPadClass = class_<FootprintPad, shared_ptr<FootprintPad> >(
            "FootprintPad",
            init<const Uuid&, const Point&, const Angle&, Shape, const Length&, const Length&, const Length&, BoardSide>()
            )
        .add_property("uuid", make_function(&FootprintPad::getUuid, return_value_policy<copy_const_reference>()))
        .add_property("packagePadUuid", make_function(&FootprintPad::getPackagePadUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(footprintPadClass, FootprintPad, Point, Position, "position");
    ADD_WRAPPED_PROPERTY(footprintPadClass, FootprintPad, Angle, Rotation, "rotation");
    ADD_WRAPPED_PROPERTY(footprintPadClass, FootprintPad, Shape, Shape, "shape");
    ADD_WRAPPED_PROPERTY(footprintPadClass, FootprintPad, Length, Width, "width");
    ADD_WRAPPED_PROPERTY(footprintPadClass, FootprintPad, Length, Height, "height");
    ADD_WRAPPED_PROPERTY(footprintPadClass, FootprintPad, Length, DrillDiameter, "drillDiameter");
    ADD_WRAPPED_PROPERTY(footprintPadClass, FootprintPad, BoardSide, BoardSide, "boardSide");
    ADD_WRAPPED_PROPERTY(footprintPadClass, FootprintPad, QString, LayerName, "layerName");

    DECLARE_SERIALIZABLE_LIST(FootprintPad);

    // Footprint
    auto footprintClass = class_<Footprint, shared_ptr<Footprint> >(
            "Footprint",
            init<const Uuid&, const QString&, const QString&>()
            )
        .add_property("uuid", make_function(&Footprint::getUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_LIST_PROPERTY(footprintClass, Footprint, FootprintPad, Pads, "pads");
    ADD_LIST_PROPERTY(footprintClass, Footprint, Text, Texts, "texts");
    ADD_LIST_PROPERTY(footprintClass, Footprint, Polygon, Polygons, "polygons");
    ADD_LIST_PROPERTY(footprintClass, Footprint, Ellipse, Ellipses, "ellipses");
    ADD_LIST_PROPERTY(footprintClass, Footprint, Hole, Holes, "holes");

    DECLARE_SERIALIZABLE_LIST(Footprint);

    // PackagePad
    auto packagePadClass = class_<PackagePad, shared_ptr<PackagePad> >(
            "PackagePad",
            init<const Uuid&, const QString&>()
            )
        .add_property("uuid", make_function(&PackagePad::getUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(packagePadClass, PackagePad, QString, Name, "name");

    DECLARE_SERIALIZABLE_LIST(PackagePad);


    // Package
    auto packageClass = class_<Package, shared_ptr<Package> >(
            "Package",
            init<FilePath, bool>()
            )
        .add_property("uuid", make_function(&Package::getUuid, return_value_policy<copy_const_reference>()))
        .def("save", &Package::save)
        ;
    ADD_LIST_PROPERTY(packageClass, Package, PackagePad, Pads, "pads");
    ADD_LIST_PROPERTY(packageClass, Footprint, Footprint, Footprints, "footprints");
}

}
}
}
