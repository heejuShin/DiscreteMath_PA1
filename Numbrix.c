#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char **argv)
{
    char filename[50];
    scanf("%s", filename);
    int N = 1;//세로
    int M = 0;//가로
    char buff;
    int check = TRUE;
    int checke = FALSE;
    int checkn = FALSE;
    int chnum = FALSE;
    //FILE* file2 = fopen(argv[1], "r");
    FILE* file2 = fopen(filename, "r");
    FILE * formula = fopen("formula", "w");

    if(file2==0){
        printf("Can't find this file\n");
        return 0;
    }
    while (1) {
      if (feof(file2)) break;
      buff = getc(file2);
      if(!(((buff >= '0' && buff <= '9') || buff == '?')))
         chnum=FALSE;
      if ((!chnum)&&check && ((buff >= '0' && buff <= '9') || buff == '?')) {
         M++;
         chnum = TRUE;
      }
      else if ((!checke) && buff == '\n') {
         check = FALSE;
         checke = TRUE;
      }
      else if (checke && ((buff >= '0' && buff <= '9') || buff == '?')) {
         checkn = TRUE;
      }
      else if (checke && checkn) {
         N++;
         checke = FALSE;
         checkn = FALSE;
      }
      else if (checke && (!checkn) && buff == '\n') {
         checke = FALSE;
      }
      //printf("%c", buff);
   }
   //printf("%d %d", N, M);
   fclose(file2);
   //printf("%d %d", N, M);
   
   //가로랑 세로 크기 읽어 왔음



    FILE* file = fopen(filename, "r");
    //FILE* file = fopen(argv[1], "r");
    int x, y;
    for (y = 0; y <= N+1; y++) 
        for (x = 0; x <= M+1; x++)
        fprintf(formula,"(declare-const p%d_%d Int)\n", y, x);
    //declare 하고

   // 테두리 부분은 모두 주변부를 0 처리
    for (int y = 0; y <= N + 1; y++) {
      for (int x = 0; x <= M + 1; x++) {
         if (y == 0) fprintf(formula,"(assert (= p%d_%d 0)) \n", y, x);
         if (y == N + 1) fprintf(formula,"(assert (= p%d_%d 0)) \n", y, x);
         if (x == 0) fprintf(formula,"(assert (= p%d_%d 0)) \n", y, x);
         if (x == M + 1) fprintf(formula,"(assert (= p%d_%d 0)) \n", y, x);
      }
    }

    //제일먼저 전체 칸에 각 다른 숫자가 1~(N*M)개 들어가야함을 나타낸다.
    
    
    fprintf(formula,"(assert (distinct ");
    for (y = 1 ; y <= N ; y++)
        for (x = 1 ; x <= M ; x++)
            fprintf(formula,"p%d_%d ", y, x) ;
    fprintf(formula,"))\n");
    


    for (int y =1; y<=N; y++){
        for (int x=1; x<M+1; x++){
            fprintf(formula,"(assert (or (= p%d_%d (- p%d_%d 1)) (= p%d_%d (- p%d_%d 1)) (= p%d_%d (- p%d_%d 1)) (= p%d_%d (- p%d_%d 1)) (= p%d_%d 1)))\n", y,x-1, y, x, y, x+1, y,x, y+1, x,y,x, y-1, x, y,x,y,x);
            fprintf(formula,"(assert (or (= p%d_%d (+ p%d_%d 1)) (= p%d_%d (+ p%d_%d 1)) (= p%d_%d (+ p%d_%d 1)) (= p%d_%d (+ p%d_%d 1)) (= p%d_%d %d)))\n", y,x-1, y, x, y, x+1, y,x, y+1, x,y,x, y-1, x, y,x,y,x, N*M);
        }
    }


    int count=0;
    //글을 읽어와야지
    for(y=1; y<=N; y++){
        for(x=1; x<=M; x++){
            char buffs[5]="";
            buff = getc(file);
            if(buff=='?'){
                fprintf(formula,"(assert (and (<= p%d_%d %d) (<= 1 p%d_%d)))\n", y, x, N*M, y, x) ;
                continue; // ? 만나면 건너뛰기
            }
            else if (buff >= '0' && buff <= '9'){
                //두자리수 이상도 받을 수 있게
                buffs[0]=buff;
                while(1){
                    buff=getc(file);
                    if(buff >= '0' && buff <= '9'){
                        buffs[++count]=buff;
                    }
                    else{
                        count=0;
                        break;
                    }
                }
                //buffs로 확인한당
                int buffn=atoi(buffs);
                fprintf(formula,"(assert (= p%d_%d %d))\n", y, x, buffn) ;
                fprintf(formula, "(assert (and ");
                if(buffn!=1){
                fprintf(formula, "(or ");
                fprintf(formula, "(= p%d_%d %d) ", y-1, x, buffn-1); 
                fprintf(formula, "(= p%d_%d %d) ", y+1, x, buffn-1); 
                fprintf(formula, "(= p%d_%d %d) ", y, x-1, buffn-1); 
                fprintf(formula, "(= p%d_%d %d) ", y, x+1, buffn-1); 
                fprintf(formula, ")");
                }
                if(buffn!=N*M){
                fprintf(formula, "(or ");
                fprintf(formula, "(= p%d_%d %d) ", y-1, x, buffn+1); 
                fprintf(formula, "(= p%d_%d %d) ", y+1, x, buffn+1); 
                fprintf(formula, "(= p%d_%d %d) ", y, x-1, buffn+1); 
                fprintf(formula, "(= p%d_%d %d) ", y, x+1, buffn+1); 
                fprintf(formula, ")");
                }
                fprintf(formula,"))\n");

            }
            else{
                if(x==1){
                    x=M;
                    y--;
                }
                else x--;

            }
            //printf("%s\n", buffs);
        }//작은 for문
    }//큰 for문

    //전체 합 배정하자
    fprintf(formula,"(assert(= (+ ") ;
    for (y = 1 ; y <= N ; y++){
        for (x = 1 ; x <= M ; x++){
            fprintf(formula,"p%d_%d ", y, x) ;
        }
    }
    fprintf(formula,") %d))\n", (1+N*M)*(N*M)/2) ;

    fprintf(formula,"(check-sat)\n(get-model)\n") ;
    
    fclose(file);
    fclose(formula);

    FILE* z3 = popen("z3 formula", "r");
   char board[N][M];
   int i, j, k;

   char b1[128];
   char b[128];
   char s[128]; //where
   char t[128];

   fscanf(z3, "%s %s", b1, b);
   if (strcmp(b1, "unsat") == 0) {
      printf("No solution\n");
      exit(0);
   }
   if (strcmp(b1, "(error") == 0) {
      printf("Error occured\n");
      exit(0);
   }

   for (int k1 = 0; k1 <= N + 1; k1++)
      for (int k2 = 0; k2 <= M + 1; k2++) {
          if(feof(z3)) break;
         fscanf(z3, "%s %s %s %s %s", b, s, b, b, t);
         char buff1[50] = "";
         char buff2[50] = "";
         int count = 0;
         int bool = 1;
         for (int a = 1; a < strlen(s); a++) {
            if (s[a] == '_') {
               bool = 0;
               continue;
            }
            if (bool) {
               buff1[a - 1] = s[a];
            }
            else {
               buff2[count++] = s[a];
            }
         }
         i = atoi(buff1);
         j = atoi(buff2);


         if (i == 0 || i == N + 1 || j == 0 || j == M + 1) continue;
         char buffc[5]="";
         for(int a=0; a<strlen(t); a++){
             if(t[a]==')') break;
             buffc[a]=t[a];
         }
        int result=atoi(buffc);
        board[i][j]=result;

      }

   for (i = 1; i <= N; i++) {
      for (j = 1; j <= M; j++) {
         printf("%4d ", board[i][j]);
      }
      printf("\n");
   }
   pclose(z3);
   

}//Main