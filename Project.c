// Prova finale API PIZZOCCHERI 10797420

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __INT_MAX__ 2147483647

typedef enum
{
   false,
   true
} bool;

int array_stazioni[56000];
int conta_stazioni;

void print_array()
{
   for (int i = 0; i < conta_stazioni; i++)
      printf("%d ", array_stazioni[i]);
   printf("\n");
}

int trova_array_staz(int n)
{
   for (int i = 0; i < conta_stazioni; i++)
   {
      if (array_stazioni[i] == n)
      {
         return n;
      }
      if (array_stazioni[i] == __INT_MAX__)
      {
         return -1;
      }
   }
   return -1;
}

int trova_indice_array_staz(int n)
{
   for (int i = 0; i < conta_stazioni; i++)
   {
      if (array_stazioni[i] == n)
      {
         return i;
      }
      if (array_stazioni[i] == __INT_MAX__)
      {
         return -1;
      }
   }
   return -1;
}

void merge_array_stazioni(int l, int m, int r)
{
   int i, j, k;
   int n1 = m - l + 1;
   int n2 = r - m;
   int *L = (int *)malloc(n1 * sizeof(int));
   int *R = (int *)malloc(n2 * sizeof(int));
   for (i = 0; i < n1; i++)
   {
      L[i] = array_stazioni[l + i];
   }
   for (j = 0; j < n2; j++)
   {
      R[j] = array_stazioni[m + 1 + j];
   }
   i = 0;
   j = 0;
   k = l;
   while (i < n1 && j < n2)
   {
      if (L[i] <= R[j])
      {
         array_stazioni[k] = L[i];
         i++;
      }
      else
      {
         array_stazioni[k] = R[j];
         j++;
      }
      k++;
   }
   while (i < n1)
   {
      array_stazioni[k] = L[i];
      i++;
      k++;
   }
   while (j < n2)
   {
      array_stazioni[k] = R[j];
      j++;
      k++;
   }
   free(L);
   free(R);
}

void mergeSort_array_stazioni(int l, int r)
{
   if (l < r)
   {
      int m = l + (r - l) / 2;
      mergeSort_array_stazioni(l, m);
      mergeSort_array_stazioni(m + 1, r);
      merge_array_stazioni(l, m, r);
   }
}

void delete_array_stazioni(int n)
{
   for (int i = 0; i < conta_stazioni; i++)
   {
      if (array_stazioni[i] == n)
      {
         array_stazioni[i] = __INT_MAX__;
         mergeSort_array_stazioni(0, conta_stazioni - 1);
         conta_stazioni--;

         return;
      }
   }
}

bool found = false;
bool scambi = true;

int findmax(int a[])
{
   int max = a[0];
   for (int i = 0; i < 512; i++)
   {
      if (a[i] > max)
      {
         max = a[i];
      }
   }
   return max;
}

typedef struct stazioneNode
{
   int colore;
   int distanza, auto_presenti;
   int parco_auto[512];
   struct stazioneNode *sx, *dx, *parent;
} stazioneNode;

typedef struct stazioneTree
{
   stazioneNode *root, *Tnil;
} stazioneTree;

stazioneNode *staz_search(stazioneTree *albero, int dist)
{
   stazioneNode *x = albero->root;

   while (x != albero->Tnil)
   {
      if (x->distanza == dist)
      {

         return x;
      }
      if (dist < x->distanza)
      {

         x = x->sx;
      }
      if (dist > x->distanza)
      {

         x = x->dx;
      }
   }
   return albero->Tnil;
}

stazioneNode *debug;

stazioneNode *staz_min(stazioneTree *T, stazioneNode *x)
{
   while (x->sx != T->Tnil)
   {
      x = x->sx;
   }
   return x;
}

stazioneNode *staz_successor(stazioneNode *x, stazioneTree *t)
{
   stazioneNode *y;
   if (x->dx != t->Tnil)
   {
      return staz_min(t, x->dx);
   }
   y = x->parent;
   while (y != t->Tnil && x == y->dx)
   {
      x = y;
      y = y->parent;
   }

   return y;
}

void staz_rot_sx(stazioneTree *T, stazioneNode *x)
{
   stazioneNode *y = x->dx;
   x->dx = y->sx;
   if (y->sx != T->Tnil)
   {
      y->sx->parent = x;
   }
   y->parent = x->parent;
   if (x->parent == T->Tnil)
   {
      T->root = y;
   }
   else if (x == x->parent->sx)
   {
      x->parent->sx = y;
   }
   else
   {
      x->parent->dx = y;
   }
   y->sx = x;
   x->parent = y;
}

