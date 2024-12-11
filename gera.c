/* Diego Miranda 2210996 3WA */
/* Felipe Cancella 2210487 3WA */

#include <stdio.h>
#include <stdlib.h>
#include "gera.h"

union conv {
  int val;
  unsigned char d[3];
};
typedef union conv Conv;

struct offs {
  // Infos necessarias para calculo do offset: conteudo[n] - (indice + 1);
  int indice; // indice do byte que guarda o offset
  int n; // linha destino

  // Depoois de finalizar o codigo de maquina (CODIGO[])
  // um loop for é rodado e calcula todos offsets guardados no vetor offs
};
typedef struct offs Offs;

typedef int (*funcp)();

static void error(const char *msg, int line) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}


funcp gera(FILE *myfp, unsigned char codigo[]) {

  // vetor pra guardar a quantidade comandos de cada linha
  int conteudo[30] = {};
  
  // vetor que guarda os offsets que precisam ser calculados
  Offs offsets_futuros[30];
  int cont_offsets = 0;

  codigo[0] = 0x55;
  codigo[1] = 0x48;
  codigo[2] = 0x89;
  codigo[3] = 0xe5;

  int line = 1;
  int indice = 4;
  char c;
  Conv convert;
  while ((c = fgetc(myfp)) != EOF) {

    // A cada linha adiciona no indice do vetor conteudo a quantidade de "indice"/comandos.
    conteudo[line] = indice;

    // Caso seja uma linha "válida", conta linha
    if (c == 'v' || c == 'r' || c == 'i')
      line++;

    if (c == 'r') {
      char var0;
      int idx0;
      if (fscanf(myfp, "et %c%d", &var0, &idx0) != 2)
        error("comando invalido", line);

      else if (var0 == '$') { // ret $x (valor inteiro)

        // mover idx0 pra %eax
        convert.val = idx0;      // idxo --> int
        codigo[indice++] = 0xb8; // mov ... %eax

        for (int i = 0; i < 4; i++) {
          codigo[indice++] = convert.d[i]; // idxo --> uns. char*
        }

        // retornar
        codigo[indice++] = 0x5d; // pop
        codigo[indice++] = 0xc3; // ret
      } 
      else if (var0 == 'v') {  // ret vi (variavel)

        // mover -vi(%rbp) pra %eax
        codigo[indice++] = 0x8b;
        codigo[indice++] = 0x45;

        int vi = -(24 - (4 * idx0)); // rbp - (24 - 4*i)

        codigo[indice++] = vi; // conteudo: -vi(%rbp)

        // retornar
        codigo[indice++] = 0x5d;
        codigo[indice++] = 0xc3;
      }

    }
    else if (c == 'v') { // vi ? xx
      int idx0, idx1, idx2;
      char c0, var1, var2;
      if (fscanf(myfp, "%d %c", &idx0, &c0) != 2)
        error("comando invalido", line);

      if (c0 == '<') { // atribuicao vi < x
        if (fscanf(myfp, " %c%d", &var1, &idx1) != 2) {
          error("comando invalido", line);
        }

        if (var1 == '$') { // vi < $xx
          // mov $idx1, -vi(%rbp)
          codigo[indice++] = 0xc7;
          codigo[indice++] = 0x45;

          int vi = -(24 - (4 * idx0)); // rbp - (24 - 4*i)
          codigo[indice++] = vi;       // conteudo: -vi(%rbp)

          // Int to Char* pra andar byte a byte
          convert.val = idx1; // idx1 --> int
          for (int i = 0; i < 4; i++) {
            codigo[indice++] = convert.d[i]; // idx1 --> uns. char*
          }

        } else if (var1 == 'p') {      // vi < pj
          int vi = -(24 - (4 * idx0)); // rbp - (24 - 4*i)
          codigo[indice++] = 0x89;

          if (idx1 == 3){
            codigo[indice++] = 0x55;
          }
          else {
            codigo[indice++] = 0x85 - (idx1 * 8);
          }

          codigo[indice++] = vi;

        } else { // var1 == 'v'
          // v1 < v2
          // mov vi2 , vi1
          int vi1 = -(24 - (4 * idx0));
          int vi2 = -(24 - (4 * idx1));
          codigo[indice++] = 0x8b;
          codigo[indice++] = 0x5d;
          codigo[indice++] = vi2;
          codigo[indice++] = 0x89;
          codigo[indice++] = 0x5d;
          codigo[indice++] = vi1;
        }
      }
      else { // (c0 == '=')  operacao aritmetica
        if (fscanf(myfp, " %c%d %c %c%d", &var1, &idx1, &c0, &var2, &idx2) !=
            5) {
          error("comando invalido", line);
        }
        // Guarda cd1 e cd2 e aloca eles em ecx e eax. Depois disso é só seguir
        // SEMPRE com esses dois registradores operando com +, - ou *.

        if (var1 == 'v') { // vi = vj op xx
          // Movendo vi pra ecx
          int vi = -(24 - (4 * idx1));
          codigo[indice++] = 0x8b;
          codigo[indice++] = 0x4d;
          codigo[indice++] = vi;

        } else if (var1 == '$') { // vi = $x op xx
          // Movendo int idx1 pra ecx
          codigo[indice++] = 0xb9;

          // Int to Char* pra andar byte a byte
          convert.val = idx1; // idx1 --> int
          for (int i = 0; i < 4; i++) {
            codigo[indice++] = convert.d[i]; // idx1 --> uns. char*
          }
        }

        if (var2 == 'v') { // vi = xx op vj
          // Movendo vi pra eax
          int vi = -(24 - (4 * idx2));
          codigo[indice++] = 0x8b;
          codigo[indice++] = 0x45;
          codigo[indice++] = vi;

        } 
        else if (var2 == '$') { // vi == xx op $x
          // Movendo int idx2 pra eax
          codigo[indice++] = 0xb8;

          // Int to Char* pra andar byte a byte
          convert.val = idx2; // idx2 --> int
          for (int i = 0; i < 4; i++) {
            codigo[indice++] = convert.d[i]; // idx2 --> uns. char*
          }
        }

        // Realizar a operacao
        if (c0 == '+') {
          codigo[indice++] = 0x01;
          codigo[indice++] = 0xc1;
        }
        if (c0 == '-') {
          codigo[indice++] = 0x29;
          codigo[indice++] = 0xc1;
        }
        if (c0 == '*') {
          codigo[indice++] = 0x0f;
          codigo[indice++] = 0xaf;
          codigo[indice++] = 0xc8;
        }

        // Movendo ecx (resultado) para variavel original
        int vi = -(24 - (4 * idx0));
        codigo[indice++] = 0x89;
        codigo[indice++] = 0x4d;
        codigo[indice++] = vi;
      }
    }
    else if (c == 'i') {
      char var0;
      int idx0, n;
      if (fscanf(myfp, "flez %c%d %d", &var0, &idx0, &n) != 3) {
        error("comando invalido", line);
      }
      int vi = -(24 - (4 * idx0));

      // cmpl   $0, vi == if vi < 0
      codigo[indice++] = 0x83;
      codigo[indice++] = 0x7d;
      codigo[indice++] = vi;
      codigo[indice++] = 0x00;

      // Fazendo o jump
      codigo[indice++] = 0x7e;

      if (n > line) {
        // offset calculado futuramente
        codigo[indice++] = 0x00; // Valor qualquer que sera alterado
        offsets_futuros[cont_offsets].indice = (indice - 1);
        offsets_futuros[cont_offsets++].n = n;
      } else {
        // offset calculado imediatamente
        int offset = conteudo[n] - (indice + 1);
        codigo[indice++] = offset;
      }
    }
  }

  for (int i = 0; i < cont_offsets; i++) {
    int new_offset = conteudo[offsets_futuros[i].n] - ((offsets_futuros[i].indice)+1);
    codigo[offsets_futuros[i].indice] = new_offset;
  }

  return (funcp)codigo;
}