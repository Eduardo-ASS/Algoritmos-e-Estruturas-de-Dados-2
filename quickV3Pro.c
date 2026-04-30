#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ordenacao.h"

// ===================================================================
// QUICKSORT PRO (Mediana de Três)
// ===================================================================

// Troca duas posições do vetor e conta as movimentações
void trocaPro(r *a, r *b, met *m) {
    r tmp = *a;
    *a = *b;
    *b = tmp;
    m->movimentacoes += 2;
}

// A técnica da Mediana de Três: garante que o pior caso seja evitado
int medianaDeTresPro(r *vet, int inicio, int fim, met *m) {
    int meio = inicio + (fim - inicio) / 2;

    // Coloca os três elementos (início, meio e fim) em ordem crescente
    m->comparacoes++;
    if (vet[inicio].user_id > vet[meio].user_id) {
        trocaPro(&vet[inicio], &vet[meio], m);
    }

    m->comparacoes++;
    if (vet[inicio].user_id > vet[fim].user_id) {
        trocaPro(&vet[inicio], &vet[fim], m);
    }

    m->comparacoes++;
    if (vet[meio].user_id > vet[fim].user_id) {
        trocaPro(&vet[meio], &vet[fim], m);
    }

    // O elemento do meio agora é a nossa mediana (o pivô ideal).
    // Escondemos ele na penúltima posição para particionar o resto.
    trocaPro(&vet[meio], &vet[fim - 1], m);

    return vet[fim - 1].user_id;
}

int particionaPRO(r *vet, int inicio, int fim, met *m) {
    // Escolhe o pivô usando a função da Mediana de Três
    int pivo = medianaDeTresPro(vet, inicio, fim, m);
    
    // Nossos ponteiros de varredura
    int i = inicio;
    int j = fim - 1;

    while (1) {
        // i avança procurando alguém maior que o pivô
        do {
            i++;
            m->comparacoes++;
        } while (vet[i].user_id < pivo);

        // j recua procurando alguém menor que o pivô
        do {
            j--;
            m->comparacoes++;
        } while (vet[j].user_id > pivo);

        // Se os ponteiros se cruzarem, acabou a partição
        if (i >= j) {
            break;
        }

        // Se não cruzou, troca os elementos que estão do lado errado
        trocaPro(&vet[i], &vet[j], m);
    }

    // Devolve o pivô para o lugar definitivo dele
    trocaPro(&vet[i], &vet[fim - 1], m);

    return i;
}

void quickSortPRO(r *vet, int inicio, int fim, met *m) {
    // Condição de parada da recursão
    if (inicio >= fim) {
        return;
    }

    // A Mediana de Três exige pelo menos 3 elementos
    // Se o pedaço tiver exatamente 2 elementos, ordena direto e encerra.
    if (fim - inicio == 1) {
        m->comparacoes++;
        if (vet[inicio].user_id > vet[fim].user_id) {
            trocaPro(&vet[inicio], &vet[fim], m);
        }
        return;
    }
    
    // Se tiver 3 ou mais elementos, particiona normalmente
    int p = particionaPRO(vet, inicio, fim, m);
    quickSortPRO(vet, inicio, p - 1, m);
    quickSortPRO(vet, p + 1, fim, m);
}

// ===================================================================
// FUNÇÕES AUXILIARES PARA TESTAR O CÓDIGO
// ===================================================================

int verificaEstabilidade(r *vet, int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        if (vet[i].user_id == vet[i+1].user_id && vet[i].chegada > vet[i+1].chegada) {
            return 0;
        }
    }
    return 1;
}

int verificaSeEstaOrdenado(r *vet, int tamanho) {
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
    long long int comp_total, mov_total; 

    // Teste 1: Vetores Aleatórios (Calculando a média de 30)
    tempo_total = 0; comp_total = 0; mov_total = 0;
    
    for (int i = 0; i < 30; i++) {
        r *vetor = geraAleatorios(tamanho, i + 1); 
        met *m = alocaMetricas();
        
        inicio_timer = clock(); 
        quickSortPRO(vetor, 0, tamanho - 1, m); 
        fim_timer = clock();
        
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
    
    printf("                   Tempo Medio: %.3f ms | Comp: %lld | Mov: %lld\n", 
           tempo_total / 30.0, comp_total / 30, mov_total / 30);

    // Teste 2: Vetor Crescente
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
    int tamanhos[] = {1000, 10000, 100000, 1000000, 10000000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    
    printf("Iniciando Benchmarking do QuickSort PRO...\n");
    
    for (int i = 0; i < num_tamanhos; i++) {
        avaliaQuickSortPRO(tamanhos[i]);
    }
    
    return 0;
}
