
## TEMA 1 - TetriBit

I-am indeplinit dorinta lui Gigel de a implementa jocul Tetris folosind doar cunostintele dobandite in primele laboratoare de programare, jocul bazandu-se in principal pe operatii pe biti.

#### _Harta_
La fiecare moment am salvate doua harti: una care reprezinta ultima actualizare (cu ultima piesa), iar a doua care contine doar piesa curenta. La sfarsitul mutarilor piesei curente (sau mai devreme daca piesa nu incape), reuniunea celor doua harti ("sau" logic intre cele doua valori pe 64 de biti) vor reprezenta harta actualizata pentru urmatoarea piesa.



### _Miscarea pieselor_
Functia move se foloseste de mapa, de verificarea coliziunilor si de verificarea liniilor complete.

La fiecare mutare se verifica daca mutarea creaza hazard. Dupa verificare se afiseaza harta combinata dintre map si temp, temp reprezentand o harta unica pentru piesa, verificarea realizandu-se prin suprapunerea celor doua.

In cazul in care la un moment dat `functia hazard_exists` a returnat valoarea `"true"`, execut o citire inutila a miscarilor ce urmau, acestea fiind ignorate.

### _Verificarea mutarii stanga/dreapta_
Deoarece piesa executa miscari stanga/dreapta, reprezentate prin semnul numarului din input (numar pozitiv pentru shiftari la dreapta, numar negativ pentru shiftari la stanga), este necesara o verificare ca piesa sa nu paraseasca limitele hartii.

### _Coliziune_
In cazul in care la suprapunerea celor doua harti (map, temp) se intampla ca doi biti de 1 sa fie suprapusi (map & temp != 0), functia hazard intoarce valoarea logica `"true"`.

Daca hazard_exists a returnat valoarea `"false"`, miscarea pieselor continua. In caz contrar, piesele se opresc si nu executa restul transformarilor.

### _Main_
In functia main declar datele de intrare, citesc valoarea hartii initiale si numarul de mutari, urmand sa aplic fiecare mutare si sa verific existenta unor linii complete sau a unei coliziuni dupa fiecare mutare. La terminarea jocului afisez harta finala si scorul.

### _Terminarea jocului_
La fiecare mutare a unei piese, daca aceasta nu incape in harta curenta, se va calcula scorul, iar jocul se va termina.

Daca piesa incape, se muta piesa si se va trece la urmatoarea piesa, pana cand se vor termina piesele sau una nu va incapea.
