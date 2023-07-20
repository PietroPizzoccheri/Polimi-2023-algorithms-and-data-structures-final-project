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

//typedef enum {r,b} colore_nodo;
// 0 = black
// 1 = red



// nodo auto
typedef struct parcoNode
{
    int colore;
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
    int colore;
    int distanza;
    struct stazioneNode *sx, *dx, *parent;
    parcoTree* autos;
} stazioneNode;

//albero stazioni
typedef struct stazioneTree
{
   stazioneNode *root,*curr,*Tnil;

}stazioneTree;


stazioneNode* staz_search(stazioneTree* albero, int dist){
   stazioneNode *x = albero->root;
   while(x != albero->Tnil){
      if(x->distanza == dist){
         printf("\n returno x");
         printf(" ");
         return x;
         printf("\nqualcosa non va");
      }
      if(dist < x->distanza){
         printf("\nvado a sx\n");
         x = x->sx;
         printf("\ntrovo %d",x->distanza);
      }
      if(dist > x->distanza){
         printf("\nvado a dx da %d\n", x->distanza);
         x = x->dx;
         printf("\ntrovo %d",x->distanza);
      }
      printf("\n esco dagli if");
   }
   return albero->Tnil;
}


parcoNode* auto_search(parcoTree* albero, int auton){
   parcoNode *x = albero->root;
   while(x != albero->Tnil){
      if(x->autonomia == auton){
         return x;
      }
      if(auton < x->autonomia){
         x = x->sx;
      }
      if(auton > x->autonomia){
         x = x->dx;
      }
   }
   return albero->Tnil;
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
      T->root->colore = 0;
      printf("         ZEBBI-root");
   }else{
      x = z->parent; // x è parent di z
      if(x->colore == 1){ // se parent è rosso
         printf("      parent rosso");




         if(x == x->parent->sx){  // se x è figlio sinistro

            printf("     x è figlio sx");
            y = x->parent->dx;  // y = figlio destro del parent di x
            if(y->colore == 1){   //se y è rosso


               printf(" caso 1");
               x->colore = 0;
               y->colore = 0;
               x->parent->colore = 1;
               staz_insert_fixup(T,x->parent);


            }
            if(y->colore == 0){ 

               if(z == x->dx){

                  printf(" caso 2");
                  z = x; // qualcosa di strano qua
                  staz_rot_sx(T,z);
                  x = z->parent;

            }

         x->colore = 0;
         x->parent->colore = 1;
         staz_rot_dx(T, x->parent);
         printf("caso  3");
            }


         }


         if(x == x->parent->dx){

            printf("x è figlio dx");
            y = x->parent->sx;  // y = figlio sinistro del parent di x
            if(y->colore == 1){   //se y è rosso


               printf(" caso 1");
               x->colore = 0;
               y->colore = 0;
               x->parent->colore = 1;
               staz_insert_fixup(T,x->parent);


               }
               if(y->colore == 0){ 

                  if(z == x->sx){

                     printf(" caso 2");
                     z = x;
                     staz_rot_dx(T,z);
                     x = z->parent;


                  }


               x->colore = 0;
               x->parent->colore = 1;
               staz_rot_sx(T, x->parent);
               printf("caso  3");
            
            
               }
            }
         }
      }

   }