void staz_rot_dx(stazioneTree *T, stazioneNode *x)
{
   stazioneNode *y = x->sx;
   x->sx = y->dx;
   if (y->dx != T->Tnil)
   {
      y->dx->parent = x;
   }
   y->parent = x->parent;
   if (x->parent == T->Tnil)
   {
      T->root = y;
   }
   else if (x == x->parent->dx)
   {
      x->parent->dx = y;
   }
   else
   {
      x->parent->sx = y;
   }
   y->dx = x;
   x->parent = y;
}

void staz_insert_fixup(stazioneTree *T, stazioneNode *z)
{
   stazioneNode *x, *y;
   if (z == T->root)
   {
      T->root->colore = 0;
   }
   else
   {
      x = z->parent;
      if (x->colore == 1)
      {
         if (x == x->parent->sx)
         {
            y = x->parent->dx;
            if (y->colore == 1)
            {
               x->colore = 0;
               y->colore = 0;
               x->parent->colore = 1;
               staz_insert_fixup(T, x->parent);
            }
            else
            {
               if (z == x->dx)
               {
                  z = x;
                  staz_rot_sx(T, z);
                  x = z->parent;
               }
               x->colore = 0;
               x->parent->colore = 1;
               staz_rot_dx(T, x->parent);
            }
         }
         else
         {
            y = x->parent->sx;
            if (y->colore == 1)
            { // se y è rosso
               x->colore = 0;
               y->colore = 0;
               x->parent->colore = 1;
               staz_insert_fixup(T, x->parent);
            }
            else
            {
               if (z == x->sx)
               {
                  z = x;
                  staz_rot_dx(T, z);
                  x = z->parent;
               }
               x->colore = 0;
               x->parent->colore = 1;
               staz_rot_sx(T, x->parent);
            }
         }
      }
   }
}

void staz_insert(stazioneTree *T, stazioneNode *z)
{
   stazioneNode *y, *x;

   y = T->Tnil;
   x = T->root;
   while (x != T->Tnil)
   {
      y = x;
      if (z->distanza < x->distanza)
      {
         x = x->sx;
      }
      else
      {
         x = x->dx;
      }
      if (z->distanza == x->distanza)
      {

         return;
      }
   }
   z->parent = y;
   if (y == T->Tnil)
   {
      T->root = z;
   }
   else
   {
      if (z->distanza < y->distanza)
      {
         y->sx = z;
      }
      else
      {
         y->dx = z;
      }
   }
   z->sx = T->Tnil;

   z->dx = T->Tnil;

   z->colore = 1;

   staz_insert_fixup(T, z);
}

void staz_delete_fixup(stazioneTree *T, stazioneNode *x)
{
   stazioneNode *w;
   if (x->colore == 1 || x->parent == T->Tnil)
   {
      x->colore = 0;
   }
   else
   {
      if (x == x->parent->sx)
      {
         w = x->parent->dx;
         if (w->colore == 1)
         {
            w->colore = 0;
            x->parent->colore = 1;
            staz_rot_sx(T, x->parent);
            w = x->parent->dx;
         }
         if (w->sx->colore == 0 && w->dx->colore == 0)
         {
            w->colore = 1;
            staz_delete_fixup(T, x->parent);
         }
         else
         {
            if (w->dx->colore == 0)
            {
               w->sx->colore = 0;
               w->colore = 1;
               staz_rot_dx(T, w);
               w = x->parent->dx;
            }
            w->colore = x->parent->colore;
            x->parent->colore = 0;
            w->dx->colore = 0;
            staz_rot_sx(T, x->parent);
         }
      }
      else
      {
         w = x->parent->sx;
         if (w->colore == 1)
         {
            w->colore = 0;
            x->parent->colore = 1;
            staz_rot_dx(T, x->parent);
            w = x->parent->sx;
         }
         if (w->dx->colore == 0 && w->sx->colore == 0)
         {
            w->colore = 1;
            staz_delete_fixup(T, x->parent);
         }
         else
         {
            if (w->sx->colore == 0)
            {
               w->dx->colore = 0;
               w->colore = 1;
               staz_rot_sx(T, w);
               w = x->parent->sx;
            }
            w->colore = x->parent->colore;
            x->parent->colore = 0;
            w->sx->colore = 0;
            staz_rot_dx(T, x->parent);
         }
      }
   }
}

