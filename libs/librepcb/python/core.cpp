#include "hypot_workaround.hpp"
#include <boost/python.hpp>
#include <QString>

#include "../common/uuid.h"
#include "../common/fileio/filepath.h"

namespace librepcb {
namespace python {

using boost::python::class_;
using boost::python::init;
using boost::python::self;
using boost::python::other;

void init_core()
{

    // uuid
    class_<Uuid>(
            "Uuid",
            init<QString>()
            )
        .def("__str__", &Uuid::toStr)
        .def(self == other<Uuid>())
        .def("createRandom", &Uuid::createRandom)
        .staticmethod("createRandom")
        ;

    class_<FilePath>("FilePath", init<QString>())
        .def("toStr", &FilePath::toStr)
        .def("getPathTo", &FilePath::getPathTo)
        ;
}


}
}
