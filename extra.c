#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extra.h"

#ifdef __linux__
    #include <unistd.h>
#elif _WIN32
    #include <windows.h>
#endif

// Função apenas para limpar o terminal e deixa-lo mais organizado
void clear_terminal() {
    #ifdef __linux__
        __attribute__((unused)) int result = system("clear");
    #elif _WIN32
        __attribute__((unused)) int result = system("cls");
    #endif
}

//Transforma o valor de 8bits para um caracteres
char byte_to_char(unsigned char byte){
    char letra = (char) byte;
    return letra;
}

// Função para pegar o nome do arquivo
void get_file_name(FILE* fp, fat16_file* file, unsigned int highlighter) {
    int aux_char;
    fseek(fp, highlighter, SEEK_SET);
    for(int i = 0; i < 8; i++) {
        aux_char = fgetc(fp);
        file->name[i] = byte_to_char(aux_char);
    }
}

// Função para pegar a extensão do arquivo
void get_file_extension(FILE* fp, fat16_file* file, unsigned int highlighter) {
    int aux_char;
    fseek(fp, highlighter + 8, SEEK_SET);
    for(int i = 0; i < 3; i++) {
        aux_char = fgetc(fp);
        file->extension[i] = byte_to_char(aux_char);
    }
}

// Função para pegar o primeiro cluster do arquivo
void get_file_first_cluster(FILE* fp, fat16_file* file, unsigned int highlighter) {
    fseek(fp, highlighter + 26, SEEK_SET);
    file->first_cluster_b = fgetc(fp);
    file->first_cluster = ftell(fp);
}

// Função para pegar o tamanho do arquivo
void get_file_size(FILE* fp, fat16_file* file, unsigned int highlighter) {
    fseek(fp, highlighter + 28, SEEK_SET);
    fread(&file->size, 4, 1, fp);
}

// Função para imprimir as informações do arquivo
void print_file_info(fat16_file file, unsigned int highlighter_type) {
    printf("Nome: ");
    for(int i = 0; i < 8; i++) {
        if(file.name[i] != ' ')
            printf("%c", file.name[i]);
    }

    printf("\nExtensão: ");
    for(int i = 0; i < 3; i++) {
        if(file.extension[i] != ' ')
            printf("%c", file.extension[i]);
    }
    
    if(highlighter_type == 32)
        printf("\nTipo: Arquivo\n");
    else if(highlighter_type == 16)
        printf("\nTipo: Diretório\n");

    printf("Primeiro cluster: %ld (0x%lx) com valor %d\n", file.first_cluster, file.first_cluster, file.first_cluster_b);
    printf("Tamanho: %ld bytes\n", file.size);
}

// Função para imprimir o conteúdo do arquivo
void print_file_content(FILE *fp, unsigned int file_start, long int size){

    unsigned int file_search = 0;
    unsigned char* vet = malloc(sizeof(unsigned char) * size);
    int j = 0;

    while(size > 0) {
        fseek(fp, file_start, SEEK_SET);
        fread(&file_search, 1, 1, fp);
        
        vet[j] = file_search;
        file_start++;
        size--;
        j++;
    }

    for(int i = 0; i < j; i++)
        printf("%c", vet[i]);

    free(vet); // Libera a memória alocada para o vetor
}

// Imprime o contador de arquivos
void print_cont(int excluded, int long_file_name, int standard_83, int directory, int cont){
    printf("Contador de arquivos:\n\n");
	printf("Arquivos Excluidos: %d\n", excluded);
	printf("Diretorios: %d\n", directory);
	printf("Arquivos Standard 8.3: %d\n", standard_83);
	printf("Arquivos Long File Name: %d\n", long_file_name);
	printf("Total de arquivos: %d\n\n", cont - 1);
	printf("---------------------------------------------------------------------------\n\n");
}