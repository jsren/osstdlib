#include <ostream>
#include <sstream>

extern "C++" {
namespace std
{
    ostream cout{new stringbuf{}};
}
}
