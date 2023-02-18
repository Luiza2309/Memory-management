#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "structs.h"

int add_last(void **arr, int *len, data_structure *data)
{
	if(*arr == NULL) {
		*arr = calloc(1, 5 + data->header->len);
		if(!arr) return 1;
	} else {
		*arr = realloc(*arr, *len + 5 + data->header->len);
		if(!arr) return 1;
	}
	// alocarea spatiului

	memcpy(*arr + *len, &data->header->type, 1);  // adauga tipul in vector
	memcpy(*arr + *len + 1, &data->header->len, 4);  // adauga lungimea datei
	memcpy(*arr + *len + 5, data->data, data->header->len);  // adauga data
	*len += 5 + data->header->len;  // nr de octeti adaugati la vector
	return 0;
}

int add_at(void **arr, int *len, data_structure *data_tot, int index)
{
	if(index < 0) { printf("Index gresit!"); exit(1); }
	
	int i = 0;  // memoreaza nr de octeti peste care am sarit
	while(index != 0 && i < *len) {
		i += 1;  // sare peste tip
		i += *(int*)(*arr + i);  // sare peste lungimea datei
		i += 4;  // sare peste len
		index--;
	}
	// parcurge vectorul pana la indexul indicat

	if(i > *len) add_last(arr, len, data_tot);  // daca indexul nu se afla in vector
	else {
		*arr = realloc(*arr, *len + 5 + data_tot->header->len);
		if(!arr) return 1;

		memcpy(*arr + i + 5 + data_tot->header->len, *arr + i, *len - i);
		// muta datele aflate dupa index mai "la dreapta" pentru a face loc elementului adaugat

		memcpy(*arr + i, &data_tot->header->type, 1);
		memcpy(*arr + i + 1, &data_tot->header->len, 4);
		memcpy(*arr + i + 5, data_tot->data, data_tot->header->len);
		*len +=  5 + data_tot->header->len;  // nr de octeti adaugati la vector
	}

	return 0;
}

void find(void *data_block, int len, int index) 
{
	if(index >= 0) {
		int i = 0;  // memoreaza nr de octeti peste care am sarit
		while(index >= 1) {
			i += 1;  // sare peste tip
			i += *(int*)(data_block + i);  // sare peste lungimea datei
			i += 4;  // sare peste len
			index--;
		}
		// parcurge vectorul pana la indexul indicat

		char tip = *(char*)(data_block + i);
		printf("Tipul %c\n", tip);
		i += 1;  // sare peste tip
		i += 4;  // sare peste len
		if(tip == '1') {
			void* p = data_block + i;
			while(*(char*)p != '\0') {
				p++;  // sare peste caracterele primului string pana la '\0'
			}
			p++;  // sare peste caracterul null
			printf("%s pentru %s\n", (char*)(data_block + i), (char*)(p + 2));
			printf("%"PRId8"\n%"PRId8"\n", *(int8_t*)(p), *(int8_t*)(p + 1));
		} else if(tip == '2') {
			void* p = data_block + i;
			while(*(char*)p != '\0') {
				p++;
			}
			p++;
			printf("%s pentru %s\n", (char*)(data_block + i), (char*)(p + 6));
			printf("%"PRId16"\n%"PRId32"\n", *(int16_t*)(p), *(int32_t*)(p + 2));
		} else if(tip == '3') {
			void* p = data_block + i;
			while(*(char*)p != '\0') {
				p++;
			}
			printf("%s pentru %s\n", (char*)(data_block + i), (char*)(p + 9));
			printf("%"PRId32"\n%"PRId32"\n", *(int32_t*)(p + 1), *(int32_t*)(p + 5));
		}
	}
}

