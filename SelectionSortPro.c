#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ordenacao.h"

// ===================================================================
// SELEÇÃO PRO (Consertando o vetor Desordenado)
// ===================================================================
met* selecaoCorrigida(r *v, int tamanho) {
    met *m = malloc(sizeof(met));
    m->comparacoes = 0;
    m->movimentacoes = 0;

    // O problema era o limite do laço
    // O "i" precisa ir até o penúltimo
    // elemento (tamanho - 1) para garantir que o último fique no lugar certo
    for (int i = 0; i < tamanho - 1; i++) { 
        int indice_menor = i; // Vamos supor que o menor cara está na posição atual
        
        // Varre o resto do vetor pra frente procurando alguém menor
        for (int j = i + 1; j < tamanho; j++) {
            m->comparacoes++;
            if (v[j].user_id < v[indice_menor].user_id) {
                indice_menor = j; // Se achou, é anotada a posição dele
            }
        }
        
        // Fazemos a troca
        // Se ele não achou ninguém (ou seja, o próprio i já era o menor), pula a troca
        // para ter menos movimentações
        if (indice_menor != i) {
            r aux = v[i];
            v[i] = v[indice_menor];
            v[indice_menor] = aux;
            m->movimentacoes += 3; 
        }
    }
    
    return m;
}

// ===================================================================
// FUNÇÕES DE AVALIAÇÃO E BENCHMARKING
// ===================================================================

int verificaEstabilidade(r *vet, int tamanho) {
    // Varre o vetor para ver se a ordem foi mantida para IDs iguais
    for (int i = 0; i < tamanho - 1; i++) {
        if (vet[i].user_id == vet[i+1].user_id && vet[i].chegada > vet[i+1].chegada) {
            return 0;
        }
    }
    return 1;
}

int verificaSeEstaOrdenado(r *vet, int tamanho) {
    // Se o da frente é maior que o de trás, o erro aparece
    for (int i = 0; i < tamanho - 1; i++) {
        if (vet[i].user_id > vet[i+1].user_id) {
            return 0;
        }
    }
    return 1;
}

void imprimeUltimos(r *vet, int tamanho) {
    // O bug estava no final do vetor,
    // essa função imprime os elementos finais do array para termos uma demonstração visual
    printf("                   Visuais (5 ultimos): ");
    
    int inicio;
    
    // Lógica para evitar que ele tente ler posições negativas se o vetor for minúsculo
    if (tamanho > 5) {
        inicio = tamanho - 5;
    } else {
        inicio = 0;
    }
    
    for (int i = inicio; i < tamanho; i++) {
        printf("%d ", vet[i].user_id);
    }
    printf("\n");
}

