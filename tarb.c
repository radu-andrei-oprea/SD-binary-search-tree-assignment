//OPREA RADU-ANDREI 335 CB
#include "tarb.h"
#include "dirnfile.h"


TArb InitA()         /* initializare arbore */
{
	return NULL;
}

TArb ConstrNod(void * x, TArb s, TArb d)/* -> adresa nod (cu informatia x, 
				fiu stanga s, fiu dreapta d) sau NULL in caz de eroare */
{
	TArb aux = (TArb)malloc (sizeof(TNod));  /* incearca alocare spatiu */
	if (!aux) return NULL;                   /* spatiu insuficient */
	aux->info = x; aux->st = s; aux->dr = d; /* actualizeaza campurile nodului */

	return aux;                              /* intoarce adresa nodului */
}

TArb ConstrFr(void * x)     /* -> adresa frunza cu informatia x, sau
					NULL daca nu exista spatiu */
{
	TArb aux = (TArb)malloc (sizeof(TNod));
	if (!aux) return NULL;
	aux->info = x; aux->st = aux->dr = NULL;

	return aux;
}

void distruge(TArb r) 	/* functie auxiliara - distruge toate nodurile */
{
	if (!r) return;
	distruge (r->st);     /* distruge subarborele stang */
	distruge (r->dr);     /* distruge subarborele drept */
	free (r);             /* distruge nodul radacina */
}

void DistrArb(TArb *a) /* distruge toate nodurile arborelui de la adresa a */
{
	if (!(*a)) return;       /* arbore deja vid */
	distruge (*a);           /* distruge toate nodurile */
	*a = NULL;               /* arborele este vid */
}

int NrNoduri(TArb r)        /* numarul de noduri din arborele r -*/
{
	if (!r) return 0;          /* arbore vid => 0 noduri */
	return 1 + NrNoduri(r->st) + NrNoduri(r->dr);
}

int NrNiv(TArb r)           /* numar niveluri (0 pentru arbore vid) */
{
	int ns, nd;
	if (!r) return 0;
	ns = NrNiv(r->st); nd = NrNiv(r->dr);
	return 1 + (ns >= nd ? ns : nd);
}



int compNume (char * a, char * b){ /*comparator pentru parcurgerera arborelui*/
									/*-1 pentru a < b, 1 contrar si 0 egal*/
	int i = 0;
	while(a[i] != 0 || b[i] != 0){

		if (a[i] < b[i])
			return -1;
		else if (a[i] > b[i])
			return 1;
		else i++;
	}

	return 0;
}


// functii pentru directoare si fisiere


Dir * createDir(char * s){  /* aloca un fisier de tip Dir */
	Dir * dir = (Dir *) malloc(sizeof(Dir));

	if (!dir)
		return NULL;
	
	dir -> nume = s;

	dir -> parinte = NULL;

	dir -> files = NULL;
	
	dir -> directories = NULL;

	return dir;
}

File * createFile(char * s) {  /* aloca un fisier de tip File */
	File * file = (File *) malloc(sizeof(File));

	file -> nume = malloc(sizeof(char*) * strlen(s));
	file -> nume = s;
	file -> parinte = NULL;

	return file;
}


void distrDir(Dir * dir){    /*functii pentru distrugerea structurilor Dir*/

	if (dir -> parinte != NULL){
		DistrArb(&(dir->parinte));
		free(dir -> parinte);
	}
	if (dir -> files != NULL){
		DistrArb(&(dir->files));
		free(dir -> files);
	}
	if (dir -> directories != NULL){
		DistrArb(&(dir->directories));
		free(dir -> directories);
	}
	free(dir);
}

void distrFile(File * file){   /* respectiv distrugerea File */
	
	if (file -> parinte != NULL){
		free(file -> parinte);
	}

	free(file);
}


// functii ABC - functii care au legatura cu Arborii de cautare

char * extrageNume(TArb a, int type){  /* extrage numele unui arbore*/

	char * nume;

	if(type == 0)
		nume = ((Dir *) (a -> info)) -> nume;
	else
		nume = ((File *) (a -> info)) -> nume;

	return nume;
}

int quickSearch(TArb a, void * x, int type){   /* cauta existenta unui nod
												intr-un arbore.
												In functie de variabila type,
												putem cauta ori un fisier, ori 
												un director.*/

	char * s = (char *) x;
	char * temp;
	int ret, ret1 = 0, ret2 = 0;

	if (type == 0)
		temp = ((File *)(a -> info)) -> nume;
	else
		temp = ((Dir *)(a -> info)) -> nume;

	if (a == NULL)
		return 0;
	

	if (compNume(s, temp) == 0)
		return 1;
	else{
		if (a -> st != NULL)					  // in cazul in care nu gasim
			ret1 = quickSearch(a -> st, x, type); //in nodul respectiv, cautam 
		if (a -> dr != NULL) 					  //recursiv intre nodurile 
			ret2 = quickSearch(a -> dr, x, type); //copii

		return ret1 | ret2;
	}

}

