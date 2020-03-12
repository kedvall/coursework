.text

.globl  sys_call_execute
.globl  sys_call_dispatch
.globl  execute, halt, read, write, open, getargs, vidmap, set_handler, sigreturn
.globl  halt_wrap

.type   sys_call_execute, @function
.type   halt_wrap, @function

sys_call_dispatch:
    cli
    cmpl $1, %eax
    jb  invalid_input
    cmpl $10, %eax
    ja  invalid_input
    pushal
    pushfl
    subl $1, %eax
    /*smth with checking system call value*/
    sti
    call *syscall_table(, %eax, 4)
    pushl %eax
    addl $4, %esp
    popfl
    popal
    movl -40(%esp), %eax
    sti
    iret
invalid_input:
    movl $-1, %eax
    sti
    iret

 sys_call_execute:
    pushl %ebp
    movl  %esp,%ebp
    pushw $0x0000
    pushw $0x002B
    pushl $0x83FFFFC /* ESP register */
    pushl $0x00000200
    pushw $0x0000
    pushw $0x0023
    pushl 8(%ebp) /* first program address placed in eip */

    # Set up the rest of the segment selector registers
    movw  $0x002B, %cx
    movw  %cx, %ds
    iret
execute_return:
    leave
    ret

/* Take another look at execute i don't think this is right */
execute_wrap:
    pushl %ebx
    call execute
    popl %ebx
    /*leave*/
    ret

read_wrap:
    pushl %edx
    pushl %ecx
    pushl %ebx
    call read
    popl %ebx
    popl %ecx
    popl %edx
    ret

write_wrap:
    pushl %edx
    pushl %ecx
    pushl %ebx
    call write
    popl %ebx
    popl %ecx
    popl %edx
    ret

close_wrap:
    pushl %ebx
    call close
    popl %ebx
    ret

open_wrap:
    pushl %ebx
    call open
    popl %ebx
    ret

halt_wrap:
    pushl %ebx
    call halt
    popl %ebx
    movzbl %bl, %eax    /* move %bl to %eax with zero extend */
    jmp execute_return

get_args_wrap:
    pushl %ecx
    pushl %ebx
    call getargs
    popl %ebx
    popl %ecx
    ret

vidmap_wrap:
    pushl   %ebx
    call vidmap
    popl    %ebx
    ret

set_handler_wrap:
    pushl %ecx
    pushl %ebx
    call set_handler
    popl %ebx
    popl %ecx
    ret

sigreturn_wrap:
    call sigreturn
    ret

 syscall_table:
    .long halt_wrap, execute_wrap, read_wrap, write_wrap, open_wrap, close_wrap, get_args_wrap, vidmap_wrap, set_handler_wrap, sigreturn_wrap