void auto_insert_fixup(parcoTree * T, parcoNode * z){ // sistemare
   parcoNode* x,*y;
   if(z == T->root){
      T->root->colore = 0;
   }else{
      x = z->parent;
      if(x->colore == 1){
         if(x == x->parent->sx){
            y = x->parent->dx;
            if(y->colore == 1){
               x->colore = 0;
               y->colore = 0;
               x->parent->colore = 1;
               auto_insert_fixup(T,x->parent);
            }
            else if(z == x->dx){
               z = x;
               parco_rot_sx(T,z);
               x = z->parent;
            }
         x->colore = 0;
         x->parent->colore = 1;
         parco_rot_dx(T, x->parent);
         }else{
         y = x->parent->sx;
            if(y->colore == 1){
               x->colore = 0;
               y->colore = 0;
               x->parent->colore = 1;
               auto_insert_fixup(T,x->parent);
            }
            else if(z == x->sx){
               z = x;
               parco_rot_dx(T,z);
               x = z->parent;
            }
         x->colore = 0;
         x->parent->colore = 1;
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
   z->colore = 1;
   staz_insert_fixup(T,z);
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
   z->colore = 1;
   //auto_insert_fixup(T,z);
   printf("\nauto con autonomia %d inserita nel parco auto", z->autonomia);
}

void staz_delete_fixup(stazioneTree *T, stazioneNode *x){
   stazioneNode *w;
   if(x->colore == 1 || x->parent == T->Tnil){
      x->colore = 0;
   }else if(x == x->parent->sx){
      w = x->parent->dx;
      if(w->colore == 1){
         w->colore = 0;
         x->parent->colore = 1;
         staz_rot_sx(T,x->parent);
         w = x->parent->dx;
      }
      if(w->sx->colore == 0 && w->dx->colore == 0){
         w->colore = 1;
         staz_delete_fixup(T,x->parent);
      }else if(w->dx->colore == 0){
         w->sx->colore = 0;
         w->colore = 1;
         staz_rot_dx(T,w);
         w = x->parent->dx;
      }
      w->colore = x->parent->colore;
      x->parent->colore = 0;
      w->dx->colore = 0;
      staz_rot_sx(T,x->parent);
   }else{
      w = x->parent->sx;
      if(w->colore == 1){
         w->colore = 0;
         x->parent->colore = 1;
         staz_rot_dx(T,x->parent);
         w = x->parent->sx;
      }
      if(w->dx->colore == 0 && w->sx->colore == 0){
         w->colore = 1;
         staz_delete_fixup(T,x->parent);
      }else if(w->sx->colore == 0){
         w->dx->colore = 0;
         w->colore = 1;
         staz_rot_sx(T,w);
         w = x->parent->sx;
      }
      w->colore = x->parent->colore;
      x->parent->colore = 0;
      w->sx->colore = 0;
      staz_rot_dx(T,x->parent);
   }
}


void auto_delete_fixup(parcoTree *T, parcoNode *x){
   parcoNode *w;
   if(x->colore == 1 || x->parent == T->Tnil){
      x->colore = 0;
   }else if(x == x->parent->sx){
      w = x->parent->dx;
      if(w->colore == 1){
         w->colore = 0;
         x->parent->colore = 1;
         parco_rot_sx(T,x->parent);
         w = x->parent->dx;
      }
      if(w->sx->colore == 0 && w->dx->colore == 0){
         w->colore = 1;
         auto_delete_fixup(T,x->parent);
      }else if(w->dx->colore == 0){
         w->sx->colore = 0;
         w->colore = 1;
         parco_rot_dx(T,w);
         w = x->parent->dx;
      }
      w->colore = x->parent->colore;
      x->parent->colore = 0;
      w->dx->colore = 0;
      parco_rot_sx(T,x->parent);
   }else{
      w = x->parent->sx;
      if(w->colore == 1){
         w->colore = 0;
         x->parent->colore = 1;
         parco_rot_dx(T,x->parent);
         w = x->parent->sx;
      }
      if(w->dx->colore == 0 && w->sx->colore == 0){
         w->colore = 1;
         auto_delete_fixup(T,x->parent);
      }else if(w->sx->colore == 0){
         w->dx->colore = 0;
         w->colore = 1;
         parco_rot_sx(T,w);
         w = x->parent->sx;
      }
      w->colore = x->parent->colore;
      x->parent->colore = 0;
      w->sx->colore = 0;
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
   if(y->colore == 0){
      staz_delete_fixup(T,x);
   }
   //free(z);
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
   if(y->colore == 0){
      auto_delete_fixup(T,x);
   }
   //free(z);
}

stazioneNode* crea_stazione( stazioneTree* x ,int dist ){  // memory leak
   struct stazioneNode* newstaz = malloc(sizeof(struct stazioneNode));
   struct parcoTree* newparco = malloc(sizeof(struct parcoTree));

   newparco->Tnil = malloc(sizeof(struct  parcoNode));

   newparco->Tnil->colore = 0;
   newparco->Tnil->autonomia = -10;
   newparco->Tnil->dx = newparco->Tnil;
   newparco->Tnil->sx = newparco->Tnil;
   newparco->Tnil->parent = newparco->Tnil;
   newparco->curr = newparco->Tnil;
   newparco->root = newparco->Tnil;


   newstaz->autos = newparco;
   newstaz->distanza = dist;
   newstaz->parent = x->Tnil;
   newstaz->dx = x->Tnil;
   newstaz->sx = x->Tnil;
   newstaz->colore = 0;

   //printf("\nstazione a %d e albero parco auto creati", newstaz->distanza);
   return newstaz;
}

parcoNode* crea_auto(parcoTree* x , int auton){
   struct parcoNode * newauto = malloc(sizeof(struct parcoNode));
   newauto->autonomia = auton;
   newauto->dx = x->Tnil;
   newauto->sx = x->Tnil;
   newauto->parent = x->Tnil;
   newauto->colore = 0;
   //printf("\nauto creata con autonomia %d", newauto->autonomia);
   return newauto;
}

stazioneTree* crea_alberoStazioni(){  // errore di segmentazione

   struct stazioneTree* newStazTree = malloc(sizeof(struct stazioneTree));

   newStazTree->Tnil = malloc(sizeof(struct stazioneNode));


   newStazTree->Tnil->colore = 0;
   newStazTree->Tnil->distanza = -10;
   newStazTree->Tnil->dx = newStazTree->Tnil;
   newStazTree->Tnil->sx = newStazTree->Tnil;
   newStazTree->Tnil->parent = newStazTree->Tnil;
   newStazTree->curr = newStazTree->Tnil;
   newStazTree->root = newStazTree->Tnil;
   return newStazTree;
}


void print_staz(stazioneNode *nil,stazioneNode *x){
   if(x != nil){
      print_staz(nil,x->sx);
      printf("\n%d ",x->distanza);
      printf("colore : %d",x->colore);
      print_staz(nil,x->dx);
   }
}


void print_auto(parcoNode *nil, parcoNode *x){
     if(x != nil){
      print_auto(nil,x->sx);
      printf("\n%d",x->autonomia);
      print_auto(nil,x->dx);
   }
}



int main(){
   stazioneTree* Stazioni;
   Stazioni = crea_alberoStazioni();
   Stazioni->Tnil->colore = b;
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
            Staz_agg = crea_stazione(Stazioni,dist);
            Staz_agg->autos->Tnil->colore = b;
            staz_insert(Stazioni, Staz_agg);
            for(int a=2; a < n_auto+2; a++){
              auto_agg = crea_auto(Staz_agg->autos, valoricomando[a]);
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