int delete_at(void **arr, int *len, int index)
{
	int i = 0;  // memoreaza nr de octeti peste care am sarit
	while(index >= 1) {
		i += 1;  // sare peste tip
		i += *(int*)(*arr + i);  // sare peste lungimea datei
		i += 4;  // sare peste len
		index--;
	}
	int l = *(int*)(*arr + i + 1) + 5;  // contine nr de octeti ai datei care trebuie stearsa
	memcpy(*arr + i, *arr + i + l, *len - i - l);
	// copiaza elementele care se afla dupa index peste elementul care trebuie sters
	*arr = realloc(*arr, *len - l);  // realoca memoria fara octetii datei sterse
	*len = *len - l;  // sterge octetii din octetii totali ai vectorului

	return 0;
}

void print(void* arr, int len)
{
	int i = 0;
	int lung = 0;
	while(len > 0) {
		char tip = *(char*)(arr + i);
		printf("Tipul %c\n", tip);
		i += 1;  // sare peste tip
		lung = *(int*)(arr + i);  // nr de octeti ai datei, fara header
		i += 4;  // sare peste len
		if(tip == '1') {
			void* p = arr + i;
			while(*(char*)p != '\0') {
				p++;  // sare peste caracterele primului string pana la '\0'
			}
			p++;  // sare peste caracterul null
			printf("%s pentru %s\n", (char*)(arr + i), (char*)(p + 2));
			printf("%"PRId8"\n%"PRId8"\n", *(int8_t*)(p), *(int8_t*)(p + 1));
		} else if(tip == '2') {
			void* p = arr + i;
			while(*(char*)p != '\0') {
				p++;
			}
			p++;
			printf("%s pentru %s\n", (char*)(arr + i), (char*)(p + 6));
			printf("%"PRId16"\n%"PRId32"\n", *(int16_t*)(p), *(int32_t*)(p + 2));
		} else if(tip == '3') {
			void* p = arr + i;
			while(*(char*)p != '\0') {
				p++;
			}
			p++;
			printf("%s pentru %s\n", (char*)(arr + i), (char*)(p + 8));
			printf("%"PRId32"\n%"PRId32"\n", *(int32_t*)(p), *(int32_t*)(p + 4));
		}

		i += lung;
		len -= 5 + lung;  // scade nr de octeti ai elementului care a fost afisat
		printf("\n");
	}
}

data_structure* convert_tip1(char *dedicator, char *suma1, char *suma2, char *dedicatul)
{
	unsigned int nr_octeti = strlen(dedicator) + 1 + 1 + 1 + strlen(dedicatul) + 1;
	data_structure *data_tot = calloc(1, 5 + nr_octeti);
	data_tot->data = calloc(1, nr_octeti);
	data_tot->header = calloc(1, 5);  // 5 octeti are headerul fara padding
	int a = atoi(suma1);
	int8_t s1 = (int8_t)a;
	int b = atoi(suma2);
	int8_t s2 = (int8_t)b;
	memcpy(data_tot->data, dedicator, strlen(dedicator) + 1);
	memcpy(data_tot->data + strlen(dedicator) + 1, &s1, 1);
	memcpy(data_tot->data + strlen(dedicator) + 2, &s2, 1);
	memcpy(data_tot->data + strlen(dedicator) + 3, dedicatul, strlen(dedicatul) + 1);
	data_tot->header->type = '1';
	data_tot->header->len = nr_octeti;

	return data_tot;
}

data_structure* convert_tip2(char *dedicator, char *suma1, char *suma2, char *dedicatul)
{
	unsigned int nr_octeti = strlen(dedicator) + 1 + 2 + 4 + strlen(dedicatul) + 1;
	data_structure *data_tot = calloc(1, 5 + nr_octeti);
	data_tot->data = calloc(1, nr_octeti);
	data_tot->header = calloc(1, 5);
	int a = atoi(suma1);
	int16_t s1 = (int16_t)a;
	int b = atoi(suma2);
	int32_t s2 = (int32_t)b;
	memcpy(data_tot->data, dedicator, strlen(dedicator) + 1);
	memcpy(data_tot->data + strlen(dedicator) + 1, &s1, 2);
	memcpy(data_tot->data + strlen(dedicator) + 3, &s2, 4);
	memcpy(data_tot->data + strlen(dedicator) + 7, dedicatul, strlen(dedicatul) + 1);
	data_tot->header->type = '2';
	data_tot->header->len = nr_octeti;

	return data_tot;
}

