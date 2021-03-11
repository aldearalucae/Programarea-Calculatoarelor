#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bmp_header.h"
#include "main.h"

// Intoarce paddingul unei linii
int getPadding(int width) {
	if (width % 4 == 0) {
		return 0;
	}

	return 4 - ((width + 4) % 4);
}

// Intoarce o valoare de tip pixel cu culorile specificate
struct pixel rgb(unsigned char red, unsigned char green, unsigned char blue) {
	struct pixel color;
	color.r = red;
	color.g = green;
	color.b = blue;

	return color;
}

// Scrie poza in fisier
void export_bmp(char* filename, Image image) {
	int real_height = image.height * SCALE;
	int real_width  = 3 * image.width  * SCALE;

	int padding = getPadding(real_width);
	struct bmp_fileheader fileheader;
	struct bmp_infoheader infoheader;

	// Complete fileheader
	fileheader.fileMarker1 = 'B';
	fileheader.fileMarker2 = 'M';
	fileheader.bfSize = 14 + 40 + real_height * (real_width + padding);
	fileheader.unused1 = 0;
	fileheader.unused2 = 0;
	fileheader.imageDataOffset = 14 + 40;

	// Complete infoheader
	infoheader.biSize = 40;
	infoheader.width = image.width * SCALE;
	infoheader.height = image.height * SCALE;
	infoheader.planes = 1;
	infoheader.bitPix = 24;
	infoheader.biCompression = 0;
	infoheader.biSizeImage = real_height * (real_width + padding);
	infoheader.biXPelsPerMeter = 0;
	infoheader.biYPelsPerMeter = 0;
	infoheader.biClrUsed = 0;
	infoheader.biClrImportant = 0;

	FILE* fptr = fopen(filename, "wb");

	fwrite(&fileheader, 1, sizeof(struct bmp_fileheader), fptr);
	fwrite(&infoheader, 1, sizeof(struct bmp_infoheader), fptr);

	int i;
	int l;
	int j;
	int k;
	int t;

	// Pentru fiecare linie din poza, de jos in sus
	for (i = image.height - 1; i >= 0; i--) {
		for (l = 0; l < SCALE; l++) {
			// Pentru fiecare pixel din linie
			for (j = 0; j < image.width; j++) {
				for (k = 0; k < SCALE; k++) {
					fwrite(&image.pixels[i][j], 1, sizeof(struct pixel), fptr);
				}
			}

			unsigned char foo = 0;
			for (t = 0; t < padding; t++) {
				fwrite(&foo, 1, 1, fptr);
			}
		}
	}
	fclose(fptr);
}

// Citeste poza din fisier
void import_bmp(char* filename, Image* image) {
	FILE* fptr = fopen(filename, "rb");
	struct bmp_fileheader fileheader;
	struct bmp_infoheader infoheader;

	fread(&fileheader, 1, sizeof(struct bmp_fileheader), fptr);
	fread(&infoheader, 1, sizeof(struct bmp_infoheader), fptr);

	(*image) = new_board(infoheader.height / SCALE, infoheader.width / SCALE);

	int i;
	int j;

	for (i = (*image).height - 1; i >= 0; i--) {
		for (j = 0; j < (*image).width; j++) {
			fread(&(*image).pixels[i][j], 1, sizeof(Pixel), fptr);
			fseek(fptr, (SCALE - 1) * sizeof(Pixel), SEEK_CUR);
		}

		int offset = getPadding((*image).width * 3 * SCALE);
		fseek(fptr, offset, SEEK_CUR);
		fseek(fptr, (SCALE - 1) * (sizeof(Pixel) * (*image).width * SCALE + offset), SEEK_CUR);
	}

	fclose(fptr);
}

// Verifica daca numarul de argumente este valid
void check_argc(int argc) {
	if (argc != 2) {
		exit(1);
	}
}

// Creeaza o imagine height x width alba
Image new_image(int height, int width) {
	Image image;
	image.pixels = (Pixel**) malloc(sizeof(Pixel*) * height);
	int i;
	int j;

	for (i = 0; i < height; i++) {
 		image.pixels[i] = (Pixel*) malloc(sizeof(Pixel) * width);
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			image.pixels[i][j] = ALB;
		}
	}

	image.height = height;
	image.width  = width;

	return image;
}

