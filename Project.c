//Prova finale API PIZZOCCHERI 10797420

// librerie di libc
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <iso646.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// strutture dati 

typedef enum {false,true} bool;

typedef enum {r,b} colore_nodo_t;

// nodo auto
typedef struct parcoNode
{
    colore_nodo_t colore;
    int autonomia;
    struct parcoNode *sx, *dx, *parent;
} parcoNode;

// albero parco auto
typedef struct parcoTree{
   parcoNode *root, *Tnil, *curr;
}parcoTree;

//nodo stazione
typedef struct stazioneNode
{
    colore_nodo_t colore;
    int distanza;
    struct stazioneNode *sx, *dx, *parent;
    parcoTree* autos;
} stazioneNode;

//albero stazioni
typedef struct stazioneTree
{
   stazioneNode* root, *Tnil, *curr;
}stazioneTree;


// quando gli si passa l'albero settare il curr = root
stazioneNode* staz_search(stazioneTree* x, int dist){
   if(x->curr == NULL || dist == x->curr->distanza){
      return x->curr;
   }
   if(dist < x->curr->distanza){
      x->curr = x->curr->sx;
      return staz_search(x, dist);
   }
   else{
      x->curr = x->curr->dx;
      return staz_search(x, dist);
   }
}

// quando gli si passa l'albero settare il curr = root
parcoNode* auto_search(parcoTree* x, int dist){
   if(x->curr == NULL || dist == x->curr->autonomia){
      return x->curr;
   }
   if(dist < x->curr->autonomia){
      x->curr = x->curr->sx;
      return auto_search(x, dist);
   }
   else{
      x->curr = x->curr->dx;
      return auto_search(x, dist);
   }
}

//sempre settare curr = root
stazioneNode* staz_min(stazioneTree* x){
   while (x->curr->sx != NULL && x->curr->sx != x->Tnil)
   {
      x->curr = x->curr->sx;
   }
   return x->curr;
}

//sempre settare curr = root
parcoNode* auto_min(parcoTree* x){
   while (x->curr->sx != NULL && x->curr->sx != x->Tnil)
   {
      x->curr = x->curr->sx;
   }
   return x->curr;
}


//sempre settare curr = root
stazioneNode* staz_max(stazioneTree* x){
   while (x->curr->sx != NULL && x->curr->sx != x->Tnil)
   {
      x->curr = x->curr->dx;
   }
   return x->curr;
}

//sempre settare curr = root
parcoNode* auto_max(parcoTree* x){
   while (x->curr->sx != NULL && x->curr->sx != x->Tnil)
   {
      x->curr = x->curr->dx;
   }
   return x->curr;
}

stazioneNode* staz_successor(stazioneNode* x, stazioneTree* t){
   stazioneNode* y;
   if(x->dx != NULL || x->dx != t->Tnil){
      t->curr = x->dx;
      return staz_min(t);
   }
   y = x->parent;
   while ( y != NULL && y != t->Tnil && x == y->dx) 
   {
      x = y;
      y = y->parent;
   }
   return y;
}

parcoNode* auto_successor(parcoNode* x, parcoTree* t){
   parcoNode* y;
   if(x->dx != NULL || x->dx != t->Tnil){
      t->curr = x->dx;
      return auto_min(t);
   }
   y = x->parent;
   while ( y != NULL && y != t->Tnil && x == y->dx) 
   {
      x = y;
      y = y->parent;
   }
   return y;
}

stazioneNode* staz_predecessor(stazioneNode* x, stazioneTree* t){
   stazioneNode* y;
   if(x->sx != NULL){
      t->curr = x->sx;
      return staz_max(t);
   }
   y = x->parent;
   while ( y != NULL && x == y->sx) 
   {
      x = y;
      y = y->parent;
   }
   return y;
}

