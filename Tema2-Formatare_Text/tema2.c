#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000 // lungimea maxima a unei linii
#define MAX_LINES_NO    1000 // numarul maxim de linii

// Citesc datele de intrare din fisier (textul)
void read_file(char *file_name, char ***text, int *text_line_count) {
	FILE *fptr = fopen(file_name, "r");

	char buf[MAX_LINE_LENGTH];

	while (fgets(buf, MAX_LINE_LENGTH, fptr)) {
    	strcpy((*text)[*text_line_count], buf);
    	(*text_line_count)++;
	}

  	fclose(fptr);
}

// Afisez datele de iesire in fisier (textul)
void write_file(char *file_name, char **result, int result_line_count) {
	FILE *fptr = fopen(file_name, "w");
	int i;

	for (i = 0; i < result_line_count; i++) {
		fputs(result[i], fptr);
  	}

  	fclose(fptr);
}

// Alocare memorie
void alloc_mem(char ***text) {
	(*text) = (char **) malloc(sizeof(char*) * MAX_LINES_NO);
	int i;

	for (i = 0; i < MAX_LINES_NO; i++) {
		(*text)[i] = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
	}
}

// Eliberare memorie
void free_mem(char ***text) {
	int i;

	for (i = 0; i < MAX_LINES_NO; i++) {
		free((*text)[i]);
	}

	free(*text);

	*text = NULL;
}

// Intoarce lungimea celei mai mari linii
int get_max_line_len(char **text, int text_line_count) {
	int max_len = strlen(text[0]);
	int i;


	for (i = 1; i < text_line_count; i++) {
		max_len = strlen(text[i]) > max_len ? strlen(text[i]) : max_len;
	}

	return max_len;
}

// Intoarce lungimea unei linii fara spatii
int get_line_len(char *line) {
	char foo[1000];
	strcpy(foo, line);

	int len = 0;

	char *token = strtok(foo, " \t\n");

	while (token != NULL) {
		len += strlen(token);
		token = strtok(NULL, " \t\n");
	}

	return len;
}

// Intoarce numarul de cuvinte de pe o linie
int get_word_count(char *line) {
	int len = 0;
	char foo[1000];
	strcpy(foo, line);

	char *token = strtok(foo, " \t\n");

	while (token != NULL) {
		len++;
		token = strtok(NULL, " \t\n");
	}

	return len;
}

// Elimina trailing spaces (tab sau spatiu)
char** remove_trailing_spaces(char **text, int text_line_count) {
	int i;

	for (i = 0; i < text_line_count; i++) {
		while (text[i][strlen(text[i]) - 2] == ' ') {
			text[i][strlen(text[i]) - 2] = '\n';
			text[i][strlen(text[i]) - 1] = '\0';
		}
	}

	return text;
}

// Functia WRAP
char** wrap_text(char **text, int *text_line_count, int max_line_length) {
	char **result;
	alloc_mem(&result);

	int j = 0;
	int i;


	for (i = 0; i < *text_line_count; i++) {
		if (strcmp(text[i], "\n") == 0) {
			result[j][strlen(result[j]) - 1] = '\n';
			j++;
			strcpy(result[j], "\n");
			j++;
			continue;
		}

		char *token = strtok(text[i], " \n\t");

		while (token != NULL) {
			if (strlen(result[j]) + strlen(token) > max_line_length) {
				result[j][strlen(result[j]) - 1] = '\n';
				j++;
			}

			strcat(result[j], token);
			strcat(result[j], " ");
			token = strtok(NULL, " \n\t");
		}
	}

	result[j][strlen(result[j]) - 1] = '\n';

	free_mem(&text);
	*text_line_count = j + 1;

	return result;
}

// Functia CENTER
char** center_text(char **text, int *text_line_count, int start_line, int stop_line) {
	char **result;
	alloc_mem(&result);

	int max_len = get_max_line_len(text, *text_line_count);
	int i;
	int j;

	for (i = 0; i < *text_line_count; i++) {
		if (start_line <= i && i <= stop_line && strcmp(text[i], "\n") != 0) {
			for (j = 0; j < (max_len - strlen(text[i]) + 1) / 2; j++) {
				strcat(result[i], " ");
			}
		}

		strcat(result[i], text[i]);
	}

	free(text);
	return result;
}

