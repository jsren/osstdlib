#include "../std"
#include "../collections"
#include "../memory.hpp"

using namespace std;

/*namespace std
{
    void *heap_allocate(UInt size) noexcept
    {
        return new byte[size];
    }
}*/

bool testy(int i)
{
    return i == 4;
}


int main(int argc, const char **args)
{
    List<int> test{};
    test.getEnumerator();

    volatile bool b = test.any();
    test.add(3);

    volatile bool c = test.any(Func<bool, int>(
        [](int i) { return i == 3; }));

    volatile bool d = test.any(Func<bool, int>(testy));

    test.add(4);

    auto array = test.toArray();

    auto a1 = array.union_(array);

    bool it = false;

    UInt i = 0;
    foreach(auto &t, &array) 
    {
        //t = i++;
    } end_foreach;

    test.reverse();
    array = test.toArray();

    int *nope = nullptr;

    int carray[5];
    UInt res = array.copyTo(carray, 5);
    
    return 0;
}
