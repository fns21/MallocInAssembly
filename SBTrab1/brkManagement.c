#include <unistd.h>
#include <stdio.h>

#define _BYTESTOALLOC_ 8

void *brk_original = 0, *brk_current = 0;

void setup_brk() {
    brk_original = sbrk(0);
    brk_current = brk_original;
}

void dismiss_brk() {
    brk_current = brk_original;
    brk(brk_original);
}

void* memory_alloc(unsigned long int bytes) {
    void *allocated = sbrk(bytes);
    if (allocated != (void*)-1) {
        brk_current += bytes;
    }
    return allocated;
}

int memory_free(void *pointer) {
    // You can implement memory freeing logic here, but it's not shown in your original code.
    return 0;
}

int main() {
    setup_brk();

    // Allocate some memory
    int *arr = (int*)memory_alloc(_BYTESTOALLOC_ * sizeof(int));

    if (arr != (void*)-1) {
        for (int i = 0; i < _BYTESTOALLOC_; i++) {
            arr[i] = i;
        }

        printf("Memory allocated successfully. Contents of the array:\n");
        for (int i = 0; i < _BYTESTOALLOC_; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else {
        printf("Memory allocation failed.\n");
    }

    dismiss_brk(); // Release the allocated memory

    return 0;
}
