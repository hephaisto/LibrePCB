#include <boost/python.hpp>
#include <QtCore>

#include "../common/application.h"

#include "../workspace/workspace.h"

#include "qstring_converters.h"

#include "core.h"
#include "geometry.h"
#include "symbol.h"
#include "package.h"
#include "component.h"
#include "device.h"

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


void initStandalone()
{
    setApplicationMetadata();
}

FilePath determineWorkspacePath()
{
    // simplified version from apps/librepcb/main.cpp
    FilePath path(Workspace::getMostRecentlyUsedWorkspacePath());
    if( !Workspace::isValidWorkspacePath(path) )
        throw librepcb::RuntimeError(__FILE__, __LINE__, path.toStr() + " is not a valid workdspace path");
    return path;
}

BOOST_PYTHON_MODULE(librepcb)
{
#ifdef PYTHON_DO_STANDALONE_INITIALISATION
    initStandalone();
#endif
    register_qstring_converters();
    def("determineWorkspacePath", determineWorkspacePath);

    register_python_classes();
    geometry::register_python_classes();
    sym::register_python_classes();
    pkg::register_python_classes();
}


} // librepcb::python
} // librepcb