parcoNode* auto_predecessor(parcoNode* x, parcoTree* t){
   parcoNode* y;
   if(x->sx != NULL){
      t->curr = x->sx;
      return auto_max(t);
   }
   y = x->parent;
   while ( y != NULL && x == y->sx) 
   {
      x = y;
      y = y->parent;
   }
   return y;
}



void staz_rot_sx(stazioneTree *T, stazioneNode *x){
   stazioneNode* y;
   y = x->dx;
   x->dx = y->sx;
   if(y->sx != T->Tnil){
      y->sx->parent = x;
   }
   y->parent = x->parent;
   if(x->parent == T->Tnil){
      T->root = y;
   }
   else{ 
      if(x == x->parent->sx){
         x->parent->sx = y;
      }else{
         x->parent->dx = y;
      }
   }
   y->sx = x;
   x->parent = y;
}

void parco_rot_sx(parcoTree *T, parcoNode *x){
   parcoNode* y;
   y = x->dx;
   x->dx = y->sx;
   if(y->sx != T->Tnil){
      y->sx->parent = x;
   }
   y->parent = x->parent;
   if(x->parent == T->Tnil){
      T->root = y;
   }
   else{ 
      if(x == x->parent->sx){
         x->parent->sx = y;
      }else{
         x->parent->dx = y;
      }
   }
   y->sx = x;
   x->parent = y;
}

void staz_rot_dx(stazioneTree *T, stazioneNode *x){
   stazioneNode* y;
   y = x->sx;
   x->sx = y->dx;
   if(y->dx != T->Tnil){
      y->dx->parent = x;
   }
   y->parent = x->parent;
   if(x->parent == T->Tnil){
      T->root = y;
   }
   else{ 
      if(x == x->parent->dx){
         x->parent->dx = y;
      }else{
         x->parent->sx = y;
      }
   }
   y->dx = x;
   x->parent = y;
}

void parco_rot_dx(parcoTree *T, parcoNode *x){
   parcoNode* y;
   y = x->sx;
   x->sx = y->dx;
   if(y->dx != T->Tnil){
      y->dx->parent = x;
   }
   y->parent = x->parent;
   if(x->parent == T->Tnil){
      T->root = y;
   }
   else{ 
      if(x == x->parent->dx){
         x->parent->dx = y;
      }else{
         x->parent->sx = y;
      }
   }
   y->dx = x;
   x->parent = y;
}

void staz_insert_fixup(stazioneTree * T, stazioneNode * z){
   stazioneNode* x,*y;


   if(z == T->root){
      T->root->colore = b;
      printf("         ZEBBI-root");
   }else{
      x = z->parent; // x è parent di z
      if(x->colore == r){ // se parent è rosso
         printf("      nodo rosso");




         if(x == x->parent->sx){  // se x è figlio sinistro

            printf("     x è figlio sx");
            y = x->parent->dx;  // y = figlio destro del parent di x
            if(y->colore == r){   //se y è rosso


               printf(" caso 1");
               x->colore = b;
               y->colore = b;
               x->parent->colore = r;
               staz_insert_fixup(T,x->parent);


            }
            if(y->colore == b){ if(z == x->dx){


               printf(" caso 2");
               z = x;
               staz_rot_sx(T,z);
               x = z->parent;


            }


         x->colore = b;
         x->parent->colore = r;
         staz_rot_dx(T, x->parent);
         printf("caso  3");
            }


         }



/*
         if(x == x->parent->dx){

            printf("x è figlio dx");
            y = x->parent->sx;  // y = figlio sinistro del parent di x
            if(y->colore == r){   //se y è rosso


               printf(" caso 1");
               x->colore = b;
               y->colore = b;
               x->parent->colore = r;
               staz_insert_fixup(T,x->parent);


            }
            if(y->colore == b){ 

               if(z == x->sx){

                  printf(" caso 2");
                  z = x;
                  staz_rot_dx(T,z);
                  x = z->parent;


            }


         x->colore = b;
         x->parent->colore = r;
         staz_rot_sx(T, x->parent);
         printf("caso  3");
            
            
         }
      }*/
   }
}

}
void auto_insert_fixup(parcoTree * T, parcoNode * z){
   parcoNode* x,*y;
   if(z == T->root){
      T->root->colore = b;
   }else{
      x = z->parent;
      if(x->colore == r){
         if(x == x->parent->sx){
            y = x->parent->dx;
            if(y->colore == r){
               x->colore = b;
               y->colore = b;
               x->parent->colore = r;
               auto_insert_fixup(T,x->parent);
            }
            else if(z == x->dx){
               z = x;
               parco_rot_sx(T,z);
               x = z->parent;
            }
         x->colore = b;
         x->parent->colore = r;
         parco_rot_dx(T, x->parent);
         }else{
         y = x->parent->sx;
            if(y->colore == r){
               x->colore = b;
               y->colore = b;
               x->parent->colore = r;
               auto_insert_fixup(T,x->parent);
            }
            else if(z == x->sx){
               z = x;
               parco_rot_dx(T,z);
               x = z->parent;
            }
         x->colore = b;
         x->parent->colore = r;
         parco_rot_sx(T, x->parent);
         }
      }
   }
}