void staz_delete(stazioneTree *T, stazioneNode *z);

void delete_staz(stazioneTree *T, stazioneNode *x)
{
   if (x == T->Tnil)
   {
      return;
   }
   else
   {
      delete_staz(T, x->sx);
      delete_staz(T, x->dx);
      staz_delete(T, x);
   }
}

void staz_delete(stazioneTree *T, stazioneNode *z)
{
   stazioneNode *y, *x;
   if (z->sx == T->Tnil || z->dx == T->Tnil)
   {
      y = z;
   }
   else
   {

      y = staz_successor(z, T);
   }
   if (y->sx != T->Tnil)
   {
      x = y->sx;
   }
   else
   {

      x = y->dx;
   }
   if (x != T->Tnil)
   {
      x->parent = y->parent;
   }
   if (y->parent == T->Tnil)
   {
      T->root = x;
   }
   else
   {

      if (y == y->parent->sx)
      {
         y->parent->sx = x;
      }
      else
      {

         y->parent->dx = x;
      }
   }
   if (y != z)
   {
      z->distanza = y->distanza;
      z->auto_presenti = y->auto_presenti;
      for (int i = 0; i < 512; i++)
      {
         z->parco_auto[i] = y->parco_auto[i];
      }
   }
   if (y->colore == 0)
   {

      staz_delete_fixup(T, x);
   }

   free(y);
}

stazioneNode *crea_stazione(stazioneTree *x, int dist)
{
   struct stazioneNode *newstaz = malloc(sizeof(struct stazioneNode));

   newstaz->distanza = dist;
   newstaz->parent = x->Tnil;
   newstaz->dx = x->Tnil;
   newstaz->sx = x->Tnil;
   newstaz->colore = 1;
   newstaz->auto_presenti = 0;
   for (int a = 0; a < 512; a++)
   {
      newstaz->parco_auto[a] = -1;
   }

   return newstaz;
}

stazioneTree *crea_alberoStazioni()
{
   struct stazioneTree *newStazTree = malloc(sizeof(struct stazioneTree));
   newStazTree->Tnil = malloc(sizeof(struct stazioneNode));

   newStazTree->Tnil->colore = 1;
   newStazTree->Tnil->distanza = -10;
   newStazTree->Tnil->dx = newStazTree->Tnil;
   newStazTree->Tnil->sx = newStazTree->Tnil;
   newStazTree->Tnil->parent = newStazTree->Tnil;
   newStazTree->root = newStazTree->Tnil;
   return newStazTree;
}

void print_staz(stazioneNode *nil, stazioneNode *x)
{
   if (x != nil)
   {
      print_staz(nil, x->sx);
      printf("\n%d ", x->distanza);
      printf("colore : %d", x->colore);
      print_staz(nil, x->dx);
   }
}

int tappe[50000], tappecounter = 0;

void pianifica_cresc(stazioneNode *inizio, stazioneNode *fine, stazioneTree *albero)
{
   mergeSort_array_stazioni(0, conta_stazioni - 1);

   stazioneNode *tappa, *buffer;

   tappa = fine;

   int autonomia;
   // int max;
   int pick = -1;
   int indice_inizio = trova_indice_array_staz(inizio->distanza);
   int staz_buf = 0;
   int salva_indice = conta_stazioni;
   tappe[tappecounter] = tappa->distanza;
   tappecounter++;

label:
   autonomia = findmax(inizio->parco_auto);

   if (tappa->distanza - inizio->distanza <= autonomia)
   {
      tappe[tappecounter] = inizio->distanza;
      tappecounter++;
      goto print;
   }
   else
   {
      for (int i = indice_inizio; i <= salva_indice - 1; i++)
      {
         staz_buf = array_stazioni[i];
         buffer = staz_search(albero, staz_buf);
         if (buffer != albero->Tnil)
         {
            autonomia = findmax(buffer->parco_auto);
            if (tappa->distanza - buffer->distanza <= autonomia)
            {
               pick = buffer->distanza;
               salva_indice = i;
               break;
            }
         }
      }

      if (pick == -1)
      {
         printf("nessun percorso\n");
         tappecounter = 0;
         memset(tappe, 0, sizeof(tappe));
         return;
      }
      else
      {

         tappe[tappecounter] = pick;
         tappecounter++;
         tappa = staz_search(albero, pick);
         pick = -1;
         goto label;
      }
   }

print:
   for (int i = tappecounter - 1; i > 0; i--)
   {
      printf("%d ", tappe[i]);
   }
   printf("%d", tappe[0]);
   printf("\n");
   tappecounter = 0;
   memset(tappe, 0, sizeof(tappe));
   return;
}