// Creeaza o imagine height x width pentru tabla
Image new_board(int height, int width) {
	int i;
	int j;
	Image board = new_image(height, width);

	for (i = 4; i < height; i++) {
		for (j = 0; j < width; j++) {
			board.pixels[i][j] = NEGRU;
		}
	}

	return board;
}

// Elibereaza memoria pentru o imagine
void free_image(Image* image) {
	int i;
	for (i = 0; i < (*image).height; i++) {
		free((*image).pixels[i]);
	}

	free((*image).pixels);
	(*image).pixels = NULL;
}

// Roteste imaginea primita cu un numar de grade
void rotate_image(Image* image, int rotation) {
	Image rotated_image;
	int i;
	int j;

	switch(rotation) {
		case 0:
			return;

		case 90:
			rotated_image = new_image((*image).width, (*image).height);

			for (i = 0; i < (*image).height; i++) {
				for (j = 0; j < (*image).width; j++) {
					rotated_image.pixels[j][(*image).height - 1 - i] = (*image).pixels[i][j];
				}
			}
			free_image(&(*image));
			(*image) = new_image((*image).width, (*image).height);
			break;

		case 180:
			rotated_image = new_image((*image).height, (*image).width);

			for (i = 0; i < (*image).height; i++) {
				for (j = 0; j < (*image).width; j++) {
					rotated_image.pixels[(*image).height - 1 - i][(*image).width - 1 - j] = (*image).pixels[i][j];
				}
			}
			free_image(&(*image));
			(*image) = new_image((*image).height, (*image).width);
			break;

		case 270:
			rotated_image = new_image((*image).width, (*image).height);

			for (i = 0; i < (*image).height; i++) {
				for (j = 0; j < (*image).width; j++) {
					rotated_image.pixels[(*image).width - 1 - j][i] = (*image).pixels[i][j];
				}
			}
			free_image(&(*image));
			(*image) = new_image((*image).width, (*image).height);
			break;

		default:
			exit(3);
	}

	(*image).height = rotated_image.height;
	(*image).width = rotated_image.width;

	for (i = 0; i < rotated_image.height; i++) {
		for (j = 0; j < rotated_image.width; j++) {
			(*image).pixels[i][j] = rotated_image.pixels[i][j];
		}
	}

	free_image(&rotated_image);
}

// Creeaza o noua imagine de tip type
Image new_piece(char type, int rotation) {
	Image image;

	switch(type) {
		case 'O':
			image = new_image(4, 4);

			image.pixels[1][1] = GALBEN;
			image.pixels[1][2] = GALBEN;
			image.pixels[2][1] = GALBEN;
			image.pixels[2][2] = GALBEN;
			break;

		case 'I':
			image = new_image(6, 3);

			image.pixels[1][1] = ALBASTRU;
			image.pixels[2][1] = ALBASTRU;
			image.pixels[3][1] = ALBASTRU;
			image.pixels[4][1] = ALBASTRU;
			break;

		case 'S':
			image = new_image(4, 5);

			image.pixels[1][2] = ROSU;
			image.pixels[1][3] = ROSU;
			image.pixels[2][1] = ROSU;
			image.pixels[2][2] = ROSU;
			break;

		case 'Z':
			image = new_image(4, 5);

			image.pixels[1][1] = VERDE;
			image.pixels[1][2] = VERDE;
			image.pixels[2][2] = VERDE;
			image.pixels[2][3] = VERDE;
			break;

		case 'L':
			image = new_image(5, 4);

			image.pixels[1][1] = PORTOCALIU;
			image.pixels[2][1] = PORTOCALIU;
			image.pixels[3][1] = PORTOCALIU;
			image.pixels[3][2] = PORTOCALIU;
			break;

		case 'J':
			image = new_image(5, 4);

			image.pixels[1][2] = ROZ;
			image.pixels[2][2] = ROZ;
			image.pixels[3][1] = ROZ;
			image.pixels[3][2] = ROZ;
			break;

		case 'T':
			image = new_image(4, 5);

			image.pixels[1][1] = MOV;
			image.pixels[1][2] = MOV;
			image.pixels[1][3] = MOV;
			image.pixels[2][2] = MOV;
			break;

		default:
			exit(2);
	}

	rotate_image(&image, rotation);

	return image;
}

