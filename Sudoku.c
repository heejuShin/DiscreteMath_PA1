#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char **argv)
{
	int x,y,z;
    int check =0;
    char filename[50];
    scanf("%s", filename);
	// if ( argc != 2 )
	//		        printf( "usage: %s is provided,filename is not provided", argv[0] );
	//	    else{
	//FILE * file = fopen(argv[1], "r" );
    FILE *file = fopen(filename, "r" );
    FILE * formula = fopen("formula", "w");
	if ( file == 0 ) printf("Could not open file\n" );
        if(file==0){
        printf("Can't find this file\n");
        return 0;
    }
								        
    //a11 ~ a99 까지 declare
    for (y = 1 ; y <= 9 ; y++)
        for (x = 1 ; x <= 9 ; x++)
            fprintf(formula,"(declare-const a%d%d Int)\n", y, x) ;

    //row (a11 ~ a19)형식의 distinct 선언
    for (y = 1 ; y <= 9 ; y++){
        fprintf(formula,"(assert (distinct ");
        for (x = 1 ; x <= 9 ; x++)
            fprintf(formula,"a%d%d ", y, x) ;
        fprintf(formula,"))\n");
    }

    //column (a11 ~ a91)형식의 distinct 선언
    for (x = 1 ; x <= 9 ; x++){
        fprintf(formula,"(assert (distinct ");
        for (y = 1 ; y <= 9 ; y++)
            fprintf(formula,"a%d%d ", y, x) ;
        fprintf(formula,"))\n");
    }

    //구역별 (a13 ~ a31 && a31 ~ a33)형식의 distinct 선언
    for(int i = 0 ; i < 3 ; i++){
        fprintf(formula,"(assert (distinct ");
        for (y = (3*i)+1 ; y <= (3*i)+3 ; y++){
            for (x = (3*i)+1 ; x <= (3*i)+3 ; x++)
                fprintf(formula,"a%d%d ", y, x) ;
            }
        fprintf(formula,"))\n");
    }

    //*의 distinct 선언
    for(y = 1 ; y <= 9 ; y++)
        for(x = 1 ; x <= 9 ; x++){
            z=getc(file);
            if(z=='*')
                check=1;
        }
    if(check==1){
        fprintf(formula,"(assert (distinct ");
        for(y = 1 ; y <= 9 ; y++){
            for(x = 1 ; x <= 9 ; x++){
                z = getc(file);
                if((z == 32) || (z == 10)){  //받은 문자가 " " or "\n"일경우 다음 문자로 넘어가라
                    z = getc(file);
                    if(z == '*')
                        fprintf(formula,"a%d%d ",y,x) ;
                }
                else{
                    if(z == '*')
                        fprintf(formula,"a%d%d ",y,x) ;
                }
            }
        }
        fprintf(formula,"))\n") ;
    }


    rewind(file);                  // 파일 포인터를 파일의 맨 처음으로 이동시킴

    for(y = 1 ; y <= 9 ; y++){
        for(x = 1 ; x <= 9 ; x++){
            z = getc(file);
            if((z == 32) || (z == 10)){  //받은 문자가 " " or "\n"일경우 다음 문자로 넘어가라
                z = getc(file);
                if(z >= 49 && z <= 57)    // 1 <= z <= 9일경우에 (assert (ayx = z))
                    fprintf(formula,"(assert (= a%d%d %c))\n", y, x, z) ;
                else                      // z가 숫자가 아닐 경우 (assert (1 <= ayx <= 9))
                    fprintf(formula,"(assert (and (<= a%d%d 9) (<= 1 a%d%d)))\n", y, x, y, x) ;

            }

            else{
                if(z >= 49 && z <= 57)
                    fprintf(formula,"(assert (= a%d%d %c))\n", y, x, z) ;
                else
                    fprintf(formula,"(assert (and (<= a%d%d 9) (<= 1 a%d%d)))\n", y, x, y, x) ;
            }
        }
    }

    for(y=1; y <=9; y++){
        for(x=1; x<=9; x++){
            fprintf(formula,"(assert ( or ");
            for(z=1; z<=9; z++)
                fprintf(formula, "(= a%d%d %d)", y,x,z);
            fprintf(formula,"))\n");
        }
    }

    fclose( file );
//}

    // (assert (a11 + a12 + … + a19 = 45)) row를 +하면 45
    for (y = 1 ; y <= 9 ; y++){
            fprintf(formula,"(assert(= (+ ") ;
                for (x = 1 ; x <= 9 ; x++){
                            fprintf(formula,"a%d%d ", y, x) ;
                                }
                    fprintf(formula,") 45))\n") ;
                        }

    // (assert (a11 + a21 + … + a91 = 45)) column를 +하면 45
    for (y = 1 ; y <= 9 ; y++){
            fprintf(formula,"(assert(= (+ ") ;
                for (x = 1 ; x <= 9 ; x++){
                            fprintf(formula,"a%d%d ", x, y) ;
                                }
                    fprintf(formula,") 45))\n") ;
                        }

    //  (assert (a11 + a12 + a13 + … + a33 = 45)) 구역의 합은 45
    fprintf(formula,"(assert(= (+ ") ;
        for (y = 1 ; y <= 3 ; y++)
                for (x = 1 ; x <= 3 ; x++)
                            fprintf(formula,"a%d%d ", y, x) ;
            fprintf(formula,") 45))\n") ;

                fprintf(formula,"(assert(= (+ ") ;
                    for (y = 4 ; y <= 6 ; y++)
                        for (x = 4 ; x <= 6 ; x++)
                            fprintf(formula,"a%d%d ", y, x) ;
                        fprintf(formula,") 45))\n") ;

                        fprintf(formula,"(assert(= (+ ") ;
                        for (y = 7 ; y <= 9 ; y++)
                        for (x = 7 ; x <= 9 ; x++)
                            fprintf(formula,"a%d%d ", y, x) ;
                        fprintf(formula,") 45))\n") ;

                        fprintf(formula,"(check-sat)\n(get-model)\n") ;
    fclose(formula);
    
    FILE *z3 = popen("z3 formula","r") ;
    int board[10][10] ;
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
	for (k = 0 ; k < 81 ; k++) {
		fscanf(z3, "%s %s %s %s %s", b, s, b, b, t) ;

		i = s[1] - '0' ;
		j = s[2] - '0' ;

        if (strcmp(t, "1)") == 0) 
            board[i][j]=1;
        else if (strcmp(t, "2)") == 0) 
            board[i][j]=2;
        else if (strcmp(t, "3)") == 0) 
            board[i][j]=3;
        else if (strcmp(t, "4)") == 0) 
            board[i][j]=4;
        else if (strcmp(t, "5)") == 0) 
            board[i][j]=5;
        else if (strcmp(t, "6)") == 0) 
            board[i][j]=6;
        else if (strcmp(t, "7)") == 0) 
            board[i][j]=7;
        else if (strcmp(t, "8)") == 0) 
            board[i][j]=8;
        else if (strcmp(t, "9)") == 0) 
            board[i][j]=9;
        else
        {
            board[i][j]=0;
        }
        
	}

	for (i = 1 ; i <= 8 ; i++) {
		for (j = 1 ; j <= 8 ; j++) {
			printf("%d ", board[i][j]) ;
		}
		printf("\n") ;
	}
    pclose(z3);
}