#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>

typedef struct Autobuz Autobuz;
struct Autobuz {
	int linie; //unic
	char* capat_linie;
};

typedef struct NodSecundar NodSecundar;


typedef struct NodPrincipal NodPrincipal;
struct NodPrincipal {
	Autobuz info;
	NodPrincipal* next;
	NodSecundar* vecini;
};

struct NodSecundar {
	NodPrincipal* nod;
	NodSecundar* next;
};

Autobuz autobuz_init(int linie, const char* capat_linie) {
	Autobuz a;

	a.linie = linie;
	a.capat_linie = (char*)malloc(sizeof(char) * (strlen(capat_linie) + 1));
	strcpy(a.capat_linie, capat_linie);

	return a;
}

void autobuz_afis(Autobuz a) {
	printf("Autobuzul de pe linia %d are capatul la %s.\n", a.linie, a.capat_linie);
}

//fct inserare sfarsit in NodPrincipal
void graf_inserare_nod(NodPrincipal** graf, Autobuz autobuz) {
	NodPrincipal* nod = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nod->info = autobuz; //shallow
	nod->next = NULL;
	nod->vecini = NULL;


	if ((*graf)) {
		NodPrincipal* temp = *graf;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nod;
	}
	else {
		*graf = nod;
	}


}


//fct cautare dupa linii in NodPrincipal
NodPrincipal* graf_cautare(NodPrincipal* graf, int linie) {
	while (graf && graf->info.linie != linie) {
		graf = graf->next;
	}
	return graf;
}

//fct inserare sfarsit in NodSecundar
NodSecundar* vecini_inserare(NodSecundar* cap, NodPrincipal* info) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->next = NULL;
	nou->nod = info;

	if (cap) {
		NodSecundar* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
		return cap;
	}
	else {
		return nou;
	}
}

//fct inserare muchie (va insera adresa lui 2 in 1 si vice versa)
void graf_inserare_muchie(NodPrincipal* graf, int linie_start, int linie_stop) {
	NodPrincipal* nod_start = graf_cautare(graf, linie_start);
	NodPrincipal* nod_stop = graf_cautare(graf, linie_stop);

	nod_start->vecini = vecini_inserare(nod_start->vecini, nod_stop);
	nod_stop->vecini = vecini_inserare(nod_stop->vecini, nod_start);
}

#pragma region Coada

typedef struct NodCoada NodCoada;
struct NodCoada {
	int id;
	NodCoada* next;
};

void inserareCoada(NodCoada** cap, int id) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->id = id;
	nou->next = NULL;
	if (*cap)
	{
		NodCoada* p = *cap;
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
	else
	{
		*cap = nou;
	}
}

void inserareStiva(NodCoada** cap, int id) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->id = id;
	nou->next = *cap;

	*cap = nou;
}

int extrageDinCoada(NodCoada** cap)
{
	if (*cap)
	{
		int rezultat = (*cap)->id;
		NodCoada* temp = (*cap)->next;
		free(*cap);
		*cap = temp;
		return rezultat;
	}
	else
	{
		return -1;
	}
}

int extrageDinStiva(NodCoada** cap) {
	return extrageDinCoada(cap);
}

#pragma endregion


int getNrNoduri(NodPrincipal* graf)
{
	int s = 0;
	while (graf)
	{
		s++;
		graf = graf->next;
	}
	return s;
}

void afisareCuParcurgereInLatime(NodPrincipal* graf, int nodPlecare)
{
	//initializari
	NodCoada* coada = NULL;
	int nrNoduri = getNrNoduri(graf);
	char* vizitate = (char*)malloc(sizeof(char) * nrNoduri);
	for (int i = 0; i < nrNoduri; i++)
	{
		vizitate[i] = 0;
	}
	inserareCoada(&coada, nodPlecare);
	vizitate[nodPlecare] = 1;

	while (coada) {
		int id_nod = extrageDinCoada(&coada);
		NodPrincipal* nod_curent = graf_cautare(graf, id_nod);

		autobuz_afis(nod_curent->info);

		NodSecundar* temp = nod_curent->vecini;
		while (temp) {
			if (vizitate[temp->nod->info.linie] == 0) {
				vizitate[temp->nod->info.linie] = 1;
				inserareCoada(&coada, temp->nod->info.linie);
			}

			temp = temp->next;
		}
	}

	if (vizitate != NULL) {
		free(vizitate);
	}
}

void afisareCuParcurgereInAdancime(NodPrincipal* graf, int nodPlecare)
{
	//initializari
	NodCoada* stiva = NULL;
	int nrNoduri = getNrNoduri(graf);
	char* vizitate = (char*)malloc(sizeof(char) * nrNoduri);
	for (int i = 0; i < nrNoduri; i++)
	{
		vizitate[i] = 0;
	}
	inserareStiva(&stiva, nodPlecare);
	vizitate[nodPlecare] = 1;

	while (stiva) {
		int id_nod = extrageDinStiva(&stiva);
		NodPrincipal* nod_curent = graf_cautare(graf, id_nod);

		autobuz_afis(nod_curent->info);

		NodSecundar* temp = nod_curent->vecini;
		while (temp) {
			if (vizitate[temp->nod->info.linie] == 0) {
				vizitate[temp->nod->info.linie] = 1;
				inserareStiva(&stiva, temp->nod->info.linie);
			}

			temp = temp->next;
		}
	}

	if (vizitate != NULL) {
		free(vizitate);
	}
}

void vecini_free(NodSecundar** cap) {
	NodSecundar* v = *cap;
	while (v) {
		NodSecundar* temp = v;
		v = v->next;
		free(temp);
	}
	*cap = NULL;
}

void graf_free(NodPrincipal** graf) {
	while (*graf) {
		free((*graf)->info.capat_linie);
		(*graf)->info.capat_linie = NULL;

		vecini_free(&((*graf)->vecini));

		NodPrincipal* aux = *graf;
		*graf = (*graf)->next;

		free(aux);
	}

}

void main() {
	NodPrincipal* graf = NULL;
	graf_inserare_nod(&graf, autobuz_init(0, "Romana"));
	graf_inserare_nod(&graf, autobuz_init(1, "Universitate"));
	graf_inserare_nod(&graf, autobuz_init(2, "Unirii"));
	graf_inserare_nod(&graf, autobuz_init(3, "Victoriei"));
	graf_inserare_nod(&graf, autobuz_init(4, "Militari"));

	//muchii
	graf_inserare_muchie(graf, 0, 1);
	graf_inserare_muchie(graf, 0, 4);
	graf_inserare_muchie(graf, 1, 2);
	graf_inserare_muchie(graf, 2, 4);
	graf_inserare_muchie(graf, 3, 4);

	afisareCuParcurgereInLatime(graf, 0);
	printf("\n---\n");
	afisareCuParcurgereInAdancime(graf, 0);

	graf_free(&graf);
}