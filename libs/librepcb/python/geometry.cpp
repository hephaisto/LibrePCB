#include <boost/python.hpp>

#include "../common/units/length.h"
#include "../common/units/point.h"
#include "../common/units/angle.h"
#include "../common/geometry/polygon.h"
#include "../common/geometry/ellipse.h"
#include "../common/geometry/text.h"

#include "serializablelist.h"

#include "geometry.h"

namespace librepcb {
namespace python {

using boost::python::class_;
using boost::python::init;
using boost::python::no_init;
using boost::python::return_internal_reference;
using boost::python::self;
using boost::python::other;


void init_geometry()
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

    // geometric objects
    class_<Polygon>(
            "Polygon",
            no_init
            )
        // TODO
        ;
    class_<Ellipse>(
            "Ellipse",
            no_init
            )
        // TODO
        ;
    class_<Text>(
            "Text",
            no_init
            )
        // TODO
        ;

    DECLARE_SERIALIZABLE_LIST(Polygon);
    DECLARE_SERIALIZABLE_LIST(Ellipse);
    DECLARE_SERIALIZABLE_LIST(Text);
}


}
}
