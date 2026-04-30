#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ordenacao.h"

// ===================================================================
// QUICKSORT PRO (Corrigindo o Stack Overflow)
// ===================================================================

int particionaPRO(r *vet, int inicio, int fim, met *m) {
    
    // Solução da versão PRO: pegar o pivô do meio do vetor
    int meio = inicio + (fim - inicio) / 2;
    
    // Agora a gente joga esse pivô do meio lá para a primeira posição
    r aux_pivo = vet[inicio];
    vet[inicio] = vet[meio];
    vet[meio] = aux_pivo;
    m->movimentacoes += 3;

    int pivo = vet[inicio].user_id;
    int i = inicio + 1;

    // Percorre o vetor inteiro a partir do pivô
    for (int j = inicio + 1; j <= fim; j++) {
        m->comparacoes++;
        
        // Se achou alguém menor que o pivô, joga para o lado esquerdo
        if (vet[j].user_id <= pivo) {
            r tmp = vet[i];
            vet[i] = vet[j];
            vet[j] = tmp;
            m->movimentacoes += 3;
            i++;
        }
    }
    
    // Agora pegamos o pivô que estava no início e colocamos ele no lugar certo
    r tmp = vet[inicio];
    vet[inicio] = vet[i - 1];
    vet[i - 1] = tmp;
    m->movimentacoes += 3;

    return i - 1;
}

void quickSortPRO(r *vet, int inicio, int fim, met *m) {
    // Condição de parada da recursão
    // Se o pedaço do vetor tiver 1 ou 0 elementos, não precisa fazer mais nada
    if (inicio >= fim) {
        return;
    }
    
    // Acha a posição do pivô e depois manda ordenar as duas metades que sobraram
    int p = particionaPRO(vet, inicio, fim, m);
    quickSortPRO(vet, inicio, p - 1, m);
    quickSortPRO(vet, p + 1, fim, m);
}

// ===================================================================
// FUNÇÕES AUXILIARES PARA TESTAR O CÓDIGO
// ===================================================================

int verificaEstabilidade(r *vet, int tamanho) {
    
    // Checa se o algoritmo bagunçou a ordem de chegada das pessoas que têm o mesmo ID.
    for (int i = 0; i < tamanho - 1; i++) {
        if (vet[i].user_id == vet[i+1].user_id && vet[i].chegada > vet[i+1].chegada) {
            return 0;
        }
    }
    return 1;
}

int verificaSeEstaOrdenado(r *vet, int tamanho) {
    // Varre o vetor procurando algum número maior antes de um menor.
    // Se achar, o algoritmo falhou.
    for (int i = 0; i < tamanho - 1; i++) {
        if (vet[i].user_id > vet[i+1].user_id) {
            return 0;
        }
    }
    return 1;
}

void imprimePrimeiros(r *vet, int tamanho) {
    printf("                   Visuais (5 primeiros): ");
    
    int limite;
    
    // Se o vetor for muito pequeno (menos de 5 posições), 
    // a gente ajusta o limite para não tentar ler memória que não existe.
    if (tamanho < 5) {
        limite = tamanho;
    } else {
        limite = 5;
    }
    
    for (int i = 0; i < limite; i++) {
        printf("%d ", vet[i].user_id);
    }
    printf("\n");
}

