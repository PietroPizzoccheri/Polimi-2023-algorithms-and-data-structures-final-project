//Prova finale API PIZZOCCHERI 10797420

// librerie di libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// strutture dati 
//long contatore=0;
typedef enum {false,true} bool;

//typedef enum {r,b} colore_nodo;
// 0 = black
// 1 = red
bool found = false;

void quicksort(int a[],int primo,int ultimo){
   int i, j, pivot, temp;
   if(primo<ultimo){
      pivot=primo;
      i=primo;
      j=ultimo;     
      while(i<j){
         while(a[i]<=a[pivot]&&i<ultimo)
            i++;
         while(a[j]>a[pivot])
            j--;
         if(i<j){   
            temp=a[i];
            a[i]=a[j];
            a[j]=temp;
         }
      }
      temp=a[pivot];
      a[pivot]=a[j];
      a[j]=temp;
      quicksort(a,primo,j-1);
      quicksort(a,j+1,ultimo);
   }
}

// nodo auto

//nodo stazione
typedef struct stazioneNode
{
    int colore;
    int distanza, auto_presenti;
    int parco_auto[700];
    struct stazioneNode *sx, *dx, *parent;
} stazioneNode;

//albero stazioni
typedef struct stazioneTree
{
   stazioneNode *root,*Tnil;
}stazioneTree;

stazioneNode* staz_search(stazioneTree* albero, int dist){
   stazioneNode *x = albero->root;

   while(x != albero->Tnil){
      if(x->distanza == dist){
         //printf("\n returno x");
         return x;
         //printf("\nqualcosa non va");
      }
      if(dist < x->distanza){
         //printf("\nvado a sx\n");
         x = x->sx;
         //printf("\ntrovo %d",x->distanza);
      }
      if(dist > x->distanza){
         //printf("\nvado a dx da %d\n", x->distanza);
         x = x->dx;
         //printf("\ntrovo %d",x->distanza);
      }
      //printf("\nesco dagli if");
   }
   return albero->Tnil;
}

stazioneNode* staz_min(stazioneTree* T , stazioneNode *x){
   while (x->sx != T->Tnil){
      x = x->sx;
   }
   return x;
}


stazioneNode* staz_successor(stazioneNode* x, stazioneTree* t){
   stazioneNode* y;
   if(x->dx != t->Tnil){
      return staz_min(t , x->dx);
   }
   y = x->parent;
   while ( y != t->Tnil && x == y->dx) 
   {
      x = y;
      y = y->parent;
   }

   return y;
}


void staz_rot_sx(stazioneTree *T, stazioneNode *x){
   stazioneNode* y = x->dx;
   x->dx = y->sx;
   if(y->sx != T->Tnil) {
     y->sx->parent = x;
   }
   y->parent = x->parent;
   if(x->parent == T->Tnil) { 
     T->root = y;
   }
   else if(x == x->parent->sx) { 
     x->parent->sx = y;
   }
   else { 
     x->parent->dx = y;
   }
   y->sx = x;
   x->parent = y;
}


void staz_rot_dx(stazioneTree *T, stazioneNode *x){
   stazioneNode* y = x->sx;
   x->sx = y->dx;
   if(y->dx != T->Tnil) {
     y->dx->parent = x;
   }
   y->parent = x->parent;
   if(x->parent == T->Tnil) { 
      T->root = y;
   }
   else if(x == x->parent->dx) { 
      x->parent->dx = y;
   }
   else { 
      x->parent->sx = y;
   }
   y->dx = x;
   x->parent = y;
}


