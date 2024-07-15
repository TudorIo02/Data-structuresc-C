#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>


typedef struct Proiect Proiect;
typedef struct Nod Nod;


struct Proiect
{
	unsigned int idProiect;
	char* contractant;
	unsigned char durataImplementare;
	float bugetAlocat;
	float stadiuExecutie;
};


struct Nod
{
	Proiect info;
	Nod* fs;
	Nod* fd;
};


Nod* inserareNod(Nod* a, Proiect p)
{
	if (a)
	{
		if (a->info.idProiect < p.idProiect)
			a->fd = inserareNod(a->fd, p);
		else
			a->fs = inserareNod(a->fs, p);
	}
	else
	{
		Nod* aux = (Nod*)malloc(sizeof(Nod));
		aux->info = p;
		aux->fd = NULL;
		aux->fs = NULL;
		a = aux;
	}
	return a;
}


void readFromFile(const char* fileName, Nod** a)
{
	FILE* f = fopen(fileName, "r");
	if (!f)
	{
		printf("\nNU exista cacatul asta de fisier");
	}
	else
	{
		Proiect p;
		char buffer[100];
		char* delimiter = ",\n";
		while (fgets(buffer,100,f))
		{
			char* element = strtok(buffer, delimiter);
			p.idProiect = atoi(element);

			element = strtok(NULL, delimiter);
			p.contractant = (char*)malloc(sizeof(char) * strlen(element) + 1);
			strcpy(p.contractant, element);

			element = strtok(NULL, delimiter);
			p.durataImplementare = (char)atoi(element);

			element = strtok(NULL, delimiter);
			p.bugetAlocat = atof(element);

			element = strtok(NULL, delimiter);
			p.stadiuExecutie = atof(element);

			*a = inserareNod(*a, p);
		}
	}
}

void afisareProj(Proiect p)
{
	printf("\n-----------------");
	printf("\n %d, %s, %d, %f, %f",p.idProiect,p.contractant,p.durataImplementare,p.bugetAlocat,p.stadiuExecutie);
}

void afisareArbore(Nod* a)
{
	if (a)
	{
		afisareArbore(a->fs);
		afisareProj(a->info);
		afisareArbore(a->fd);
	}
}

int catiCuNumeleX(const char* numeleX, Nod* a)
{
	if (!a)
	{
		return 0;
	}
	else
	{
		int contor = 0;
		if (strcmp(a->info.contractant, numeleX) == 0)
		{
			contor = 1;
		}
		return contor + catiCuNumeleX(numeleX,a->fs) + catiCuNumeleX(numeleX,a->fd);
	}
}

void schimbareProcent(Nod* a, int id, float newProc)
{
	if (a)
	{
		if (a->info.idProiect < id)
		{
			schimbareProcent(a->fd, id, newProc);
		}
		else if(a->info.idProiect>id)
		{
			schimbareProcent(a->fs, id, newProc);
		}
		else
		{
			a->info.stadiuExecutie = newProc;
		}
	}
	else
	{
		printf("\nNu s-a gasit proiectul");
	}
}


int main()
{
	Nod* arb = NULL;
	readFromFile("date.txt", &arb);
	afisareArbore(arb);
	printf("\n%d", catiCuNumeleX("vali", arb));
	schimbareProcent(arb, 3, 34.6);
	printf("\n**************************");
	afisareArbore(arb);
	return 0;
}