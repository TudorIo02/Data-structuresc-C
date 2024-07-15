#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nod Nod;
typedef struct Rezervare Rezervare;
typedef struct HashTable HashTable;

struct Rezervare
{
	unsigned int idRezervare;
	char* denumireHotel;
	unsigned char categorieHotel;
	char* numeClient;
	unsigned char nrZileRezervate;
	float sumaPlata;
};

struct Nod
{
	Rezervare info;
	Nod* next;
};

struct HashTable
{
	Nod** vector;
	int dim;
};

HashTable initializareTabela(int dimensiune)
{
	HashTable h;
	h.dim = dimensiune;
	h.vector = (Nod**)malloc(sizeof(Nod*)*dimensiune);
	for (int i = 0; i < dimensiune; i++)
	{
		h.vector[i] = NULL;
	}
	return h;
}

int calculHash(Rezervare r)
{
	return strlen(r.denumireHotel) * r.idRezervare % r.categorieHotel;
}

void inserareFinal(Nod** lista, Rezervare r)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = r;
	nou->next = NULL;
	if (*lista)
	{
		Nod* aux = *lista;
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
	else
	{
		*lista = nou;
	}
}

void inserareTabela(HashTable h, Rezervare r)
{
	inserareFinal(&(h.vector[calculHash(r)]), r);
}


void citireFisier(const char* fileName, HashTable h)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		printf("\Fisier inexistent!");
	}
	else
	{
		Rezervare r;
		char buffer[100];
		char* delimiter = ",\n";
		while (fgets(buffer,100,f))
		{
			char* token = strtok(buffer, delimiter);
			r.idRezervare = atoi(token);

			token = strtok(NULL, delimiter);
			r.denumireHotel = (char*)malloc(strlen(token) + 1);
			strcpy(r.denumireHotel, token);

			token = strtok(NULL, delimiter);
			r.categorieHotel = (unsigned char)atoi(token);

			token = strtok(NULL, delimiter);
			r.numeClient = (char*)malloc(strlen(token) + 1);
			strcpy(r.numeClient, token);

			token = strtok(NULL, delimiter);
			r.nrZileRezervate = (unsigned char)atoi(token);

			token = strtok(NULL, delimiter);
			r.sumaPlata = atof(token);

			inserareTabela(h, r);
		}
	}
}


void afisareRezervare(Rezervare r)
{
	printf("\n %d, %s, %d, %s, %d, %f", r.idRezervare, r.denumireHotel, r.categorieHotel, r.numeClient, r.nrZileRezervate, r.sumaPlata);
	printf("\n________________________");
}

void afisareLista(Nod* lista)
{
	while (lista)
	{
		afisareRezervare(lista->info);
		lista = lista->next;
	}
}

void afisareHash(HashTable h)
{
	for (int i = 0; i < h.dim; i++)
	{
		afisareLista(h.vector[i]);
	}
}


void stergereDupaNrZile(HashTable h, int limit)
{
	for (int i = 0; i < h.dim; i++)
	{
		Nod* current = h.vector[i];
		Nod* prev = NULL;

		while (current != NULL)
		{
			if (current->info.nrZileRezervate < limit)
			{
				if (prev == NULL)
				{
					h.vector[i] = current->next;
				}
				else
				{
					prev->next = current->next;
				}

				free(current->info.denumireHotel);
				free(current->info.numeClient);
				Nod* temp = current;
				current = current->next;
				free(temp);
			}
			else
			{
				prev = current;
				current = current->next;
			}
		}
	}
}



int main()
{
	HashTable hash = initializareTabela(5);
	citireFisier("date.txt", hash);
	afisareHash(hash);
	stergereDupaNrZile(hash, 3);
	afisareHash(hash);
	return 0;
}