void staz_insert_fixup(stazioneTree * T, stazioneNode * z){
   stazioneNode* x,*y;
   if(z == T->root){
      T->root->colore = 0;
   }else{
      x = z->parent; // x è parent di z
      if(x->colore == 1){ // se parent è rosso
         if(x == x->parent->sx){  // se x è figlio sinistro
            y = x->parent->dx;  // y = figlio destro del parent di x
            if(y->colore == 1){   //caso 1
               x->colore = 0;
               y->colore = 0;
               x->parent->colore = 1;
               staz_insert_fixup(T,x->parent);
            }else{
               if(z == x->dx){//caso 2
                  z = x;
                  staz_rot_sx(T,z);
                  x = z->parent;
               }
               x->colore = 0; // caso 3
               x->parent->colore = 1;
               staz_rot_dx(T , x->parent);
            }
         }else{
            y = x->parent->sx;  // y = figlio sinistro del parent di x
            if(y->colore == 1){   //se y è rosso
               x->colore = 0;
               y->colore = 0;
               x->parent->colore = 1;
               staz_insert_fixup(T,x->parent);
            }else{
               if( z == x->sx){
                  z = x;
                  staz_rot_dx(T,z);
                  x = z->parent;
               }
               x->colore = 0;
               x->parent->colore = 1;
               staz_rot_sx(T , x->parent);
            }
         }
      }
   }
}



void staz_insert(stazioneTree* T, stazioneNode* z){
   stazioneNode *y,*x;
   //printf("\nnodo da inserire %d", z->distanza);
   //printf("\nparent nodo da inserire %d", z->parent->distanza);
   //printf("\nsx nodo da inserire %d", z->sx->distanza);
   //printf("\ndx nodo da inserire %d", z->dx->distanza);
   y = T->Tnil;
   x = T->root;
   while( x != T->Tnil){
      y = x;
      if (z->distanza < x->distanza){
         x = x->sx;
         //printf("\nvado a sx da %d e mi ritrovo a %d",y->distanza,x->distanza);
      }else{
         x = x->dx;
         //printf("\nvado a dx da %d e mi ritrovo a %d",y->distanza,x->distanza);
      }
      if (z->distanza == x->distanza){
         //printf("\nnon aggiunta\n");
         return;
      }
   }
   z->parent = y;
   if(y == T->Tnil){
      T->root = z;
      //printf("\ninserisco radice %d", z->distanza);
   }else{
      if(z->distanza < y->distanza){
         y->sx = z;
         //printf("\n%d diventa figlio sx di %d",z->distanza,y->distanza);
      }else{
         y->dx = z;
         //printf("\n%d diventa figlio dx di %d",z->distanza,y->distanza);
      }
   }
   z->sx = T->Tnil;
   //printf("\nfiglio sinistro del nuovo nodo %d e' %d",z->distanza,z->sx->distanza);
   z->dx = T->Tnil;
   //printf("\nfiglio destro del nuovo nodo %d e' %d",z->distanza,z->dx->distanza);
   z->colore = 1;
   //printf("\nparent del nuovo nodo %d e' %d",z->distanza,z->parent->distanza);
   staz_insert_fixup(T,z);
   //printf("\naggiunta\n");
}


