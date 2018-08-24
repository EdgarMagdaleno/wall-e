#include <stdio.h>
#include <string.h>
// #include "instructions.h"
#include "hashtable.c"

#define MAGIC_NUMBER "\x00\x45\x56\x41\x00\x00\x00\x00"

struct state {
	unsigned short data_size;
	unsigned short exec_size; 
};

void decli(FILE *fp, struct state *s) {
	
	s->data_size += 4;
}

/*
 * TODO: Change this for a hash table.
 */
void *search(char *buf) {
	if (!strcmp(buf, "decli")) {
		return decli;		
	}

	return NULL;
}

void lex(FILE *fp, FILE *fp_out, struct state *s) {
	char buffer[64];
	void (*body)(FILE *, struct state *);
	int c = 0;

	while ((c = fscanf(fp, "%s", buffer)) != -1) {
		body = search(buffer);

		if (!body) {
			printf("Invalid instruction\n");
			return;
		}

		body(fp, s);
	}
}

int main(int argc, char *args[]) {
	FILE *fp, *fp_out;
/*
	hashtable *ht = new_ht(20);
	ht_item *item = new_ht_item("PUSHI", 5);
	hash_item(ht, item);
	item = new_ht_item("PUSHD", 7);
	hash_item(ht, item);
	item = new_ht_item("PUSHC", 2);
	hash_item(ht, item);
	item = new_ht_item("PUSHS", 10);
	hash_item(ht, item);
	item = new_ht_item("WRTLN", 1);
	hash_item(ht, item);
	item = new_ht_item("WRTLN", 1);
	hash_item(ht, item);

	for (int i = 0; i < ht->size; i++) {
		ht_item *item = ht->items[i];
		
		if (item) {
			printf("%s -> %i\n", item->label, item->cost);

			while (item->next) {
				item = item->next;
				printf("%s -> %i\n", item->label, item->cost);
			}
		}
	}
*/
	if (!args[1]) {
		printf("Needs more arguments\n");
		return 1;
	}

	fp = fopen(args[1], "r");
	if (!fp) {
		printf("Invalid file\n");
		return 1;
	}
	
	fp_out = fopen("out.bin", "wb");
	if (!fp_out) {
		printf("Could not create output file\n");
	}

	fwrite(MAGIC_NUMBER, sizeof(MAGIC_NUMBER) - 1, 1, fp_out);
	
	struct state s = {};
	lex(fp, fp_out, &s);

	printf("data: %i, exec: %i\n", s.data_size, s.exec_size);
	fclose(fp);
	fclose(fp_out);
	return 0;
}
