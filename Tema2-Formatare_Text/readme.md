# TEMA 2 - Formatare text

### Wrap
Functia wrap_text se foloseste de text, numarul de linii al textului, si marimea maxima a liniei.
Extrag fiecare cuvant din textul original, se verifica daca incape pe linia finala, daca nu incape adaugandu-se o noua linie in final.
Functia returneaza sirul prelucrat.


### Center text

Functia `center_text` se foloseste de text, numarul de linii al textului, inceputul si sfarsitul liniei.
Pentru fiecare linie a carei lungime este diferita de `max_len`, sirul se concateneaza cu spatii , pana in momentul in care sirul are un numar egal de spatii stanga-dreapta.
Functia returneaza sirul prelucrat.

### Align Left
Functia `align_left` se foloseste de text, numarul de linii al textului, inceputul si sfarsitul liniei.
Numar cate trailing spaces am pe fiecare linie, dupa care elimin fix 
atatea caractere de la inceput.
Functia returneaza sirul prelucrat.

### Align right
Functia `align_right` se foloseste de text, numarul de linii al textului, inceputul si sfarsitul liniei.
Pentru fiecare linie a carei lungime este diferita de `max_len`, sirul se concateneaza la inceput cu spatii (" "), pana in momentul in care lungimea sirului este chiar lungimea maxima.
Functia returneaza sirul prelucrat.


### Justify
Functia `align_justify` se foloseste de text, numarul de linii al textului, inceputul si sfarsitul liniei.
	Folosindu-ma de `max_len`, adaug spatii liniilor ce nu sunt egale cu cea mai mare linie, egalandu-le.
Functia returneaza sirul prelucrat.


### Paragrafe

Functia `paragraph` se foloseste de text, numarul de linii al textului, numarul de spatii, inceputul si sfarsitul liniei.
Pentru fiecare paragraf indentificat se adauga "p" spatii (indent_length) la inceputul liniei.
Functia returneaza sirul prelucrat.


### Main
In functia main citesc textul din fisier, initializez numarul de linii cu 0 (text_line_count = 0) si aloc memorie pentru text. Elimin trailing spaces si transform `"array to int"` (atoi).
	Pentru fiecare caz (W, C, L, R, J, P), folosesc functia corespunzatoare si prelucrez textul.
In final, dezaloc memoria si afisez textul prelucrat.
