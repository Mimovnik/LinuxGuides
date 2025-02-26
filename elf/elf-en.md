# Extensible and Linkable Format (ELF)

The ELF (Executable and Linkable Format) is the standard format for executable files, object files, shared libraries, and core dumps in Unix-like operating systems, including Linux. ELF contains information about file segments and sections, enabling efficient management of code and data in memory.

To analyze the structure of an ELF file, tools such as `readelf` and `objdump` can be used.

## 0. Navigate to the `code` directory

All commands below should be executed from the `code` directory:
```bash
cd code
```

## 1. Executable File Analysis

### 1.1 Code:

`main.c`
```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```
### 1.2 Compilation:

```bash
gcc -o hello main.c
```

### 1.3 Analysis:

Using `readelf`:
```bash
readelf -h hello  # Display ELF header
readelf -l hello  # Display program headers
readelf -S hello  # Display section headers
```

Using `objdump`:
```bash
objdump -d hello  # Disassemble the .text section
objdump -x hello  # Display all headers and sections
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

