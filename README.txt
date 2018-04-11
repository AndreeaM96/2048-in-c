Implementare 2048 by Mirescu Andreea Alexandra|
==============================================+

Instalare ncurses:
sudo apt-get install ncurses-dev


Marime recomandata a terminalului:
tput cols -> 78
tput lines -> 24

Pentru a putea vedea si salva High Scores este necesara prezenta unui fisier "scores.txt" in acelasi director cu executabilul.

=========+
Controale|
=========+

Meniu:
Sus/jos -> miscare intre optiunile meniului
Enter -> selectare optiune

Joc:
Sus/jos/stanga/dreapta -> miscare a numerelor in directia dorita
Q -> revenire la meniu
A -> miscare in directia cea mai favorabila
U -> undo

=========+
Interfata|
=========+

Meniu:
In meniu sunt prezentate optiunile, care se pot selecta folosind enter
New Game -> porneste un joc nou
Resume -> reia jocul de unde ai ramas (optiune prezenta doar daca s-a creat un nou joc cu "New Game" si apoi s-a iesit apasand tasta Q)
High Scores -> afiseaza continutul fisierului scores.txt, daca exista (fisier in care se stocheaza scorurile atunci cand jocul este castigat)
Quit -> inchide jocul

In joc:
In mijlocul ecranului se afla tabla de joc de dimensiune 4x4, initial goala cu exceptia a 2 celule cu valori aleatoare (dintre 2 sau 4), in pozitii aleatoare.
In stanga ecranului este prezentat scopul jocului.
In dreapta sunt afisate controalele.
In josul ecranului sunt afisate scorul curent si data si ora curenta, care se actualizeaza la fiecare mutare.

===================+
Informatii generale|
===================+

Scopul jocului este unirea celulelor cu aceasi valoare pana cand se ajunge la o celula cu valoarea 2048.
Cand 2 celule se undesc, se transforma intr-o singura celula, cu valoarea insumata a celor doua. De exemplu, daca unim 2 celule cu valoarea 8, va rezulta o celula cu valoarea 16.
Dupa o mutare, apare o noua piesa pe tabla, cu valoarea 2 sau 4. De exemplu, daca avem 2 piese cu valoare 2 pe tabla si le unim, la urmatorul pas vom avea piesa noastra cu valoarea 4 impreuna cu o alta piesa cu valoare 2 sau 4, aparuta intr-o pozitie aleatoare.
Jocul este pierdut in momentul in care tabla se umple si nu mai pot fi unite piese, deci nu mai exista spatiu unde sa apara o noua piesa la urmatorul pas.
Daca se obtine o piesa cu valoarea 2048, jocul este castigat.

Apasarea tastei A cauzeaza o miscare automata, cea mai benefica la pasul respectiv (care aduce scorul maxim), insa nu garanteaza ca miscarea a fost cea mai favorabila strategic, este urmarita doar maximizarea scorului.
Dupa aproximativ 10 secunde de inactivitate se executa automat miscarea cea mai favorabila (ca si cum s-ar fi apasat tasta 'A').

Undo-ul are o adancime de maxim 1, nu se pot da doua undo-uri la rand.

Din joc se poate reveni oricand la meniu apasand tasta Q. Se poate relua jocul selectand optiunea "Resume" din meniu, sau sa se creeze un nou joc selectand optiunea "New Game".