#include <boost/python.hpp>
#include <QString>

#include "../common/uuid.h"
#include "../common/fileio/filepath.h"

namespace librepcb {
namespace python {

using boost::python::class_;
using boost::python::init;

void init_core()
{

    // uuid
    class_<Uuid>(
            "Uuid",
            init<QString>()
            )
        .def("__str__", &Uuid::toStr)
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
