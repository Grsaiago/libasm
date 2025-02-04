# libasm

Recreating a bunch of glibc functions in x86 asm

## References

[Main blog post](https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/)
[Calling convention for 64](https://www.uclibc.org/docs/psABI-x86_64.pdf): On A.2.1: Calling Conventions
[Nasm instructions in depth documentation](https://nasm.us/doc/)
[Documentation on each instruction](https://www.felixcloutier.com/x86/)
[Intel in depth docs](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)

## FAQ

Q: Where can I find which integers map to which syscall on my linux os?\
A: Go to /usr/include/x86_64-linux-gnu/asm/unistd_<64||32>.h

Q: How do I know where each function parameter in C is on my asm code?\
A: See [Calling Conventions](https://www.uclibc.org/docs/psABI-x86_64.pdf),
chapter A.2.1: Calling Conventions

Q: In which register should I store the return value of a function?\
A: See [Calling Conventions](https://www.uclibc.org/docs/psABI-x86_64.pdf),
end of chapter 3.2.3, on the "Returning of Values".