data_structure* convert_tip3(char *dedicator, char *suma1, char *suma2, char *dedicatul)
{
	unsigned int nr_octeti = strlen(dedicator) + 1 + 4 + 4 + strlen(dedicatul) + 1;
	data_structure *data_tot = calloc(1, 5 + nr_octeti);
	data_tot->data = calloc(1, nr_octeti);
	data_tot->header = calloc(1, 5);
	int a = atoi(suma1);
	int32_t s1 = (int32_t)a;
	int b = atoi(suma2);
	int32_t s2 = (int32_t)b;
	memcpy(data_tot->data, dedicator, strlen(dedicator) + 1);
	memcpy(data_tot->data + strlen(dedicator) + 1, &s1, 4);
	memcpy(data_tot->data + strlen(dedicator) + 5, &s2, 4);
	memcpy(data_tot->data + strlen(dedicator) + 9, dedicatul, strlen(dedicatul) + 1);
	data_tot->header->type = '3';
	data_tot->header->len = nr_octeti;

	return data_tot;
}

int main() {
	void *arr = NULL;
	int len = 0;  // total octeti din vector
	data_structure *data_tot;
	char tip;
	char *dedicator;
	char *suma1;
	char *suma2;
	char *dedicatul;
	int index = 0;
	char *linie = calloc(256, 1);
	char *task;
	
	while(fgets(linie, 256, stdin)) {
		task = strtok(linie, " \n\r");
		if(strcmp(task, "insert") == 0) {
			task = strtok(NULL, " ");  // citeste tipul
			tip = *task;  // converteste tipul in char
			dedicator = strtok(NULL, " ");
			suma1 = strtok(NULL, " ");
			suma2 = strtok(NULL, " ");
			dedicatul = strtok(NULL, " \n\r");

			if(tip == '1') {
				data_tot = convert_tip1(dedicator, suma1, suma2, dedicatul);
			} else {
				if(tip == '2') {
					data_tot = convert_tip2(dedicator, suma1, suma2, dedicatul);
				} else {
					data_tot = convert_tip3(dedicator, suma1, suma2, dedicatul);
				}
			}

			add_last(&arr, &len, data_tot);

			// eliberare memorie
			free(data_tot->header);
			free(data_tot->data);
			free(data_tot);
		}
		else if(strcmp(task, "insert_at") == 0) {
			task = strtok(NULL, " ");  // citeste indexul
			index = atoi(task);  // converteste indexul in numar
			task = strtok(NULL, " ");
			tip = *task;
			dedicator = strtok(NULL, " ");
			suma1 = strtok(NULL, " ");
			suma2 = strtok(NULL, " ");
			dedicatul = strtok(NULL, " \n\r");
			
			if(tip == '1') {
				data_tot = convert_tip1(dedicator, suma1, suma2, dedicatul);
			} else {
				if(tip == '2') {
					data_tot = convert_tip2(dedicator, suma1, suma2, dedicatul);
				} else {
					data_tot = convert_tip3(dedicator, suma1, suma2, dedicatul);
				}
			}

			add_at(&arr, &len, data_tot, index);

			// eliberare memorie
			free(data_tot->header);
			free(data_tot->data);
			free(data_tot);
		}
		else if(strcmp(task, "delete_at") == 0) {
			task = strtok(NULL, " ");
			index = atoi(task);
			delete_at(&arr, &len, index);
		}
		else if(strcmp(task, "find") == 0) {
			task = strtok(NULL, " ");
			index = atoi(task);
			find(arr, len, index);
			printf("\n");
		}
		else if(strcmp(task, "print") == 0) {
			print(arr, len);
		}
		else if(strcmp(task, "exit") == 0) {
			free(linie);
			free(arr);
			break;
		}
	}

	return 0;
}
