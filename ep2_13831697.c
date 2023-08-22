#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 120

typedef struct{
    int index;
    int key;
}REGISTER;

typedef struct{
    int elementQnt;
    REGISTER A[MAX];
}SEQ_LIST;

void initialize(SEQ_LIST* l){
    l->elementQnt = 0;
}

bool annex(SEQ_LIST* l, int key){
    if(l->elementQnt == (MAX)) return false;
    l->A[l->elementQnt].key = key;
    l->elementQnt++;
    return true;
}

void show(SEQ_LIST* l){
    int i;
    for(i = 0; i < l->elementQnt; i++){
        printf("%d ", l->A[i].key);
    }
}

void showIndex(SEQ_LIST* l){
    int i;
    for(i = 0; i < l->elementQnt; i++){
        printf("%d ", l->A[i].index);
    }
}

void shellSort(SEQ_LIST* l, int size) {
    int i, j, key, index;
    int h = 1;
    while (h < size) h = 3*h+1;
        while (h > 0) {
            for(i = h; i < size; i++) {
                key = l->A[i].key;
                index = l->A[i].index;
                j = i;
                while (j > h-1 && key < l->A[j - h].key) {
                    l->A[j].key = l->A[j - h].key;
                    l->A[j].index = l->A[j - h].index;
                    j = j - h;
                }
                l->A[j].key = key;
                l->A[j].index = index;
            }
            h = h/3;
        }
}

void setIndex(SEQ_LIST* l, int newIndex, int position){
    l->A[position].index = newIndex;
}

void fixIndexPosition(SEQ_LIST* l){
    int aux;
    for(int i = 0; i < l->elementQnt - 1; i++){
        if(l->A[i].key == l->A[i+1].key && l->A[i].index > l->A[i+1].index){
            aux = l->A[i].index;
            l->A[i].index = l->A[i+1].index;
            l->A[i+1].index = aux;
        }
    }
}

//cenario 2: dinamico (heap)
void heapsort(SEQ_LIST* l, int updateSize){
   int i = updateSize / 2;
   int parent;
   int descendant;
   int aux;
   int aux2;
   while(true){
      if (i > 0){
          i--;
          aux = l->A[i].key;
          aux2 = l->A[i].index;
      }else{
          updateSize--;
          if (updateSize <= 0) return;
          aux = l->A[updateSize].key;
          aux2 = l->A[updateSize].index;
          l->A[updateSize].key = l->A[0].key;
          l->A[updateSize].index = l->A[0].index;
      }
      parent = i;
      descendant = i * 2 + 1;
      while (descendant < updateSize){
          if ((descendant + 1 < updateSize)  &&  (l->A[descendant + 1].key > l->A[descendant].key))
              descendant++;
          if (l->A[descendant].key > aux){
             l->A[parent].key = l->A[descendant].key;
             l->A[parent].index = l->A[descendant].index;
             parent = descendant;
             descendant = parent * 2 + 1;
          }else{
             break;
          }
      }
      l->A[parent].key = aux;
      l->A[parent].index = aux2;
   }
}

