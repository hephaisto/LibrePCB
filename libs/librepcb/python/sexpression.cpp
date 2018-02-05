#include <stdexcept>
#include <boost/python.hpp>

#include "../common/fileio/filepath.h"
#include "../common/fileio/sexpression.h"

namespace librepcb {
namespace sexpressionpython {

using librepcb::SExpression;
using boost::python::class_;
using boost::python::def;
using boost::python::init;
using boost::python::return_internal_reference;

QString qstring(std::wstring str)
{
	return QString::fromStdWString(str);
}

SExpression createToken(const std::wstring &str)
{
	return SExpression::createToken(QString::fromStdWString(str));
}

SExpression createString(const std::wstring &str)
{
	return SExpression::createString(QString::fromStdWString(str));
}

SExpression createList(const std::wstring &str)
{
	return SExpression::createList(QString::fromStdWString(str));
}

SExpression parse(const std::wstring &str, const std::wstring &filePath)
{
	return SExpression::parse(QString::fromStdWString(str), librepcb::FilePath(QString::fromStdWString(filePath)));
}

const SExpression& getChildByPath(const SExpression *t, const std::wstring &path)
{
	return t->getChildByPath(QString::fromStdWString(path));
}

const std::wstring getName(const SExpression *t)
{
	return t->getName().toStdWString();
}

const std::wstring getValue(const SExpression *t)
{
	return t->getValue<QString>(false).toStdWString();
}

typedef QList<SExpression> SexList;
int test(const SexList &l)
{
	return l.count();
}
int (SexList::*sexListSize)(void) const noexcept = &SexList::size;
const SexList& (SExpression::*sexGetChildren)(void) const noexcept = &SExpression::getChildren;

SExpression& getSexListItem(SexList *list, int i)
{
	if( i >= list->count() )
		throw std::out_of_range("list index out of range");
	return (*list)[i];
}

BOOST_PYTHON_MODULE(librepcbsexpression)
{
	def("createList", SExpression::createList);
	def("q", qstring);
	class_<QString>("QString")
		.def("__str__", &QString::toStdWString)
		;
	class_<FilePath>("FilePath", init<QString>())
		.def("toStr", &FilePath::toStr)
		;

	class_<SExpression>("SExpression")
		.def("toString", &SExpression::toString)
		.def("appendChild", &SExpression::appendChild, return_internal_reference<1>())
		.def("getChildren", sexGetChildren, return_internal_reference<1>())
		.def("getChildByPath", getChildByPath, return_internal_reference<1>())
		.def("isList", &SExpression::isList)
		.def("isToken", &SExpression::isToken)
		.def("isString", &SExpression::isString)
		.def("getName", getName)
		.def("getValue", getValue)
		;

	def("createToken", &createToken);
	def("createString", &createString);
	def("createList", &createList);
	def("parse", &parse);


	class_<SexList>("SexList")
		.def("__len__", sexListSize)
		.def("__getitem__", &getSexListItem, return_internal_reference<1>())
		;
}


}
}
