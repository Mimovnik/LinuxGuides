# Extensible and Linkable Format
Format ELF (Executable and Linkable Format) jest standardowym formatem plików wykonywalnych, obiektowych, biblioteki współdzielonej oraz core dump'ów w systemach operacyjnych z rodziny Unix, w tym Linux. ELF zawiera informacje o segmentach i sekcjach pliku, które pozwalają na efektywne zarządzanie kodem i danymi w pamięci.

Aby zbadać strukturę pliku ELF, można użyć narzędzi takich jak readelf i objdump.

## 0. Przejdź do katalogu `code`

Wszystkie komendy poniżej wykonywane są z katalogu `code`
```bash
cd code
```

## 1. Analiza pliku wykonywalnego

### 1.1 Kod:

`main.c`
```c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```
### 1.2 Kompilacja:

```bash
gcc -o hello main.c
```

### 1.3 Analiza:

Za pomocą readelf:
```bash
readelf -h hello  # Wyświetl nagłówek ELF
readelf -l hello  # Wyświetl nagłówki programu
readelf -S hello  # Wyświetl nagłówki sekcji
```

Za pomocą objdump:
```bash
objdump -d hello  # Disasembluj sekcję .text
objdump -x hello  # Wyświetl wszystkie nagłówki i sekcje
```

## 2. Analiza biblioteki współdzielonej

### 2.1 Kod:

`lib.c`
```c
#include <stdio.h>

void hello() {
    printf("Hello from shared library!\n");
}
```

### 2.2 Kompilacja:
```bash
gcc -shared -fPIC -o libhello.so lib.c
```

### 2.3 Analiza:

```bash
readelf -h libhello.so
readelf -l libhello.so
readelf -S libhello.so
```

## 3. Analiza core dump'u:

### 3.1 Kod:

`crash.c`
```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr = NULL;
    *ptr = 10;  // Dereferencja NULL pointera spowoduje crash
    return 0;
}
```
### 3.2 Kompilacja i wykonanie programu:

```bash
gcc -o crash crash.c
ulimit -c unlimited  # Włącz generowanie core dumpów
./crash  # Powinien powstać plik core
```

> Jeśli po wykonaniu `./crash` nie powstaje plik core to może oznaczać, że systemd-coredump obsługuje core dumpy. Możesz to sprawdzić wpisując `cat /proc/sys/kernel/core_pattern`.
> Jeśli jest tam systemd-coredump użyj `coredumpctl`.
> Wyświetl listę core dump'ów:
> ```bash
> coredumpctl list
> ```
> 
> 
> Następnie skopiuj PID procesu ./crash
>
>
> Zrzuć core dump to pliku `core`:
> 
> ```bash
> coredumpctl dump <PID> > core
> ```


### 3.3 Analiza:

```bash
readelf -h core  # Wyświetl nagłówek ELF core dumpu
```

