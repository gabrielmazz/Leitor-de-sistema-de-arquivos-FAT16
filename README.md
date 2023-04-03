# # Leitor de sistema de arquivos FAT16

### Trabalho de Sistemas Operacionais
>Colaborador: *Gabriel Mazzuco* ([Github Profile](https://github.com/gabrielmazz))
- 3ª ano de Ciências da Computação
---

## Introdução

Este é um programa em C que lê o boot record de uma imagem de disco e imprime algumas informações relevantes sobre o sistema de arquivos FAT16, como bytes por setor, setores por cluster, setores reservados, número de FATs, número de setores por FAT, número total de setores e número de entradas no diretório raiz. Em seguida, o programa lê o diretório raiz e imprime o nome, extensão, tamanho e primeiro cluster de cada arquivo, bem como algumas informações adicionais se o arquivo for um long file name ou um diretório.

## Desenvolvimento do código

O código foi desenvolvido em C e faz uso das bibliotecas padrão `stdio.h`, `stdlib.h` e `string.h`. Além disso, utiliza uma biblioteca personalizada `extra.h`, que contém algumas funções auxiliares para a leitura de informações do arquivo FAT16. Dentro da `extra.h` estão declarados as estruturas que foram usadas no programa 

```c
typedef struct fat_BS{
    unsigned char       bootjmp[3];       
    unsigned char       oem_name[8];
    unsigned short      bytes_per_sector;    
    unsigned char       sectors_per_cluster; 
    unsigned short      reserved_sector_count;  
    unsigned char       table_count;            
    unsigned short      root_entry_count;       
    unsigned short      total_sectors_16;       
    unsigned char       media_type;             
    unsigned short      table_size_16;          
    unsigned short      sectors_per_track;       
    unsigned short      head_side_count;        
    unsigned int        hidden_sector_count;   
    unsigned int        total_sectors_32;      
    unsigned char       extended_section[54];
}__attribute__((packed)) fat_BS_t;
```

```c
typedef struct arquivos{ 
    unsigned char name[8];
    unsigned char extension[3];
    unsigned char type[10];
    unsigned long int first_cluster;
    unsigned int first_cluster_b;
    unsigned int size;
}fat16_file;
```

O programa começa abrindo um arquivo de imagem de disco FAT16, lê o boot record e armazena suas informações em uma estrutura `fat_BS_t`. Em seguida, calcula o início do diretório raiz, o tamanho do diretório raiz e o início da área de dados. A partir daí, lê o diretório raiz e imprime as informações de cada arquivo.

```c
//Calcula o inicio do Root Diretory
    unsigned int start_rootDir = boot_record.reserved_sector_count + (boot_record.table_size_16 * boot_record.table_count);
    unsigned int root_dir_size = start_rootDir * boot_record.bytes_per_sector;
    unsigned int data_start = root_dir_size + (boot_record.root_entry_count * 32);
```

```c
//Recebe as informações do arquivo
get_file_name(fp, &file, highlighter);
get_file_extension(fp, &file, highlighter);
get_file_first_cluster(fp, &file, highlighter);
get_file_size(fp, &file, highlighter);
```

```c
//Printa as informações do arquivo
print_file_info(file, highlighter_type);
```

```c
//Printa o conteúdo do arquivos
print_file_content(fp, file_start, size);
```

## Como executá-lo

### Com makefile

Para compilar o programa usando o `makefile`, basta abrir o terminal na pasta do projeto e digitar o comando `make`. Isso compilará o código e gerará um executável chamado `main`. Em seguida, para executar o programa, digite `make run`.

```bash
make && make run
```

### Sem makefile

Para compilar o programa sem usar o `makefile`, basta abrir o terminal na pasta do projeto e digitar o seguinte comando:

```bash
gcc main.c extra.c -o main
```

Isso compilará o código e gerará um executável chamado `fat16_reader`. Em seguida, para executar o programa, digite `./main.

```bash
./main
```

## Conclusão

Este programa é uma ferramenta útil para explorar o sistema de arquivos FAT16 e entender como ele é estruturado. Ele pode ser facilmente modificado para adicionar mais funcionalidades, como a leitura de arquivos individuais ou a edição de arquivos existentes.