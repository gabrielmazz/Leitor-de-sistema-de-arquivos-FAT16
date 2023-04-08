#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extra.h"


int main(void){

	//Arquivo fat
    FILE *fp;
    fat_BS_t  boot_record;
	fat16_file file;

	memset(&file, 0, sizeof(file));

	//Opções de arquivos na aba de menu
	char file_name[35];
	int opcao;

	clear_terminal();

	//Menu para escolher o arquivo que deseja ler
	printf("Determine o arquivo que deseja ler: \n");
	printf("1 - fat16_1sectorpercluster.img \n");
	printf("2 - fat16_4sectorpercluster.img \n");
	printf("Escolha: "); 
	scanf("%d", &opcao);

	if (opcao != 1 && opcao != 2) {
		printf("Opção inválida\n");
		EXIT_SUCCESS;
	}

	switch(opcao){
		case 1:
			strcpy(file_name, "fat16_1sectorpercluster.img");
			break;
		
		case 2:
			strcpy(file_name, "fat16_4sectorpercluster.img");
			break;
		
		default:
			EXIT_SUCCESS;
	}

	clear_terminal();

	//Abre o arquivo escolhido
    fp = fopen(file_name, "rb");

	//Move o ponteiro para o inicio do arquivo no byte 0
    fseek(fp, 0, SEEK_SET);

	size_t read_elements = fread(&boot_record, sizeof(fat_BS_t), 1, fp);

	if (read_elements != 1) {
		printf("Erro na leitura do arquivo\n");
		exit(EXIT_FAILURE);
	}

	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");

	//Imprime qual arquivo foi aberto
	printf("Arquivo selecionado: %s\n\n", file_name);

	//Imprime as informações do boot record
	printf("Informações sobre o Boot Record\n");
	printf("Bytes por setor: %d\n", boot_record.bytes_per_sector);
	printf("Setores por cluster: %d\n", boot_record.sectors_per_cluster);

	printf("Setores reservados: %d\n", boot_record.reserved_sector_count);
	printf("Número de FAT: %d\n", boot_record.table_count);
	printf("Número de setores por FAT: %d\n", boot_record.table_size_16);
	printf("Número total de setores: %d\n", boot_record.total_sectors_16);
	printf("Número de entradas no root dir: %d\n\n", boot_record.root_entry_count);

	printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");

	//Calcula o inicio do Root Diretory
	unsigned int start_rootDir = boot_record.reserved_sector_count + (boot_record.table_size_16 * boot_record.table_count);
	unsigned int root_dir = start_rootDir * boot_record.bytes_per_sector;
	unsigned int data_start = root_dir + (boot_record.root_entry_count * 32);
	unsigned int set_reserv = boot_record.reserved_sector_count * boot_record.bytes_per_sector;
	unsigned int tam_fat = boot_record.table_size_16 * boot_record.bytes_per_sector;
	unsigned int qtd_fat[boot_record.table_count];
	
	//Calcula quantas FAT tem no arquivo
	int i = 1;
	while(boot_record.table_count >= i){
		qtd_fat[i] = set_reserv + (i - 1) * tam_fat;
		i++;
	}

	printf("Inicio da Root: byte %u\n", root_dir);			//Início do diretório raiz
	printf("Início da área de dados: %u bytes\n", data_start);		//Início da área de dados
	
	i = 1;
	while(boot_record.table_count >= i){
		printf("Início da FAT %d: %u\n", i, qtd_fat[i]);	//Setores reservados
		i++;
	}

	printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n\n");

	//Move o ponteiro para o inicio da área de dados
	fseek(fp, root_dir, SEEK_SET);

	//Variável para percorrer o diretório raiz e seus arquivos
	unsigned int highlighter = root_dir;
	unsigned int highlighter_type;

	//Variável para descobrir aonde esta o arquivo
	unsigned int file_start;

	//Contador de arquivos
	int cont = 1;

	//Percorre todos os arquivos do diretório raiz
	while(1){

		//Move o ponteiro
		fseek(fp, highlighter, SEEK_SET);

		//Descobre se o arquivo não esta excluido
		highlighter_type = fgetc(fp);

		//Verifica se as entradas do diretório raiz já foram lidas
		if(highlighter_type == 0){
			printf("Todas as entradas da Root Diretory já foram lidas\n\n");
			break;

		}else if(highlighter_type == 229){	//Se o arquivo estiver excluido, pula para o próximo "E5"
			printf("Arquivo %d\n", cont);
			printf("Arquivo excluido\n\n---------------------------------------------------------------------------\n\n");
			highlighter += 32;
			cont++;

		}else{
			
			//Anda o ponteiro para descobrir o tipo de arquivo
			highlighter += 11;

			//Move o ponteiro para o tipo de arquivo
			fseek(fp, highlighter, SEEK_SET);

			//Descobre o tipo de arquivo
			highlighter_type = fgetc(fp);

			if(highlighter_type == 0)
				break;

			printf("Arquivo %d:\n", cont);

			//Verifica se é long file name com o valor de 15 bytes
			if(highlighter_type == 15){

				printf("Long File Name\n\n---------------------------------------------------------------------------\n\n");

				//Move o ponteiro para o primeiro ponto e depois passa para o próximo possivel arquivo
				highlighter -= 11;
				highlighter += 32;

				//Passa o contador de arquivos
				cont++;
			}

			//Verifica se é um diretório
			else if(highlighter_type == 16){

				//Pega o nome do arquivo
				highlighter -= 11;

				//Volta o ponteiro para a posição do nome do arquivo
				fseek(fp, highlighter, SEEK_SET);

				//Pega o nome do arquivo
				get_file_name(fp, &file, highlighter);
				get_file_extension(fp, &file, highlighter);
				get_file_first_cluster(fp, &file, highlighter);
				get_file_size(fp, &file, highlighter);

				//Printa as informações do arquivo
				print_file_info(file, highlighter_type);

				//Move o ponteiro para o proximo arquivo
				highlighter += 32;
				fseek(fp, highlighter, SEEK_SET);

				//Passa o contador de arquivos
				cont++;

				printf("\n---------------------------------------------------------------------------\n\n");
				
			}

			//Verifica se é um arquivo em formato 8.3
			else if(highlighter_type == 32){
				//Pega o nome do arquivo
				highlighter -= 11;

				//Volta o ponteiro para a posição do nome do arquivo
				fseek(fp, highlighter, SEEK_SET);

				//Pega o nome do arquivo
				get_file_name(fp, &file, highlighter);
				get_file_extension(fp, &file, highlighter);
				get_file_first_cluster(fp, &file, highlighter);
				get_file_size(fp, &file, highlighter);

				//Printa as informações do arquivo
				print_file_info(file, highlighter_type);

				//Mostra oque tem no arquivo
				file_start = (file.first_cluster_b - 2) * boot_record.sectors_per_cluster * boot_record.bytes_per_sector  + data_start;

				//Move o ponteiro para o inicio do arquivo
				printf("\n\nInicio do Arquivo: (%s): %d (0x%x)\n", file.name, file_start, file_start);
				fseek(fp, file_start, SEEK_SET);

				//Printa o conteúdo do arquivo
				printf("Conteúdo do arquivo:\n\n");
				print_file_content(fp, file_start, file.size);

				//Move o ponteiro para o proximo arquivo
				highlighter += 32;
				fseek(fp, highlighter, SEEK_SET);

				//Passa o contador de arquivos
				cont++;

				printf("\n\n---------------------------------------------------------------------------\n\n");

			}
		}
	
	}
    return 0;
}
