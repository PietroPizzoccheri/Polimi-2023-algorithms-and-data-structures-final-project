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



typedef struct parco
{
    colore_nodo_t colore;
    int autonomia;
    struct parco *sx, *dx, *parent;
} parcoTree_t;

struct parco* sentinel_parco = NULL;


typedef struct stazione
{
    colore_nodo_t colore;
    int distanza;
    struct stazione *sx, *dx, *parent;
    parcoTree_t *autos;
} stazioneTree_t;

struct stazione* sentinel_stazione = NULL;

stazioneTree_t* staz_search(stazioneTree_t* x, int dist){
   if (x == NULL || dist == x->distanza){
      return x;
   }
   if(dist < x->distanza){
      return staz_search(x->sx , dist);
   }
   else{
      return staz_search(x->dx , dist);
   }
}

parcoTree_t* auto_search(parcoTree_t* x, int dist){
   if (x == NULL || dist == x->autonomia){
      return x;
   }
   if(dist < x->autonomia){
      return auto_search(x->sx , dist);
   }
   else{
      return auto_search(x->dx , dist);
   }
}


void rot_sx_staz(stazioneTree_t* root, stazioneTree_t* x){
   stazioneTree_t *y;
   y = x->dx;
   x -> dx = y-> sx;
   if (y->sx != sentinel_stazione){
      y->sx->parent = x;
   }
   y->parent = x->parent ;
   if (x->parent == sentinel_stazione){
      root = y;
   }
   else{
      if(x == x->parent->sx){
         x->parent->sx = y;
      }
      else{
         x->parent->dx = y;
         
      }
   }
   y->sx = x;
   x->parent = y;
}


void rot_sx_parco(parcoTree_t* root, parcoTree_t* x){
   parcoTree_t *y;
     y = x->dx;
   x -> dx = y-> sx;
   if (y->sx != sentinel_parco){
      y->sx->parent = x;
   }
   y->parent = x->parent ;
   if (x->parent == sentinel_parco){
      root = y;
   }
   else{
      if(x == x->parent->sx){
         x->parent->sx = y;
      }
      else{
         x->parent->dx = y;
         
      }
   }
   y->sx = x;
   x->parent = y;
}


// modificare sotto per rotaz a dx


void rot_dx_staz(stazioneTree_t* root, stazioneTree_t* x){
   stazioneTree_t *y;
   y = x->sx;
   x -> sx = y-> dx;
   if (y->dx != sentinel_stazione){
      y->dx->parent = x;
   }
   y->parent = x->parent ;
   if (x->parent == sentinel_stazione){
      root = y;
   }
   else{
      if(x == x->parent->dx){
         x->parent->dx = y;
      }
      else{
         x->parent->sx = y;
         
      }
   }
   y->dx = x;
   x->parent = y;
}






void rot_dx_parco(parcoTree_t* root, parcoTree_t* x){
   parcoTree_t *y;
   y = x->sx;
   x -> sx = y-> dx;
   if (y->dx != sentinel_parco){
      y->dx->parent = x;
   }
   y->parent = x->parent ;
   if (x->parent == sentinel_parco){
      root = y;
   }
   else{
      if(x == x->parent->dx){
         x->parent->dx = y;
      }
      else{
         x->parent->sx = y;
         
      }
   }
   y->dx = x;
   x->parent = y;
}







void fixup_staz(stazioneTree_t* staz, stazioneTree_t* aggiunta)   
{
  




};








void fixup_parco(parcoTree_t* parco, parcoTree_t* nodo)   
{
  parcoTree_t *zio;
  
  while(nodo->parent->colore == r)
  {
     if(nodo->parent == nodo->parent->parent->sx)
     {
        zio = nodo->parent->parent->dx;
        if(zio->colore == r)
        {
           nodo->parent->colore = b;
           zio->colore = b;
           nodo->parent->parent->colore = r;
           nodo = nodo->parent->parent;           
        }
        else
        {
           if(nodo == nodo->parent->dx)
           {
              nodo = nodo->parent;
              rot_sx_parco(parco, nodo);
           }
           nodo->parent->colore = b;
           nodo->parent->parent->colore = r;
           rot_dx_parco(parco, nodo->parent->parent);           
        }
     }
     else
     {
        zio = nodo->parent->parent->sx;
        if(zio->colore == r)
        {
           nodo->parent->colore = b;
           zio->colore = b;
           nodo->parent->parent->colore = r;
           nodo = nodo->parent->parent;           
        }
        else
        {
           if(nodo == nodo->parent->sx)
           {
              nodo = nodo->parent;
              rot_dx_parco(parco, nodo);
           }
           nodo->parent->colore = b;
           nodo->parent->parent->colore = r;
           rot_sx_parco(parco, nodo->parent->parent);           
        }
     }
  }
  parco->sx->colore = b;  
};








int inserisci_stazione(stazioneTree_t* staz, int dist){
    int check = 0;
    stazioneTree_t *nodo, *parent, *new;

    for (nodo = staz->sx , parent = staz ; ((nodo!=staz)) ; parent = nodo , 
    nodo = (dist <= nodo->distanza)?   nodo->sx :nodo->dx);

    if(nodo != staz){
        check = 0;
    }
    else{
        check = 1;
        new = (stazioneTree_t*)malloc(sizeof(stazioneTree_t));
        new -> distanza = dist;
        new -> colore = r;
        new -> sx = new -> dx = staz;
        new -> parent = parent;

        if(parent == staz){
            staz -> sx  = staz -> dx = new;
        }
        else{
            if(dist < parent->distanza){
                parent -> sx = new;
            }
            else{
                parent -> dx = new;
            }
        }
        fixup_staz(staz , new);
    }
    return (check);
};



int inserisci_auto(parcoTree_t* staz, int dist){
    int check = 0;
    parcoTree_t *nodo, *parent, *new;

    for (nodo = staz->sx , parent = staz ; ((nodo!=staz)) ; parent = nodo , 
    nodo = (dist <= nodo->autonomia)?   nodo->sx :nodo->dx);

    if(nodo != staz){
        check = 0;
    }
    else{
        check = 1;
        new = (parcoTree_t*)malloc(sizeof(parcoTree_t));
        new -> autonomia = dist;
        new -> colore = r;
        new -> sx = new -> dx = staz;
        new -> parent = parent;

        if(parent == staz){
            staz -> sx  = staz -> dx = new;
        }
        else{
            if(dist < parent->autonomia){
                parent -> sx = new;
            }
            else{
                parent -> dx = new;
            }
        }
        fixup_parco(staz , new);
    }
    return (check);
};






int main(){
   for(int i=0;i<15;i++){
   char c=getchar();
   printf("Hai inserito il carattere: %c\n", c);}
   printf("n");
   return 0;
};