// thanks to https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/
#include <boost/python.hpp>
#include <QString>
#include <Python.h>
#include <boost/python/def.hpp>

#include "qstring_converters.h"

namespace librepcb
{
namespace python
{

struct QStringToPyStr
{
	static PyObject* convert(QString const &s)
	{
		return boost::python::incref(boost::python::str(s.toStdWString()).ptr());
	}
};

struct QStringFromPyStr
{
	static void* convertible(PyObject* obj)
	{
		if( !PyUnicode_Check(obj) )
			return NULL;
		return obj;
	}
	// Convert obj_ptr into a QString
	static void construct(
			PyObject* obj_ptr,
			boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		// Extract the character data from the python string
		const wchar_t* value = PyUnicode_AsUnicode(obj_ptr);

		// Verify that obj_ptr is a string (should be ensured by convertible())
		assert(value);

		// Grab pointer to memory into which to construct the new QString
		void* storage = (
				(boost::python::converter::rvalue_from_python_storage<QString>*)
				data)->storage.bytes;

		// in-place construct the new QString using the character data
		// extraced from the python object
		new (storage) QString(QString::fromStdWString(value));

		// Stash the memory chunk pointer for later use by boost.python
		data->convertible = storage;
	}

};

void register_qstring_converters()
{
	using boost::python::to_python_converter;
	to_python_converter<QString, QStringToPyStr>();
	boost::python::converter::registry::push_back(&QStringFromPyStr::convertible, &QStringFromPyStr::construct, boost::python::type_id<QString>());
}

} // namespace python
} // namespace librepcb