void staz_insert(stazioneTree* T, stazioneNode* z){
   stazioneNode *y,*x;
   y = T->Tnil;
   x = T->root;
   while( x != T->Tnil){
      y = x;
      if (z->distanza < x->distanza){
         x = x->sx;
      }else{
         x = x->dx;
      }
   }
   z->parent = y;

   if(y == T->Tnil){

      T->root = z;

   }else{

      if(z->distanza < y->distanza){

         y->sx = z;

      }else{

         y->dx = z;

      }
   }
   z->sx = T->Tnil;
   z->dx = T->Tnil;
   z->colore = r;
   //staz_insert_fixup(T,z);
   printf("\nstazione inserita");
}


void auto_insert(parcoTree* T, parcoNode* z){
   parcoNode *y,*x;
   y = T->Tnil;
   x = T->root;
   while( x != T->Tnil){
      y = x;
      if (z->autonomia < x->autonomia){
         x = x->sx;
      }else{
         x = x->dx;
      }
   }
   z->parent = y;

   if(y == T->Tnil){

      T->root = z;

   }else{

      if(z->autonomia < y->autonomia){

         y->sx = z;

      }else{

         y->dx = z;

      }
   }
   z->sx = T->Tnil;
   z->dx = T->Tnil;
   z->colore = r;
   //auto_insert_fixup(T,z);
   printf("\nauto con autonomia %d inserita nel parco auto", z->autonomia);
}

void staz_delete_fixup(stazioneTree *T, stazioneNode *x){
   stazioneNode *w;
   if(x->colore == r || x->parent == T->Tnil){
      x->colore = b;
   }else if(x == x->parent->sx){
      w = x->parent->dx;
      if(w->colore == r){
         w->colore = b;
         x->parent->colore = r;
         staz_rot_sx(T,x->parent);
         w = x->parent->dx;
      }
      if(w->sx->colore == b && w->dx->colore == b){
         w->colore = r;
         staz_delete_fixup(T,x->parent);
      }else if(w->dx->colore == b){
         w->sx->colore = b;
         w->colore = r;
         staz_rot_dx(T,w);
         w = x->parent->dx;
      }
      w->colore = x->parent->colore;
      x->parent->colore = b;
      w->dx->colore = b;
      staz_rot_sx(T,x->parent);
   }else{
      w = x->parent->sx;
      if(w->colore == r){
         w->colore = b;
         x->parent->colore = r;
         staz_rot_dx(T,x->parent);
         w = x->parent->sx;
      }
      if(w->dx->colore == b && w->sx->colore == b){
         w->colore = r;
         staz_delete_fixup(T,x->parent);
      }else if(w->sx->colore == b){
         w->dx->colore = b;
         w->colore = r;
         staz_rot_sx(T,w);
         w = x->parent->sx;
      }
      w->colore = x->parent->colore;
      x->parent->colore = b;
      w->sx->colore = b;
      staz_rot_dx(T,x->parent);
   }
}


