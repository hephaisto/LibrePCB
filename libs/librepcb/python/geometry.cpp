#include <boost/python.hpp>

#include "../common/units/length.h"
#include "../common/units/point.h"
#include "../common/units/angle.h"

#include "../common/geometry/polygon.h"
#include "../common/geometry/cmd/cmdpolygonedit.h"

#include "../common/geometry/ellipse.h"
#include "../common/geometry/cmd/cmdellipseedit.h"

#include "../common/geometry/text.h"
#include "../common/geometry/cmd/cmdtextedit.h"

#include "../common/geometry/hole.h"
#include "../common/geometry/cmd/cmdholeedit.h"

#include "serializablelist.h"
#include "propertywrapper.h"

#include "geometry.h"

namespace librepcb {
namespace python {

namespace geometry {

using boost::python::class_;
using boost::python::init;
using boost::python::no_init;
using boost::python::return_internal_reference;
using boost::python::return_value_policy;
using boost::python::copy_const_reference;
using boost::python::self;
using boost::python::other;
using boost::python::def;

Alignment makeAlign(std::string tag)
{
    if( tag.length() != 2 )
    {
        throw std::runtime_error("tag has to be of length 2");
    }

    HAlign h;
    if( tag[0] == 'l' )
    {
        h = HAlign::left();
    }
    else if( tag[0] == 'c' )
    {
        h = HAlign::center();
    }
    else if( tag[0] == 'r' )
    {
        h = HAlign::right();
    }
    else
    {
        throw std::runtime_error("unknown horizontal align flag");
    }

    VAlign v;
    if( tag[1] == 't' )
    {
        v = VAlign::top();
    }
    else if( tag[1] == 'c' )
    {
        v = VAlign::center();
    }
    else if( tag[1] == 'b' )
    {
        v = VAlign::bottom();
    }
    else
    {
        throw std::runtime_error("unknown vertical align flag");
    }

    return Alignment(h, v);
}

void register_python_classes()
{

    // geometry atomics
    class_<Point>(
            "Point",
            init<const Length&, const Length&>()
            )
        .add_property("x", make_function(&Point::getX, return_internal_reference<1>()), &Point::setX)
        .add_property("y", make_function(&Point::getY, return_internal_reference<1>()), &Point::setY)

        .def(self -  other<Point>())
        .def(self +  other<Point>())
        .def(self -= other<Point>())
        .def(self += other<Point>())

        .def(self *  other<Length>())
        .def(self /  other<Length>())

        .def(self == other<Point>())
        ;

    class_<Length>(
            "Length",
            init<LengthBase_t>()
            )
        .def("setLengthNm", &Length::setLengthNm)
        .def("toNm", &Length::toNm)
        .def("__str__", &Length::toNmString)

        .def(self -  other<Length>())
        .def(self +  other<Length>())
        .def(self -= other<Length>())
        .def(self += other<Length>())
        .def(self *  other<LengthBase_t>())
        .def(self *= other<LengthBase_t>())
        .def(self /  other<LengthBase_t>())
        .def(self /= other<LengthBase_t>())
        .def(self <  other<Length>())
        .def(self >  other<Length>())
        .def(self <= other<Length>())
        .def(self >= other<Length>())
        .def(self == other<Length>())
        .def(self == other<LengthBase_t>())
        .def(self != other<Length>())
        ;

    class_<Angle>(
            "Angle",
            init<qint32>()
            )
        .def("toMicroDeg", &Angle::toMicroDeg)
        .def("__str__", &Angle::toDegString)

        .def(self +  other<Angle>())
        .def(self -  other<Angle>())
        .def(self += other<Angle>())
        .def(self -= other<Angle>())

        .def(self == other<Angle>())
        .def(self == other<qint32>())
        ;

    // GEOMETRIC OBJECTS

    // polygon
    auto polygonClass = class_<Polygon, shared_ptr<Polygon> >(
            "Polygon",
            no_init
            )
        .add_property("layerName", make_function(&Polygon::getLayerName, return_value_policy<copy_const_reference>()), &Polygon::setLayerName)
        ;
    ADD_WRAPPED_PROPERTY(polygonClass, Polygon, QString, LayerName, "layerName");

    // ellipse
    auto ellipseClass = class_<Ellipse, shared_ptr<Ellipse> >(
            "Ellipse",
            no_init
            )
        .add_property("uuid", make_function(&Ellipse::getLayerName, return_value_policy<copy_const_reference>()))
      ;
    ADD_WRAPPED_PROPERTY(ellipseClass, Ellipse, QString, LayerName, "layerName");
    ADD_WRAPPED_PROPERTY(ellipseClass, Ellipse, Length, LineWidth, "lineWidth");
    ADD_WRAPPED_PROPERTY(ellipseClass, Ellipse, Point, Center, "center");
    ADD_WRAPPED_PROPERTY(ellipseClass, Ellipse, Length, RadiusX, "rx");
    ADD_WRAPPED_PROPERTY(ellipseClass, Ellipse, Length, RadiusY, "ry");
    ADD_WRAPPED_PROPERTY(ellipseClass, Ellipse, Angle, Rotation, "rotation");

    // alignment
    auto alignmentClass = class_<Alignment, shared_ptr<Alignment> >(
        "Alignment",
        init<const HAlign&, const VAlign&>()
        )
        ;
    def("makeAlign", makeAlign, "Pass a two-character string, the first character indicating horizontal alignment (lcr), the second vertical alignment (tcb)");

    // text
    auto textClass = class_<Text, shared_ptr<Text> >(
            "Text",
            init<const Uuid&, const QString&, const QString&, const Point&, const Angle&, const Length&, const Alignment&>()
            )
        .add_property("uuid", make_function(&Text::getUuid, return_value_policy<copy_const_reference>()))
        ;
    ADD_WRAPPED_PROPERTY(textClass, Text, QString, LayerName, "layerName");
    ADD_WRAPPED_PROPERTY(textClass, Text, Point, Position, "position");
    ADD_WRAPPED_PROPERTY(textClass, Text, Angle, Rotation, "rotation");
    ADD_WRAPPED_PROPERTY(textClass, Text, Length, Height, "height");
    ADD_WRAPPED_PROPERTY(textClass, Text, QString, Text, "text");
    addWrappedProperty<decltype(textClass), Text, Alignment, CmdTextEdit, &Text::getAlign, &Text::setAlign, &CmdTextEdit::setAlignment>("align", textClass); // classes sometimes use Align and sometimes use Alignment

    // hole
	auto holeClass = class_<Hole, shared_ptr<Hole> >(
			"Hole",
			init<const Uuid&, const Point&, const Length&>()
			)
        .add_property("uuid", make_function(&Hole::getUuid, return_value_policy<copy_const_reference>()))
		;
    ADD_WRAPPED_PROPERTY(holeClass, Hole, Point, Position, "position");
    ADD_WRAPPED_PROPERTY(holeClass, Hole, Length, Diameter, "diameter");

	// list declarations
    DECLARE_SERIALIZABLE_LIST(Polygon);
    DECLARE_SERIALIZABLE_LIST(Ellipse);
    DECLARE_SERIALIZABLE_LIST(Text);
}

} // geometry

} // python
} // librepcb
