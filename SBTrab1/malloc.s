.section .data
.global brk_original
.global brk_current

brk_original: .quad 0
brk_current: .quad 0

.section .text
.global setup_brk
.global dismiss_brk
.global memory_alloc
.global memory_free

setup_brk:
    movq $12, %rax
    movq $0, %rdi                   # A constante 0 em %rdi faz com que o começo da heap seja retornado em %rax
    syscall

    movq %rax, brk_original         # Guarda o começo da heap no brk_original (imutável)
    movq %rax, brk_current          # Guarda o começo da heap no brk_current (muda a cada nova alocação)

    ret

dismiss_brk:
    movq brk_original, %rax         # Reseta o limite da heap para o valor original
    movq %rax, brk_current

    ret

memory_alloc:
    movq %rdi, %rdx                 # Guarda o argumento da função (tamanho da alocação) em %rdx
    movq brk_original, %rcx         # Guarda o brk_original em %rcx
    cmpq %rcx, brk_current          # Verifica se é a primeira alocação
    je _alloc                       # Se for, aloca normalmente, sem gastar tempo procurando espaço vago

    _search_block:
        cmpq brk_current, %rcx      # Condição de parada de loop: verifica se chegou no limite da heap alocada
        jge _alloc                  # Se sim, não há bloco vago, deve-se alocar novo bloco
        movq (%rcx), %rsi           
        cmpq $0, %rsi               # Se não, verifica se o bloco atual está em uso
        jne _skip_block             # Se estiver em uso, ele é skipado
        movq 8(%rcx), %rdi          
        cmpq %rdx, %rdi             # Se estiver em desuso, verifica a requisição cabe
        jl _skip_block              # Se não cabe (%rdx > %rdi), o bloco é skipado
        movq $1, (%rcx)             # Se cabe, seta a flag de uso do bloco para 1 (em uso)
        movq %rdx, 8(%rcx)          # Coloca %rdx como novo tamanho do bloco

    _break_block:
        addq $16, %rcx              # %rcx aponta para inicio do bloco
        subq %rdx, %rdi             
        subq $16, %rdi              # Calcula o espaço livre do bloco quebrado (Original - Requisitado - Tamanho do bloco de registro)
        cmpq $1, %rdi               # Verifica se há ao menos 1 byte livre para alocação
        jl _return                  # Se não, ele não quebra o bloco
        addq %rdx, %rcx             # Se sim, %rcx aponta para o fim do bloco alocado e em uso (cria novo bloco)
        movq $0, (%rcx)             # Seta a flag uso do bloco quebrado para 0 (desuso)
        movq %rdi, 8(%rcx)          # Seta o tamanho do bloco quebrado para o valor de %rdi
        subq %rdx, %rcx             # %rcx aponta  para o inicio do bloco alocado e em uso
        
    _return:
        movq %rcx, %rax             # Retorna o primeiro byte do bloco de dados alocado e em uso
        jmp _end

    _skip_block:
        addq 8(%rcx), %rcx          # Adiciona o tamanho do bloco em %rcx
        addq $16, %rcx              # Adiciona o bloco de registro em %rcx
        jmp _search_block

    _alloc:
        # Alocação do bloco de registro
        addq $16, brk_current       # 8 bytes para uso e 8 bytes para o tamanho

        movq $12, %rax
        movq brk_current, %rdi      # Aloca o bloco de registro
        syscall

        movq %rax, %rbx             # Guarda o valor do começo do bloco alocado

        movq $1, -16(%rax)          # Seta a flag de uso para 1(em uso)
        movq %rdx, -8(%rax)         # Tamanho da alocação 
        
        # Alocar os bytes da chamada da funcao
        addq %rdx, brk_current      # Atualiza o limite da heap 
        movq $12, %rax              # Chamada do sys_brk
        movq brk_current, %rdi      # Aloca o topo da pilha 
        syscall

        movq %rbx, %rax             # Aponta %rax para o começo do bloco alocado

    _end:
        ret

memory_free:
    cmpq brk_original, %rdi   # Comparação com o início do intervalo
    jl _return_null           # Salta se %rdi for menor que brk_original

    cmpq brk_current, %rdi    # Comparação com o final do intervalo
    jge _return_null          # Salta se %rdi for maior ou igual a brk_current

    movq $0, -16(%rdi)        # Move $0 para -16(%rdi) se estiver no intervalo desejado
    jmp _ret                  # Pula para _not_free após a movimentação

    _return_null:
        movq $0, %rax          # Define %rax para 0 (null)
        ret

    _ret:
        ret
