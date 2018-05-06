#include <boost/python.hpp>
using std::shared_ptr;

#include "../common/fileio/serializableobjectlist.h"
#include "embedding.h"

namespace librepcb {
namespace python {

using std::shared_ptr;
using boost::python::class_;
using boost::python::no_init;
using boost::python::return_value_policy;
using boost::python::manage_new_object;
using boost::python::make_function;
using boost::python::detail::make_function_aux;


template<typename ItemType, typename NameProvider>
class ListWrapper
{
    using TList = SerializableObjectList<ItemType, NameProvider>;
public:
    ListWrapper() = delete;

    ListWrapper(TList &list)
    :mList(list)
    {
    }

    void append(const shared_ptr<ItemType> &item)
    {

        if(ScriptingEnvironment::instance())
        {
            UndoStack* undoStack = ScriptingEnvironment::instance()->getUndoStack();
            undoStack->appendToCmdGroup(new CmdListElementInsert<ItemType, NameProvider>(mList, item));
        }
        else
        {
            mList.append(item);
        }
    }
    void remove(const ItemType *element)
    {
        if( !mList.contains(element) )
            throw std::out_of_range("SerializableObjectList invalid key");
        if(ScriptingEnvironment::instance())
        {
            UndoStack* undoStack = ScriptingEnvironment::instance()->getUndoStack();
            undoStack->appendToCmdGroup(new CmdListElementRemove<ItemType, NameProvider>(mList, element));
        }
        else
        {
            mList.remove(element);
        }
    }

    int count() {return mList.count(); }

    std::shared_ptr<ItemType> getByIndex(int index) 
    {
        if( !mList.contains(index) )
            throw std::out_of_range("SerializableObjectList out of range");
        return mList[index];
    }

    std::shared_ptr<ItemType> getByUuid(const Uuid& uuid)
    {
        return mList.get(uuid);
    }

    std::shared_ptr<ItemType> getByName(const QString& name)
    {
        return mList.get(name);
    }

private:
    TList &mList;
};


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
    using TList = ListWrapper<ItemType, NameProvider>;
    shared_ptr<ItemType> (TList::*GetterString)(const QString&) = &TList::getByName;
    cls
        .def("__getitem__", GetterString)
    ;
}

// stub if no string-based indexing
template<typename ItemType, typename NameProvider, typename PythonClassPlaceholder> typename std::enable_if<!hasNameGetter<ItemType>::value, void>::type addNameGetter(PythonClassPlaceholder &cls)
{
    Q_UNUSED(cls);
}

//
// ADD COMMON FUNCTIONS
//

template<typename T, typename NameProvider>
    void declList(const char* name)
    {
        using TList = ListWrapper<T, NameProvider>;


        auto cls = class_<TList, boost::noncopyable>(
                name,
                no_init
                )
            .def("__len__", &TList::count)
            .def("__getitem__", &TList::getByIndex)
            .def("__getitem__", &TList::getByUuid)

            .def("remove", &TList::remove)

            .def("append", &TList::append)
        ;
        addNameGetter<T, NameProvider, decltype(cls)>(cls);
    }


#define DECLARE_SERIALIZABLE_LIST(name) declList<name, name##ListNameProvider>(#name "List")

// creates a wrapper to obtain a ListWrapper<> as a property
template<typename ItemType, typename NameProvider, typename ClassWithProperty, SerializableObjectList<ItemType, NameProvider>& (ClassWithProperty::*getterMethod)(void)>
ListWrapper<ItemType, NameProvider>* listPropertyWrapper(ClassWithProperty *instance)
{
    return new ListWrapper<ItemType, NameProvider>((instance->*getterMethod)());
}

template<typename ItemType, typename NameProvider, typename ClassWithProperty, typename PythonClass, SerializableObjectList<ItemType, NameProvider>& (ClassWithProperty::*getterMethod)(void)>
void addListProperty(const char* name, PythonClass &cls)
{
    auto wrapper = listPropertyWrapper<ItemType, NameProvider, ClassWithProperty, getterMethod>;
    cls.add_property(name, make_function(wrapper, return_value_policy<manage_new_object>()));
}

#define ADD_LIST_PROPERTY(pythonClass, ContainerType, ItemType, getterFunctionName, propertyName) addListProperty<ItemType, ItemType##ListNameProvider, ContainerType, decltype(pythonClass), (&ContainerType::get##getterFunctionName)>(propertyName, pythonClass);



}
}