TArb pointTo(TArb a, void * x, int type) {     /* cauta intr-un arbore nodul cu
											numele respectiv, si il trimite mai 
											departe*/

	char * s = (char *) x;
	char * temp;
	TArb ret;

	if (type == 0)
		temp = ((Dir *)(a -> info)) -> nume;
	else
		temp = ((File *)(a -> info)) -> nume;


	if (strcmp(temp, s) == 0)
		return a;
	else{
		if (a -> st != NULL)
			ret = (pointTo(a -> st, x, type));
		if (a -> dr != NULL)
			ret = (pointTo(a -> dr, x, type));
	}
	return ret;
}


/*inserarea intr-un arbore binar de cautare, functie inspirata din
cea de laborator*/
int Inserare_ABC(TArb* a, void * x, TArb parinte, int type)
{									
	TArb aux, p, n;
	Dir * dir;
	File * file;

	char * s = (char *) x;

	char * temp; 

	if (type == 0){                  /*pentru informatii precum numele*/
		dir = createDir(s);          /*trebuie sa distingem intre cele doua*/
		dir -> parinte = parinte;	 /*structuri - ne folosim de variabila*/
	}                               /*type, 0 pentru director, 1 pentru */
	else {                          /*fisiere */
		file = createFile(s);
		file -> parinte = parinte;
	}


	if(*a == NULL)
	{	
		if (type == 0)
			*a = ConstrFr(dir);
		else
			*a = ConstrFr(file);

		if(!*a) return 0;
		return 1;
	}


	n = *a;
	while(n)
	{
		p = n;

		if (type == 0)
			temp = ((Dir *)(n -> info)) -> nume;
		else
			temp = ((File *)(n -> info)) -> nume;

		if(compNume(s, temp) == 0){
			return 0;
		}
		if(compNume(s, temp) < 0)
			n = n->st;
		else	
			n = n->dr;
	}

	if (type == 0)
		aux = ConstrFr(dir);
	else
		aux = ConstrFr(file);


	if(!aux) return 0;

	if (type == 0)
		temp = ((Dir *)(p -> info)) -> nume;
	else
		temp = ((File *)(p -> info)) -> nume;

	if(compNume(s, temp) < 0)
		p->st = aux;
	else if (compNume(s, temp) > 0)	
		p->dr = aux;



	return 1;
}

TArb stang(TArb a) {              /*se foloseste pentru stergerea din arbore*/
								  /*duce arborele in cel mai stang nod posibil.*/
	TArb temp = a;

	while(temp && temp -> st != NULL)
		temp = temp -> st;

	return temp;
}


/* stergere recursiva a nodului si rearanjarea arborelui in forma sa corecta*/
TArb Stergere_ABC(TArb * a, void * x, int type){
	if (*a == NULL)
		return *a;

	TArb n = *a, temp;

	char * s = (char *) x;
	char * nume = malloc(sizeof(char));


	if (type == 0)
		nume = ((Dir *) (n -> info)) -> nume;
	else
		nume = ((File *) (n -> info)) -> nume;


	/*in cazul in care nu gasim, verificam si in nodurile copii*/
	if (compNume(s, nume) < 0){                      
		n -> st = Stergere_ABC(&(n -> st), x, type);
		return n;
	}
	else if (compNume(s, nume) > 0){
		n -> dr = Stergere_ABC(&(n -> dr), x, type);
		return n;
	}

	else {

		/*in situatiile cu doar un copil, acesta devine urmatorul succesor*/
		if( n -> st == NULL){
			temp = n -> dr;

			free(n -> info);
			
			free(n);
			
			return temp;
		}
		else if( n -> dr == NULL){
			temp = n -> st;
			
			free(n -> info);
			
			free(n);
			
			return temp;
		}
		
		/* se cauta valoarea cat mai minima si se continua procesul*/
			temp = stang(n -> dr);

			if (type == 0){
				nume = ((Dir *) (temp -> info)) -> nume;
				((Dir *) (n -> info)) -> nume = nume;
			}
			else{
				nume = ((File *) (temp -> info)) -> nume;
				((File *) (n -> info)) -> nume = nume;
			}

			n -> dr = Stergere_ABC(&(n -> dr), ((Dir *) (temp -> info)) -> nume, type);
		
	}
	return n;

}


/*functie obtinuta pentru inserarea numelor intr-o lista de char** */
void insertInList(TArb a, char ** list, int * i, int type){
	
	if (a != NULL){
		*(list + *(i)) = extrageNume(a, type);


		(*i) += 1;
		insertInList(a -> st, list, i, type);
		insertInList(a -> dr, list, i, type);
	}

}

/* se vor insera numele fisierelor intr-o lista, urmand sa fie sortata
printr-un bubblesort*/
char ** list_of_content(TArb a, int type) {
	char ** list = (char**) malloc(sizeof(char*) * NrNoduri(a));

	int i;

	for(i = 0; i < NrNoduri(a); i++)
		list[i] = (char*) malloc(sizeof(char) * 50 );


	i = 0; 
	insertInList(a, list, &i, type);

	int j;
	for (i = 0; i < NrNoduri(a) - 1; i++)
	{
		for (j = i; j < NrNoduri(a); j++)
		{	
			if (compNume(list[i],list[j]) > 0){

				char * tmp = list[i];
				list[i] = list[j];
				list[j] = tmp;
			}
		}
	}

	return list;
}