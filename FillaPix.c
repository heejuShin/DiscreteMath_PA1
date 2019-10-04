#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int main(int argc, char* argv[]) {
    char filename[50];
    scanf("%s", filename);
   int x, y;
   char buff;
   int num;
   int N = 1;// 행
   int M = 0;// 열
   int check = TRUE;
   int checke = FALSE;
   int checkn = FALSE;
   FILE* file2 = fopen(filename, "r");
   if(file2==0){
        printf("Can't find this file\n");
        return 0;
   }
   //FILE* file2 = fopen(argv[1], "r");
   FILE* formula = fopen("formula0", "w");

   while (1) {
      if (feof(file2)) break;
      buff = getc(file2);
      if (check && ((buff >= '0' && buff <= '9') || buff == '?')) {
         M++;
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
   }
   //printf("%d %d", N, M);
   

   fclose(file2);
   // N = 행,  M이 열
   // x = 행 , y = 열
   /*x,y모두 각각 한줄씩 더 만들어줌*/
   for (x = 0; x <= N + 1; x++) 
      for (y = 0; y <= M + 1; y++)
         fprintf(formula, "(declare-const p%d_%d Int)\n", x, y); // int형식으로 각각 선언해줌
   FILE* file = fopen(filename, "r");
   //FILE * file = fopen(argv[1], "r");
   if (file == 0)
      printf("Could not open file\n");


   // 테두리 부분은 모두 주변부를 0 처리
   for (int x = 0; x <= N + 1; x++) {
      for (int y = 0; y <= M + 1; y++) {
         if (y == 0) fprintf(formula, "(assert (= p%d_%d 0)) \n", x, y);
         if (y == M + 1) fprintf(formula, "(assert (= p%d_%d 0)) \n", x, y);
         if (x == 0) fprintf(formula, "(assert (= p%d_%d 0)) \n", x, y);
         if (x == N + 1) fprintf(formula, "(assert (= p%d_%d 0)) \n", x, y);
      }
   }

   for (int x = 1; x <= N; x++) {
      for (int y = 1; y <= M; y++) {
         fprintf(formula, "(assert (and (>= p%d_%d 0) (<= p%d_%d 1) )) \n", x, y, x, y);
      }
   }


   for (int x = 1; x <= N; x++) {
      for (int y = 1; y <= M; y++) {
         buff = getc(file);
         //printf("IM%c \n", buff);

         if (buff == '?') {
            continue; // ? 만나면 건너뛰기
         }
         if (buff >= '0' && buff <= '9') {

            if (buff == '0') { // 0 일때
               int i = x; // 현제좌표
               int j = y; // 현제좌표
               for (int n = i - 1; n <= i + 1; n++)
                  for (int m = j - 1; m <= j + 1; m++) {
                     fprintf(formula, "(assert (= p%d_%d 0)) \n", n, m);
                  }

            }

            else { // 1~9일때
               int i = x; // 현제좌표
               int j = y; // 현제좌표
               fprintf(formula, "(assert ");
               fprintf(formula, "(= (+ "); // 모든 항목들 다 더해주기
               for (int n = i - 1; n <= i + 1; n++)
                  for (int m = j - 1; m <= j + 1; m++) {
                     fprintf(formula, "p%d_%d ", n, m);
                  }
               fprintf(formula, ") %c))\n", buff); // sum 값이 1이다
            }

         }
         else { // 그 외의 space나 \n 이 나오면 count를 하나 줄여주기 ( 그 부분을 무시하기 ) 
            y--;
         }
      }
   }
   fprintf(formula, "(check-sat)\n(get-model)\n");
   fclose(file2);
   fclose(formula);
   

   char answer[5][N][M];
   for (int mul=0; mul<5; mul++){
         char fn[100];
         char fn2[100];
         char fn3[100];
         sprintf(fn, "z3 formula%d",mul);
         FILE* z3 = popen(fn, "r");
         char board[N][M];
         int i, j, k;

         char b1[128];
         char b[128];
         char s[128]; //where
         char t[128];

         fscanf(z3, "%s %s", b1, b);
         if ((strcmp(b1, "unsat") == 0)&&mul==0) {
            printf("No solution\n");
            exit(0);
         }
         else if((strcmp(b1, "unsat") == 0))
            exit(0);
         if (strcmp(b1, "(error") == 0) {
            printf("Error occured\n");
            exit(0);
         }
         //for (int k1 = 0; k1 <= N + 1; k1++)
         //   for (int k2 = 0; k2 <= M + 1; k2++) {
         while(1){
               fscanf(z3, "%s", b);
               if(feof(z3)) break;
               fscanf(z3, "%s %s %s %s", s, b, b, t);
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

               //printf("%d %d\n", i, j);

               if (i == 0 || i == N + 1 || j == 0 || j == M + 1) continue;
               if (strcmp(t, "0)") == 1) {
                  board[i][j] = 1;
               }
               else
               {
                  board[i][j] = 0;
               }

            }
         for (i = 1; i <= N; i++) {
            for (j = 1; j <= M; j++) {
               printf("%d ", board[i][j]);
            }
            printf("\n");
         }
         printf("\n");
         pclose(z3);


      if(mul!=5){
         sprintf(fn2, "formula%d", mul+1);
         sprintf(fn3, "formula%d", mul);
         FILE *next = fopen(fn2, "w");
         FILE *file3 = fopen(fn3,"r");
         int count;
         char buf[1024];
   
         while (1)
         {
            fgets(buf, 1024, file3);
            if(feof(file3)||buf[1]=='c') {break;}
            fputs(buf,next);
         }
        fclose(file3);
        fclose(next);
         //int copy=fileCopy("formula%d", "formula%d", mul+1, mul);
         //basicCondition(arr, N, M, next);
         
         for(int i=1; i<=N; i++)
            for(int j=1; j<=M; j++)
            answer[mul][i][j]=board[i][j];


         next = fopen(fn2, "aw");
         for(int i=0; i<=mul; i++){
            fprintf(next, "(assert (not (and ");
            for(int j=1; j<=N; j++){
                  for(int k=1; k<=M; k++){
                     fprintf(next, "(= p%d_%d %d) ", j, k, answer[i][j][k]);
                  }
            }
            fprintf(next, ")))\n");
         }
         fprintf(next, "(check-sat)\n(get-model)\n");
         fclose(next);
      }
   }

   
}//main
