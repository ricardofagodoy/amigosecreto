#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>
#include <time.h>

#define ARQUIVO_NOMES "nomes.txt"
#define TAMANHO_NOME 30
#define EXTENSAO_ARQUIVO ".txt"

void inicializaArrayDinamico(char*** vet, int tamanho);
void destroiArrayDinamico(char*** vet, int tamanho);
int executaSorteio(char** vet, int tamanho, char* pasta);
int geraInteiroAleatorio(int limite);
int geraRegistro(char* nome, char* secreto, char* pasta);
void removeDoVetor(int *index, int n, int tamanho);

int main() {
    
    srand((unsigned)time(NULL));

    char **vetNomes;
    char nomeSorteio[TAMANHO_NOME];
    char nome[TAMANHO_NOME];
    char escolha;

    /* Inicio do programa */
    printf("##### Programa sorteador de amigo secreto - RG #####\n\n");

    FILE *nomes = fopen(ARQUIVO_NOMES, "r");

    if(nomes) {

        int quantosNomes = 0;

        printf("Participantes:\n\n");

        while(fgets(nome, TAMANHO_NOME, nomes) != NULL) {
            quantosNomes++;
            printf("%s", nome);
        }

        printf("\n\nForam encontrados %d nomes de participantes.\nProsseguir com sorteio? (S ou N): \n\n", quantosNomes);

        scanf("%c", &escolha);

        switch(escolha) {

            case 's':
            case 'S':
            case 'y':
            case 'Y':
                printf("\nDigite um nome para esse sorteio (nome de pasta): ");
                fflush(stdin);
                fgets(nomeSorteio, sizeof(nomeSorteio), stdin);
                
                nomeSorteio[strlen(nomeSorteio)-1] = '\0';
                CreateDirectory(nomeSorteio, NULL);
                
                printf("\nIniciando sorteio identificado como \"%s\" ...\n\n", nomeSorteio);
                
                inicializaArrayDinamico(&vetNomes, quantosNomes);
                
                if(!vetNomes) {
                    printf("\n\nMemoria insuficiente!\n\n");
                    system("PAUSE");
                    exit(1);
                }
                
                rewind(nomes);
                quantosNomes = 0;
                
                while(fgets(nome, TAMANHO_NOME, nomes) != NULL) {
                   strncpy(vetNomes[quantosNomes], nome, TAMANHO_NOME);
                   
                   if(vetNomes[quantosNomes][strlen(vetNomes[quantosNomes])-1] == '\n')
                      vetNomes[quantosNomes][strlen(vetNomes[quantosNomes])-1] = '\0';
                      
                   quantosNomes++;
                }   

                if(quantosNomes == executaSorteio(vetNomes, quantosNomes, nomeSorteio))
                   printf("\nSorteio finalizado com sucesso!!\nPasta \"%s\" criada com os resultados.\n\n", nomeSorteio);
                else
                   printf("\nSorteio finalizado com erros!\nFavor, descartar a pasta gerada e repetir o programa.\n\n");
                
                destroiArrayDinamico(&vetNomes, quantosNomes);
                
            break;

            default:
                printf("\nSorteio cancelado!\n");
            break;
        }

        fclose(nomes);

    } else {
        printf("Crie um arquivo nomes.txt neste mesmo diretorio, com um nome em cada linha...\n\n");
    }

    system("PAUSE");
    return 0;
}

/****** Funções ******/


int executaSorteio(char** vet, int tamanho, char* pasta) {
    
    int conter = 0, aleatorio, secreto, i, totalAmigos;
    
    int* index = (int*) malloc(sizeof(int) * tamanho);
    
    for(i = 0; i < tamanho; i++)
       index[i] = i;
    
    totalAmigos = tamanho;
    
    for(i = 0; i < totalAmigos; i++) {
   
       do {
           aleatorio = geraInteiroAleatorio(tamanho);
           secreto = index[aleatorio];
       } while(secreto == i);    
            
      conter += geraRegistro(vet[i], vet[secreto], pasta);   
       
      removeDoVetor(index, aleatorio, tamanho); 
       
      tamanho--;                               
    }
    
    free(index);
    
    return conter;
}

int geraRegistro(char* nome, char* secreto, char* pasta) {
 
    char nomeArquivo[TAMANHO_NOME + 4];
    strncpy(nomeArquivo, pasta, TAMANHO_NOME);
    strncat(nomeArquivo, "/", TAMANHO_NOME);
    strncat(nomeArquivo, nome, TAMANHO_NOME);
    strncat(nomeArquivo, EXTENSAO_ARQUIVO, sizeof(EXTENSAO_ARQUIVO));

    FILE *file = fopen(nomeArquivo, "w");
    
    if(file) {
             
       fprintf(file, "%s, voce tirou %s", nome, secreto);
    
       fclose(file);
       
       return 1;
    }
    
    return 0;
}

void removeDoVetor(int *index, int n, int tamanho) {
     
     int i;
     
     for(i = n; i < tamanho - 1; i++)
        index[i] = index[i+1];
}

int geraInteiroAleatorio(int limite) {
    return (int) rand()%limite;
}

void destroiArrayDinamico(char*** vet, int tamanho) {
       
       int i;
       
       for(i = 0; i < tamanho; i++)
          free((*vet)[i]);  
          
       free(*vet);
}

void inicializaArrayDinamico(char*** vet, int tamanho) {
       
       int i;
       
       *vet = (char**) malloc(sizeof(char*) * tamanho);
       
        if(*vet) {
          for(i = 0; i < tamanho; i++) {
             (*vet)[i] = (char*) malloc(sizeof(char) * TAMANHO_NOME);
             
             if(!(*vet)[i]) {
                *vet = NULL;
                break;
             }
             
          }
       }
}
