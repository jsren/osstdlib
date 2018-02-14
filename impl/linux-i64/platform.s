.globl __platform__pre_start
;# Linux-x64 compiler ABI requires 16-byte stack alignment
;# to an 8-byte boundary. So add 8 to offset stack.
;# Once GCC8 is common, we can switch to using __attribute__((naked)) instead
;# Of the separate assembly file
__platform__pre_start:
    pop %rdi
    sub $0x8, %rsp
    push %rdi
    ret