void ottimizza_decresc2(stazioneTree *albero)
{
   stazioneNode *buf, *monte, *valle;
   int autonomia;
   int autonomia_buf;
   int pick;
   int indice_tappe_sopra;
   int indice_tappe_sotto;
   int staz_buf;
   for (int i = 1; i < tappecounter - 1; i++)
   {
      valle = staz_search(albero, tappe[i - 1]);
      monte = staz_search(albero, tappe[i + 1]);
      autonomia = findmax(monte->parco_auto);
      indice_tappe_sopra = trova_indice_array_staz(tappe[i]);
      indice_tappe_sotto = trova_indice_array_staz(tappe[i - 1]);
      for (int j = indice_tappe_sopra - 1; j > indice_tappe_sotto; j--)
      {
         staz_buf = array_stazioni[j];
         buf = staz_search(albero, staz_buf);
         if (buf != albero->Tnil)
         {
            if ((monte->distanza - buf->distanza) <= autonomia)
            {
               autonomia_buf = findmax(buf->parco_auto);
               if ((buf->distanza - valle->distanza) <= autonomia_buf)
               {
                  pick = buf->distanza;
                  scambi = true;
                  tappe[i] = pick;

                  break;
               }
            }
         }
      }
      if (scambi == true)
      {
         break;
      }
   }
}

