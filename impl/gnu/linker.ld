SECTIONS
{
    .init_array : {
        __abi_init_array_start = .;
        *(.init_array);
        __abi_init_array_end = .;
    }
}
INSERT BEFORE .text