int main(int argc, char** argv){

    int maxTime = atoi(argv[1]);
    int whichCase = atoi(argv[2]);

    //inicialização arquivo
    FILE* fileInput;
    fileInput = fopen(argv[3], "r");
    if(!fileInput){
        printf("Erro na abertura do arquivo de entrada");
        return -1;
    }

    //inicialização lista sequencial
    SEQ_LIST l;
    initialize(&l);

    //inicialização periodo de atualizações
    if(maxTime < 14 || maxTime > 20){
        printf("Erro no resgistro de tempo máximo. Aceita-se somente valores entre, e inclusive, 14 e 20");
        return -1;
    }

    //escolha do caso
    if(whichCase != 1 && whichCase != 2){
        printf("Erro na escolha de caso. Aceita-se somente (1) ou (2)");
        return -1;
    }

    if(whichCase == 1){
        //leitura e armazenamento das informações do arquivo
        int reader;
        int fileInfo [120];
        for(int i = 0; i < 120; i++){
            fileInfo[i] = -1; //valor de controle
        }
        int k = 0;
        while(!feof(fileInput)){
            fscanf(fileInput, "%d\n", &reader);
            fileInfo[k] = reader;
            k++;
        }

        for (int i = 0; i < k; i++){
            printf("%d ", fileInfo[i]);
        }
        printf("\n");

        fclose(fileInput);

        //transferencia de informações para a lista sequencial
        int updateSize = fileInfo[0];
        int z = 1;
        for(int i = 0; i < updateSize; i++){
            annex(&l, fileInfo[z]);
            setIndex(&l, z, i);
            z++;
        }

        //shellsort e indices
        shellSort(&l, updateSize);
        fixIndexPosition(&l);

        //inicializando vetor soluçao
        int genSolution[updateSize];
        for(int i = 0; i < updateSize; i++){
            genSolution[i] = -1;
        }

        //algoritmo guloso
        int counter = 0;
        int a = 0;
        while(counter <= maxTime && counter + l.A[a].key <= maxTime){
            genSolution[a] = l.A[a].index;
            counter = counter + l.A[a].key;
            a++;
        }

        int counter2 = 0;
        int b = 0;
        while(genSolution[b] != -1){
            counter2++;
            b++;
        }

        int c = 0;
        int exactSolution [counter2];
        while(genSolution[c] != -1){
            exactSolution [c] = genSolution[c];
            c++;
        }
        //exactSolution cria vetor com quantidade exata de elementos

        showIndex(&l);
        printf("\n");
        show(&l);
        printf("\n");
        printf("Quantidade de atualizacoes: %d", counter2);
        printf("\n");
        printf("%d ", counter2);
        for(int i = 0; i < counter2; i++){
            printf("%d ", exactSolution[i]);
        }

        FILE* fileOutput;
        fileOutput = fopen("saida1.txt", "w");
        fprintf(fileOutput, "%d ", counter2);
        for(int i = 0; i < counter2; i++){
            fprintf(fileOutput, "%d ", exactSolution[i]);
        }
        fprintf(fileOutput, "\n");
    }

    if(whichCase == 2){
        int reader2;
        int updateSize;
        fscanf(fileInput, "%d\n", &reader2);
        updateSize = reader2;
        int originalUpdate [updateSize];

        for(int i = 0; i < updateSize; i++){
            fscanf(fileInput, "%d\n", &originalUpdate[i]);
            printf("%d ", originalUpdate[i]);
        }

        printf("\n");

        SEQ_LIST l;
        initialize(&l);
        int z = 0;
        for(int i = 0; i < updateSize; i++){
            annex(&l, originalUpdate[z]);
            setIndex(&l, z, i);
            z++;
        }

        heapsort(&l, updateSize);
        fixIndexPosition(&l);

        show(&l);
        printf("\n");
        showIndex(&l);

        int *genSolution = malloc(sizeof(int)*updateSize);
        for(int i = 0; i < updateSize; i++){
            genSolution[i] = -1;
        }

        genSolution[0] = l.A[0].index;

        int timeCount = l.A[0].key;
        l.A[0].key = 999;
        heapsort(&l, updateSize);

        int arrayTimeSetI[100];
        int arrayLocationI[100];
        int arrayChangeI[100];
        int arrayTimeSetC[100];
        int arrayLocationC[100];
        int arrayChangeC[100];
        char c;
        int auxCounter1 = 0;
        int auxCounter2 = 0;

        do{
            c = fgetc(fileInput);
            if(c == 'i'){
                fscanf(fileInput, "%d", &arrayTimeSetI[auxCounter1]);
                fscanf(fileInput, "%d", &arrayLocationI[auxCounter1]);
                fscanf(fileInput, "%d", &arrayChangeI[auxCounter1]);
                auxCounter1++;
            }
            if(c == 'c'){
                fscanf(fileInput, "%d", &arrayTimeSetC[auxCounter2]);
                fscanf(fileInput, "%d", &arrayLocationC[auxCounter2]);
                fscanf(fileInput, "%d", &arrayChangeC[auxCounter2]);
                auxCounter2++;
            }
        }while(c != EOF);

        int aux1 = 0;
        int aux2 = 0;
        int aux3 = 1;

        while(timeCount <= maxTime){
            //caso C
            if(arrayTimeSetC[aux1] <= timeCount || arrayTimeSetC[aux1] == timeCount + 1){
                for(int i = 0; i < updateSize; i++){
                    if(l.A[i].index == arrayLocationC[aux1]){
                        l.A[i].key = arrayChangeC[aux1];
                        heapsort(&l, updateSize);
                        fixIndexPosition(&l);
                        aux1++;
                    }
                }
            }
            //caso I
            if(arrayTimeSetI[aux2] <= timeCount || arrayTimeSetI[aux2] == timeCount + 1){
                updateSize++;
                genSolution = realloc(genSolution, updateSize * sizeof(int));
                l.A[updateSize].index = arrayLocationI[aux2];
                l.A[updateSize].key = arrayChangeI[aux2];
                heapsort(&l, updateSize);
                fixIndexPosition(&l);
                aux2++;
            }
            genSolution[aux3] = l.A[aux3].index;
            timeCount = timeCount + l.A[aux3].key;
            aux3++;
        }
        printf("\n");

        int aux4 = 0;
        while(genSolution[aux4] != -1){
            printf("%d ", genSolution[aux4]);
            aux4++;
        }

        FILE* fileOutput;
        fileOutput = fopen("saida2.txt", "w");
        fprintf(fileOutput, "%d ", aux4);
        for(int i = 0; i < aux4; i++){
            fprintf(fileOutput, "%d ", genSolution[i]);
        }
        fprintf(fileOutput, "\n");
    }

    return 0;
}