namespace librepcb {
namespace python {

using boost::python::return_value_policy;
using boost::python::copy_const_reference;

template<
    typename ClassWithProperty,
    typename PropertyType,
    typename EditCommand,
    const PropertyType& (ClassWithProperty::*getterMethod)(void) const,
    void (ClassWithProperty::*directSetterMethod)(const PropertyType &),
    void (EditCommand::*commandSetterMethod)(const PropertyType &, bool)
    >
void wrappedPropertySetter(ClassWithProperty *instance, const PropertyType &newValue)
{
    UndoStack* undoStack = ScriptingEnvironment::instance()->getUndoStack();
    if(undoStack)
    {
        EditCommand *cmd = new EditCommand(*instance);
        (cmd->*commandSetterMethod)(newValue, true);
        undoStack->appendToCmdGroup(cmd);
    }
    else
    {
        (instance->*directSetterMethod)(newValue);
    }
}

// example usage: addWrappedProperty<decltype(symbolPinClass), SymbolPin, QString, CmdSymbolPinEdit, &SymbolPin::getName, &SymbolPin::setName, &CmdSymbolPinEdit::setName>("name", symbolPinClass)

template<
    typename PythonClass,
    typename ClassWithProperty,
    typename PropertyType,
    typename EditCommand,
    const PropertyType& (ClassWithProperty::*getterMethod)(void) const,
    void (ClassWithProperty::*directSetterMethod)(const PropertyType &),
    void (EditCommand::*commandSetterMethod)(const PropertyType &, bool)
    >
void addWrappedProperty(const char *name, PythonClass &cls)
{
    auto wrapper = wrappedPropertySetter<ClassWithProperty, PropertyType, EditCommand, getterMethod, directSetterMethod, commandSetterMethod>;
    cls.add_property(
            name,
            make_function(getterMethod, return_value_policy<copy_const_reference>()),
            wrapper
            );
}

// usage: ADD_WRAPPED_PROPERTY(symbolPinClass, SymbolPin, QString, Name, "name")
#define ADD_WRAPPED_PROPERTY(pythonClassObject, className, propertyType, uppercaseProperty, propertyName) addWrappedProperty<decltype(pythonClassObject), className, propertyType, Cmd##className##Edit, &className::get##uppercaseProperty, &className::set##uppercaseProperty, &Cmd##className##Edit::set##uppercaseProperty>(propertyName, pythonClassObject)

}
}
