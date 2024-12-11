#include "gera.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  FILE *myfp;
  unsigned char codigo[200];
  funcp funcaoSimples;
  int res, erros=0, cont=0;
  
  //CASO DE TESTE 1
  /* Abre o arquivo fonte */
  if ((myfp = fopen("simples1.txt", "r")) == NULL) {
    perror("Falha na abertura do arquivo fonte");
    exit(1);
  }
  /* compila a função Simples */
  funcaoSimples = gera(myfp, codigo);
  fclose(myfp);

  /* chama a função */
  res = (*funcaoSimples) ();  /* passando argumentos apropriados */
  cont++;
  if(res!=100){   
    printf("Erro no teste 1\n");
    erros++;
  }

  
  //CASO DE TESTE 2
  if ((myfp = fopen("simples2.txt", "r")) == NULL) {
    perror("Falha na abertura do arquivo fonte");
    exit(1);
  }
  /* compila a função Simples */
  funcaoSimples = gera(myfp, codigo);
  fclose(myfp);

  /* chama a função */
  res = (*funcaoSimples) ();  /* passando argumentos apropriados */
  cont++;
  if(res!=99){   
    printf("Erro no teste 2\n");
    erros++;
  }
  
  
  //CASO DE TESTE 3
  if ((myfp = fopen("simples3.txt", "r")) == NULL) {
    perror("Falha na abertura do arquivo fonte");
    exit(1);
  }
  /* compila a função Simples */
  funcaoSimples = gera(myfp, codigo);
  fclose(myfp);

  /* chama a função */
  res = (*funcaoSimples) (-2,-3);  /* passando argumentos apropriados */
  cont++;
  if(res!=-3){   
    printf("Erro no teste 3\n");
    erros++;
  }

  
  //CASO DE TESTE 4
  if ((myfp = fopen("simples4.txt", "r")) == NULL) {
    perror("Falha na abertura do arquivo fonte");
    exit(1);
  }
  /* compila a função Simples */
  funcaoSimples = gera(myfp, codigo);
  fclose(myfp);

  /* chama a função */
  res = (*funcaoSimples) (2);  /* passando argumentos apropriados */
  cont++;
  if(res!=-4){    
    printf("Erro no teste 4\n");
    erros++;
  }
  

  //CASO DE TESTE 5
  if ((myfp = fopen("simples5.txt", "r")) == NULL) {
    perror("Falha na abertura do arquivo fonte");
    exit(1);
  }
  /* compila a função Simples */
  funcaoSimples = gera(myfp, codigo); //ESTA DANDO SEG FAULT!!!!!!!!!!
  fclose(myfp);
  
  /* chama a função */
  res = (*funcaoSimples) (1);  /* passando argumentos apropriados */
  cont++;
  if(res!=39){    
    printf("Erro no teste 5\n");
    erros++;
  }


  //CASO DE TESTE 6
  if ((myfp = fopen("simples6.txt", "r")) == NULL) {
    perror("Falha na abertura do arquivo fonte");
    exit(1);
  }
  /* compila a função Simples */
  funcaoSimples = gera(myfp, codigo); //ESTA DANDO SEG FAULT!!!!!!!!!!
  fclose(myfp);
  
  /* chama a função */
  res = (*funcaoSimples) (1);  /* passando argumentos apropriados */
  cont++;
  if(res!=-5){   
    printf("Erro no teste 6\n");
    erros++;
  }


  //CASO DE TESTE 7
  if ((myfp = fopen("simples7.txt", "r")) == NULL) {
    perror("Falha na abertura do arquivo fonte");
    exit(1);
  }
  /* compila a função Simples */
  funcaoSimples = gera(myfp, codigo); //ESTA DANDO SEG FAULT!!!!!!!!!!
  fclose(myfp);
  
  /* chama a função */
  res = (*funcaoSimples) (4);  /* passando argumentos apropriados */
  cont++;
  if(res!=24){
    printf("Erro no teste 7\n");
    erros++;
  }


  //CASO DE TESTE 8
  if ((myfp = fopen("simples8.txt", "r")) == NULL) {
    perror("Falha na abertura do arquivo fonte");
    exit(1);
  }
  /* compila a função Simples */
  funcaoSimples = gera(myfp, codigo); //ESTA DANDO SEG FAULT!!!!!!!!!!
  fclose(myfp);
  
  /* chama a função */
  res = (*funcaoSimples) (1,3,4);  /* passando argumentos apropriados */
  cont++;
  if(res!=7){
    printf("Erro no teste 8\n");
    erros++;
  }
  
  printf("Total de %d erros em %d casos de teste\n",erros,cont);
  return 0;
}