// Verifica egalitatea a doi pixeli
bool cmp_rgb(Pixel pixel, Pixel clone) {
	return pixel.r == clone.r & pixel.g == clone.g & pixel.b == clone.b;
}

// Adauga piesa pe tabla (o scrie)
void add_piece_to_board(Image piece, Image* board, int line, int column) {
	int i;
	int j;
	for (i = 1; i < piece.height - 1; i++) {
		for (j = 1; j < piece.width - 1; j++) {
			if (!cmp_rgb(piece.pixels[i][j], ALB)) {
				(*board).pixels[i - 1 + line][j - 1 + column].r = piece.pixels[i][j].r;
				(*board).pixels[i - 1 + line][j - 1 + column].g = piece.pixels[i][j].g;
				(*board).pixels[i - 1 + line][j - 1 + column].b = piece.pixels[i][j].b;
			}
		}
	}
}

// Muta piesa cat de jos se poate
void move(Image* board, Image piece, int* line, int column) {
	int i;
	int j;

	while (true) {
		if ((*line) + piece.height - 2 == (*board).height) {
			return;
		} else {
			(*line)++;
		}

		for (i = 1; i < piece.height - 1; i++) {
			for (j = 1; j < piece.width - 1; j++) {
				if (!cmp_rgb(piece.pixels[i][j], ALB)
						&& !cmp_rgb((*board).pixels[i + (*line) - 1][j + column - 1], ALB)
						&& !cmp_rgb((*board).pixels[i + (*line) - 1][j + column - 1], NEGRU)
					) {
						(*line)--;
						return;
				}
			}
		}
	}
}

// Verifica daca ultima piesa nu a avut loc pe tabla
void test_running(Image* board, bool* running) {
	int i;
	int j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < (*board).width; j++) {
			if (!cmp_rgb((*board).pixels[i][j], ALB)) {
				*running = false;
			}
		}
	}
}

// Adauga piesa pe tabla, o muta si sterge liniile completate
void update_board(Image* board, bool* running, Image piece, int column) {
	int line = 0;

	move(board, piece, &line, column);
	add_piece_to_board(piece, board, line, column);
	test_running(board, running);
}

void clear_line(Image* board, int line) {
	int i;
	int j;
	// Sterg linia curenta
	for (j = 0; j < (*board).width; j++) {
		(*board).pixels[line][j] = NEGRU;
	}

	// Toate liniile de mai sus le coboara cu o pozitie
	for (i = line; i > 4; i--) {
		for (j = 0; j < (*board).width; j++) {
			(*board).pixels[i][j].r = (*board).pixels[i - 1][j].r;
			(*board).pixels[i][j].g = (*board).pixels[i - 1][j].g;
			(*board).pixels[i][j].b = (*board).pixels[i - 1][j].b;
		}
	}

	// Face linia 4 neagra (prima linie utila din tabla)
	for (j = 0; j < (*board).width; j++) {
		(*board).pixels[4][j] = NEGRU;
	}
}

void clear_full_lines(Image* board) {
	int line = (*board).height - 1;

	while (line != 4) {
		int j;
		bool can_be_cleared = true;
		for (j = 0; j < (*board).width; j++) {
			if (cmp_rgb((*board).pixels[line][j], NEGRU)) {
				can_be_cleared = false;
				break;
			}
		}

		if (can_be_cleared) {
			clear_line(board, line);
			line++;
		}

		line--;
	}
}

bool is_empty(Image board) {
	
	int i;
	int j;

	for (i = 4; i < board.height; i++) {
		for (j = 0; j < board.width; j++) {
			if (!cmp_rgb(board.pixels[i][j], NEGRU)) {
				return false;
			}
		}
	}
	return true;
}

bool match(Image board, Image piece, int line, int column) {

	if (line + piece.height - 2 > board.height) {
		return false;
	}

	if (column + piece.width - 2 > board.width) {
		return false;
	}

	int i;
	int j;

	for (i = 1; i < piece.height - 1; i++) {
		for (j = 1; j < piece.width - 1; j++) {
			if (!cmp_rgb(piece.pixels[i][j], ALB)) {
				if (!cmp_rgb(piece.pixels[i][j], board.pixels[line + i - 1][column + j - 1])) {
					return false;
				}
			}
		}
	}

	return true;
}

