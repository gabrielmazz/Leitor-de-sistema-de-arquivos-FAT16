//Estrutura da fat16
typedef struct fat_BS{
	unsigned char 		bootjmp[3];				//Define o código de inicialização da unidade	
	unsigned char 		oem_name[8];			//Define o nome do fabricante do software que formatou a unidade
	unsigned short 	    bytes_per_sector;		//Define o número de bytes em cada setor
	unsigned char		sectors_per_cluster;	//Define o número de setores em cada cluster
	unsigned short		reserved_sector_count;	//Define o número de setores reservados no início da unidade
	unsigned char		table_count;			//Define o número de FATs na unidade
	unsigned short		root_entry_count;		//Define o número de entradas na raiz da unidade
	unsigned short		total_sectors_16;		//Define o número total de setores em unidades com até 32.768 setores
	unsigned char		media_type;				//Define o tipo de mídia
	unsigned short		table_size_16;			//Define o tamanho da FAT em setores em unidades com até 32.768 setores
	unsigned short		sectors_per_track;		//Define o número de setores por trilha na unidade		
	unsigned short		head_side_count;		//Define o número de cabeças de leitura/gravação na unidade
	unsigned int 		hidden_sector_count;	//Define o número de setores ocultos na unidade
	unsigned int 		total_sectors_32;		//Define o número total de setores em unidades com mais de 32.768 setores
 
	//this will be cast to it's specific type once the driver actually knows what type of FAT this is.
	unsigned char		extended_section[54];
 
}__attribute__((packed)) fat_BS_t;


//Estrutura usada no arquivo
typedef struct arquivos{
	int indice;
	unsigned char name[8];
	unsigned char extension[3];
	unsigned char type[10];
	unsigned long int first_cluster;
	unsigned int first_cluster_b;
	unsigned int size;
}fat16_file;

//Função extras
void clear_terminal(void);

//Função para transformar um byte em um caractere
char byte_to_char(unsigned char byte);

//Funções para pegar os valores do arquivo
void get_file_name(FILE* fp, fat16_file* file, unsigned int highlighter);
void get_file_extension(FILE* fp, fat16_file* file, unsigned int highlighter);
void get_file_first_cluster(FILE* fp, fat16_file* file, unsigned int highlighter);
void get_file_size(FILE* fp, fat16_file* file, unsigned int highlighter);

//Funções para printar todos as arquivos
void print_file_info(fat16_file file, unsigned int highlighter_type);
void print_file_content(FILE *fp, unsigned int file_start, int size);
