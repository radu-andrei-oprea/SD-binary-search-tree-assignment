//OPREA RADU-ANDREI 335 CB
/*--- tarb.h ---*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef ARBORE_BINAR
#define ARBORE_BINAR

typedef struct nod {
	void * info;
	struct nod *st, *dr;

} TNod, *TArb, **AArb;

/*-- Operatii elementare --*/
TArb InitA();           /* initializare arbore */
TArb ConstrNod(void * x, TArb s, TArb d); 
     /* -> adresa nod (cu info x, fiu stanga s, fiu dreapta d)
		   sau NULL daca nu exista spatiu */
TArb ConstrFr(void * x);
     /* -> adresa frunza cu informatia x, sau NULL daca nu exista spatiu */
void DistrArb(AArb);    /* distruge arbore */
int NrNoduri(TArb);     /* numar noduri */
int NrNiv(TArb);        /* numar niveluri (0 pentru arbore vid) */

/*-- Operatii suplimentare --*/
void AfiArb(TArb r);
	  /*- afiseaza arborele r -*/

char * extrageNume(TArb a, int type);
	  /*- extrage nume din arborele a -*/

int quickSearch(TArb a, void *x, int type);

TArb pointTo(TArb a, void * x, int type);
	
int Inserare_ABC(TArb* a, void * x, TArb parinte, int type);
	  /*- insereaza x in arborele binar de cautare a cu parintele parinte -*/

void insertInList(TArb a, char ** list, int * i, int type);
	  /*- introduce recursiv nume in liste -*/

char ** list_of_content(TArb a, int type);
	  /*- extrage lista de nume din arbore -*/

TArb Stergere_ABC(TArb * a, void * x, int type);
	  /*- stergere x din arborele a-*/

#endif

#ifndef _random_
#define _random_
#define random(n) (rand() % (n))
#define randomize() (srand((unsigned int)time(NULL)))
#endif
