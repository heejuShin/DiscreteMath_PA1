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
void print_counter (gpointer key, gpointer value, gpointer userdata) 
{
   char * t = key ;
   int * d = value ;

   printf("(%s, %d, %.3f)\n", t, *d, (*d)*100.0/(*lcp)) ;
}

void save_counter(gpointer key, gpointer value, gpointer userdata){
    char * t = key ;
   int * d = value ;
    FILE * fp = fopen("../data/model.csv", "aw");
    FILE * fpl = fopen("../data/model_logproba.csv", "aw");
    int * check = g_hash_table_lookup(nsave, t) ;
    
    if (check == NULL){
        fprintf(fp, "%s, %f, -10\n", t, (*d)*1.0/(*lcp));
        fprintf(fpl, "%s, %f, -100\n", t, log10f((*d)*1.0/(*lcp)));
    }   
    else{
        fprintf(fp, "%s, %f, %f\n", t, (*d)*1.0/(*lcp), (*check)*1.0/(*nlcp));
        fprintf(fpl, "%s, %f, %f\n", t, log10f((*d)*1.0/(*lcp)), log10f((*check)*1.0/(*nlcp)));   
        g_hash_table_remove(nsave, t); 
    }   
    fclose(fp);
   fclose(fpl);
}
void save_counter2(gpointer key, gpointer value, gpointer userdata){
    char * t = key ;
   int * d = value ;
    FILE * fp = fopen("../data/model.csv", "aw")
;
    fprintf(fp, "%s, 0, %f\n", t, (*d)*1.0/(*nlcp));
     FILE * fpl = fopen("../data/model_logproba.csv", "aw");
   // fprintf(fpl, "%s, 0, %f\n", t, -log10f((*d)*1.0/(*nlcp))); 
	fprintf(fpl, "%s, -100, %f\n", t, log10f((*d)*1.0/(*nlcp)));
   fclose(fp);
   fclose(fpl);
}

int main () 
{
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

    GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ;
    GHashTable * ncounter = g_hash_table_new(g_str_hash, g_str_equal) ;
    nsave = ncounter;

   FILE * f = fopen("../data/train.negative.csv", "r") ;
    //FILE * f = fopen("test1.csv", "r") ;
   char * line = 0x0 ;
    char * stemmer = "./stemmer \"";
   size_t r ; 
   size_t n = 0 ;

   while (getline(&line, &n, f) >= 0) {
        *lcp=*lcp+1;
      char * t ;
      char * _line = line ;

      for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {   
            int j=0;
            char line[10240];
            for(int i=0; i<strlen(t); i++){
                if(t[i]<65&&t[i]>39||t[i]<39||t[i]>122||t[i]>90&&t[i]<97) continue;
                if(isupper(t[i]))
                    t[j++]=t[i]+32;
                else
                    t[j++]=t[i];
            }
            t[j]='\0';
            struct sb_stemmer * stemmer ;
            stemmer = sb_stemmer_new("english", 0x0) ;
            const char * s;
         int * d ;
            int * d3 = malloc(sizeof(int)) ; //compare
            s = sb_stemmer_stem(stemmer, t, strlen(t)) ;
            d3 = g_hash_table_lookup(compare, strdup(s));

            if(d3==NULL){
                d = g_hash_table_lookup(counter, strdup(s)) ;
                char* point = strstr(t, "http");
                if(point == NULL){
                   if (d == NULL) {
                       d = malloc(sizeof(int)) ;
                       *d = 1 ;
                       g_hash_table_insert(counter, strdup(s), d);
                   }
            
                else {
                    *d = *d + 1 ;
                }}//http
                sb_stemmer_delete(stemmer) ;
            }//d2==null
            *wcp=*wcp+1;
      } //단어별
      free(_line) ;
      line = 0x0 ;
   }//line

    //non-negative
    FILE * nf = fopen("../data/train.non-negative.csv", "r") ;
    //FILE * nf = fopen("test2.csv", "r") ;
    char * nline = 0x0 ;
    char * nstemmer = "./stemmer \"";
   size_t nr ; 
   size_t nn = 0 ;

   while (getline(&nline, &nn, nf) >= 0) {
        *nlcp=*nlcp+1;
      char * t ;
      char * _nline = nline ;

      for (t = strtok(nline, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {   
            int j=0;
            char nline[10240];
            for(int i=0; i<strlen(t); i++){
                if(t[i]<65&&t[i]>39||t[i]<39||t[i]>122||t[i]>90&&t[i]<97)  continue;
                if(isupper(t[i]))
                    t[j++]=t[i]+32;
                else
                    t[j++]=t[i];
            }
            t[j]='\0';
            struct sb_stemmer * nstemmer ;
            nstemmer = sb_stemmer_new("english", 0x0) ;
            const char * s;
         int * d ;
            s = sb_stemmer_stem(nstemmer, t, strlen(t)) ;
            int * d3 = malloc(sizeof(int)) ; //compare
            d3 = g_hash_table_lookup(compare, strdup(s));
            if(d3==NULL){      
                d = g_hash_table_lookup(ncounter, strdup(s)) ;
                char* point = strstr(t, "http");
                if(point == NULL){
                   if (d == NULL) {
                       d = malloc(sizeof(int)) ;
                       *d = 1 ;
                       g_hash_table_insert(ncounter, strdup(s), d) ;
                   }
              
                else {
                    *d = *d + 1 ;
                }
                }//http
                sb_stemmer_delete(nstemmer) ;
            }
            *nwcp=*nwcp+1;
      }
      free(_nline) ;
      nline = 0x0 ;
   }

   //g_hash_table_foreach(counter, print_counter, 0x0) ; //negative를 출력
    //FILE * fp = fopen("model.csv", "w");
    FILE * fp = fopen("../data/model.csv", "w");
    fclose(fp);//file초기화

    g_hash_table_foreach(counter, save_counter, 0x0); //non-negative출력(negative랑 중복되는 것 제외)
    //g_hash_table_foreach(ncounter, print_counter, 0x0) ;
    g_hash_table_foreach(ncounter, save_counter2, 0x0);
    
   //printf("worst: %d\n", *((int *) g_hash_table_lookup(counter, "worst"))) ;
   fclose(f) ;
    fclose(nf) ;
    printf("[System] model.csv is made\n");
}
