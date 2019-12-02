#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

int main(){
    char filename[50];
    scanf("%s", filename);
    int N = 0;//세로
    int M = 0;//가로
    char buff;
    int check = TRUE;
    int checke = FALSE;
    int checkn = FALSE;
    int chnum = FALSE;
    FILE* file2 = fopen(filename, "r");
    FILE * formula = fopen("formula", "w");

    if(file2==0){
        printf("Can't find this file\n");
        return 0;
    }
    while (1) {
      buff = getc(file2);
      if (feof(file2)) {break;}
      if(!(((buff >= '0' && buff <= '9'))))
         chnum=FALSE;
      if ((!chnum)&&check && ((buff >= '0' && buff <= '9'))) {
         M++;
         chnum = TRUE;
      }
      else if ((!checke) && (buff == '\n')) {
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
      else if (checke && (!checkn) && (buff == '\n' || buff==' ')) {
         checke = FALSE;
      }
      //printf("->%c", buff);
   }
   //printf("\n%d %d\n", N, M);
   //printf(",,,");
   fclose(file2);

    FILE* file = fopen(filename, "r");
    int row[N]; //가로 개수 저장
    int col[M]; //세로 개수 저장
    int rrow[N];
    int rcol[M];
    for(int i=0; i<N; i++)
        rrow[i]=0;
    for(int i=0; i<M; i++)
        rcol[i]=0;
    int x, y;
    for (y = 1; y <= N; y++) 
        for (x = 1; x <= M; x++)
            fprintf(formula,"(declare-const p%d_%d Int)\n", y, x);

    int count=0;
    //글을 읽어와야지
    for(y=0; y<=N; y++){
        for(x=1; x<=M+1; x++){
            if(y==0 && x==M+1) continue;
            char buffs[5]="";
            buff = getc(file);
            if (buff >= '0' && buff <= '9'){
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
                if(y==0){
                    col[x-1]=buffn;
                }
                else if(x==M+1){
                    row[y-1]=buffn;
                }
                else{
                    rcol[x-1]+=buffn;
                    rrow[y-1]+=buffn;
                    fprintf(formula,"(assert(or (= p%d_%d %d) (= p%d_%d %d)))\n", y,x,buffn*100, y,x,buffn);
                }
            }//숫자일 때
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
/*
    for(int i=0; i<M; i++)
        printf("%d ", col[i]);
    printf("\n");
    for(int i=0; i<N; i++)
        printf("%d ", row[i]);
    printf("\n\n");
    for(int i=0; i<M; i++)
        printf("%d ", rcol[i]);
    printf("\n");
    for(int i=0; i<N; i++)
        printf("%d ", rrow[i]);
*/

    for(int y=1; y<=N; y++){
        fprintf(formula, "(assert (= (+ ");
        for(int x=1; x<=M; x++){
            fprintf(formula, "p%d_%d ", y, x);
        }
        fprintf(formula, ")%d))\n", row[y-1]+(rrow[y-1]-row[y-1])*100);
    }

    for(int x=1; x<=M; x++){
        fprintf(formula, "(assert (= (+ ");
        for(int y=1; y<=N; y++){
            fprintf(formula, "p%d_%d ", y, x);
        }
        fprintf(formula, ")%d))\n", col[x-1]*100+(rcol[x-1]-col[x-1]));
    }

    fprintf(formula,"(check-sat)\n(get-model)\n") ;

    fclose(formula);


    
    FILE *z3 = popen("z3 formula","r") ;
    int board[N+1][M+1] ;
    int i, j, k;

    char b1[128];
	char b[128] ;
	char s[128] ;
	char t[128] ;

	fscanf(z3,"%s %s", b1, b) ;
    if(strcmp(b1, "unsat") == 0){
        printf("No solution\n");
        exit(0);
    }
	for (k = 0 ; k < (N*M) ; k++) {
		fscanf(z3, "%s %s %s %s %s", b, s, b, b, t) ;

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

        if(t[strlen(t)-3]=='0'&&t[strlen(t)-2]=='0')
            board[i][j]=1;
        else
        {
            board[i][j]=0;
        }
        
	}

	for (i = 1 ; i <= N ; i++) {
		for (j = 1 ; j <= M ; j++) {
			printf("%d ", board[i][j]) ;
		}
		printf("\n") ;
	}
    pclose(z3);
    fclose(file);
}//main