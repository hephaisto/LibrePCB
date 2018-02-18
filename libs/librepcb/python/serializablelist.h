#include <boost/python.hpp>

#include "../common/fileio/serializableobjectlist.h"

namespace librepcb {
namespace python {

using std::shared_ptr;
using boost::python::class_;
using boost::python::no_init;

template<typename T, typename NameProvider>
    void declListUuidName(const char* name)
    {
        using TList = SerializableObjectList<T, NameProvider>;
        shared_ptr<T> (TList::*GetterInt)(int)               = &TList::at_with_exception;
        shared_ptr<T> (TList::*GetterString)(const QString&) = &TList::get;
        shared_ptr<T> (TList::*GetterUuid)(const Uuid&)      = &TList::get;

        void (TList::*RemoverInt)(int) = &TList::remove;
        void (TList::*RemoverString)(const QString&) = &TList::remove;
        void (TList::*RemoverUuid)(const Uuid&) = &TList::remove;

        class_<TList, boost::noncopyable>(
                name,
                no_init
                )
            .def("__len__", &TList::count)
            .def("__getitem__", GetterInt)
            .def("__getitem__", GetterString)
            .def("__getitem__", GetterUuid)

            .def("remove", RemoverInt)
            .def("remove", RemoverString)
            .def("remove", RemoverUuid)

            .def("append", &TList::append)
            ;
    }

template<typename T, typename NameProvider>
    void declList(const char* name)
    {
        using TList = SerializableObjectList<T, NameProvider>;
        shared_ptr<T> (TList::*GetterInt)(int)               = &TList::at_with_exception;

        void (TList::*RemoverInt)(int) = &TList::remove;

        class_<TList, boost::noncopyable>(
                name,
                no_init
                )
            .def("__len__", &TList::count)
            .def("__getitem__", GetterInt)

            .def("remove", RemoverInt)

            .def("append", &TList::append)
            ;
    }


#define DECLARE_SERIALIZABLE_LIST(whatParams, name) declList##whatParams<name, name##ListNameProvider>(#name "List")

}
}
