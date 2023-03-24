//OPREA RADU-ANDREI 335 CB
#include "tarb.h"
#include "dirnfile.h"

/*proprie functie folosita pentru verificarea egalitatii string-urilor, 
deoarece functia strtok este instabila si putem avea probleme la cicitul
inputului*/
int SDstrcmp(char * a, char * b) {

	int i;

	for (i = 0; i < (int)strlen(b); i++)
	{
		if (a[i] != b[i])
			return 0;
	}

	return 1;
}

/*pornind de la nodul fisierului, mergem inapoi si contruim path-ul*/
/* se foloseste pentru pwd*/
char * getPath(TArb a) {

	char * path = (char*) malloc(sizeof(char));
	char * s = ((Dir*) (a -> info)) -> nume;

	TArb parinte = ((Dir*) (a -> info)) -> parinte;

	if (parinte != NULL)
		strcat(path, getPath(parinte));
	
	strcat(path, "/");
	strcat(path, s);

	return path;

}

/* functie ce are ca scop identificarea pozitiei fisierelor, pentru a indica
daca sunt gasite sau nu, urmand sa primim si calea acestora*/
void extract_file(TArb a, void * x) {
	char * s = (char*)x;
	char * temp = (char*) malloc(sizeof(char));
	

	TArb fisiere = ((Dir*) (a -> info)) -> files;
	TArb subdr = ((Dir*) (a -> info)) -> directories;
		
	if (fisiere != NULL){
		if(quickSearch(fisiere , s, 0) == 1){
			strcat(temp, getPath(a));
			strcat(temp, "/");
			strcat(temp, s);
			printf("File %s found!\n", temp);
			printf("%s\n", temp);
		}
	}
	else {
		if(subdr != NULL)
			extract_file(subdr, x);
		if(a -> st != NULL)
			extract_file(a -> st, x);
		if(a -> dr != NULL)
			extract_file(a -> dr, x);
	}
}

/* aici, precum si in file, verificam daca acea persoana este in arbore.
Daca nu, o vom cauta in urmatoarele*/

void extract_dir(TArb a, void * x) {
	char * s = (char*)x;
	char * temp = (char*) malloc(sizeof(char));
	
	TArb subdr = ((Dir*) (a -> info)) -> directories;

	if (subdr != NULL)
		if(quickSearch(subdr , s, 1) == 1){
			strcat(temp, getPath(a));
			strcat(temp, "/");
			strcat(temp, s);
			printf("Directory %s found!\n", temp);
			printf("%s\n", temp);
		}

	else {
		if( ((Dir*) (subdr -> info)) -> directories != NULL)
			extract_dir(((Dir*) (subdr -> info)) -> directories, x);
		if(a -> st != NULL)
			extract_dir(a -> st, x);
		if(a -> dr != NULL)
			extract_dir(a -> dr, x);
	}
}

/*in spatiul separat dedicat fisierelor intr-un nod. Verificam mai intai 
existenta fisierelor de acelasi nume dar tip diferit
Pe urma, verificam daca nu exista deja cuvantul in arbore. in caz contrar, 
vom mentiona acest lucru/*/
void touch(char * param, TArb currentDir){

	TArb n = currentDir;

	TArb fisiere = ((Dir*) (n -> info)) -> files;

	if (((Dir*) (n -> info)) -> directories != NULL)
		if (quickSearch(((Dir*) (n -> info)) -> directories , param, 1) == 1){
			printf("Directory %s already exists!\n", param);
			return;
		}

	int x = Inserare_ABC(&fisiere, param, n, 1);

	if(x == 0)
		printf("File %s already exists!\n", param);


	((Dir*) (n -> info)) -> files = fisiere;

	currentDir = n;
}


/*acelasi principiu ca si touch dar cu directoare.*/
void mkdir(char * param, TArb currentDir){

	TArb n = currentDir;

	TArb subdr = ((Dir*) (n -> info)) -> directories;

	if (((Dir*) (n -> info)) -> files != NULL)
		if (quickSearch(((Dir*) (n -> info)) -> files , param, 0) == 1){
			printf("File %s already exists!\n", param);
			return;
		}

	int x = Inserare_ABC(&subdr, param, currentDir, 0);

	if(x == 0)
		printf("Directory %s already exists!\n", param);


	((Dir*) (n -> info)) -> directories = subdr;

	currentDir = n;
}


