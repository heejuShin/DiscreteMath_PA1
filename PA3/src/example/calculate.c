#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/libstemmer.h"
#include <math.h>
#define SIZE 512
int wc = 0; //word count (중복포함)
int * wcp = &wc; //word count pointer
int wc2 = 0; //word count (중복 미포함)
int * wc2p = &wc2;
int lc = 0; //line count
int * lcp = &lc; //line count pointer

int nwc = 0; //word count (중복포함)
int * nwcp = &nwc; //word count pointer
int nwc2 = 0; //word count (중복 미포함)
int * nwc2p = &nwc2;
int nlc = 0; //line count
int * nlcp = &nlc; //line count pointer

GHashTable * nsave;

int main (int argc, char ** argv)
{
    GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ;
    GHashTable * ncounter = g_hash_table_new(g_str_hash, g_str_equal) ;
 FILE * fc = fopen("../data/model_logproba.csv", "r") ;

  char * line = 0x0 ;

  size_t r ;
  size_t n = 0 ;

     GHashTable * linecheck = g_hash_table_new(g_str_hash, g_str_equal) ;
  while (getline(&line, &n, fc) >= 0) {
         *lcp=*lcp+1;
   char * t ;
   char * _line = line ;



   for (t = strtok(line, "\n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {

    char * entire = malloc(sizeof(int)*1024);
    char * title = malloc(sizeof(int)*1024);
    strcpy(entire, t);

    char * tok = strtok(entire, ",");
    strcpy(title, tok);


    for(int i = 0 ; i < 3;  i ++){
     if(i == 1 ){


       g_hash_table_insert(counter, strdup(title), tok);
     }
     if(i == 2 ){


       g_hash_table_insert(ncounter, strdup(title), tok);
        }
     tok = strtok(NULL, ",");
    }

   }
         g_hash_table_remove_all(linecheck);
   free(_line) ;
   line = 0x0 ;
  }//line


 // g_hash_table_foreach(counter, print_counter, 0x0) ;
 //g_hash_table_foreach(ncounter, print_counter2, 0x0) ;

     fclose(fc) ;

    GHashTable * compare = g_hash_table_new(g_str_hash, g_str_equal) ;
    char* stword[SIZE] ={
                "may","next","nor","ok","r","see","saw","say",
              "somewhat","somewhat","someone","somebody","slightly","slight","since","side",
              "sometime","something","soon","sub","tak","take","tell",
              "think","thank","ve","thin","thing","thirty","thorough","thou","thousand",
                 "thus","thru","throughout","three","one","two","four","five","six","seven","eight","nine","ten",
                  "way","want","till","tip","sure","","",
                   "wherein","wherea","whereafter","whatever","weren","went",
                   "whose","whomever","whim","whilst","wherever","whether","whereby",
                   "without","wonder", "within","willing",
                  "d","re","would","word","won",
                    "i", "me", "my", "myself", "we", "our", "ours",
                    "ourselves", "you", "your", "yours", "yourself",
                    "yourselves", "he", "him", "his", "himself", "she",
                    "her", "hers", "herself", "it", "its", "itself",
                    "they", "them", "their", "theirs", "themselv",
                    "what", "which", "who", "whom", "this", "that",
                    "these", "those", "am", "is", "are", "was", "were",
                    "be", "been", "being", "have", "has", "had", "having",
                    "do", "does", "did", "doing", "a", "an", "the", "and",
                    "but", "if", "or", "because", "as", "until", "while",
                    "of", "at", "by", "for", "with", "about", "against",
                    "between", "into", "through", "during", "before", "after",
                    "above", "below", "to", "from", "up", "down", "in", "out",
                    "on", "off", "over", "under", "again", "further", "then",
                    "once", "here", "there", "when", "where", "why", "how",
                    "all", "any", "both", "each", "few", "more", "most", "other",
                    "some", "such", "no", "nor", "not", "only", "own", "same",
                    "so", "than", "too", "very", "s",
                    "t", "can", "will", "just", "don", "should", "now", "alway"
                    "d","re", "ll","z","yet","yes","x","y","world"
                    };
    for(int i=0; i<SIZE; i++){
            char *b= stword[i];
            if(b!=NULL) {
                    int * a = malloc(sizeof(int)) ;
                    *a=1;
                    g_hash_table_insert(compare, strdup(b), a) ;
                   // printf("%s", b);
            }
    }

    nsave = ncounter;
    char buffer[100000];
 FILE * farr = fopen(argv[1], "r") ;
    //FILE * f = fopen("test1.csv", "r") ;
    char * stemmer = "./stemmer \"";

    double NEGPROB = 0.0;
    double NONNEGPROB = 0.0;
    double *arr;
    int index = 0; int row = 0;

    while(1){
      fgets(buffer, sizeof(buffer), farr);
      if(feof(farr))break;
        row++;

     }
    fclose(farr);
    arr = (double*)malloc((row+1) * sizeof(double));
    FILE * f = fopen(argv[1], "r") ;

 while (getline(&line, &n, f) >= 0) {
        NEGPROB = 0.0;  NONNEGPROB = 0.0;
        *lcp=*lcp+1;
  char * t ;
  char * _line = line ;
  for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {
            int j=0;
            char line[10240];
            for(int i=0; i<strlen(t); i++){
                if(t[i]<65||t[i]>122) continue;
                if(isupper(t[i]))
                    t[j++]=t[i]+32;
                else
                    t[j++]=t[i];
            }
            t[j]='\0';
            struct sb_stemmer * stemmer ;
            stemmer = sb_stemmer_new("english", 0x0) ;
            const char * s;
       char *d = malloc(1024*sizeof(char));
       int * d2 = malloc(sizeof(int)) ;

            s = sb_stemmer_stem(stemmer, t, strlen(t)) ;
            d2 = g_hash_table_lookup(compare, strdup(s)) ;
            if(d2==NULL){
                char* point = strstr(t, "http");
                    if(point == NULL){
                    d = g_hash_table_lookup(counter, strdup(s)) ;
                    if (d == NULL) {
                        NEGPROB += 0.0;
                    }
                    else {
                        NEGPROB += atof(d);
                    }
                    d = g_hash_table_lookup(ncounter, strdup(s)) ;
                    if (d == NULL) {
                        NONNEGPROB += 0.0;
                    }
                    else {
                        NONNEGPROB += atof(d) ;
                    }
                }
                sb_stemmer_delete(stemmer) ;
            }//d2==null
            *wcp=*wcp+1;
   //free(d);
   //free(d2);
  } //단어별
        if((NONNEGPROB+NEGPROB) != 0.0) arr[index]  = (NEGPROB)/(NONNEGPROB+NEGPROB);
        else arr[index] = -1.0;
        index +=1;
        g_hash_table_remove_all(linecheck);
  free(_line) ;
  line = 0x0 ;
 }//line
 fclose(f) ;
 //g_hash_table_foreach(counter, print_counter, 0x0) ; //negative를 출력
    //FILE * fp = fopen("model.csv", "w");
    FILE * fp = fopen("../data/probability.csv", "w");
    for (int i =0; i<row; i++){
      if(arr[i]<=0.47){
        fprintf(fp,"NEGATIVE_PREDICTED, %lf\n", arr[i]);
        printf("NEGATIVE_PREDICTED, %lf\n",  arr[i]);
      }
    else{
        fprintf(fp,"NON-NEGATIVE_PREDICTED, %lf\n",  arr[i]);
        printf("NON-NEGATIVE_PREDICTED, %lf\n",  arr[i]);
      }
    }
    fclose(fp);//file초기화
 free(arr);


    //printf("[System] probability.csv is made\n");
}
