#include "../library/sym/symbol.h"
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

    // setters
    void setSymbol(librepcb::library::Symbol *symbol);

    // run script
    void runScript(const QString &filename);

    // singleton instance
    static ScriptingEnvironment* instance();
private:
    // instance management
    void registerInstance();
    static ScriptingEnvironment *currentInstance;

    librepcb::library::Symbol *mSymbol;
    UndoStack *mUndoStack;
};



}
}