/*vom avea doua array-uri, in care vom lista toate numele persoanelor\
listele vor urma sa fie sortate si afisate, directori si dupa fisiere*/
void ls(TArb currentDir) {

	TArb subdr = ((Dir*) currentDir -> info) -> directories;
	TArb fisiere = ((Dir*) currentDir -> info) -> files;

	// printf("extrage liste\n");

	char ** subdr_list ;
	char ** fisiere_list ;


	subdr_list = list_of_content(subdr, 0);
	fisiere_list = list_of_content(fisiere, 1);


	int i;


	for (i = 0; i < NrNoduri(subdr); i++) {
		printf("%s ", subdr_list[i]);
	}

	for (i = 0; i < NrNoduri(fisiere); i++) {
		printf("%s ", fisiere_list[i]);
	}
	printf("\n");
}

/*pentru stergeri se aplica functia de stergere din arbori*/
void rm(char * param, TArb currentDir){

	TArb n = currentDir;

	TArb fisiere = ((Dir*) (n -> info)) -> files;

	if (quickSearch(fisiere, param, 1) == 0){
		printf("File %s doesn't exist!\n", param);
		return;
	}

	fisiere = Stergere_ABC(&fisiere, param, 1);

	((Dir*) (n -> info)) -> files = fisiere;

	currentDir = n;
	
}

void rmdir(char * param, TArb currentDir) {


	TArb n = currentDir;


	TArb subdr = ((Dir*) (n -> info)) -> directories;

	if (quickSearch(subdr, param, 1) == 0){
		printf("Directory %s doesn't exist!\n", param);
		return;
	}

	subdr = Stergere_ABC(&subdr, param, 0);
	
	((Dir *) (n -> info)) -> directories = subdr;

	currentDir = n;
}


/*extragem fiecare parinte, unde il vom concatena pentru a afla 
adresa iepurasului.*/
TArb cd(char * param, TArb currentDir) {


	TArb n = currentDir;

	TArb subdr = ((Dir*) n -> info) -> directories;
	TArb parinte = ((Dir*) n -> info) -> parinte;

	if (SDstrcmp(param, "..") == 1){
		n = parinte;
	}
	else {
		if (quickSearch(subdr, param, 0) == 0){
			printf("Directory not found!\n");
			return currentDir;
		}
		else{
			n = pointTo(subdr, param, 0);
		}
	}

	currentDir = n; 

	return currentDir;
}

void pwd(TArb currentDir) {

	TArb n = currentDir;

	printf("%s\n", getPath(n));
}

/*la fel, in functie de persoana/*/
void find(char * param, char * arg, TArb currentDir){

	if (SDstrcmp(arg, "-f")){
		extract_file(currentDir, param);
	}
	else if (SDstrcmp(arg, "-d")){
		extract_dir(currentDir, param);
	}
}

int main(int argc, char *argv[]) {


	Dir * infoRoot = createDir("root");

	TArb root = NULL; root = ConstrFr(infoRoot);

	TArb currentDir = root;

	char * line = NULL;
	size_t len;
	char * p;

	/* se va citi tot inputul intr-un array, urmand sa fie parcurs.*/
	char ** inps = (char**) malloc(sizeof(char*) * 100);

	int i;
    for (i = 0; i < 100; i++) {
        inps[i] = malloc(sizeof(char) * 50);
    }

    int j = 0;

    while(1){
        getline(&line, &len, stdin);
        strcpy(inps[j], line);


        if(strcmp(line, "quit\n") == 0){
            break;
        }
        
        j++;
    }

    
    while(inps[i] != NULL){
        printf("%s", inps[i]);
        i++;
    }



    i = 0;

    /*se va lua fiecare element din array si se formeaza comanda prin apeluri
    de functie si strtok*/
    while(inps[i] != NULL){

		if (strcmp(inps[i], "quit\n") == 0){
			return 0;
		}
		else if (strcmp(inps[i], "ls\n") == 0)
			ls(currentDir);
		else if (strcmp(inps[i], "pwd\n") == 0)
			pwd(currentDir);
		else {

			p = strtok(inps[i], " \n");


			if (SDstrcmp(p, "touch")){
				p = strtok(NULL, " \n");
				touch(p, currentDir);

			}
			else if (SDstrcmp(p, "mkdir")){
				p = strtok(NULL, " \n");
				mkdir(p, currentDir);
			}
			else if (SDstrcmp("rm", p)){
				p = strtok(NULL, " \n");
				rm(p, currentDir);
			}
			else if (SDstrcmp(p, "rmdir")){
				p = strtok(NULL, " \n");
				rmdir(p, currentDir);
			}
			else if (SDstrcmp(p, "cd")){
				p = strtok(NULL, " \n");
				currentDir = cd(p, currentDir);
			}
			else if (SDstrcmp(p, "find")){
				p = strtok(NULL, " \n");
				find(p, strtok(NULL, " \n"), currentDir);
			}
		}
		i++;
	}

}