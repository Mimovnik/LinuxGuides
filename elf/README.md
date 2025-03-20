# Extensible and Linkable Format (ELF)

The ELF (Executable and Linkable Format) is the standard format for executable files, object files, shared libraries, and core dumps in Unix-like operating systems, including Linux. ELF contains information about file segments and sections, enabling efficient management of code and data in memory.


In this guide you will compile simple programs and analyze the format of compiled files.
You will learn the structure of the ELF format and see what happens when you start a program.
You will see how executables differ from .so files and why you compile them differently.

## Overview
An ELF file contains:

- ELF Header: Provides metadata about the file (e.g., entry point, program header table offset, section header table offset).

- Program Headers: Describes segments (e.g., loadable segments, dynamic linking information) used by the OS to load the program into memory.

- Section Headers: Describes sections (e.g., .text, .data, .plt, .got.plt) used by the linker and debuggers for symbol resolution and debugging.

When compiling a program, the format of the output file may differ based on how you compiled it.
You may compile the code into the executable or the shared library (shared object).
## 1. Executable

Copy the code to `hello.c` file (if you cloned this repo it's already in the current directory)
```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```

Compile the code into the object file:
```bash
gcc -o hello hello.c
```

Now we have a `hello` file in the current directory.

Run the program to verify that it works:
```bash
./hello
```

<details>
  <summary>See the output</summary>

    Hello, World!
</details>

### 1.1 The ELF Header

We have generated some file from our source code but what is this file?
You can discover it's format:
```bash
file hello
```

<details>
  <summary>See the output</summary>

    hello: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /n
    ix/store/6q2mknq81cyscjmkv72fpcsvan56qhmg-glibc-2.40-66/lib/ld-linux-x86-64.so.2, for GNU/Linu
    x 3.10.0, not stripped
</details>

As you can see it's an 'ELF' file.
The command also prints some other information about the file like the OS bitness (**64-bit**), endianness (**LSB**- little endian), cpu architecture (**0x86-64**), OS type (**SYSV**) etc.


The question is how does the `file` command know this?


The first part of the ELF file is the ELF Header. It's used to identify the file as ELF, tell for which cpu architecture the file was compiled, how to link shared libraries into this executable (Section Headers) and how to load the file into the memory (Program Headers).


```c
#define EI_NIDENT (16)

typedef struct {
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info */
  Elf64_Half	e_type;			    /* Object file type */
  Elf64_Half	e_machine;		    /* Architecture */
  Elf64_Word	e_version;		    /* Object file version */
  Elf64_Addr	e_entry;		    /* Entry point virtual address */
  Elf64_Off	    e_phoff;		    /* Program header table file offset */
  Elf64_Off	    e_shoff;		    /* Section header table file offset */
  Elf64_Word	e_flags;		    /* Processor-specific flags */
  Elf64_Half	e_ehsize;		    /* ELF header size in bytes */
  Elf64_Half	e_phentsize;	    /* Program header table entry size */
  Elf64_Half	e_phnum;		    /* Program header table entry count */
  Elf64_Half	e_shentsize;	    /* Section header table entry size */
  Elf64_Half	e_shnum;		    /* Section header table entry count */
  Elf64_Half	e_shstrndx;		    /* Section header string table index */
} Elf64_Ehdr;
```

The first bytes of the file `hello` contains all of this information.

<details>
  <summary>More about those types and fields</summary>

    Types:
    Elf64_Off  64-bit type
    Elf64_Word 32-bit type
    Elf64_Half 16-bit type

    Note: Adding up field sizes 16 + 2 + 2 + 4 + 8 + 8 + 8 + 4 + 2 + 2 + 2 + 2 + 2 + 2 = **64 bytes** matches the typical size of a 64-bit ELF header.

    Fields:
    e_ident: It is a 16-byte array that identifies the ELF object, it always starts with "\x7fELF".
    e_type: Specifies the ELF type:
        ET_NONE (Undefined): ELF Format unknown or not specified.
        ET_EXEC: (Executable file): An ELF executable.
        ET_DYN: (Shared object): A library or a dynamically-linked executable.
        ET_REL (Relocatable file): Relocatable files (.o object files).
        ET_CORE (Core dump): A core dump file.
    e_machine: Target architecture.
    e_version: ELF file version. Always '1'.
    e_entry: Entry point address.
    e_phoff: Program Headers offset.
    e_shoff: Section Headers offset.
    e_flags: Processor-specific flags.
    e_ehsize ELF Header (this header) size in bytes. (Usually 64 bytes in 64-bit ELF and 52 bytes for 32 bits)
    e_phentsize: Program Headers entry size.
    e_phnum: Program Headers entries.
    e_shentsize: Section Headers entry size.
    e_shnum: Section Headers entries.
    e_shstrndx: Section Headers string table index (.shstrtab, it contains null terminated-strings with the name of each section)

    Note: e_phoff and e_shoff are offsets of the ELF file, e_entry instead is a virtual address.
</details>

You can print the hex and ASCII dumps of the header of our `hello` file:
```bash
xxd -l 64 hello
```
`-l 64` flag limits the output to first 64 bytes which is the ELF header size (usually 64 bytes in 64-bit ELF and 52 bytes for 32 bits)

<details>
  <summary>See the output</summary>

    00000000: 7f45 4c46 0201 0100 0000 0000 0000 0000  .ELF............
    00000010: 0200 3e00 0100 0000 6010 4000 0000 0000  ..>.....`.@.....
    00000020: 4000 0000 0000 0000 0836 0000 0000 0000  @........6......
    00000030: 0000 0000 4000 3800 0d00 4000 1d00 1c00  ....@.8...@.....
</details>


Not much can be seen here besides the "ELF" string so it's better to print the header in the human-readible format:
```bash
readelf -h hello
```

<details>
  <summary>See the output</summary>

    ELF Header:
      Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
      Class:                             ELF64
      Data:                              2's complement, little endian
      Version:                           1 (current)
      OS/ABI:                            UNIX - System V
      ABI Version:                       0
      Type:                              EXEC (Executable file)
      Machine:                           Advanced Micro Devices X86-64
      Version:                           0x1
      Entry point address:               0x401060
      Start of program headers:          64 (bytes into file)
      Start of section headers:          13832 (bytes into file)
      Flags:                             0x0
      Size of this header:               64 (bytes)
      Size of program headers:           56 (bytes)
      Number of program headers:         13
      Size of section headers:           64 (bytes)
      Number of section headers:         29
      Section header string table index: 28
</details>

### 1.2 The Section Headers

Now that we know how to identify the file we can analyze what happens when we run the program.
What happens after:
```bash
./hello
```

## 2. Shared Library Analysis

### 2.1 Code:

`lib.c`
```c
#include <stdio.h>

void hello() {
    printf("Hello from shared library!\n");
}
```

### 2.2 Compilation:
```bash
gcc -shared -fPIC -o libhello.so lib.c
```

### 2.3 Analysis:

```bash
readelf -h libhello.so
readelf -l libhello.so
readelf -S libhello.so
```

## 3. Core Dump Analysis

### 3.1 Code:

`crash.c`
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr = NULL;
    *ptr = 10;  // Dereferencing a NULL pointer will cause a crash
    return 0;
}
```
### 3.2 Compilation and Execution:

```bash
gcc -o crash crash.c
ulimit -c unlimited  # Enable core dump generation
./crash  # A core dump file should be generated
```

> If no core dump file is created after running `./crash`, it may indicate that `systemd-coredump` is handling core dumps. You can check this by running `cat /proc/sys/kernel/core_pattern`.
> If `systemd-coredump` is listed, use `coredumpctl`:
> ```bash
> coredumpctl list
> ```
> Then, copy the PID of the `./crash` process.
>
> Dump the core file to `core`:
> ```bash
> coredumpctl dump <PID> > core
> ```

### 3.3 Analysis:

```bash
readelf -h core  # Display ELF header of the core dump
```