// Functia ALIGN LEFT
char** align_left(char **text, int *text_line_count, int start_line, int stop_line) {
	int i;
	int j;

	for (i = start_line; i <= stop_line; i++) {
		int no_trailing_spaces = 0;		

		while (text[i][no_trailing_spaces] == ' ') {
			no_trailing_spaces++;
		}

		int new_len = strlen(text[i]) - no_trailing_spaces;

		for (j = 0; j < new_len; j++) {
			text[i][j] = text[i][j + no_trailing_spaces];
		}
		text[i][new_len] = '\0';
	}

	return text;
}

// Functia ALIGN RIGHT
char** align_right(char **text, int *text_line_count, int start_line, int stop_line) {
	char **result;
	alloc_mem(&result);

	int max_len = get_max_line_len(text, *text_line_count);
	int i;
	int j;


	for (i = 0; i < *text_line_count; i++) {
		if (start_line <= i && i <= stop_line && strcmp(text[i], "\n") != 0) {
			for (j = 0; j < max_len - strlen(text[i]); j++) {
				strcat(result[i], " ");
			}
		}

		strcat(result[i], text[i]);
	}

	free(text);
	return result;
}

// Functia JUSTIFY
char** align_justify(char **text, int *text_line_count, int start_line, int stop_line) {
	char **result;
	alloc_mem(&result);

	int max_len = get_max_line_len(text, *text_line_count) - 1;
	int i;
	int j;


	for (i = 0; i < *text_line_count; i++) {
		if (i <= *text_line_count - 2 && strcmp(text[i + 1], "\n") != 0 && start_line <= i && i <= stop_line) {
			int len = get_line_len(text[i]);
			int no_words = get_word_count(text[i]);
			
			char *rest;
			rest = (char*) malloc(1000);
			strcpy(rest, text[i]);
			
			if (no_words > 1) {
				int spaces = (max_len - len) / (no_words - 1);
				int no_more_spaces = (max_len - len) % (no_words - 1);

				// printf("%d %d\n", spaces, no_more_spaces);

				char *token = strtok_r(rest, " \t\n", &rest);

				strcat(result[i], token);

				for (j = 0; j < no_words - 1; j++) {

					for (j = 0; j < spaces; j++) {
						strcat(result[i], " ");
					}

					if (no_more_spaces > 0) {
						strcat(result[i], " ");
						no_more_spaces--;
					}
					token = strtok_r(rest, " \t\n", &rest);
					strcat(result[i], token);
				}
				strcat(result[i], "\n");
			} else {
				strcpy(result[i], text[i]);
			}
		}

		else if ((i == *text_line_count - 1 || strcmp(text[i + 1], "\n") == 0) && start_line <= i && i <= stop_line) {
			text = align_left(text, text_line_count, i, i);
			strcpy(result[i], text[i]);
		} else {
			strcpy(result[i], text[i]);
		}
	}

	free(text);
	return result;
}

// Functia align PARAGRAPH
char** paragraph(char **text, int *text_line_count, int start_line, int stop_line, int p) {
	char **result;
	alloc_mem(&result);
	int i;
	int j;


	for (i = 0; i < *text_line_count; i++) {
		if (strcmp(text[i], "\n") != 0 && start_line <= i && i <= stop_line) {
			if (i == 0 || strcmp(text[i - 1], "\n") == 0) {
				for (j = 0; j < p; j++) {
					strcat(result[i], " ");
				}
				strcat(result[i], text[i]);
			} else {
				strcat(result[i], text[i]);

			}
		} else {
			strcat(result[i], text[i]);
		}
	}

	return result;
}

