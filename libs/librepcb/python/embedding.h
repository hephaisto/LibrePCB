#include "../library/sym/symbol.h"

namespace librepcb {
namespace python {

void init();
void runScript(const QString &filename, librepcb::library::Symbol *symbol=nullptr);

}
}
