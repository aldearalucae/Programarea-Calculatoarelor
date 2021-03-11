
## Tema 3 - TetriPic

###### Tema reprezinta creearea unor imagini de tip BMP cu piese din jocul Tetris, implementarea mecanicii de joc si afisarea starii jocului in ultima etapa (plecand de la o tabla goala sau intr-o anumita forma), dar si recunoasterea unor piese intr-o tabla primita ca intrare.

### Implementarea

Tema este structurata in 3 fisiere: main.c, main.h si bmp_header.h.
Main.c este sursa principala, care contine functia principala main, dar si functiile folosite in rezolvarea temei.

Main.h este header-ul principal, care contine define-uri pentru culori, structurile utilizate, dar si semnaturile tuturor functiilor utilizare in main.c.

Tema respecta toate conditiile impuse in enuntul acesteia. Am utilizat structuri reprezentative, alocare dinamica fara pierderi de memorie, codul este modularizat, aerisit, structurat, identat si usor de parcurs.

Executabilul generat in urma compilarii a fost testat cu valgrind: "All heap blocks were freed -- no leaks are possible".

### Structuri

Pe langa structurile utilizate in bmp_header.h, le-am utilizat si pe cele prezente in main.h.
Structura pixel (Pixel) contine 3 valori de tip unsigned char, corespunzatoare celor 3 culori: red, green si blue.
Structura image (Image) contine 2 valori de tip int care reprezinta dimensiunile imaginii si o valoare de tip Pixel**, care reprezinta matricea de pixeli de dimensiune inaltime * latime.

### Functii

	int main() - functia principala
	
	void task_1() - rezolva task-ul 1 (vezi Task 1)
	
	void task_2() - rezolva task-ul 2 (vezi Task 2)
	
	void task_3() - rezolva task-ul 3 (vezi Task 3)
	
	void task_4() - rezolva task-ul 4 (vezi Task 4)
	
	void task_bonus() - rezolva task-ul bonus (vezi Task bonus)
	
	int get_offset(int) - primeste latimea unei poze si calculeaza valoarea padding-ului, pe care o si intoarce

	Pixel rgb(unsigned char, unsigned char, unsigned char) - primeste cele trei valori pentru culori (r, g, b) si intoarce o noua valoare de tip Pixel

	void export_bmp(char*, Image) - primeste numele unui fisier si o imagine; completeaza cele doua header-e cu valorile specifice; scrie in fisier cele doua header-e, dupa care scrie si poza, tinand cont de SCALE = 10.

	void import_bmp(char*, Image) - primeste numele unui fisier si o imagine; citeste din fisier cele doua header-e, aloca memorie pentru imaginea specifica, dupa care citeste poza in intregime.

	void chek_args(int) - primeste numarul de argumente (din main); daca acesta nu este 2 (numele executabilului si cifra corespunzatoare task-ului), se iese fortat din program cu valoarea 1.

	void test_file(char*) - primeste numele unui fisier si verifica daca acesta exista; daca nu exista, se iese fortat din program cu valoarea 3.

	Image new_image(int, int) - primeste doua dimensiuni pentru inaltime si latime; aloca memorie pentru o imagine cu aceste dimensiuni si o intoarce; imaginea este umpluta cu culoarea ALB;

	Image new_board(Image*) - primeste dimensiunile unei imagini; creeaza o imagine cu new_image; umple cele height - 4 linii cu NEGRU si intoarce noua imagine creeata (numita board)

	void free_image(Image*) - primeste o imagine si elibereaza memoria alocata acesteia

	void rotate_image(Image*, int) - primeste o imagine cu un numar de grade intreg pentru rotatie; aloca memorie pentru o noua imagine, in se copiaza datele din imaginea originala (in functie de rotatie); se realoca imaginea initiala si se copiaza in aceasta noua imagine; se elibereaza memoria alocata pentru rotated_image

	Image new_piece(char, int) - primeste tipul piesei si numarul de grade cu care este rotita aceasta; in functie de tipul piesei, se creeaza noua imagine; imaginea este rotita cu numarul de grade primit ca parametru

	bool cmp_rgb(Pixel, Pixel) - functia primeste doua valori Pixel si intoarce true, daca acestea sunt egale si false, altfel
	
	void add_piece_to_board(Image, Image*, int, int) - primeste o imagine (piesa), o tabla de joc si coltul stanga sus; se adauga piesa pe tabla la locatia corespunzatoare

	void move(Image*, Image, int* line, int column) - primeste o tabla, o piesa si locatia acesteia; cat timp piesa se poate muta cu o linie mai jos, aceasta este mutata

	void test_running(Image*, bool*) - primeste o tabla si variabila running; se verifica daca jocul mai poate continua - adica daca ultima piesa a avut sau nu loc in tabla; se modifica astfel variabila running

	void update_board(Image*, bool*, Image, int) - primeste tabla, variabila running, o piesa si o coloana; incepand de la linia 0, se muta piesa cat de jos se poate, se adauga piesa pe tabla si se testeaza running

	void clear_line(Image*, int) - primeste o tabla si o linie; se sterge linia indicata din tabla, se coboara toate liniile superiare cu cate o pozitie si se coloreaza cu NEGRU prima linie utila din tabla

	void clear_full_lines(Image*) - primeste tabla; functia sterge toate liniile completate

	bool match(Image, Image, int, int) - primeste o tabla, o piesa si pozitia acesteia pe tabla; se verifica daca la pozitia indicata exista piesa pe tabla

	void delete_piece(Image, Image, int, int) - primeste o tabla, o piesa si pozitia acesteia pe tabla; se sterge piesa de pe tabla

	void check_for_pieces(Image*) - primeste o tabla; functia verifica pentru fiecare pozitie din tabla de joc ce piesa se afla acolo; cand a gasit o piesa, adauga in cei trei vectori detaliile acesteia: nume piesei, unghiul si coloana pe care a fost adaugata; la sfarsit se scriu in fisier datele obtinute


### Task 1
Pentru orice litera din lista "OISZLJT":
>- se creeaza o piesa te tipul literei, cu rotatia 0
>- se scrie poza in fisierul corespunzator
>- se elibereaza memoria

### Task 2
Pentru orice litera din lista "OISZLJT":
	Pentru fiecare unghi din {90, 180, 270}
>	- se creeaza o piesa te tipul literei, cu rotatia corespunzatoare		
>	- se scrie poza in fisierul corespunzator
>	- se elibereaza memoria

### Task 3
~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~
Se citesc din valorile N, H si L.
Se creeaza tabla.
Cat timp running este setat pe true si nu s-au terminat mutarile:
>	- se citesc numele, rotatia si coloana unei piese
>	- se creeaza piesa
>	- se face update la tabla

Se scrie tabla in fisierul de output.
Se elibereaza memoria alocata.

### Task 4
Task-ul 4 se rezolva asemanator task-ului 3.
La inceput, tabla se citeste dintr-un fisier, apoi se aplica acelasi algoritm ca la task-ul 3.

###  Bonus
Se citeste tabla din fisierul de intrare.
Se aplica functia check_for_pieces pe tabla, care gaseste toate piesele.
Aceasta scrie in fisierul de iesire piesele gasite.
Se elibereaza memoria alocata

### Exit Codes

> Exit Code 0 - rulare corecta
	Exit Code 1 - numarul de argumente este gresit
	Exit Code 2 - default exit pe switch case-uri
	Exit Code 3 - lipsa fisier de intrare