int main(int argc, char *argv[]) {
	char **text;             // Textul citit din fisier
	int text_line_count = 0; // Numarul de linii al textului

	// Alocare memorie
	alloc_mem(&text);

	// Citire text
	read_file(argv[2], &text, &text_line_count);

	// Eliminare trailing spaces
	text = remove_trailing_spaces(text, text_line_count);

	char *rest = argv[1];
	char *token = strtok_r(rest, " ,\n", &rest);

	while (token != NULL) {
		int start_line = 0;
		int stop_line = text_line_count;
		int P_ARG = 0;

		if ('a' <= token[0] && token[0] <= 'z') {
			token[0] -= 'a' - 'A';
		}

		switch (token[0]) {
			case 'W':
				// Extrag max_line_length
				token = strtok_r(rest, " ,\n", &rest);
				int max_line_length = atoi(token);

				// Prelucrez textul
				text = wrap_text(text, &text_line_count, max_line_length);

				// Extrag urmatoarea comanda
				token = strtok_r(rest, " ,\n", &rest);
				break;
			
			case 'C':
				// Extrag start sau urmatoarea comanda
				token = strtok_r(rest, " ,\n", &rest);
				if (token != NULL && '0' <= token[0] && token[0] <= '9') {
					start_line = atoi(token);
					token = strtok_r(rest, " ,\n", &rest);
					
					// Extrag stop sau urmatoarea comanda
					if (token != NULL && '0' <= token[0] && token[0] <= '9') {
						stop_line = atoi(token);
						// Extrag urmatoarea comanda
						token = strtok_r(rest, " ,\n", &rest);
					}
				}

				// Prelucrez textul
				text = center_text(text, &text_line_count, start_line, stop_line);
				break;
			case 'L':
				// Extrag start sau urmatoarea comanda
				token = strtok_r(rest, " ,\n", &rest);
			
				if (token != NULL && '0' <= token[0] && token[0] <= '9') {
					start_line = atoi(token);
					token = strtok_r(rest, " ,\n", &rest);
					
					// Extrag stop sau urmatoarea comanda
					if (token != NULL && '0' <= token[0] && token[0] <= '9') {
						stop_line = atoi(token);
						// Extrag urmatoarea comanda
						token = strtok_r(rest, " ,\n", &rest);
					}
				}
				// Prelucrez textul
				text = align_left(text, &text_line_count, start_line, stop_line);
				break;
			case 'R':
				// Extrag start sau urmatoarea comanda
				token = strtok_r(rest, " ,\n", &rest);
			
				if (token != NULL && '0' <= token[0] && token[0] <= '9') {
					start_line = atoi(token);
					token = strtok_r(rest, " ,\n", &rest);
					
					// Extrag stop sau urmatoarea comanda
					if (token != NULL && '0' <= token[0] && token[0] <= '9') {
						stop_line = atoi(token);
						// Extrag urmatoarea comanda
						token = strtok_r(rest, " ,\n", &rest);
					}
				}
				// Prelucrez textul
				text = align_right(text, &text_line_count, start_line, stop_line);
				break;
			case 'J':
				// Extrag start sau urmatoarea comanda
				token = strtok_r(rest, " ,\n", &rest);
			
				if (token != NULL && '0' <= token[0] && token[0] <= '9') {
					start_line = atoi(token);
					token = strtok_r(rest, " ,\n", &rest);
					
					// Extrag stop sau urmatoarea comanda
					if (token != NULL && '0' <= token[0] && token[0] <= '9') {
						stop_line = atoi(token);
						// Extrag urmatoarea comanda
						token = strtok_r(rest, " ,\n", &rest);
					}
				}
				// Prelucrez textul
				text = align_justify(text, &text_line_count, start_line, stop_line);
				break;
			case 'P':
				token = strtok_r(rest, " ,\n", &rest);
				P_ARG = atoi(token);

				// Extrag start sau urmatoarea comanda
				token = strtok_r(rest, " ,\n", &rest);
			
				if (token != NULL && '0' <= token[0] && token[0] <= '9') {
					start_line = atoi(token);
					token = strtok_r(rest, " ,\n", &rest);
					
					// Extrag stop sau urmatoarea comanda
					if (token != NULL && '0' <= token[0] && token[0] <= '9') {
						stop_line = atoi(token);
						// Extrag urmatoarea comanda
						token = strtok_r(rest, " ,\n", &rest);
					}
				}
				// Prelucrez textul
				text = paragraph(text, &text_line_count, start_line, stop_line, P_ARG);
				break;
			default:
				token = strtok_r(rest, " ,\n", &rest);
		}
	}

	// Sciere text
	write_file(argv[3], text, text_line_count);

	// Eliberare memorie
	free_mem(&text);

	return 0;
}
