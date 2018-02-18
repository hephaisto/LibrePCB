#include <boost/python.hpp>
#include <QtCore>

#include "../common/application.h"

#include "../workspace/workspace.h"

#include "qstring_converters.h"

#include "core.h"
#include "geometry.h"
#include "library.h"

namespace librepcb {
namespace python {


using boost::python::def;

using librepcb::workspace::Workspace;

static void setApplicationMetadata() noexcept
{
    Application::setOrganizationName("LibrePCB");
    Application::setOrganizationDomain("librepcb.org");
    Application::setApplicationName("LibrePCB");
}


void init_python_subsystem()
{
    // TODO only init qt stuff if standalone, not embedded
    setApplicationMetadata();
    register_qstring_converters();
}

FilePath determineWorkspacePath()
{
    // simplified version from apps/librepcb/main.cpp
    FilePath path(Workspace::getMostRecentlyUsedWorkspacePath());
    if( !Workspace::isValidWorkspacePath(path) )
        throw librepcb::RuntimeError(__FILE__, __LINE__, path.toStr() + " is not a valid workdspace path");
    return path;
}

BOOST_PYTHON_MODULE(librepcbpy)
{
    def("init", init_python_subsystem);
    def("determineWorkspacePath", determineWorkspacePath);

    init_core();
    init_geometry();
    init_library();
}


} // librepcb::python
} // librepcb