void auto_delete_fixup(parcoTree *T, parcoNode *x){
   parcoNode *w;
   if(x->colore == r || x->parent == T->Tnil){
      x->colore = b;
   }else if(x == x->parent->sx){
      w = x->parent->dx;
      if(w->colore == r){
         w->colore = b;
         x->parent->colore = r;
         parco_rot_sx(T,x->parent);
         w = x->parent->dx;
      }
      if(w->sx->colore == b && w->dx->colore == b){
         w->colore = r;
         auto_delete_fixup(T,x->parent);
      }else if(w->dx->colore == b){
         w->sx->colore = b;
         w->colore = r;
         parco_rot_dx(T,w);
         w = x->parent->dx;
      }
      w->colore = x->parent->colore;
      x->parent->colore = b;
      w->dx->colore = b;
      parco_rot_sx(T,x->parent);
   }else{
      w = x->parent->sx;
      if(w->colore == r){
         w->colore = b;
         x->parent->colore = r;
         parco_rot_dx(T,x->parent);
         w = x->parent->sx;
      }
      if(w->dx->colore == b && w->sx->colore == b){
         w->colore = r;
         auto_delete_fixup(T,x->parent);
      }else if(w->sx->colore == b){
         w->dx->colore = b;
         w->colore = r;
         parco_rot_sx(T,w);
         w = x->parent->sx;
      }
      w->colore = x->parent->colore;
      x->parent->colore = b;
      w->sx->colore = b;
      parco_rot_dx(T,x->parent);
   }
}


// return y?
void staz_delete(stazioneTree *T, stazioneNode *z){
   stazioneNode *y,*x;
   if(z->sx == T->Tnil || z->dx == T->Tnil){
      y = z;
   }else{
      y = staz_successor(z,T);
   }
   if(y->sx != T->Tnil){
      x = y->sx;
   }
   else{
      x = y->dx;
      x->parent = y->parent;
   }
   if(y->parent == T->Tnil){
      T->root = x;
   }else if(y == y->parent->sx){
      y->parent->sx = x;
   }else{
      y->parent->dx = x;
   }
   if(y != z){
      z->distanza = y->distanza;
   }
   if(y->colore == b){
      staz_delete_fixup(T,x);
   }
}

void auto_delete(parcoTree *T, parcoNode *z){
   parcoNode *y,*x;
   if(z->sx == T->Tnil || z->dx == T->Tnil){
      y = z;
   }else{
      y = auto_successor(z,T);
   }
   if(y->sx != T->Tnil){
      x = y->sx;
   }
   else{
      x = y->dx;
      x->parent = y->parent;
   }
   if(y->parent == T->Tnil){
      T->root = x;
   }else if(y == y->parent->sx){
      y->parent->sx = x;
   }else{
      y->parent->dx = x;
   }
   if(y != z){
      z->autonomia = y->autonomia;
   }
   if(y->colore == b){
      auto_delete_fixup(T,x);
   }
   free(z);
}

stazioneNode* crea_stazione(int dist){
   stazioneNode* newstaz = (struct stazioneNode*)malloc(sizeof(struct stazioneNode));
   parcoTree* newparco = (struct parcoTree*)malloc(sizeof(struct parcoTree));
   newparco->Tnil = NULL;
   newparco->curr = newparco->Tnil;
   newparco->root = newparco->Tnil;
   newstaz->distanza = dist;
   newstaz->parent = NULL;
   newstaz->dx = NULL;
   newstaz->sx = NULL;
   newstaz->colore = b; // da verificare cosa convenga
   newstaz->autos = newparco;
   printf("\nstazione a %d e albero parco auto creati", newstaz->distanza);
   return newstaz;
}

