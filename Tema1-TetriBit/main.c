/*
 * Nume   : Aldea 
 * Prenume: Raluca-Elena
 * Grupa  : 311CD
 */

#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

// Citeste valoarea hartii pe 64 de biti
uint64_t read_map() {
	uint64_t map = 0;
	char c;
	scanf("%c", &c);

	while (c >= '0' && c <= '9') {
		map = map * 10 + c - '0';
		scanf("%c", &c);
	}

	return map;
}

// Afiseaza o linie din harta
void print_line(unsigned char line) {
	unsigned char mask = 0b10000000;
	int i;

	for (i = 0; i < 8; i++) {
		if ((line & mask) == 0)
			printf(".");
		else
			printf("#");

		mask >>= 1;
	}
	printf("\n");
}

// Intoarce a index-a linie din harta map
uint64_t get_line(uint64_t map, int index) {
	return ((map) << index * 8) >> 7 * 8;
}

bool hazard_exists(uint64_t* map1, uint64_t map2) {
	if ((*map1 & map2) > 0)
		return true;

	return false;
}

// Afiseaza intreaga harta
void print_map(uint64_t map) {
	int i;

	for (i = 0; i < 8; i++)
		print_line(get_line(map, i));

	printf("\n");
}

// Calculeaza numarul de zerouri din harta
int get_zeros_number(uint64_t map) {
	int zeros_number = 0;
	int i, j;

	for (i = 0; i < 8; i++) {
		unsigned char mask = 0b00000001;
		unsigned char line = get_line(map, i);

		for (j = 0; j < 8; j++) {
			if ((line & mask) == 0) {
				zeros_number++;
			}
			mask <<= 1;
		}
	}

	return zeros_number;
}

// Calculeaza scorul
float get_score(uint64_t map, int completed_lines) {
	return sqrt(get_zeros_number(map)) + pow(1.25, completed_lines);
}

// Afiseaza scorul
void print_score(uint64_t map, int completed_lines) {
	float score = get_score(map, completed_lines);

	printf("GAME OVER!\n");
	printf("Score:%.2f\n", score);
}

// Shifteaza harta cu o coloana la stanga / dreapta
//  1 pentru dreapta
// -1 pentru stanga
uint64_t shift(uint64_t map, int left_or_right) {
	uint64_t new_map = 0;
	int i;

	for (i = 0; i < 8; i++) {
		new_map <<= 8;

		if (left_or_right == 1)
			new_map += get_line(map, 0) >> 1;
		else
			new_map += get_line(map, 0) << 1;

		map = map << 8;
	}

	return new_map;
}

// Verific daca se poate shifta la dreapta / stanga harta
//  1 pentru dreapta
// -1 pentru stanga
// Verific daca se poate shifta la dreapta harta
bool can_shift(uint64_t map, int left_or_right) {
	unsigned char mask;
	int i;

	if (left_or_right == 1)
		mask = 0b00000001;
	else
		mask = 0b10000000;		

	for (i = 0; i < 8; i++) {
		unsigned char line = get_line(map, i);
		if ((line & mask) > 0)
			return false;
	}

	return true;
}

// Intoarce numarul primei linii care este completa
// sau -1 daca nu exista o linie completa
int line_completed(uint64_t map) {
	unsigned char mask = 0b11111111;
	int i;

	for (i = 0; i < 8; i++)
		if (get_line(map, i) == mask)
			return i;

	return -1;
}

void move(uint64_t* map, bool* running, int* completed_lines) {
	uint64_t piece;    // piesa
	uint64_t temp = 0; // harta care contine doar piesa
	int moves;         // mutarea citita
	int moves_offset = 0;
	int i;

	// Citesc mutarile
	piece = read_map();

	// Pentru fiecare mutare
	for (i = 0; i < 8; i++) {
		scanf("%d ", &moves);

		// Verific daca ultima linie din piesa creaza hazard
		if (hazard_exists(map, (temp >> 8) + (get_line(piece, 7) << 7 * 8))) {

			if (piece > 0) {
				*running = false;
				print_map(*map + temp);			
			}

			moves_offset = 7 - i;
			break;
		}
		
		print_map(*map + temp);

		temp = (temp >> 8) + (get_line(piece, 7) << 7 * 8);
		piece = piece >> 8;

		while (moves != 0) {
			int sign = (moves / fabs(moves));

			if (!can_shift(temp, sign) || hazard_exists(map, shift(temp, sign)))
				break;

			temp   = shift(temp,  sign);
			piece  = shift(piece, sign);
			moves -= sign;
		}
	}

	// Citire inutila
	for (i = 0; i < moves_offset; i++) {
		scanf("%d ", &moves);
	}

	// Verificare linie completa
	int nr_line = line_completed(*map + temp);

	if (nr_line != -1)
		print_map(*map + temp);

	while (nr_line != -1) {
		uint64_t new_map  = 0;
		uint64_t new_temp = 0;

		// Copiez liniile de sub linia care trebuie stearsa
		for (i = 7; i > nr_line; i--) {
			new_map  += get_line(*map, i) << ((7 - i) * 8);
			new_temp += get_line(temp, i) << ((7 - i) * 8);	
		}

		// Copiez liniile de deasupra liniei care trebuie stearsa
		for (i = nr_line - 1; i >= 0; i--) {
			new_map  = new_map  + (get_line(*map, i) << (6 - i) * 8);
			new_temp = new_temp + (get_line(temp, i) << (6 - i) * 8);
		}

		// Adaug urmatoarea linie din piesa in temp
		new_temp = new_temp + get_line(piece, 7);
		piece = piece >> 8;

		// Actualizez starea jocului
		*map = new_map;
		temp = new_temp;

		// Incrementez numarul de linii completate
		(*completed_lines)++;

		// Verific daca mai exista linii completate
		nr_line = line_completed(*map + temp);
	}

	// Actualizez harta cu mutarile piesei (toate cele 8)
	*map = *map + temp;
}

int main(void)
{
	// Declarare date
	uint64_t map;
	int moves;
	int completed_lines = 0;
	bool running = true;

	// Citesc harta si numarul de mutari
	map = read_map();
	scanf("%d\n", &moves);

	// Aplica fiecare mutare
	while (running == true && moves > 0) {
		move(&map, &running, &completed_lines);
		moves--;
	}

	// Afisare finala
	print_map(map);
	print_score(map, completed_lines);
	
	return 0;
}
