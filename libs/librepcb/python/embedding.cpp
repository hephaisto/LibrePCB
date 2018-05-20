// "fixes double free or corruption" error on close
#define WORKAROUND_DOUBLE_FREE
#ifdef WORKAROUND_DOUBLE_FREE
#include <fstream>
#endif

#ifdef HAS_PYTHON

#include <boost/python.hpp>

#endif // HAS_PYTHON

#include <QDebug>
#include <QString>

#include "../workspace/workspace.h"

#include "embedding.h"

namespace librepcb {
namespace python {


#ifdef HAS_PYTHON

using boost::python::object;
using boost::python::import;
using boost::python::error_already_set;
using boost::python::str;
using boost::python::handle;
using boost::python::extract;
using boost::python::ptr;
using boost::python::class_;
using boost::python::no_init;
using boost::python::return_internal_reference;
using boost::python::make_function;
using boost::python::list;

// forward declaration of initialisation function provided by boost::python
PyMODINIT_FUNC PyInit_librepcb(void);

#endif // HAS_PYTHON

/*ScriptingEnvironment::ScriptingEnvironment():
mUndoStack(nullptr),
mSymbol(nullptr)
{
    registerInstance();
}*/

ScriptingEnvironment::ScriptingEnvironment():
mUndoStack(nullptr),
mSymbol(nullptr),
mPackage(nullptr),
mComponent(nullptr),
mDevice(nullptr)
{
    registerInstance();
}

ScriptingEnvironment::~ScriptingEnvironment() noexcept
{
    currentInstance = nullptr;
}

void ScriptingEnvironment::registerInstance()
{
    Q_ASSERT(!currentInstance);
    currentInstance = this;
}
ScriptingEnvironment* ScriptingEnvironment::instance()
{
    return currentInstance;
}

void ScriptingEnvironment::setUndoStack(UndoStack *stack)
{
    mUndoStack = stack;
}
UndoStack* ScriptingEnvironment::getUndoStack() const noexcept
{
    return mUndoStack;
}

// symbol
void ScriptingEnvironment::setSymbol(librepcb::library::Symbol *symbol)
{
    mSymbol = symbol;
}

librepcb::library::Symbol* ScriptingEnvironment::getSymbol() const noexcept
{
    return mSymbol;
}


// package
void ScriptingEnvironment::setPackage(librepcb::library::Package *package)
{
    mPackage = package;
}

librepcb::library::Package* ScriptingEnvironment::getPackage() const noexcept
{
    return mPackage;
}


// component
void ScriptingEnvironment::setComponent(librepcb::library::Component *component)
{
    mComponent = component;
}

librepcb::library::Component* ScriptingEnvironment::getComponent() const noexcept
{
    return mComponent;
}


// device
void ScriptingEnvironment::setDevice(librepcb::library::Device *device)
{
    mDevice = device;
}

librepcb::library::Device* ScriptingEnvironment::getDevice() const noexcept
{
    return mDevice;
}

bool embedding_initialized = false;
void initEmbeddingIfNecessary()
{
#ifdef HAS_PYTHON
    if(!embedding_initialized)
    {
        qInfo() << "Initializing python interpreter";

        PyImport_AppendInittab( "librepcb", PyInit_librepcb );
        Py_Initialize();

        qInfo() << "Importing python module";

        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__");
        object cpp_module( (handle<>(PyImport_ImportModule("librepcb"))) );
        main_namespace["lp"] = cpp_module;

        embedding_initialized = true;

        class_<ScriptingEnvironment, boost::noncopyable>(
            "ScriptingEnvironment",
            no_init
            )
        .add_property("symbol", make_function(&ScriptingEnvironment::getSymbol, return_internal_reference<1>()))
        .add_property("package", make_function(&ScriptingEnvironment::getPackage, return_internal_reference<1>()))
        .add_property("component", make_function(&ScriptingEnvironment::getComponent, return_internal_reference<1>()))
        .add_property("device", make_function(&ScriptingEnvironment::getDevice, return_internal_reference<1>()))
        ;
    }
#endif // HAS_PYTHON
}

#ifdef HAS_PYTHON
std::wstring getPythonTraceback()
{
    // from https://wiki.python.org/moin/boost.python/EmbeddingPython
    using boost::python::allow_null;
    std::wstring result;

    PyObject* pType;
    PyObject* pValue;
    PyObject* pTraceback;

    PyErr_Fetch(&pType, &pValue, &pTraceback);
    PyErr_NormalizeException(&pType, &pValue, &pTraceback);

    handle<> hType(pType);
    handle<> hValue(allow_null(pValue));
    handle<> hTraceback(allow_null(pTraceback));

    if( !hValue )
    {
        result = extract<std::wstring>(str(hType));
    }
    else
    {
        object tracebackModule(import("traceback"));
        object formatExceptionFunction(tracebackModule.attr("format_exception"));
        object formattedList(formatExceptionFunction(hType, hValue, hTraceback));
        object formattedString(str("").join(formattedList));
        result = extract<std::wstring>(formattedString);
    }
    Py_XDECREF(pType);
    Py_XDECREF(pValue);
    Py_XDECREF(pTraceback);
    return result;
}
#endif // HAS_PYTHON

QString ScriptingEnvironment::runScript(const QString &command)
{
    QString filename{""};
#ifdef HAS_PYTHON
    try
    {
        qInfo() << "running script " << command;
        initEmbeddingIfNecessary();

        mUndoStack->beginCmdGroup(tr("execute script %1").arg(command));

        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__"); // copy!

        main_namespace["env"] = ptr(this);

        // get stdout/stderr
        exec(str(
            "import sys, io\n"
            "_stdStreamCatcher = io.StringIO()\n"
            "sys.stdout = _stdStreamCatcher\n"
            "sys.stderr = _stdStreamCatcher\n"
            ), main_namespace);

        // parse command-line arguments
        object shlex = import("shlex");
        list argv = extract<list>(shlex.attr("split")(command));
        main_namespace["sys"].attr("argv") = argv;
        filename = extract<QString>(argv[0]);

        // find script file and set the module include search path
        FilePath pythonDir(librepcb::workspace::Workspace::getMostRecentlyUsedWorkspacePath().getPathTo("python_scripts"));
        if(!pythonDir.isExistingDir())
            throw librepcb::RuntimeError(__FILE__, __LINE__, pythonDir.toStr() + " does not exist");

        FilePath scriptPath(pythonDir.getPathTo(filename));
        if(!scriptPath.isExistingFile())
            throw librepcb::RuntimeError(__FILE__, __LINE__, scriptPath.toStr() + " does not exist");
        list moduleSearchPath = extract<list>(main_namespace["sys"].attr("path"));
        moduleSearchPath.append(pythonDir);


        #ifdef WORKAROUND_DOUBLE_FREE
            std::ifstream in(scriptPath.toStr().toStdString());
            std::string buf( (std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()) );
            exec(str(buf), main_namespace);
        #else
            exec_file(str(scriptPath.toStr().toStdWString()), main_namespace);
        #endif

        QString output = extract<QString>(eval(str("_stdStreamCatcher.getvalue()"), main_namespace));
        exec(str("_stdStreamCatcher.close()"), main_namespace);

#if(QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
        qInfo().noquote() << output;
#else
        qInfo() << output;
#endif

        qInfo() << "script " << filename << " exited normally";
        mUndoStack->commitCmdGroup();
        return output;
    }
    catch(error_already_set const &)
    {
        QString traceback(QString::fromStdWString(getPythonTraceback()));

        qWarning() << "Error in python script";
#if(QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
        qWarning().noquote() << traceback;
#else
        qWarning() << traceback;
#endif

        QMessageBox msgBox;
        msgBox.setText(tr("Python error"));
        msgBox.setInformativeText(tr("Error while executing %1").arg(filename));
        msgBox.setDetailedText(traceback);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    catch(RuntimeError const &e)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Scripting error"));
        msgBox.setInformativeText(e.getMsg());
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    catch(...)
    {
        qWarning() << "UNHANDLED EXCEPTION WHILE EXECUTING PYTHON SCRIPT";
    }
    if(mUndoStack->isCommandGroupActive())
    {
        mUndoStack->abortCmdGroup();
    }
#else // ! HAS_PYTHON
    Q_UNUSED(command);
    QMessageBox msgBox;
    msgBox.setText(tr("Not supported"));
    msgBox.setInformativeText(tr("Python scripting was not enabled during compilation"));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
#endif // HAS_PYTHON
    return "";
}


ScriptingEnvironment* ScriptingEnvironment::currentInstance = nullptr;


} // python
} // librepcb
