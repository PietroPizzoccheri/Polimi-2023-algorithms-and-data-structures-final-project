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
   while (x->curr->sx != NULL)
   {
      x->curr = x->curr->sx;
   }
   return x->curr;
}

//sempre settare curr = root
parcoNode* auto_min(parcoTree* x){
   while (x->curr->sx != NULL)
   {
      x->curr = x->curr->sx;
   }
   return x->curr;
}


//sempre settare curr = root
stazioneNode* staz_max(stazioneTree* x){
   while (x->curr->dx != NULL)
   {
      x->curr = x->curr->dx;
   }
   return x->curr;
}

//sempre settare curr = root
parcoNode* auto_max(parcoTree* x){
   while (x->curr->dx != NULL)
   {
      x->curr = x->curr->dx;
   }
   return x->curr;
}

stazioneNode* staz_successor(stazioneNode* x, stazioneTree* t){
   stazioneNode* y;
   if(x->dx != NULL){
      t->curr = x->dx;
      return staz_min(t);
   }
   y = x->parent;
   while ( y != NULL && x == y->dx) 
   {
      x = y;
      y = y->parent;
   }
   return y;
}

parcoNode* auto_successor(parcoNode* x, parcoTree* t){
   parcoNode* y;
   if(x->dx != NULL){
      t->curr = x->dx;
      return auto_min(t);
   }
   y = x->parent;
   while ( y != NULL && x == y->dx) 
   {
      x = y;
      y = y->parent;
   }
   return y;
}





int main(){
   for(int i=0;i<15;i++){
   char c=getchar();
   printf("Hai inserito il carattere: %c\n", c);}
   printf("n");
   return 0;
};