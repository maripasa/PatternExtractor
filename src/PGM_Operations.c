// PGM_Operations.c

/*************************************************************/
/* Aluno 01: Ásafe Silva Duarte Ximenes Frota                */
/* Matricula: 20231045050254                                 */
/*                                                           */
/* Aluno 02: Antonio Lucas Pereira Holanda                   */
/* Matricula: 20231045050246                                 */
/*                                                           */
/* Aluno 03: João Pedro Ibiapina Portela Neime               */
/* Matricula: 20231045050327                                 */
/*                                                           */
/* Aluno 04: Rafael Jorge Cabral Cardoso                     */
/* Matricula: 20231045050181                                 */
/*                                                           */
/* Avaliação 04: Trabalho Final                              */
/* 04.505.23 - 2023.2 - Prof. Daniel Ferreira                */
/* Compilador: gcc.exe (Rev7, Built by MSYS2 project) 13.1.0 */
/*************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../include/PGM_Operations.h"

void readPGMImage(struct pgm *pio, char *filename){

	FILE *fp;
	char ch;

	if (!(fp = fopen(filename,"r"))){
		perror("Erro.");
		exit(1);
	}

	if ( (ch = getc(fp))!='P'){
		printf("Erro: O arquivo '%s' não está no formato pgm (primeiro caractere é '%c', mas 'P' era esperado.)", filename, ch);
		exit(2);
	}
	
	pio->tipo = getc(fp)-48;
	
	fseek(fp,1, SEEK_CUR);

	while((ch=getc(fp))=='#'){
		while( (ch=getc(fp))!='\n');
	}

	fseek(fp,-1, SEEK_CUR);

	fscanf(fp, "%d %d",&pio->c,&pio->r);
	if (ferror(fp)){ 
		perror(NULL);
		exit(3);
	}	
	fscanf(fp, "%d",&pio->mv);
	fseek(fp,1, SEEK_CUR);

	pio->pData = (unsigned char*) malloc(pio->r * pio->c * sizeof(unsigned char));

	switch(pio->tipo){
		case 2:
			puts("Lendo imagem PGM (dados em texto)");
			for (int k=0; k < (pio->r * pio->c); k++){
				fscanf(fp, "%hhu", pio->pData+k);
			}
		break;	
		case 5:
			puts("Lendo imagem PGM (dados em binário)");
			fread(pio->pData,sizeof(unsigned char),pio->r * pio->c, fp);
		break;
		default:
			puts("Não está implementado");
	}
	
	fclose(fp);

}

void writePGMImage(struct pgm *pio, char *filename){
	
	FILE *fp;

	if (!(fp = fopen(filename,"wb"))){
		perror("Erro.");
		exit(1);
	}

	fprintf(fp, "%s\n","P5");
	fprintf(fp, "%d %d\n",pio->c, pio->r);
	fprintf(fp, "%d\n", 255);

	fwrite(pio->pData, sizeof(unsigned char),pio->c * pio->r, fp);

	fclose(fp);

}

void viewPGMImage(struct pgm *pio){
	printf("Tipo: %d\n",pio->tipo);
	printf("Dimensões: [%d %d]\n",pio->c, pio->r);
	printf("Max: %d\n",pio->mv);

	for (int k=0; k < (pio->r * pio->c); k++){
		if (!( k % pio->c)) printf("\n");
		printf("%2hhu ",*(pio->pData+k));
	}	
	printf("\n");
}
