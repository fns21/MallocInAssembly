#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#define _BYTESTOALLOC_ 100

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
    // Você pode implementar a lógica de liberação de memória aqui, mas não está mostrada no seu código original.
    bool* free = (bool*)pointer;
    *free = false;
    return 0;
}

int main() {
    setup_brk();

    printf("%p\n", brk_current);

    bool uso = true;
    unsigned long int tam_bloco = _BYTESTOALLOC_;

    // Alocar 'uso', 'tam_bloco' e 'dados' sequencialmente
    brk_current += sizeof(uso);
    bool *ptr_uso = (bool*)brk_current;
    *ptr_uso = uso;

    brk_current += sizeof(tam_bloco);
    unsigned long int *ptr_tam_bloco = (unsigned long int*)brk_current;
    *ptr_tam_bloco = tam_bloco;

    int *dados = (int *)memory_alloc(_BYTESTOALLOC_);

    // Acessar os valores
    printf("uso: %d\n", *ptr_uso);
    printf("tam_bloco: %lu\n", *ptr_tam_bloco);

    // Imprimir os valores do vetor 'dados'
    printf("Dados: ");
    for (int i = 0; i < _BYTESTOALLOC_ / sizeof(int); i++) {
        printf("%d ", dados[i]);
    }
    printf("\n");

    dismiss_brk(); // Liberar a memória alocada

    return 0;
}