void delete(Image* board, Image piece, int line, int column) {

	int i;
	int j;

	for (i = 1; i < piece.height - 1; i++) {
		for (j = 1; j < piece.width - 1; j++) {
			if (!cmp_rgb(piece.pixels[i][j], ALB)) {
				(*board).pixels[line + i - 1][column + j - 1] = NEGRU;
			}
		}
	}
}

void check_for_pieces(Image* board) {
	int count = 0;
	char letters[8];
	int angles[4] = {0, 90, 180, 270};
	strcpy(letters, "OISZLJT");

	char   nume_piese[5000];
	int   unghi_piese[5000];
	int coloane_piese[5000];

	int i;
	int j;
	int p;
	int a;

	for (i = 4; i < (*board).height; i++) {
		for (j = 0; j < (*board).width; j++) {
			for (p = 0; p < 7; p++) {
				for (a = 0; a < 4; a++) {
					Image image = new_piece(letters[p], angles[a]);

					if (match(*board, image, i, j)) {
						nume_piese[count] = letters[p];
						unghi_piese[count] = angles[a];
						coloane_piese[count] = j;
						count++;

						delete(board, image, i, j);
						p = 7;
						a = 4;
						export_bmp("part.bmp", *board);
					}
					free_image(&image);
				}
			}
		}
	}

	FILE* fptr = fopen("bonus.out", "wt");
	fprintf(fptr, "%d\n", count);

	for (i = 0; i < count; i++) {
		fprintf(fptr, "%c %d %d\n", nume_piese[i], unghi_piese[i], coloane_piese[i]);
	}

}

void task_1() {
	Image image;

	char letters[8];
	strcpy(letters, "OISZLJT");

	int i;

	for (i = 0; i < 7; i++) {
		image = new_piece(letters[i], 0);

		char *filename = malloc(256);
		sprintf(filename, "piesa_%c.bmp", letters[i]);

		export_bmp(filename, image);
		free_image(&image);
		free(filename);
	}
}

void task_2() {
	Image image;

	char letters[8];
	int angles[3] = {90, 180, 270};
	strcpy(letters, "OISZLJT");

	int i;
	int j;

	for (i = 0; i < 7; i++) {
		for (j = 0; j < 3; j++) {
			image = new_piece(letters[i], angles[j]);

			char *filename = malloc(256);
			sprintf(filename, "piesa_%c_%d.bmp", letters[i], angles[j]);

			export_bmp(filename, image);
			free_image(&image);
			free(filename);
		}
	}
}

void task_3() {
	FILE* fptr = fopen("cerinta3.in", "rt");

	int N;
	int H;
	int L;

	fscanf(fptr, "%d %d %d\n", &N, &H, &L);

	Image board = new_board(H + 4, L);

	bool running = true;

	int i;

	for (i = 0; i < N && running; i++) {

		char piece_name;
		int rotation;
		int column;

		fscanf(fptr, "%c %d %d\n", &piece_name, &rotation, &column);

		Image piece = new_piece(piece_name, rotation);

		update_board(&board, &running, piece, column);

		if (running) {
			clear_full_lines(&board);
		}
	}

	export_bmp("task3.bmp", board);
	free_image(&board);
	fclose(fptr);
}

void task_4() {
	Image board;
	import_bmp("cerinta4.bmp", &board);

	FILE* fptr = fopen("cerinta4.in", "rt");

	int N;
	fscanf(fptr, "%d\n", &N);

	bool running = true;

	int i;

	for (i = 0; i < N && running; i++) {

		char piece_name;
		int rotation;
		int column;

		fscanf(fptr, "%c %d %d\n", &piece_name, &rotation, &column);

		Image piece = new_piece(piece_name, rotation);

		update_board(&board, &running, piece, column);

		if (running) {
			clear_full_lines(&board);
		}
	}

	export_bmp("task4.bmp", board);
	free_image(&board);
	fclose(fptr);
}

void task_bonus() {
	Image board;
	import_bmp("bonus.bmp", &board);

	while (!is_empty(board)) {
		check_for_pieces(&board);
	}

	free_image(&board);
}

int main(int argc, char** argv) {
	check_argc(argc);

	switch (argv[1][0]) {
		case '1': task_1(); break;
		case '2': task_2(); break;
		case '3': task_3(); break;
		case '4': task_4(); break;
		case '5': task_bonus(); break;
		default: exit(4);
	}

	return 0;
}
