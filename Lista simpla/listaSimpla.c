
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Santier Santier;
typedef struct Nod Nod;

struct Santier {
	char* nume_proiect;
	int nr_muncitori;
	float suprafata;
};

struct Nod {
	Santier santier;
	Nod* next;
};

Santier init_santier(const char* nume_proiect, int nr_muncitori, float suprafata) {
	Santier s;
	s.nume_proiect = (char*)malloc(strlen(nume_proiect) + 1);
	strcpy(s.nume_proiect, nume_proiect);
	s.nr_muncitori = nr_muncitori;
	s.suprafata = suprafata;

	return s;
}

Nod* inserare_inceput(Santier santier, Nod* former_start) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->santier = santier; //shallow copy
	nod->next = former_start;
	return nod;
}

void afisare_santier(Santier s) {
	/*if (s.nume_proiect != NULL) {
		printf("Denumire: %s\n", s.nume_proiect);
	}
	else printf("Denumire: -\n");
	printf("Numar muncitori: %d\n", s.nr_muncitori);
	printf("Suprafata: %.2f\n", s.suprafata);
	printf("\n");*/

	printf("Santierul \'%s\' are %d muncitori si %.2fmp suprafata.\n", s.nume_proiect, s.nr_muncitori, s.suprafata);
}

void afisare_lista(Nod* lista) {
	if (lista != NULL) {
		afisare_santier(lista->santier);

		if (lista->next != NULL) {
			afisare_lista(lista->next);
		}
	}
	/*else {
		printf("---FINAL LISTA---\n");
	}*/ //nu e neaparat necesar
}

int suma_muncitori(Nod* lista) {
	int s = lista->santier.nr_muncitori;
	if (lista->next != NULL) {
		s += suma_muncitori(lista->next);
	}
	return s;
}

void stergere_santier(Santier* santier) {
	if (santier->nume_proiect != NULL) {
		free(santier->nume_proiect);
		santier->nume_proiect = NULL;
	}
}

float densitate_muncitori(Santier santier) {
	if (santier.suprafata > 0) return santier.nr_muncitori / santier.suprafata;
	return 0;
}

float max_dens_lista(Nod* lista) {
	float max = 0;

	if (lista->next != NULL) {
		max = max_dens_lista(lista->next);
	}

	if (densitate_muncitori(lista->santier) > max)
		max = densitate_muncitori(lista->santier);

	return max;
}

char* santier_densitate_max(Nod* lista) {
	float max = max_dens_lista(lista); //posibil ineficient 
	char* nume = NULL;
	if (lista->next != NULL) {
		nume = santier_densitate_max(lista->next);
	}

	if (densitate_muncitori(lista->santier) == max) {
		nume = malloc(strlen(lista->santier.nume_proiect) + 1);
		strcpy(nume, lista->santier.nume_proiect);
	}

	return nume;
}

Nod* stergere_lista(Nod** lista) {
	stergere_santier(&((*lista)->santier));

	if ((*lista)->next != NULL) {
		stergere_lista(&((*lista)->next));

		free((*lista)->next);
		(*lista)->next = NULL;
	}

	free((*lista));
	(*lista) = NULL;

	return lista;
}

int main() {
	Nod* lista = NULL;
	Santier s1 = init_santier("Santier 1", 3, 7.5);
	Santier s2 = init_santier("Santier 2", 7, 24.77);
	Santier s3 = init_santier("Santier 3", 15, 44.37);
	Santier s4 = init_santier("Santier 4", 22, 83.10);
	lista = inserare_inceput(s1, lista);
	lista = inserare_inceput(s2, lista);
	lista = inserare_inceput(s4, lista);
	lista = inserare_inceput(s3, lista);

	afisare_lista(lista);
	int suma = suma_muncitori(lista);
	printf("Suma muncitori santier: %d\n", suma);

	printf("Densitate maxima: %.2f\n", max_dens_lista(lista));
	printf("Santierul cu densitatea maxima este: %s", santier_densitate_max(lista));

	lista = stergere_lista(&lista);
	//afisare_lista(lista);
}