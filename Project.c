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
   else if(x == x->parent->sx){
      x->parent->sx = y;
   }else{
      x->parent->dx = y;
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
   else if(x == x->parent->sx){
      x->parent->sx = y;
   }else{
      x->parent->dx = y;
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
   else if(x == x->parent->dx){
      x->parent->dx = y;
   }else{
      x->parent->sx = y;
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
   else if(x == x->parent->dx){
      x->parent->dx = y;
   }else{
      x->parent->sx = y;
   }
   y->dx = x;
   x->parent = y;
}

void staz_insert_fixup(stazioneTree * T, stazioneNode * z){
   stazioneNode* x,*y;
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
               staz_insert_fixup(T,x->parent);
            }
            else if(z == x->dx){
               z = x;
               staz_rot_sx(T,z);
               x = z->parent;
            }
         x->colore = b;
         x->parent->colore = r;
         staz_rot_dx(T, x->parent);
         }else{
         y = x->parent->sx;
            if(y->colore == r){
               x->colore = b;
               y->colore = b;
               x->parent->colore = r;
               staz_insert_fixup(T,x->parent);
            }
            else if(z == x->sx){
               z = x;
               staz_rot_dx(T,z);
               x = z->parent;
            }
         x->colore = b;
         x->parent->colore = r;
         staz_rot_sx(T, x->parent);
         }
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
   }else if(z->distanza < y->distanza){
      y->sx = z;
   }else{
      y->dx = z;
      z->sx = T->Tnil;
      z->dx = T->Tnil;
      z->colore = r;
   }
   staz_insert_fixup(T,z);
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
   }else if(z->autonomia < y->autonomia){
      y->sx = z;
   }else{
      y->dx = z;
      z->sx = T->Tnil;
      z->dx = T->Tnil;
      z->colore = r;
   }
   auto_insert_fixup(T,z);
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
}









int main(){
   for(int i=0;i<15;i++){
   char c=getchar();
   printf("Hai inserito il carattere: %c\n", c);}
   printf("n");
   return 0;
};