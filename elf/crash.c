#include <stdio.h>
#include <stdlib.h>

int main() {
    int *ptr = NULL;
    *ptr = 10;  // Dereferencja NULL pointera spowoduje crash
    return 0;
}
