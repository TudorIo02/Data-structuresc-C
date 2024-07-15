#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>


typedef struct Pisica Pisica;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;
struct Pisica
{
	char* rasa;
	float varsta;
	int nrVaccinuri;
	char** denumireVaccin;
};

struct Nod
{
	Pisica info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla
{
	Nod* prim;
	Nod* ultim;
};


//inserare in ordine dupa varsta
void inserareLista(ListaDubla* lista, Pisica p) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = p;
	nod->next = NULL;
	nod->prev = NULL;

	if (lista->prim == NULL) {
		lista->prim = nod;
		lista->ultim = nod;
	}
	else if (p.varsta <= lista->prim->info.varsta) {
		nod->next = lista->prim;
		lista->prim->prev = nod;
		lista->prim = nod;
	}
	else if (p.varsta >= lista->ultim->info.varsta) {
		nod->prev = lista->ultim;
		lista->ultim->next = nod;
		lista->ultim = nod;
	}
	else {
		Nod* aux = lista->prim;
		while (aux != NULL && nod->info.varsta > aux->info.varsta) {
			aux = aux->next;
		}
		nod->next = aux;
		nod->prev = aux->prev;
		aux->prev->next = nod;
		aux->prev = nod;
	}
}


void citireFisier(const char* fileName,ListaDubla* lista)
{
	FILE* f = fopen(fileName, "r");
	char buffer[100];
	char* delimiter = ",\n";
	while (fgets(buffer,100,f)!=NULL)
	{
		Pisica p;
		char* element = strtok(buffer, delimiter);
		p.rasa = malloc(sizeof(char) * strlen(element) + 1);
		strcpy(p.rasa, element);
		element = strtok(NULL, delimiter);
		p.varsta = atof(element);
		element = strtok(NULL, delimiter);
		p.nrVaccinuri = atoi(element);
		p.denumireVaccin = (char**)malloc(p.nrVaccinuri*sizeof(char*));
		for (int i = 0; i < p.nrVaccinuri; i++)
		{
			element = strtok(NULL, delimiter);
			p.denumireVaccin[i] = malloc(sizeof(char) * strlen(element) + 1);
			strcpy(p.denumireVaccin[i], element);
		}
		inserareLista(lista, p);
	}
}

void afisarePisica(Pisica p)
{
	printf("\nRasa: %s", p.rasa);
	printf("\nNr vaccinuri: %d", p.nrVaccinuri);
	printf("\n");
	for (int i = 0; i < p.nrVaccinuri; i++)
	{
		printf("%s", p.denumireVaccin[i]);
	}
	printf("\n------------------------");
}


void afisareLista(ListaDubla lista)
{
	for (Nod* n = lista.prim; n != NULL; n = n->next)
	{
		afisarePisica(n->info);
	}
}


int adaugareVector(ListaDubla lista, Pisica** vectorPisicesc)
{
	int contor = 0;
	for (Nod* n = lista.prim; n != NULL; n = n->next)
	{
		for (int i = 0; i < n->info.nrVaccinuri; i++)
		{
			if (strcmp(n->info.denumireVaccin[i], "HIV") == 0)
			{
				contor++;
				break;
			}
		}
	}
	*vectorPisicesc = malloc(sizeof(Pisica) * contor);
	int index = 0;
	for (Nod* n = lista.prim; n != NULL; n = n->next)
	{
		for (int i = 0; i < n->info.nrVaccinuri; i++)
		{
			if (strcmp(n->info.denumireVaccin[i], "HIV") == 0)
			{
				(*vectorPisicesc)[index++] = n->info;
			}
		}
	}
	return contor;
}


void stergerePisica(ListaDubla* lista, const char* rasa)
{
	Nod* n = lista->prim;
	while (n != NULL)
	{
		if (strcmp(n->info.rasa, rasa) == 0)
		{
			Nod* aux = n->next;
			if (n->prev != NULL)
			{
				n->prev->next = n->next;
			}
			else
			{
				lista->prim = n->next;
			}

			if (n->next != NULL)
			{
				n->next->prev = n->prev;
			}
			else
			{
				lista->ultim = n->prev;
			}
			for (int i = 0; i < n->info.nrVaccinuri; i++)
				free(n->info.denumireVaccin[i]);
			free(n->info.denumireVaccin);
			free(n->info.rasa);
			free(n);
			n = aux;
		}
		else
		{
			n = n->next;
		}
	}
}



int main()
{
	ListaDubla ls;
	ls.prim = NULL;
	ls.ultim = NULL;
	citireFisier("date.txt", &ls);
	afisareLista(ls);

	Pisica* vector = NULL;
	int lungimeVect = adaugareVector(ls, &vector);
	for (int i = 0; i < lungimeVect; i++)
	{
		afisarePisica(vector[i]);
	}

	stergerePisica(&ls, "egipteana");

	printf("\n&^$#$%^&*(*&^%$%^&*(*&^%");
	afisareLista(ls);
	return 0;
}