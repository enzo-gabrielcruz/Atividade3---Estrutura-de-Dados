#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_MAX_SIZE 801 // máximo de caracteres presentes em uma linha

typedef struct arvB{
    int valor;
    struct arvB * esq;
    struct arvB * dir;
    int altura;
    int usado; // 0 para elementos que já foram buscados e 1 para elementos livres para busca;
}arvB;

arvB * criar(int num){
    arvB * novo = malloc(sizeof(arvB));
    novo->valor = num;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->altura = 0;
    novo->usado = 1;
    return novo;
}

void adcionar(arvB * arv, int num){
    if (num < arv->valor){
        if(arv->esq == NULL){
             arv->esq = criar(num);
             arv->esq->altura = arv->altura + 1; 
        }
           
        else
            adcionar(arv->esq,num);
    }
    else{
        if(arv->dir == NULL){
            arv->dir = criar(num);
            arv->dir->altura = arv->altura + 1;
        }
        else
            adcionar(arv->dir,num);
    }
}

int contar_altura (arvB * arv){
   if (arv == NULL)
        return -1; // altura de um nó nulo é -1 (folha + 1 = 0)

    int alt_esq = contar_altura(arv->esq);
    int alt_dir = contar_altura(arv->dir);

    arv->altura = (alt_esq > alt_dir ? alt_esq : alt_dir) + 1;

    return arv->altura;
} 

int altura_chaveMax(arvB * arv){
    while (arv->dir != NULL) 
        arv = arv->dir;
    return arv->altura;
}

int valor_maximo (arvB * arv){
    while (arv->dir != NULL) 
        arv = arv->dir;
    return arv->valor;
}


int buscar_altura_de_um_no(arvB* raiz, int valor_procurado) {
    arvB* atual = raiz;
    int altura = 0;
    while (atual != NULL) {
        if (valor_procurado == atual->valor && atual->usado == 1) {
            atual->usado = 0;
            return altura; // Achou! Retorna a altura atual.
        }
        if (valor_procurado < atual->valor) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
        altura++; // Incrementa a altura a cada passo.
    }
    return -1;
}


arvB* buscar_no_maximo(arvB* raiz) {
    if (raiz == NULL) return NULL;
    arvB* atual = raiz;
    while (atual->dir != NULL) {
        atual = atual->dir;
    }
    return atual;
}

arvB* buscar_pred_do_max(arvB* raiz, arvB* no_max) {
    // Caso 1: Se o nó máximo tiver uma subárvore esquerda, 
    // o predecessor é o maior de lá.
    if (no_max->esq != NULL) {
        arvB* pred = no_max->esq;
        while (pred->dir != NULL) {
            pred = pred->dir;
        }
        return pred;
    }
    // Caso 2: Se não tiver subárvore esquerda, o predecessor é o pai.
    // Precisamos encontrar o pai do nó máximo.
    else {
        arvB* pred = NULL;
        arvB* atual = raiz;
        while (atual != no_max) {
            // Se o valor do nó máximo for maior, o nó atual é um
            // possível pai (predecessor), e andamos para a direita.
            if (no_max->valor >= atual->valor) { // Usar >= por causa dos duplicados
                pred = atual;
                atual = atual->dir;
            } else {
                atual = atual->esq;
            }
        }
        return pred;
    }
}

void liberar_arvore(arvB *arv) {
    if (arv == NULL) return;
    liberar_arvore(arv->esq);
    liberar_arvore(arv->dir);
    free(arv);
}

void remover_ultima_linha_em_branco(const char *file_out) { // função utilizada para apagar a última linha em branco
    FILE *arquivo = fopen(file_out, "rb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo para leitura");
        return;
    }

    // Descobre tamanho do arquivo
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    rewind(arquivo);

    // Lê todo o conteúdo
    char *conteudo = malloc(tamanho + 1);
    if (!conteudo) {
        fclose(arquivo);
        fprintf(stderr, "Erro de alocação\n");
        return;
    }

    fread(conteudo, 1, tamanho, arquivo);
    conteudo[tamanho] = '\0';
    fclose(arquivo);

    // Remove último \n se existir
    if (tamanho > 0 && conteudo[tamanho - 1] == '\n') {
        conteudo[tamanho - 1] = '\0';
        tamanho--;
    }

    // Regrava o arquivo sem a última quebra de linha
    arquivo = fopen(file_out, "wb");
    if (!arquivo) {
        perror("Erro ao abrir arquivo para escrita");
        free(conteudo);
        return;
    }

    fwrite(conteudo, 1, tamanho, arquivo);
    fclose(arquivo);
    free(conteudo);
}

int main (){
    FILE *file_in = fopen("L2Q1.in", "r");
    FILE *file_out = fopen("L2Q1.out", "w");
        if (!file_in) {
            perror("Erro ao abrir L2Q1.in");
            return 1;
        }
        if (!file_out) {
            perror("Erro ao abrir L2Q1.out");
            return 1;
        }

    char line[LINE_MAX_SIZE];
    
    while(fgets(line,LINE_MAX_SIZE,file_in)){
        //este fgets lê uma linha completa do arquivo in e armazena em um ponteiro e faz isso até não existir mais linhas a serem lidas
            char *apontador = line; // percorre toda a linha, caractere por caractere
            

            if (*apontador == '\0') continue;  // linha vazia → pula

            char *token = strtok(line, " \n");// quebra a linha em pedaços
            
            int tam_vetor = 0;
            int *vetor = NULL;
            arvB * raiz = NULL;
        
            while (token){
                int num = atoi(token);
                vetor = realloc(vetor, (tam_vetor + 1) * sizeof(int));
                vetor[tam_vetor] = num;  
                tam_vetor++;

                token = strtok(NULL, " \n");
            }

            for(int i = 0; i < tam_vetor; i++){ // preenchendo a árvore com os números guardados no vetor
                if(i == 0){
                    raiz = criar(vetor[i]);
                    raiz->altura = 0;
                }
                else
                    adcionar(raiz,vetor[i]);
            } 
           
            arvB* no_max = buscar_no_maximo(raiz);
            int max_value = no_max ? no_max->valor : 0; // Pega o valor

            arvB* pred_node = NULL;
            if (no_max != raiz) { // O nó raiz não tem predecessor neste contexto
            pred_node = buscar_pred_do_max(raiz, no_max);
            }

            // IMPRIMINDO AS ALTURAS
            for (int i = 0; i < tam_vetor; i++) {
                int altura_do_no_i = buscar_altura_de_um_no(raiz, vetor[i]);
                fprintf(file_out, "%d ", altura_do_no_i);                
            }
            
        fprintf(file_out, "max %d ", max_value); //MAX
        fprintf(file_out, "alt %d ", no_max->altura); //Altura chave máxima

        if (pred_node)
            fprintf(file_out, "pred %d\n", pred_node->valor); // PRED
        else
            fprintf(file_out, "pred NaN\n"); // PRED

            // liberando espaço de memória
            liberar_arvore(raiz);
            free(vetor);
    }

    fclose(file_in);
    fclose(file_out);
    remover_ultima_linha_em_branco("L2Q1.out");
}