void pianifica_decresc(stazioneNode *inizio, stazioneNode *fine, stazioneTree *albero)
{
   mergeSort_array_stazioni(0, conta_stazioni - 1);

   int salva_indice = trova_indice_array_staz(fine->distanza);
   int indice_inizio = trova_indice_array_staz(inizio->distanza);

   stazioneNode *tappa, *buffer;
   tappa = fine;
   int autonomia;
   int staz_buf;

   int pick = -1;
   tappe[tappecounter] = tappa->distanza;
   tappecounter++;
label:
   autonomia = findmax(inizio->parco_auto);

   if (inizio->distanza - tappa->distanza <= autonomia)
   {
      tappe[tappecounter] = inizio->distanza;
      tappecounter++;
      goto fix_print;
   }
   else
   {
      for (int i = indice_inizio; i > salva_indice; i--)
      {
         staz_buf = array_stazioni[i];

         buffer = staz_search(albero, staz_buf);
         if (buffer != albero->Tnil)
         {
            autonomia = findmax(buffer->parco_auto);
            if (buffer->distanza - tappa->distanza <= autonomia)
            {
               pick = buffer->distanza;

               salva_indice = i;
               break;
            }
         }
      }
      if (pick == -1)
      {
         printf("nessun percorso\n");
         tappecounter = 0;
         memset(tappe, 0, sizeof(tappe));
         return;
      }
      else
      {

         tappe[tappecounter] = pick;
         tappecounter++;
         tappa = staz_search(albero, pick);
         pick = -1;
         goto label;
      }
   }
fix_print:
   scambi = true;
   while (scambi == true)
   {
      scambi = false;
      ottimizza_decresc2(albero);
   }
   scambi = true;

   for (int i = tappecounter - 1; i > 0; i--)
   {
      printf("%d ", tappe[i]);
   }
   printf("%d", tappe[0]);
   printf("\n");
   tappecounter = 0;
   memset(tappe, 0, sizeof(tappe));
   return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
   for (int s = 0; s < 56000; s++)
   {
      array_stazioni[s] = __INT_MAX__;
   }
   int staz_buf = 0;
   stazioneTree *Stazioni;
   Stazioni = crea_alberoStazioni();
   stazioneNode *Staz_agg, *Staz_buff;
   bool check = false, rottam = false;
   char c = getchar();
   char input[10000], comando[20], buffer[5000];
   int valori[520], val = 0;
   int i = 0, count = 0, buf = 0;
   int trovaposto = 0;
   int inizio, fine;

   while (c != EOF)
   {

      while (c != '\n')
      {
         input[i] = c;
         i++;
         c = getchar();
      }
      i = 0;

      while (count < strlen(input))
      {

         if (check == false)
         {
            while (input[count] != ' ')
            {
               comando[count] = input[count];
               count++;
            }

            while (check == false)
            {
               count++;

               while (input[count] != ' ' && input[count] != '\0')
               {
                  buffer[buf] = input[count];
                  buf++;
                  count++;
               }

               buf = 0;
               val = atoi(buffer);
               memset(buffer, 0, sizeof(buffer));
               valori[i] = val;
               i++;

               if (input[count] == '\0')
               {
                  check = true;
               }
            }
         }
      }
      i = 0;
      count = 0;

      if (c == '\n')
      {
         if (strcmp(comando, "aggiungi-stazione") == 0)
         {
            Staz_agg = staz_search(Stazioni, valori[0]);
            if (Staz_agg == Stazioni->Tnil)
            {
               Staz_agg = crea_stazione(Stazioni, valori[0]);
               staz_insert(Stazioni, Staz_agg);
               for (int x = 2; x < valori[1] + 2; x++)
               {
                  Staz_agg->parco_auto[x - 2] = valori[x];
               }

               Staz_agg->auto_presenti = valori[1];

               array_stazioni[conta_stazioni] = Staz_agg->distanza;
               conta_stazioni++;
               printf("aggiunta\n");
            }
            else
            {
               printf("non aggiunta\n");
            }
         }

         if (strcmp(comando, "demolisci-stazione") == 0)
         {
            Staz_agg = staz_search(Stazioni, valori[0]);

            if (Staz_agg == Stazioni->Tnil)
            {
               printf("non demolita\n");
            }
            else
            {
               staz_delete(Stazioni, Staz_agg);
               staz_buf = trova_array_staz(valori[0]);
               delete_array_stazioni(staz_buf);
               printf("demolita\n");
            }
         }

         if (strcmp(comando, "aggiungi-auto") == 0)
         {
            Staz_agg = staz_search(Stazioni, valori[0]);
            if (Staz_agg == Stazioni->Tnil)
            {
               printf("non aggiunta\n");
            }
            else
            {
               if (Staz_agg->auto_presenti == 0)
               {
                  Staz_agg->parco_auto[0] = valori[1];
                  Staz_agg->auto_presenti = 1;
                  printf("aggiunta\n");
               }
               else
               {
                  for (trovaposto = 0; trovaposto < 512; trovaposto++)
                  {
                     if (Staz_agg->parco_auto[trovaposto] == -1)
                     {
                        Staz_agg->parco_auto[trovaposto] = valori[1];
                        ++Staz_agg->auto_presenti;

                        printf("aggiunta\n");
                        break;
                     }
                  }
               }
            }
         }

         if (strcmp(comando, "rottama-auto") == 0)
         {
            Staz_agg = staz_search(Stazioni, valori[0]);
            if (Staz_agg == Stazioni->Tnil)
            {
               printf("non rottamata\n");
            }
            else
            {
               for (int a = 0; a < 512; a++)
               {

                  if (Staz_agg->parco_auto[a] == valori[1])
                  {
                     Staz_agg->parco_auto[a] = -1;

                     --Staz_agg->auto_presenti;
                     printf("rottamata\n");
                     rottam = true;
                     break;
                  }
               }
               if (rottam == false)
               {
                  printf("non rottamata\n");
               }
               rottam = false;
            }
         }

         if (strcmp(comando, "pianifica-percorso") == 0)
         {
            Staz_agg = staz_search(Stazioni, valori[0]);
            Staz_buff = staz_search(Stazioni, valori[1]);
            if (Staz_agg == Stazioni->Tnil)
            {
               printf("nessun percorso\n");
            }
            else
            {
               if (Staz_buff == Stazioni->Tnil)
               {
                  printf("nessun percorso\n");
               }
               else
               {

                  inizio = Staz_agg->distanza;
                  fine = Staz_buff->distanza;

                  if (inizio == fine)
                  {
                     printf("%d\n", inizio);
                  }

                  if (inizio < fine)
                  {
                     pianifica_cresc(Staz_agg, Staz_buff, Stazioni);
                  }
                  else
                  {
                     pianifica_decresc(Staz_agg, Staz_buff, Stazioni);
                  }
               }
            }
         }
      }
      check = false;
      memset(input, 0, sizeof(input));
      memset(comando, 0, sizeof(comando));
      memset(valori, 0, sizeof(valori));

      c = getchar();
   }
   delete_staz(Stazioni, Stazioni->root);
   free(Stazioni->Tnil);
   free(Stazioni);
   return 0;
};