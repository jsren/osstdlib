#include <ostream>
#include <sstream>

namespace std
{
    ostream cout{new stringbuf{}};
}
