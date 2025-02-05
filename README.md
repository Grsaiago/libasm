
# Libasm

This project provides x86-64 assembly implementations of common C standard
library functions, including `read`, `write`, `strlen`, `strcmp`, `strcpy`, and `strdup`.\
It uses `libcriterion` as a testing framework, so you can see not only
what I've done, but how I've tested it!


## 📂 Project Structure

```
.
├── Makefile                # Build automation
├── Dockerfile              # Container setup for testing
├── README.md               # This file
├── src/                    # Assembly source files
│   ├── read.s              # Implementation of `read`
│   ├── strcmp.s            # Implementation of `strcmp`
│   ├── strcpy.s            # Implementation of `strcpy`
│   ├── strdup.s            # Implementation of `strdup`
│   ├── strlen.s            # Implementation of `strlen`
│   └── write.s             # Implementation of `write`
├── tests/                  # Test suite using libcriterion
│   ├── test_read.c         # Tests for `read`
│   ├── test_strcmp.c       # Tests for `strcmp`
│   ├── test_strcpy.c       # Tests for `strcpy`
│   ├── test_strdup.c       # Tests for `strdup`
│   ├── test_strlen.c       # Tests for `strlen`
│   └── test_write.c        # Tests for `write`
└── objects/                # Compiled object files
```

## Features

### Assembly Implementations

All functions follow their glibc counterparts.\
Want to know how to call one of my functions?
Just `man <function_name>` and that's it!

- `read`: Reads n bytes from a file descriptor into a buffer.
- `strcmp`: Compares two strings.
- `strcpy`: Copies a string to a pre allocated buffer.
- `strdup`: Duplicates a string, giving a newly allocated copy.
- `strlen`: Computes the length of a string.
- `write`: Writes n bytes from a buffer into a file descriptor.

### Interoperability with C

The assembly functions are designed to be called from C code, adhering to the x86-64 calling convention.
For example, to call `strcmp` from C:

```c
extern int asm_strcmp(const char *s1, const char *s2);
int result = asm_strcmp("hello", "world");
```

### Full Test Coverage

There's a series of tests for each function using `libcriterion`.
There's tests for happy paths and for not so happy paths xD.

### Easy Setup

A `Dockerfile` is provided to containerize the testing environment,
so you don't have to install `libcriterion` locally.

### Makefile Automation

The included `Makefile` simplifies building, testing, and cleaning the project,
so I don't have to write a lot of documentation and you don't have to copy-paste
a bunch of commands on the CLI.\
Just run `make` or `make help` and you'll see the available rules!

## Setup

### Prerequisites

#### For Local Development

- NASM (Netwide Assembler)
- GCC (GNU Compiler Collection)
- Make
- `libcriterion` (for testing)

#### For Docker

- Docker (what else did you expect? xD)

### How to Run?

#### Locally

**Build the Project:**

```bash
make build
```

**Clean Build Artifacts:**

```bash
make clean
```

Just compile your C project with the libasm.a and you're good to go!

#### Using Docker

**Build the Docker Image:**

```bash
docker image build --tag libasm .
```

**Run Tests in the Container:**

```bash
docker container run --rm libasm
```

## 🧪 Testing

The project uses `libcriterion` (an absolute banger of a framework) for testing.\
All tests are in the same file, divided by each function. To run all the tests
just go for:

```bash
make run-tests
```

Or, if you've built the docker image, just run the image.
The image's default entrypoint is the test suite

### Example Test Output

```shell
[----] tests/test_strcmp.c:25: test_strcmp_equal
[OK]  test_strcmp_equal
[----] tests/test_strcmp.c:35: test_strcmp_not_equal
[OK]  test_strcmp_not_equal
...
[====] Synthesis: Tested: 6 | Passing: 6 | Failing: 0 | Crashing: 0
```

## References

[Main blog post](https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/)\
[Calling convention for 64](https://www.uclibc.org/docs/psABI-x86_64.pdf):
On A.2.1: Calling Conventions\
[Nasm instructions in depth documentation](https://nasm.us/doc/)\
[Documentation on each instruction](https://www.felixcloutier.com/x86/)\
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

Q: Which register should I use to store a counter?\
A: rcx is commonly the one used. It is implied by the loop section on the intel
documentation.

## TODOS

- [ ] Add a function prelude to all functions ([reference](https://www.youtube.com/watch?v=U9HXtrDwxVM))
- [ ] Add memcpy
- [ ] Add memset

## 🙏 Acknowledgments

- `libcriterion`: For providing a robust testing framework.
- `NASM`: For making x86-64 assembly development accessible.
- `Docker`: For simplifying environment setup and testing.