parcoNode* crea_auto(int auton){
   parcoNode * newauto = (struct parcoNode*)malloc(sizeof(struct parcoNode));
   newauto->autonomia = auton;
   newauto->dx = NULL;
   newauto->sx = NULL;
   newauto->parent = NULL;
   newauto->colore = b;
   printf("\nauto creata con autonomia %d", newauto->autonomia);
   return newauto;
}

stazioneTree* crea_alberoStazioni(){
   stazioneTree* newStazTree = (struct stazioneTree*)malloc(sizeof(struct stazioneTree));
   newStazTree->Tnil = NULL;
   newStazTree->curr = newStazTree->Tnil;
   newStazTree->root = newStazTree->Tnil;
   return newStazTree;
}



int main(){
   stazioneTree* Stazioni;
   Stazioni = crea_alberoStazioni();
   stazioneNode* Staz_agg;
   parcoNode* auto_agg;
   bool check = false;
   char c = getchar();
   char *buffer, *comando, *valore;
   int i=0,counter=0,val,*valoricomando;
   valoricomando = malloc(sizeof(int));
   while(c != EOF){

      if(check == false){

      i=0;
      buffer = malloc(sizeof(char));

      while(c != ' ' && c != '\n'){
         i++;
         buffer = realloc(buffer,sizeof(char)*i);
         buffer[i-1] = c;
         c=getchar();
      }
      comando = malloc(sizeof(char)*(strlen(buffer)-1));
      strcpy(comando,buffer);
      free(buffer);
      check = true;

      }
      if(check == true){  

         if(c == ' '){
            c=getchar();
         }
         i=0;
         buffer = malloc(sizeof(char));

         while(c != ' ' && c != '\n'){
            i++;
            buffer = realloc(buffer,sizeof(char)*i);
            buffer[i-1] = c;
            c = getchar();
         }
         valore = malloc(sizeof(char)*(strlen(buffer)-1));
         strcpy(valore,buffer);
         free(buffer);
         val = atoi(valore);
         free(valore);
         valoricomando = realloc(valoricomando,sizeof(int)*(counter+1));
         valoricomando[counter] = val;     
        // printf( " %d",valoricomando[counter]);
         counter ++;
      }
      


      // fino  a qui tutto bene, aggiungere free al cambio linea e aggiunte valori nell'albero ||  ho i valori in valoricomando in ordine e il comando in comando

      
      

      if(c == '\n'){

         if(strcmp(comando , "aggiungi-stazione") == 0){
            /*for(int a=0; a<5;a++){
              printf( "%d ",valoricomando[a]);
            }*/
            int dist = valoricomando[0];
            int n_auto = valoricomando[1];
            Staz_agg = crea_stazione(dist);
            staz_insert(Stazioni, Staz_agg);
            for(int a=2; a < n_auto+2; a++){
              auto_agg = crea_auto(valoricomando[a]);
              auto_insert(Staz_agg->autos , auto_agg);
            }
      }


      if(strcmp(comando , "demolisci-stazione") == 0){
         printf("AAAAA");
      }


      if(strcmp(comando , "aggiungi-auto") == 0){
         printf("AAAAA");
      }


      if(strcmp(comando , "rottama-auto") == 0){
         printf("AAAAA");
      }


      if(strcmp(comando , "pianifica-percorso") == 0){



         printf("AAAAA");
      }

      free(valoricomando);
      valoricomando = malloc(sizeof(int));
      //free(comando);
     




      counter=0;
      check = false;


      // fino a qui dentro l'if di \n

      }
      /*POSTO PER CHIAMATA A FUNZIONI SUGLI ALBERI E FUNZIONI DEL RPOBLEMA
      
      DA DEFINIRE              */
     

    //  free(comando);
     // free(valoricomando);
     // counter=0;
      i=0;
      // mettere check a true quando becco \n
      // AGGIUNGERE FREE QUANDO BECCO \n
      c = getchar();
   }
   return 0;
};