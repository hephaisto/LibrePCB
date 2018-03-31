#include "../library/sym/symbol.h"
#include "../library/pkg/package.h"
#include "../library/cmp/component.h"
#include "../library/dev/device.h"

#include "../common/undostack.h"

namespace librepcb {
namespace python {

class ScriptingEnvironment
{
    Q_DECLARE_TR_FUNCTIONS(ScriptingEnvironment)
public:
    ScriptingEnvironment() = delete;
    ScriptingEnvironment(ScriptingEnvironment &other) = delete;
    explicit ScriptingEnvironment(UndoStack *undoStack = nullptr);
    virtual ~ScriptingEnvironment() noexcept;

    // getters
    UndoStack *getUndoStack() const noexcept;
    librepcb::library::Symbol* getSymbol() const noexcept;
    librepcb::library::Package* getPackage() const noexcept;
    librepcb::library::Component* getComponent() const noexcept;
    librepcb::library::Device* getDevice() const noexcept;

    // setters
    void setSymbol(librepcb::library::Symbol *symbol);
    void setPackage(librepcb::library::Package *package);
    void setComponent(librepcb::library::Component *component);
    void setDevice(librepcb::library::Device *device);

    // run script
    void runScript(const QString &filename);

    // singleton instance
    static ScriptingEnvironment* instance();
private:
    // instance management
    void registerInstance();
    static ScriptingEnvironment *currentInstance;

    librepcb::library::Symbol *mSymbol;
    librepcb::library::Package *mPackage;
    librepcb::library::Component *mComponent;
    librepcb::library::Device *mDevice;
    UndoStack *mUndoStack;
};



}
}