void staz_delete_fixup(stazioneTree *T, stazioneNode *x){
   stazioneNode *w;
   if(x->colore == 1 || x->parent == T->Tnil){
      x->colore = 0; //caso 0
   }else{ 
      if(x == x->parent->sx){
         w = x->parent->dx;
         if(w->colore == 1){
            w->colore = 0;
            x->parent->colore = 1;
            staz_rot_sx(T,x->parent);
            w = x->parent->dx; //caso 1
         }
         if(w->sx->colore == 0 && w->dx->colore == 0){
            w->colore = 1;
            staz_delete_fixup(T,x->parent); //caso 2
         }else{ 
            if(w->dx->colore == 0){
               w->sx->colore = 0;
               w->colore = 1;
               staz_rot_dx(T,w);
               w = x->parent->dx; //caso 3
            }
         w->colore = x->parent->colore;
         x->parent->colore = 0;
         w->dx->colore = 0;
         staz_rot_sx(T,x->parent); //caso 4
         }
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
         }else{ 
            if(w->sx->colore == 0){
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
   }
}


void staz_delete(stazioneTree *T, stazioneNode *z);



void delete_staz(stazioneTree *T, stazioneNode *x){ // problemi qua
     if(x == T->Tnil){
      return;
   }else{
      delete_staz(T,x->sx);
      delete_staz(T,x->dx);
      staz_delete(T,x);
      //printf("debug");
   }
}

void staz_delete(stazioneTree *T, stazioneNode *z){
   stazioneNode *y,*x;
   if(z->sx == T->Tnil || z->dx == T->Tnil){
      y = z;
      //printf("\n HA UN FIGLIO O NON NE HA");
   }else{
      //printf("\n qualcosa non va");
      y = staz_successor(z,T);
   }
   if(y->sx != T->Tnil){
      x = y->sx;
   }
   else{
      //printf("\n ok");
      x = y->dx;
   }if(x != T->Tnil){
      x->parent = y->parent;
   }
   if(y->parent == T->Tnil){
      T->root = x;
   }else{
      if(y == y->parent->sx){
         y->parent->sx = x;
      }else{
        // printf("\n ok");
         y->parent->dx = x;
      }
   }
   if(y != z){
      z->distanza = y->distanza;
   }
   if(y->colore == 0){
      //printf("\n faccio fixup");
      staz_delete_fixup(T,x);
   }
   //printf("stazione %d demolita\n",y->distanza);
   free(y);
   //contatore = contatore - 1;
   //printf("\nstazione rimossa leaks : %li",contatore);
}





stazioneNode* crea_stazione( stazioneTree* x ,int dist ){  // memory leak
   struct stazioneNode* newstaz = malloc(sizeof(struct stazioneNode));
   //contatore = contatore + 1;
   //printf("\naggiungo stazione di dimensioni %ld leaks : %li",sizeof(*newstaz),contatore);
   newstaz->distanza = dist;
   newstaz->parent = x->Tnil;
   newstaz->dx = x->Tnil;
   newstaz->sx = x->Tnil;
   newstaz->colore = 1;
   newstaz->auto_presenti = 0;
   for( int a = 0 ; a<700 ; a++){
      newstaz->parco_auto[a] = -1;
   }

   //printf("\nstazione a %d e albero parco auto creati", newstaz->distanza);
   return newstaz;
}



stazioneTree* crea_alberoStazioni(){  // errore di segmentazione

   struct stazioneTree* newStazTree = malloc(sizeof(struct stazioneTree));
   newStazTree->Tnil =  malloc(sizeof(struct stazioneNode));
   //contatore = contatore + 2;
   //printf("\naggiungo albero di dimensioni %ld staz leaks : %li : \n",sizeof(*newStazTree),contatore);
   newStazTree->Tnil->colore = 1;
   newStazTree->Tnil->distanza = -10;
   newStazTree->Tnil->dx = newStazTree->Tnil;
   newStazTree->Tnil->sx = newStazTree->Tnil;
   newStazTree->Tnil->parent = newStazTree->Tnil;
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


void pianifica_cresc(stazioneNode *inizio , stazioneNode *fine , stazioneTree * albero){
   stazioneNode *tappa, *buffer;
   tappa = inizio;
   //for(int i = 0; i<tappa->auto_presenti ; i++){
    //  printf("%d ",tappa->parco_auto[i]);
  // }
   int tappe[1000], tappecounter = 0;
   int autonomia;
   int max;
   tappe[tappecounter] = tappa->distanza;
   tappecounter++;
label:  autonomia = tappa->parco_auto[tappa->auto_presenti-1];
//printf("\n%d", autonomia);
   if(  (fine->distanza - tappa->distanza)  < autonomia){
      tappe[tappecounter] = fine->distanza;
      tappecounter++;
      for(int i = 0 ; i < tappecounter-1 ; i++){
         // funzione per trovare stazione ottimale
      }
      for(int i = 0 ; i < tappecounter-1 ; i++){
         printf("%d ",tappe[i]);
      }
      printf("%d",tappe[tappecounter-1]);
      printf("\n");
      return;
   }else{
      max = tappa->distanza + autonomia;
      for( int i = max ; i > tappa->distanza ; i--){
         buffer = staz_search(albero , i);
         if(buffer != albero->Tnil){
            if( (buffer->distanza - tappa->distanza) <= autonomia){
               tappa = buffer;
               tappe[tappecounter] = tappa->distanza;
               tappecounter++;
               goto label;
            }
         }
      }
      printf("nessun percorso\n");
      return;
   }

}




void pianifica_decresc(stazioneNode *inizio , stazioneNode *fine , stazioneTree * albero){
   stazioneNode *tappa, *buffer;
   tappa = inizio;
   int tappe[1000], tappecounter = 0;
   int autonomia;
   tappe[tappecounter] = tappa->distanza;
   tappecounter++;

label:  

autonomia = tappa->parco_auto[tappa->auto_presenti-1];
   if(  (tappa->distanza - fine->distanza)  < autonomia){ //arrivo alla fine

      tappe[tappecounter] = fine->distanza;
      tappecounter++;
      for(int i = 0 ; i < tappecounter-1 ; i++){
         // funzione per trovare stazione ottimale
      }
      for(int i = 0 ; i < tappecounter-1 ; i++){
         printf("%d ",tappe[i]);
      }
      printf("%d",tappe[tappecounter-1]);
      printf("\n");
      return;

   }else{  // tappe intermedie

      for( int i = (tappa->distanza - autonomia); i < tappa->distanza ; i++){
         buffer = staz_search(albero , i);
         if(buffer != albero->Tnil){
            if( (buffer->distanza - tappa->distanza) <= autonomia){
               tappa = buffer;
               tappe[tappecounter] = tappa->distanza;
               tappecounter++;
               goto label;
            }
         }
      }
      printf("nessun percorso\n");
      return;

   }

}


/////////////////////////////////////////////////////////////////////////////////////////////////////





int main(){

   stazioneTree* Stazioni;
   Stazioni = crea_alberoStazioni();
   stazioneNode* Staz_agg , *Staz_buff;
   bool check = false;
   char c = getchar();
   char input[10000] , comando[20], buffer[5000];
   int valori[514], val =0 ,contaauto =0;
   int i=0 , count=0 , buf=0;
    int inizio,fine;

   //int trov, conto = 0;
   //int percorso [1000];

   while(c != EOF){

      while(c != '\n'){
         input[i] = c;
         i++;
         c = getchar();
      } // salvo una riga di input
      //printf("\ninput : %s\n",input);
      i=0;

      while( count < strlen(input)){ // passo la riga di input

         if(check == false){
            while(input[count] != ' ' ){
               comando[count] = input[count];
               count ++;
            }
            //printf("\ncomando :%s ",comando);


            while(check == false){
               count++;

               while(input[count] != ' ' && input[count] != '\0'){
                  buffer[buf] = input[count];
                  buf++;
                  count++;
               }

               buf=0;
               val = atoi(buffer);
               memset(buffer , 0 , sizeof(buffer));
               valori[i]=val;
               i++;

               if(input[count] == '\0'){
                  check = true;
               }
            }
            /*printf(" valori : ");
            for(int a=0 ; a<i; a++){
               
               printf(" %d",valori[a]);
            }*/
         }
      }
      i=0;
      count=0;


      
      
     
      if(c == '\n'){
         
         if(strcmp(comando , "aggiungi-stazione") == 0){
            Staz_agg = staz_search(Stazioni , valori[0]);
            if( Staz_agg == Stazioni->Tnil){
               Staz_agg = crea_stazione(Stazioni,valori[0]);
               staz_insert(Stazioni, Staz_agg);
               contaauto = 0;
               for( int x=2; x < valori[1]+2 ; x++){
                  Staz_agg->parco_auto[contaauto] = valori[x];
                  contaauto++;
               }
               //Staz_agg->parco_auto[512] = 0;
               Staz_agg->auto_presenti = valori[1];
               contaauto = 0;
               quicksort(Staz_agg->parco_auto , 0 , Staz_agg->auto_presenti-1);
               printf("aggiunta\n");
            }else{
               printf("non aggiunta\n");
            }
         }





         if(strcmp(comando , "demolisci-stazione") == 0){
            Staz_agg = staz_search(Stazioni, valori[0]);
            //printf("\ntrovata : %d\n",Staz_agg->distanza);
            if(Staz_agg == Stazioni->Tnil){
               printf("non demolita\n");
            }else{
               staz_delete(Stazioni , Staz_agg);
               printf("demolita\n");
            }
            
         }
         

         if(strcmp(comando , "aggiungi-auto") == 0){
            Staz_agg = staz_search(Stazioni, valori[0]);
            if(Staz_agg == Stazioni->Tnil){
               printf("non aggiunta\n");
            }else{
               if( Staz_agg->auto_presenti == 0){
                  Staz_agg->parco_auto[0] = valori[1];
                  Staz_agg->auto_presenti = 1;
                  printf("aggiunta\n");
               }else{
                  Staz_agg->parco_auto[Staz_agg->auto_presenti] = valori[1];
                  ++Staz_agg->auto_presenti;
                  quicksort(Staz_agg->parco_auto , 0 , Staz_agg->auto_presenti-1);
                  printf("aggiunta\n");
               }
            }
         }
         

         if(strcmp(comando , "rottama-auto") == 0){
            Staz_agg = staz_search(Stazioni, valori[0]);
            if(Staz_agg == Stazioni->Tnil){
               printf("non rottamata\n");
            }else{
               for(int a =0 ; a < 700 ; a++){
                  
                  //printf(" %d", Staz_agg->parco_auto[a]);
                  if(valori[1] == Staz_agg->parco_auto[a]){
                     Staz_agg->parco_auto[a] = __INT_MAX__;
                     quicksort(Staz_agg->parco_auto , 0 , Staz_agg->auto_presenti-1);
                     --Staz_agg->auto_presenti;
                     printf("rottamata\n");
                     break;
                  }else{
                     if(Staz_agg->parco_auto[a] == -1 ){
                        printf("non rottamata\n");
                        break;
                     }
                     if( a == 698 && Staz_agg->parco_auto[a] != valori[1] ){
                        printf("non rottamata\n");
                        break;
                     }
                  }
               }

               // AGGIUNGERE NON ROTTAMATA SE NON TROVATA IN ARRAY
            }
            //printf("\n");
            // FORSE QUI
         }


         if(strcmp(comando , "pianifica-percorso") == 0){
            Staz_agg = staz_search(Stazioni , valori[0]);
            Staz_buff = staz_search(Stazioni , valori[1]);
            if(Staz_agg == Stazioni->Tnil){
               printf("nessun percorso\n");
            }else{
               if(Staz_buff == Stazioni->Tnil){
                  printf("nessun percorso\n");
               }else{

                  inizio = Staz_agg->distanza;
                  fine = Staz_buff->distanza;

                  if(inizio == fine){
                     printf("%d\n",inizio);
                  }

                  if(inizio < fine){
                     pianifica_cresc(Staz_agg , Staz_buff , Stazioni);
                  }else{
                     pianifica_decresc(Staz_agg , Staz_buff , Stazioni);
                  }
               }
            } 
         }

      // fino a qui dentro l'if di \n /////////////////////////////////////////

      }
      check = false;
      memset(input , 0 , sizeof(input));
      memset(comando , 0 , sizeof(comando));
      memset(valori , 0 , sizeof(valori));
   
      c = getchar();
      
   }
   delete_staz(Stazioni , Stazioni->root);   // problema utilizzo memoria non sta qui
   free(Stazioni->Tnil);
   free(Stazioni);
   //contatore = contatore - 2;
   //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
   return 0;
};