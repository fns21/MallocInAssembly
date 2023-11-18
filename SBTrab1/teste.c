#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>

#include "malloc.h" 							

extern void *brk_original;	
extern void *brk_current;
									//Você precisa ter a variável global que armazena o valor de brk como um extern aqui.
									//No código de teste estou chamandando de brk_original, mas se utilizarem outro nome,
									//substituir as ocorrências por ele aqui.

void quad_alloc_after_free(void **pointers_array){
	void *a = memory_alloc(50);
	printf("%p %p\n", brk_original, brk_current);
	void *b = memory_alloc(100);
	printf("%p %p\n", brk_original, brk_current);
	void *c = memory_alloc(70);
	printf("%p %p\n", brk_original, brk_current);
	void *d = memory_alloc(34);
	printf("%p %p\n", brk_original, brk_current);
	pointers_array[1] = a;
	pointers_array[2] = b;
	pointers_array[3] = c;
	pointers_array[4] = d;
}

void simple_alloc(void **pointers_array){
	void* a = memory_alloc(100);
	printf("%p %p\n", brk_original, brk_current);

	pointers_array[0] = a;
}

int main() { 

	printf("============================== ROTINAS DE TESTE ==============================\n");

    setup_brk();

	void *initial_brk = brk_original;
	void *alloc_pointers[5];

    simple_alloc(alloc_pointers);
	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[0]-16 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("%p %p\n", alloc_pointers[0]-16, initial_brk);
	
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[0]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[0]-8)) == 100 ? "CORRETO!" : "INCORRETO!");

		// Teste usando os 100 bytes alocados
	printf("==>> TESTE USANDO OS 100 BYTES ALOCADOS:\n");
	char *data = (char *)(alloc_pointers[0]); // Considerando que o bloco começa 16 bytes após o início

	// Preencher os 100 bytes com algum dado
	for (int i = 0; i < 100; ++i) {
		data[i] = 'A' + (i % 26); // Preencher com letras de A a Z
	}

	// Imprimir o conteúdo dos 100 bytes
	printf("\tCONTEÚDO DOS 100 BYTES:\n\t");
	for (int i = 0; i < 100; ++i) {
		printf("%c ", data[i]);
	}
	printf("\n");

	// Liberação da memória
	printf("==>> DESALOCANDO OS 100 BYTES:\n");
	memory_free(alloc_pointers[0]);

	// Tentativa de acesso após a liberação
	printf("==>> TENTATIVA DE ACESSO APÓS LIBERAÇÃO:\n");
	printf("\tIND. DE USO: %s\n", *((long long *)(alloc_pointers[0] - 16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long *)(alloc_pointers[0] - 8)) == 100 ? "CORRETO!" : "INCORRETO!");

	quad_alloc_after_free(alloc_pointers);
	printf("==>> ALOCANDO UM ESPAÇO DE 50 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[1]-16 == initial_brk ? "CORRETO!" : "INCORRETO!");
	printf("%p %p\n", alloc_pointers[1]-16, initial_brk);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]-8)) == 50 ? "CORRETO!" : "INCORRETO!");
	printf("%lld\n", *((long long*) (alloc_pointers[1]-8)));

	printf("==>> ALOCANDO UM ESPAÇO DE 100 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[1]+100 == alloc_pointers[2]-16 ? "CORRETO!" : "INCORRETO!");
	printf("%p %p\n", alloc_pointers[1]+100, alloc_pointers[2]-16);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[2]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("%lld\n", *((long long*) (alloc_pointers[2]-16)));
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[2]-8)) == 100 ? "CORRETO!" : "INCORRETO!");
	printf("%lld\n", *((long long*) (alloc_pointers[2]-8)));

	printf("==>> ALOCANDO UM ESPAÇO DE 70 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[2]+100 == alloc_pointers[3]-16 ? "CORRETO!" : "INCORRETO!");
	printf("%p %p\n", alloc_pointers[2]+100, alloc_pointers[3]-16);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[3]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[3]-8)) == 70 ? "CORRETO!" : "INCORRETO!");

	printf("==>> ALOCANDO UM ESPAÇO DE 34 BYTES:\n");
	printf("\tLOCAL: %s\n", alloc_pointers[1]+50 == alloc_pointers[4]-16 ? "CORRETO!" : "INCORRETO!");
	printf("%p %p\n", alloc_pointers[1]+50, alloc_pointers[4]-16);
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[4]-16)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[4]-8)) == 34 ? "CORRETO!" : "INCORRETO!");


	printf("==> VERIFICANDO A FRAGMENTAÇÃO DE MEMÓRIA:\n");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]+50)) == 1 ? "CORRETO!" : "INCORRETO!");
	printf("%lld\n", *((long long*) (alloc_pointers[1]+50)));
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]+58)) == 34 ? "CORRETO!" : "INCORRETO!");
	printf("%lld\n", *((long long*) (alloc_pointers[1]+58)));


	printf("==>> DESALOCANDO TUDO:\n");
	memory_free(alloc_pointers[1]);
	memory_free(alloc_pointers[2]);
	memory_free(alloc_pointers[3]);
	memory_free(alloc_pointers[4]);

	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[1]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[1]-8)) == 50 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[2]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[2]-8)) == 100 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[3]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[3]-8)) == 70 ? "CORRETO!" : "INCORRETO!");
	printf("\tIND. DE USO: %s\n", *((long long*) (alloc_pointers[4]-16)) == 0 ? "CORRETO!" : "INCORRETO!");
	printf("\tTAMANHO: %s\n", *((long long*) (alloc_pointers[4]-8)) == 34 ? "CORRETO!" : "INCORRETO!");


	printf("==>> DESALOCANDO A PILHA (ILEGAL):\n");
	unsigned int alloc_return = memory_free((void*) alloc_pointers);
	if (!alloc_return) printf("\tO RETORNO DA LIBERAÇÃO FOI NULL!\n");

	dismiss_brk();

	printf("%p %p\n", brk_original, brk_current);

	return 0;
}