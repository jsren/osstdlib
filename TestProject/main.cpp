#include "../std"
#include "../collections"
#include "../memory.cpp"

using namespace std;

/*namespace std
{
    void *heap_allocate(UInt size) noexcept
    {
        return new byte[size];
    }
}*/

int main(int argc, const char **args)
{
    List<int> test{};
    
    volatile bool b = test.any();
    test.add(3);

    volatile bool c = test.any();
    test.add(4);

    volatile auto array = test.toArray();

    int *asda = heap_allocate<int, 3>();

    return 0;
}
