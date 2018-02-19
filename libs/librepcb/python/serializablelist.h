#include <boost/python.hpp>

#include "../common/fileio/serializableobjectlist.h"

namespace librepcb {
namespace python {

using std::shared_ptr;
using boost::python::class_;
using boost::python::no_init;

//
// ADD NAME GETTER IF POSSIBLE
// This adds __getitem__(QString) to the SerializableObjectList<T>, if T::getName() exists
//

template<class ItemType> struct hasNameGetter
{
    template<typename C> static constexpr decltype(std::declval<C>().getName(), bool()) test(int)
    {
        return true;
    }

    template<typename C> static constexpr bool test(...)
    {
        return false;
    }

    static constexpr bool value = test<ItemType>(int());
};

// adds string-based indexing
template<typename ItemType, typename NameProvider, typename PythonClassPlaceholder> typename std::enable_if<hasNameGetter<ItemType>::value, void>::type addNameGetter(PythonClassPlaceholder &cls)
{
    using TList = SerializableObjectList<ItemType, NameProvider>;
    shared_ptr<ItemType> (TList::*GetterString)(const QString&) = &TList::get;
    void (TList::*RemoverString)(const QString&) = &TList::remove;
    cls
        .def("__getitem__", GetterString)
        .def("remove", RemoverString)
    ;
}

// stub if no string-based indexing
template<typename ItemType, typename NameProvider, typename PythonClassPlaceholder> typename std::enable_if<!hasNameGetter<ItemType>::value, void>::type addNameGetter(PythonClassPlaceholder &cls)
{
}

//
// ADD COMMON FUNCTIONS
//

template<typename T, typename NameProvider>
    void declList(const char* name)
    {
        using TList = SerializableObjectList<T, NameProvider>;
        shared_ptr<T> (TList::*GetterInt)(int)               = &TList::at_with_exception;
        shared_ptr<T> (TList::*GetterUuid)(const Uuid&)      = &TList::get;

        void (TList::*RemoverInt)(int) = &TList::remove;
        void (TList::*RemoverUuid)(const Uuid&) = &TList::remove;

        auto cls = class_<TList, boost::noncopyable>(
                name,
                no_init
                )
            .def("__len__", &TList::count)
            .def("__getitem__", GetterInt)
            .def("__getitem__", GetterUuid)

            .def("remove", RemoverInt)
            .def("remove", RemoverUuid)

            .def("append", &TList::append)
        ;
        addNameGetter<T, NameProvider, decltype(cls)>(cls);
    }

#define DECLARE_SERIALIZABLE_LIST(name) declList<name, name##ListNameProvider>(#name "List")

}
}