void avaliaSelecao(int tamanho) {
    printf("\n==================================================\n");
    printf("   SELECAO PRO - Avaliacao para N = %d\n", tamanho);
    printf("==================================================\n");

    clock_t inicio_timer, fim_timer; 
    double tempo_total;
    long long int comp_total, mov_total; // Variável "long long" para comparações muito grandes 

    // Teste 1: Vetores Aleatórios (Calculando a média de 30)
    tempo_total = 0; comp_total = 0; mov_total = 0;
    
    for (int i = 0; i < 30; i++) {
        r *vetor = geraAleatorios(tamanho, i + 1); 
        
        inicio_timer = clock(); 
        met *m = selecaoCorrigida(vetor, tamanho); 
        fim_timer = clock();
        
        // Print só no primeiro vetor gerado pra não poluir o terminal
        if (i == 0) {
            if (verificaEstabilidade(vetor, tamanho) == 1) {
                printf("[Estabilidade]     Status: ESTAVEL\n");
            } else {
                printf("[Estabilidade]     Status: INSTAVEL\n");
            }

            if (verificaSeEstaOrdenado(vetor, tamanho) == 1) {
                printf("[Aleatorio]        Status: SUCESSO\n");
            } else {
                printf("[Aleatorio]        Status: FALHA\n");
            }
            imprimeUltimos(vetor, tamanho);
        }
        
        tempo_total += ((double)(fim_timer - inicio_timer) / CLOCKS_PER_SEC) * 1000.0;
        comp_total += m->comparacoes; 
        mov_total += m->movimentacoes;
        
        liberaVetor(vetor); 
        liberaMetricas(m);
    }
    printf("                   Tempo Medio: %.3f ms | Comp: %lld | Mov: %lld\n", 
           tempo_total / 30.0, comp_total / 30, mov_total / 30);

    // Teste 2: Vetor Crescente
    r *vet_cres = geraOrdenados(tamanho, 42);
    
    inicio_timer = clock(); 
    met *m_cres = selecaoCorrigida(vet_cres, tamanho); 
    fim_timer = clock();
    
    if (verificaSeEstaOrdenado(vet_cres, tamanho) == 1) {
        printf("[Crescente]        Status: SUCESSO\n");
    } else {
        printf("[Crescente]        Status: FALHA\n");
    }
    imprimeUltimos(vet_cres, tamanho);
    printf("                   Tempo:       %.3f ms | Comp: %lld | Mov: %lld\n", 
           ((double)(fim_timer - inicio_timer) / CLOCKS_PER_SEC) * 1000.0, m_cres->comparacoes, m_cres->movimentacoes);
           
    liberaVetor(vet_cres); 
    liberaMetricas(m_cres);

    // Teste 3: Vetor Decrescente
    r *vet_decres = geraDecrescente(tamanho, 42);
    
    inicio_timer = clock(); 
    met *m_decres = selecaoCorrigida(vet_decres, tamanho); 
    fim_timer = clock();
    
    if (verificaSeEstaOrdenado(vet_decres, tamanho) == 1) {
        printf("[Decrescente]      Status: SUCESSO\n");
    } else {
        printf("[Decrescente]      Status: FALHA\n");
    }
    imprimeUltimos(vet_decres, tamanho);
    printf("                   Tempo:       %.3f ms | Comp: %lld | Mov: %lld\n", 
           ((double)(fim_timer - inicio_timer) / CLOCKS_PER_SEC) * 1000.0, m_decres->comparacoes, m_decres->movimentacoes);
           
    liberaVetor(vet_decres); 
    liberaMetricas(m_decres);

    // Teste 4: Vetor Quase Ordenado (Média de 30 rodadas)
    tempo_total = 0; comp_total = 0; mov_total = 0;
    
    for (int i = 0; i < 30; i++) {
        r *vetor = geraQuaseOrdenados(tamanho, i + 1, 10); 
        
        clock_t t_inicio = clock(); 
        met *m = selecaoCorrigida(vetor, tamanho);
        
        tempo_total += ((double)(clock() - t_inicio) / CLOCKS_PER_SEC) * 1000.0;
        
        if (i == 0) {
            if (verificaSeEstaOrdenado(vetor, tamanho) == 1) {
                printf("[Quase Ordenado]   Status: SUCESSO\n");
            } else {
                printf("[Quase Ordenado]   Status: FALHA\n");
            }
            imprimeUltimos(vetor, tamanho);
        }
        
        comp_total += m->comparacoes; 
        mov_total += m->movimentacoes;
        
        liberaVetor(vetor); 
        liberaMetricas(m);
    }
    printf("                   Tempo Medio: %.3f ms | Comp: %lld | Mov: %lld\n", 
           tempo_total / 30.0, comp_total / 30, mov_total / 30);
}

int main() {
    // Array com os cenários
    int tamanhos[] = {1000, 10000, 100000, 1000000, 10000000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    
    printf("Iniciando Benchmarking de Selecao Corrigida...\n");
    
    for (int i = 0; i < num_tamanhos; i++) {
        avaliaSelecao(tamanhos[i]);
    }
    
    return 0;
}
