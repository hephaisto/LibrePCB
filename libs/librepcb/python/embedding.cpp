// "fixes double free or corruption" error on close
#define WORKAROUND_DOUBLE_FREE
#ifdef WORKAROUND_DOUBLE_FREE
#include <fstream>
#endif

#include "hypot_workaround.hpp"
#include <boost/python.hpp>
#include <QDebug>
#include <QString>

#include "embedding.h"

namespace librepcb {
namespace python {

using boost::python::object;
using boost::python::import;
using boost::python::error_already_set;
using boost::python::str;
using boost::python::handle;
using boost::python::extract;
using boost::python::ptr;

/*ScriptingEnvironment::ScriptingEnvironment():
mUndoStack(nullptr),
mSymbol(nullptr)
{
    registerInstance();
}*/

ScriptingEnvironment::ScriptingEnvironment(UndoStack *undoStack):
mUndoStack(undoStack),
mSymbol(nullptr)
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

UndoStack* ScriptingEnvironment::getUndoStack() const noexcept
{
    return mUndoStack;
}

void ScriptingEnvironment::setSymbol(librepcb::library::Symbol *symbol)
{
    mSymbol = symbol;
}

librepcb::library::Symbol* ScriptingEnvironment::getSymbol() const noexcept
{
    return mSymbol;
}

bool embedding_initialized = false;
void initEmbeddingIfNecessary()
{
    if(!embedding_initialized)
    {
        qInfo() << "Initializing python interpreter";
        Py_Initialize();
        embedding_initialized = true;
    }
}

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

void ScriptingEnvironment::runScript(const QString &filename)
{
    try
    {
        qInfo() << "running script " << filename;
        initEmbeddingIfNecessary();

        mUndoStack->beginCmdGroup(tr("execute script %1").arg(filename));

        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__"); // copy!
        //object cpp_module( (handle<>(PyImport_ImportModule("Heinz"))) );
        object cppModule = import("librepcbpy");
        main_namespace["lp"] = cppModule;
        

        main_namespace["symbol"] = ptr(mSymbol);

        #ifdef WORKAROUND_DOUBLE_FREE
            std::ifstream in(filename.toStdString());
            std::string buf( (std::istreambuf_iterator<char>(in)), (std::istreambuf_iterator<char>()) );
            exec(str(buf), main_namespace);
        #else
            exec_file(str(filename.toStdWString()), main_namespace);
        #endif

        qInfo() << "script " << filename << " exited normally";
        mUndoStack->commitCmdGroup();
    }
    catch(error_already_set const &)
    {
        mUndoStack->abortCmdGroup();
        QString traceback(QString::fromStdWString(getPythonTraceback()));

        qWarning() << "Error in python script";
        qWarning().noquote() << traceback;

        QMessageBox msgBox;
        msgBox.setText(tr("Python error"));
        msgBox.setInformativeText(tr("Error while executing %1").arg(filename));
        msgBox.setDetailedText(traceback);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();


        /*try
        {
            PyObject* ptype;
            PyObject* pvalue;
            PyObject* ptraceback;

            PyErr_Fetch(&ptype, &pvalue, &ptraceback);

            handle<> hType(ptype);
            object extype(hType);
            handle<> hTraceback(ptraceback);
            object traceback(hTraceback);

            std::wstring strErrorMessage = extract<std::wstring>(pvalue);
            qWarning() << "Exception in python script: " << QString::fromStdWString(strErrorMessage);

            long lineno = extract<long> (traceback.attr("tb_lineno"));
            std::wstring filename = extract<std::wstring>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
            std::wstring funcname = extract<std::wstring>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));
            Py_XDECREF(ptype);
            Py_XDECREF(pvalue);
            Py_XDECREF(ptraceback);
            qWarning() << "in " << QString::fromStdWString(funcname) << "(" << QString::fromStdWString(filename) << ": l." <<lineno << ")";

        }
        catch(error_already_set const &)
        {
            qWarning() << "Error while handling exception in python script";
        }*/
    }
    catch(...)
    {
        qWarning() << "UNHANDLED EXCEPTION WHILE EXECUTING PYTHON SCRIPT";
    }
}


ScriptingEnvironment* ScriptingEnvironment::currentInstance = nullptr;


} // python
} // librepcb