void avaliaQuickSortPRO(int tamanho) {
    printf("\n==================================================\n");
    printf("   QUICKSORT PRO (CORRIGIDO) - Avaliacao para N = %d\n", tamanho);
    printf("==================================================\n");

    clock_t inicio_timer, fim_timer; 
    double tempo_total; 
    long long int comp_total, mov_total;  // Variável "long long" para comparações muito grandes 

    // Teste 1: Vetores Aleatórios (Calculando a média de 30)
    tempo_total = 0; comp_total = 0; mov_total = 0;
    
    for (int i = 0; i < 30; i++) {
        r *vetor = geraAleatorios(tamanho, i + 1); 
        met *m = alocaMetricas();
        
        // Liga o cronômetro, ordena, e desliga
        inicio_timer = clock(); 
        quickSortPRO(vetor, 0, tamanho - 1, m); 
        fim_timer = clock();
        
        // Print só no primeiro vetor gerado pra não poluir o terminal
        if (i == 0) {
            if (verificaEstabilidade(vetor, tamanho) == 1) {
                printf("[Estabilidade]     Status: ESTAVEL (ALERTA!)\n");
            } else {
                printf("[Estabilidade]     Status: INSTAVEL (Esperado)\n");
            }

            if (verificaSeEstaOrdenado(vetor, tamanho) == 1) {
                printf("[Aleatorio]        Status: SUCESSO\n");
            } else {
                printf("[Aleatorio]        Status: FALHA\n");
            }
            imprimePrimeiros(vetor, tamanho);
        }
        
        tempo_total += ((double)(fim_timer - inicio_timer) / CLOCKS_PER_SEC) * 1000.0;
        comp_total += m->comparacoes; 
        mov_total += m->movimentacoes;
        
        liberaVetor(vetor); 
        liberaMetricas(m);
    }
    
    // Mostra as médias calculadas
    printf("                   Tempo Medio: %.3f ms | Comp: %lld | Mov: %lld\n", 
           tempo_total / 30.0, comp_total / 30, mov_total / 30);

    // Teste 2: Vetor Crescente (Antes acontecia o Stack Overflow)
    r *vet_cres = geraOrdenados(tamanho, 42); 
    met *m_cres = alocaMetricas();
    
    inicio_timer = clock(); 
    quickSortPRO(vet_cres, 0, tamanho - 1, m_cres); 
    fim_timer = clock();
    
    if (verificaSeEstaOrdenado(vet_cres, tamanho) == 1) {
        printf("[Crescente]        Status: SUCESSO\n");
    } else {
        printf("[Crescente]        Status: FALHA\n");
    }
    imprimePrimeiros(vet_cres, tamanho);
    printf("                   Tempo:       %.3f ms | Comp: %lld | Mov: %lld\n", 
           ((double)(fim_timer - inicio_timer) / CLOCKS_PER_SEC) * 1000.0, 
           m_cres->comparacoes, m_cres->movimentacoes);
           
    liberaVetor(vet_cres); 
    liberaMetricas(m_cres);

    // Teste 3: Vetor Decrescente
    r *vet_decres = geraDecrescente(tamanho, 42); 
    met *m_decres = alocaMetricas();
    
    inicio_timer = clock(); 
    quickSortPRO(vet_decres, 0, tamanho - 1, m_decres); 
    fim_timer = clock();
    
    if (verificaSeEstaOrdenado(vet_decres, tamanho) == 1) {
        printf("[Decrescente]      Status: SUCESSO\n");
    } else {
        printf("[Decrescente]      Status: FALHA\n");
    }
    imprimePrimeiros(vet_decres, tamanho);
    printf("                   Tempo:       %.3f ms | Comp: %lld | Mov: %lld\n", 
           ((double)(fim_timer - inicio_timer) / CLOCKS_PER_SEC) * 1000.0, 
           m_decres->comparacoes, m_decres->movimentacoes);
           
    liberaVetor(vet_decres); 
    liberaMetricas(m_decres);

    // Teste 4: Vetor Quase Ordenado (Média de 30 rodadas)
    tempo_total = 0; comp_total = 0; mov_total = 0;
    
    for (int i = 0; i < 30; i++) {
        r *vetor = geraQuaseOrdenados(tamanho, i + 1, 10); 
        met *m = alocaMetricas();
        
        clock_t t_inicio = clock(); 
        quickSortPRO(vetor, 0, tamanho - 1, m);
        
        tempo_total += ((double)(clock() - t_inicio) / CLOCKS_PER_SEC) * 1000.0;
        
        if (i == 0) {
            if (verificaSeEstaOrdenado(vetor, tamanho) == 1) {
                printf("[Quase Ordenado]   Status: SUCESSO\n");
            } else {
                printf("[Quase Ordenado]   Status: FALHA\n");
            }
            imprimePrimeiros(vetor, tamanho);
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
    
    printf("Iniciando Benchmarking do QuickSort PRO...\n");
    
    for (int i = 0; i < num_tamanhos; i++) {
        avaliaQuickSortPRO(tamanhos[i]);
    }
    
    return 0;
}