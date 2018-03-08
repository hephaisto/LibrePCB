// this is necessary to fix
// C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include/c++/cmath:1119:11: error: '::hypot' has not been declared
// likely fixed by https://github.com/python/cpython/pull/880
/// @TODO remove this file, replace its inclusions by a plain #include<boost/python.hpp> and remove macro definition in ROOT/appveyor.yml when upstream fix is applied

#ifdef ENABLE_HYPOT_WORKAROUND
#include <cmath>
#endif

