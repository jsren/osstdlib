

namespace abi
{
    struct __file_handle
    {
        unsigned int value;
    };

    extern __file_handle __stdin;
    extern __file_handle __stdout;
    extern __file_handle __stderr;


    int __open(const char* filename) noexcept;
    int __close(__file_handle handle) noexcept;
    int __write(__file_handle handle, const void* data, unsigned long size) noexcept;
}
