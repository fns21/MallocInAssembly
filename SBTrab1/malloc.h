#ifndef _MALLOC_H_
#define _MALLOC_H_

void setup_brk();

void dismiss_brk(); 

void* memory_alloc(unsigned long int byte);

int memory_free(void *pointer);

#endif