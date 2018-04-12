#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Structura in care am stocat scorurile extrase din fisierul scores.txt
typedef struct {
	char name[60];
	int score;
	char date1[50];
	char date2[50];
} Scores;

//Initializare ncurses.
void init() {
	initscr();
	cbreak();
	start_color();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	init_pair(1, COLOR_WHITE, COLOR_CYAN);		//Culoare background
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);	//Culoare pt celulele cu 0
	init_pair(3, COLOR_WHITE, COLOR_RED);		//Culoare pt celulele cu 2
	init_pair(4, COLOR_WHITE, COLOR_GREEN);		//Culoare pt celulele cu 4
	init_pair(5, COLOR_WHITE, COLOR_BLUE);		//Culoare pt celulele cu 8
	init_pair(6, COLOR_WHITE, COLOR_BLACK);		//Culoare pt celulele cu 16
	init_pair(7, COLOR_WHITE, COLOR_MAGENTA);	//Culoare pt celulele cu 32
	init_pair(8, COLOR_BLACK, COLOR_WHITE);		//Culoare pt celulele cu 64
	init_pair(9, COLOR_BLUE, COLOR_GREEN);		//Culoare pt celulele cu 128
	init_pair(10, COLOR_YELLOW, COLOR_RED);		//Culoare pt celulele cu 256
	init_pair(11, COLOR_BLACK, COLOR_MAGENTA);	//Culoare pt celulele cu 512
	init_pair(12, COLOR_YELLOW, COLOR_BLACK);	//Culoare pt celulele cu 1024
	init_pair(13, COLOR_BLUE, COLOR_YELLOW);	//Culoare pt celulele cu 2048
	bkgd(COLOR_PAIR(1));
}

//Functia pentru afisarea primului ASCII, cel din meniu.
void ascii1() {
	mvprintw(1, 18, " .-----.   .----.      .---.    .-----.   ");
	mvprintw(2, 18, "/ ,-.   \\ /  ..  \\    / .  |   /  .-.  \\  ");
	mvprintw(3, 18, "'-'  |  |.  /  \\  .  / /|  |  |   \\_.' /  ");
	mvprintw(4, 18, "   .'  / |  |  '  | / / |  |_  /  .-. '.  ");
	mvprintw(5, 18, " .'  /__ '  \\  /  '/  '-'    ||  |   |  | ");
	mvprintw(6, 18, "|       | \\  `'  / `----|  |-' \\  '-'  /  ");
	mvprintw(7, 18, "`-------'  `---''       `--'    `----''   ");
	mvprintw(19, 22, "=====================================");
	mvprintw(20, 37, "Made by");
	mvprintw(21, 28, "Mirescu Andreea-Alexandra");
	mvprintw(22, 38, "313CC");
}

//Functia pentru afisarea celui de al doilea ASCII, cel din joc.
void ascii2() {
	mvprintw(1, 30, " ___ ___ ___ ___ ");
	mvprintw(2, 30, "|_  |   | | | . |");
	mvprintw(3, 30, "|  _| | |_  | . |");
	mvprintw(4, 30, "|___|___| |_|___|");
}

//Functia pentru afisarea meniului si crearea vectorului de optiuni
void menuCreate(int *exist, char menu_items[][15]) {
	clear();
	ascii1();
	attron(A_UNDERLINE);
	mvprintw(10, 36, "MENU");
	attroff(A_UNDERLINE);
	if((*exist) == 0) {			//Daca nu exista un joc deja pornit, atunci nu mai afisez optiunea "Resume"
		attron(A_STANDOUT);
		mvprintw(12, 35, "New Game");
		attroff(A_STANDOUT);
		mvprintw(13, 35, "High Scores");
		mvprintw(14, 35, "Quit");
		strcpy(menu_items[0], "New Game");		//Adaug optiunile in vector
		strcpy(menu_items[1], "High Scores");
		strcpy(menu_items[2], "Quit");
	}
	else {						//Daca exista un joc pornit, trebuie sa afisez si optiunea "Resume"
		attron(A_STANDOUT);
		mvprintw(12,35, "New Game");
		attroff(A_STANDOUT);
		mvprintw(13,35, "Resume");
		mvprintw(14,35, "High Scores");
		mvprintw(15,35, "Quit");
		strcpy(menu_items[0], "New Game");		//Adaug optiunile in vector
		strcpy(menu_items[1], "Resume");
		strcpy(menu_items[2], "High Scores");
		strcpy(menu_items[3], "Quit");
	}
	refresh();
}

//Functia care creeaza bucla din meniu pentru a permite miscarea intre optiuni si evidentierea acestora.
//Salveaza in "opt" optiunea selectata curent
void menuLoop(char menu_items[][15], int *exist, int *opt) {
	int i = 0, a;
	while((a = getch()) != '\n') {
		ascii1();
		attron(A_UNDERLINE);
		mvprintw(10, 36, "MENU");
		attroff(A_UNDERLINE);
		mvprintw(12 + i, 35, "%s", menu_items[i]);
		switch(a) {
			case KEY_UP:		//Daca apas tasta "sus" atunci ma mut cu o pozitie mai in sus, sau pe ultima pozitie daca ma aflu pe prima in momentul apasarii
				i--;
				if(i < 0) {
					if((*exist) == 0)
						i = 2;
					else
						i = 3;
				}
				break;
			case KEY_DOWN:		//Daca apas tasta "jos" atunci ma mut cu o pozitie mai in jos, sau pe prima pozitie daca ma aflu pe ultima in momentul apasarii
				i++;
				if((*exist == 0)) {
					if(i > 2)
						i = 0;
				}
				else {
					if(i > 3)
						i = 0;
				}
				break;
		}
		attron(A_STANDOUT);
		mvprintw(12 + i, 35, "%s", menu_items[i]);		//Printez si evidentiez optiunea selectata curent
		attroff(A_STANDOUT);
		refresh();
	}
	(*opt) = i + 1;
}

//Functie pentru generarea aleatoare a unui numar din multimea {2, 4}.
int randNum() {
	int num, ok = 0;
	while(ok == 0) {
		num = rand() % 5;
		if(num % 2 == 0 && num != 0)
			ok = 1;
	}
	return num;
}

//Generarea unei pozitii in care sa apara piesa din intervalul {2, 4}, prin generarea aleatoare a doua coordonate.
void randPos(int *pos1, int *pos2) {
	(*pos1) = rand() % 4;
	(*pos2) = rand() % 4;
}

//Functia care adauga o valoare aleatoare din interval, la o pozitie aleatoare, pe tabla curenta.
void addRandomValue(int b[][4]) {
	int pos1 = 0, pos2 = 0;
	do {
		randPos(&pos1, &pos2);
	} while(b[pos1][pos2] != 0);
	b[pos1][pos2] = randNum();
}

//Initializarea tablei cu valori de 0 si generarea celor 2 celule care contin valori din {2, 4}.
void initBoard(int b[][4]) {
	int i, j;
	for(i = 0 ; i < 4 ; i++)
		for(j = 0 ; j < 4 ; j++)
			b[i][j] = 0;
	addRandomValue(b);
	addRandomValue(b);
}

//Functie pentru afisarea tablei si a altor elemente care apar pe ecranul de joc (al doilea ASCII, panoul de control etc.)
void printBoard(int b[][4]) {	
	int i, j, p_num;
	clear();
	ascii2();
	for(i = 0 ; i < 5 ; i++)
		for(j = 0 ; j < 24 ; j++)
			mvprintw(6 + 2 * i, 27 + j, "-");
	for(j = 0 ; j < 5 ; j++)
		for(i = 0 ; i < 7 ; i++)
			mvprintw(7 + i, 26 + 6 * j, "|");
	for(i = 0 ; i < 5 ; i++)
		for(j = 0 ; j < 5 ; j++)
			mvprintw(6 + 2 * i, 26 + 6 * j, "+");
	for(i = 0 ; i < 4 ; i++){
		for(j = 0 ; j < 4 ; j++) {
			switch(b[i][j]) {	//Colorarea celulelor in functie de valoarea fiecareia
				case 0:
					p_num = 2;
					break;
				case 2:
					p_num = 3;
					break;
				case 4:
					p_num = 4;
					break;
				case 8:
					p_num = 5;
					break;
				case 16:
					p_num = 6;
					break;
				case 32:
					p_num = 7;
					break;
				case 64:
					p_num = 8;
					break;
				case 128:
					p_num = 9;
					break;
				case 256:
					p_num = 10;
					break;
				case 512:
					p_num = 11;
					break;
				case 1024:
					p_num = 12;
					break;
				case 2048:
					p_num = 13;
					break;
			}	
			attron(COLOR_PAIR(p_num));			//Selectarea perechii de culori corespunzatoare
			mvprintw(7 + 2 * i, 27 + 6 * j, "%d", b[i][j]);		//Printare
			attroff(COLOR_PAIR(p_num));
		}
	}
	refresh();
}

//Verificare daca linia este goala.
int notEmptyLine(int b[][4], int i, int k) {
	int notEmpty = 0, j;
	for(j = k ; j < 4 ; j++) {
		if(b[i][j] != 0) {
			notEmpty = 1;
			break;
		}
	}
	return notEmpty;
}

//Aceasi functie, in sens invers.
int reverseNotEmptyLine(int b[][4], int i, int k) {
	int notEmpty = 0, j;
	for(j = k ; j >= 0 ; j--) {
		if(b[i][j] != 0) {
			notEmpty = 1;
			break;
		}
	}
	return notEmpty;
}

//Verificare daca coloana e goala.
int notEmptyColumn(int b[][4], int k, int j) {
	int notEmpty = 0, i;
	for(i = k + 1 ; i < 4 ; i++) {
		if(b[i][j] != 0) {
			notEmpty = 1;
			break;
		}
	}
	return notEmpty;
}

//Aceasi functie, in sens invers.
int reverseNotEmptyColumn(int b[][4], int k, int j) {
	int notEmpty = 0, i;
	for(i = 0 ; i < k ; i++) {
		if(b[i][j] != 0) {
			notEmpty = 1;
			break;
		}
	}
	return notEmpty;
}

//Permutarea celulelor in stanga.
void permLineToLeft(int b[][4], int line, int col) {
    int j;
	for(j = col ; j < 3 ; j++) {
		b[line][j] = b[line][j + 1];
    }
    b[line][3] = 0;
}

//Permutarea celulelor in dreapta.
void permLineToRight(int b[][4], int line, int col) {
	int j;
	for(j = col ; j > 0 ; j--) {
		b[line][j] = b[line][j - 1];
	}
	b[line][0] = 0;
}

//Permutarea celulelor in sus.
void permLineOnTop(int b[][4], int line, int col) {
	int i;
	for(i = line ; i < 3 ; i++) {
		b[i][col] = b[i + 1][col];
	}
	b[3][col] = 0;
}

//Permutarea celulelor in jos.
void permLineOnBottom(int b[][4], int line, int col) {
	int i;
	for(i = line ; i > 0 ; i--) {
		b[i][col] = b[i - 1][col];
	}
	b[0][col] = 0;
}

//Mutarea efectiva la stanga.
int moveToLeft(int b[][4]) {
	int perms = 0;
	int i = 0;
	while(i < 4) {
		int j = 0;
		while(j < 4) {
			while(b[i][j] == 0 && notEmptyLine(b, i, j)) {
				perms++;
				permLineToLeft(b, i, j);
			}
			j++;
		}
		i++;
	}
	return perms;
}

//Mutarea efectiva la dreapta.
int moveToRight(int b[][4]) {
	int perms = 0;
	int i = 0;
	while(i < 4) {
		int j = 3;
		while(j > 0) {
			while(b[i][j] == 0 && reverseNotEmptyLine(b, i, j)) {
				perms++;
				permLineToRight(b, i, j);
			}
			j--;
		}
		i++;
	}
	return perms;
}

//Mutarea efectiva in sus.
int moveOnTop(int b[][4]) {
	int perms = 0;
	int j = 0;
	while(j < 4) {
		int i = 0;
		while(i < 4) {
			while(b[i][j] == 0 && notEmptyColumn(b, i, j)) {
				perms++;
				permLineOnTop(b, i, j);
			}
			i++;
		}
		j++;
	}
	return perms;
}

//Mutarea efectiva in jos.
int moveOnBottom(int b[][4]) {
	int perms = 0;
	int j = 3;
	while(j >= 0) {
		int i = 3;
		while(i >= 0) {
			while(b[i][j] == 0 && reverseNotEmptyColumn(b, i, j)) {
				perms++;
				permLineOnBottom(b, i, j);
			}
			i--;
		}
		j--;
	}
	return perms;
}

//Lipirea celulelor la stanga si actualizarea scorului.
int sumOnLeft(int b[][4], int *score) {
	int perms = 0;
	int i, j;
	for(i = 0 ; i < 4 ; i++) {
		for(j = 0 ; j < 3 ; j++) {
			if(b[i][j] != 0 && b[i][j + 1] != 0 && b[i][j] == b[i][j + 1]) {
				b[i][j] = 2 * b[i][j];
				(*score) = (*score) + b[i][j];
				b[i][j + 1] = 0;
				perms++;
			}
		}
	}
	return perms;
}

//Lipirea celulelor la dreapta si actualizarea scorului.
int sumOnRight(int b[][4], int *score) {
	int perms = 0;
	int i, j;
	for(i = 0 ; i < 4 ; i++) {
		for(j = 3 ; j > 0 ; j--) {
			if(b[i][j] != 0 && b[i][j - 1] != 0 && b[i][j] == b[i][j - 1]) {
				b[i][j] = 2 * b[i][j];
				(*score) = (*score) + b[i][j];
				b[i][j - 1] = 0;
				perms++;
			}
		}
	}
	return perms;
}

//Lipirea celulelor in sus si actualizarea scorului.
int sumOnTop(int b[][4], int *score) {
	int perms = 0;
	int i, j;
	for(j = 0 ; j < 4 ; j++) {
		for(i = 0 ; i < 3 ; i++) {
			if(b[i][j] != 0 && b[i + 1][j] != 0 && b[i][j] == b[i + 1][j]) {
				b[i][j] = 2 * b[i][j];
				(*score) = (*score) + b[i][j];
				b[i + 1][j] = 0;
				perms++;
			}
		}
	}
	return perms;
}

//Lipirea celulelor in jos si actualizarea scorului.
int sumOnBottom(int b[][4], int *score) {
	int perms = 0;
	int i, j;
	for(j = 0 ; j < 4 ; j++) {
		for(i = 3 ; i > 0 ; i--) {
			if(b[i][j] != 0 && b[i - 1][j] != 0 && b[i][j] == b[i - 1][j]) {
				b[i][j] = 2 * b[i][j];
				(*score) = (*score) + b[i][j];
				b[i - 1][j] = 0;
				perms++;
			}
		}
	}
	return perms;
}

//Salveaza tabla curenta intr-un auxiliar.
void saveBoard(int b[][4], int aux[][4]) {
	int i, j;
	for(i = 0 ; i < 4 ; i++)
		for(j = 0 ; j < 4 ; j++)
			aux[i][j] = b[i][j];
}

//Functia care face undo.
void doUndo(int b[][4], int aux[][4]) {
	int i, j;
	for(i = 0 ; i < 4 ; i++)
		for(j = 0 ; j < 4 ; j++)
			b[i][j] = aux[i][j];
}

//Verifica daca a fost deja facut undo.
//Va verifica daca toti termenii sunt identici, cu exceptia a unuia singur
//pentru ca salvarea tablei se face inainte de generarea celulei din intervalul {2, 4}
int checkIfUndone(int b[][4], int aux[][4]) {
	int i, j, k = 0;
	for(i = 0 ; i < 4 ; i++)
		for(j = 0 ; j < 4 ;j++)
			if(b[i][j] != aux[i][j])
				k++;
	if(k == 1)
		return 1;
	return 0;
}

//Determinarea miscarii ideale, in cazul automutarii, folosind functiile de mutare
//pe niste auxiliare si preluand valorile returnate.
//Functia returneaza urmatoarelevalori: 1=sus, 2=jos, 3=stanga, 4=dreapta
//move=cate miscari produce o anumita directie
//nrfcells=numarul de celule eliberate de fiecare directie in parte.
int whichIsBestMove(int b[][4]) {
	int aux1[4][4], aux2[4][4];
	int	aux3[4][4], aux4[4][4];
	int	i, j, which = 0;
	int score2 = 0, max;
	int move[4] = {0, 0, 0, 0};
	int nrfcells[4] = {0, 0, 0, 0};
	for(i = 0 ; i < 4 ; i++)
		for(j = 0 ; j < 4 ; j++) {
			aux1[i][j] = b[i][j];
			aux2[i][j] = b[i][j];
			aux3[i][j] = b[i][j];
			aux4[i][j] = b[i][j];
		}
	move[0] = moveOnTop(aux1);
    nrfcells[0] = sumOnTop(aux1, &score2);
    move[0] = move[0] + moveOnTop(aux1);

	move[1] = moveOnBottom(aux2);
    nrfcells[1] = sumOnBottom(aux2, &score2);
    move[1] = move[1] + moveOnBottom(aux2);

	move[2] = moveToLeft(aux3);
    nrfcells[2] = sumOnLeft(aux3, &score2);
    move[2] = move[2] + moveToLeft(aux3);

	move[3] = moveToRight(aux4);
    nrfcells[3] = sumOnRight(aux4, &score2);
    move[3]= move[3] + moveToRight(aux4);

	max = nrfcells[0];
	which = 1;

	if((nrfcells[0] == 0) && (nrfcells[1] == 0) && (nrfcells[2] == 0) && (nrfcells[3] == 0)) {
		int i;
		for(i = 0 ; i < 4 ; i++) {
			if(move[i] != 0) {
				which = i + 1;
				mvprintw(4, 1, "%d", which);
				return which;
			}
		}
	}
	for(i = 1 ; i < 4 ; i++) {
		if(nrfcells[i] > max && move[i] != 0) {
			max = nrfcells[i];
			which = i + 1;
		}
		
	}
	return which;
}

//Verificare daca jocul s-a terminat. Acelasi principiu ca la functia precendenta.
//move=cate miscari produce o anumita directie.
int checkGameOver(int b[][4]) {
	int aux1[4][4], aux2[4][4];
	int aux3[4][4], aux4[4][4], i, j, score2 = 0;
	int move[4] = {0, 0, 0, 0};
	for(i = 0 ; i < 4 ; i++)
		for(j = 0 ; j < 4 ; j++) {
			aux1[i][j] = b[i][j];
			aux2[i][j] = b[i][j];
			aux3[i][j] = b[i][j];
			aux4[i][j] = b[i][j];
		}
	move[0] = moveOnTop(aux1);
    sumOnTop(aux1, &score2);
    move[0] = move[0] + moveOnTop(aux1);

	move[1] = moveOnBottom(aux2);
    sumOnBottom(aux2, &score2);
    move[1] = move[1] + moveOnBottom(aux2);

	move[2] = moveToLeft(aux3);
    sumOnLeft(aux3, &score2);
    move[2] = move[2] + moveToLeft(aux3);

	move[3] = moveToRight(aux4);
    sumOnRight(aux4, &score2);
    move[3]= move[3] + moveToRight(aux4);

	if((move[0] == 0) && (move[1] == 0) && (move[2] == 0) && (move[3] == 0))
		return 1;
	else
		return 0;
}

//Verificare daca jocul a fost castigat (cauta piesa de 2048).
int checkIfWin(int b[][4]) {
	int i, j;
	for(i = 0 ; i < 4 ; i++)
		for(j = 0 ; j < 4 ; j++)
			if(b[i][j] == 2048)
				return 1;
	return 0;
}

//Functia care preia un nume si salveaza scorul, numele dat si data terminarii jocului in fisierul scores.txt
int highScoreWrite(int *score) {
	time_t t;
	struct tm *c_tm;
	time(&t);
	c_tm = localtime(&t);
	int a, ok = 0;
	char menu[2][12];
	strcpy(menu[0], "Yes");
	strcpy(menu[1], "No, retype");
	FILE *f;
	f = fopen("scores.txt", "a");
	if(f == NULL) {
		clear();
		mvprintw(11, 21, "The scores.txt file could not be open!");
		mvprintw(12, 19, "Make sure it exists in your current folder.");
		refresh();
		timeout(15000);
		getch();
		return 0;
	}
	char name[45];

	//Bucla pentru interogarea utilizatorului.
	//In cazul confirmarii cere nu nume si apoi il afisaza pentru verificarea scrierii corecte.
	while(ok == 0) {		
		int i = 0;
		clear();
		mvprintw(11, 25, "Type your name and press ENTER:");
		refresh();
		curs_set(1);
		getnstr(name, 45);
		curs_set(0);
		clear();
		mvprintw(11, 27, "Is %s what you meant?", name);
		attron(A_STANDOUT);
		mvprintw(15, 27, "Yes");
		attroff(A_STANDOUT);
		mvprintw(15, 41, "No, retype");
		refresh();
		fflush(stdin);
		timeout(80000);
		while((a = getch()) != '\n') {
			mvprintw(15, 30 + 20 * i, "%s", menu[i]);
			switch(a) {
				case KEY_LEFT:
					i--;
					if(i < 0)
						i = 1;
					break;
				case KEY_RIGHT:
					i++;
					if(i > 1)
						i = 0;
					break;
			}
			attron(A_STANDOUT);
			mvprintw(15, 30 + 20 * i, "%s", menu[i]);
			attroff(A_STANDOUT);
		}
		if(i == 0) {
			ok = 1;

			//Scrierea in fisier a datelor.
			fprintf(f, "%s %d ", name, *score);
			fprintf(f, "on %d/%d/%d ", (*c_tm).tm_mday, ((*c_tm).tm_mon) + 1, ((*c_tm).tm_year) + 1900);
			fprintf(f, "%d:%d:%d\n", (*c_tm).tm_hour, (*c_tm).tm_min, (*c_tm).tm_sec);
		}
	}
	fclose(f);
	return 0;
}

//Functie pentru afisarea ecranului final. In cazul in care jocul a fost castigat,
//utilizatorul este intrebat daca vrea sa isi salveze scorul in "High Scores".
int endScreen(int *cond, int *score) {
	int a, i = 0;
	char menu[2][4];
	strcpy(menu[0], "Yes");
	strcpy(menu[1], "No");
	clear();
	if((*cond) == 0) {
		mvprintw(11, 35, "Game over!");
		mvprintw(12, 30, "Your score was: %d", (*score));
		timeout(20000);
		getch();
		return 0;
	}
	else
		mvprintw(11, 35, "You won!");
	
	mvprintw(12, 30, "Your score was: %d", (*score));
	mvprintw(13, 15, "Do you want to save your score to the high scores?");
	attron(A_STANDOUT);
	mvprintw(15, 30, "Yes");
	attroff(A_STANDOUT);
	mvprintw(15, 50, "No");
	refresh();
	fflush(stdin);
	timeout(80000);
	while((a = getch()) != '\n') {
		mvprintw(15, 30 + 20 * i, "%s", menu[i]);
		switch(a) {
			case KEY_LEFT:
				i--;
				if(i < 0)
					i = 1;
				break;
			case KEY_RIGHT:
				i++;
				if(i > 1)
					i = 0;
				break;
		}
		attron(A_STANDOUT);
		mvprintw(15,30 + 20 * i, "%s", menu[i]);
		attroff(A_STANDOUT);
		refresh();
	}
	switch(i) {
		case 0:
			highScoreWrite(score);
			break;
		case 1:
			break;
	}
	return 0;			
}

//Afisarea panoului de control.
void controls() {
	int i, j;
	for(j = 19 ; j < 59 ; j++)
		mvprintw(16, j, "=");
	for(i = 16 ; i < 24 ; i++) {
		mvprintw(i, 19, "|");
		mvprintw(i, 58, "|");
	}
	mvprintw(16, 19, "+");
	mvprintw(16, 58, "+");
	for(i = 0 ; i < 16 ; i++) {
		mvprintw(i, 58, "|");
		mvprintw(i, 19, "|");
	}
	mvprintw(1, 68, "UP");
	mvprintw(3, 68, "^");
	mvprintw(4, 60, "LEFT  < + >  RIGHT");
	mvprintw(5, 68, "v");
	mvprintw(7, 67, "DOWN");
	mvprintw(10, 61, "q = Back to Main");
	mvprintw(11, 65, "Menu");
	mvprintw(15, 61, "a = Immediate");
	mvprintw(16, 65, "auto move");
	mvprintw(20, 61, "u = Undo last");
	mvprintw(21, 65, "move");
	mvprintw(4, 1, "Merge tiles with");
	mvprintw(6, 1, "the same number");
	mvprintw(8, 1, "to obtain one");
	mvprintw(10, 1, "with double the");
	mvprintw(12, 1, "value. Try to");
	mvprintw(14, 1, "get to the 2048");
	mvprintw(16, 1, "tile, if you can!");
}

//Afisarea ceasului.
void timer() {
	time_t t;
	struct tm *c_tm;
	time(&t);
	c_tm = localtime(&t);
	mvprintw(21, 28, "%d/%d/%d ", (*c_tm).tm_mday, ((*c_tm).tm_mon) + 1, ((*c_tm).tm_year) + 1900);
	mvprintw(21, 39, "%d:%d:%d", (*c_tm).tm_hour, (*c_tm).tm_min, (*c_tm).tm_sec);
}

//Functie activata in momentul in care a fost selectata optiunea "High Scores" din meniu.
//Afiseaza primele 18 cele mai mari scoruri din fisier sortate descrescator,
//numele jucatorului respectiv si data jocului in cauza.
int printHighscores() {
	FILE *f;
	int k = 0, i, ok = 0;
	int auxs;
	char auxn[60];
	char auxd1[60];
	char auxd2[60];
	f = fopen("scores.txt", "r");
	if(f == NULL) {
		clear();
		mvprintw(11, 21, "The scores.txt file could not be open!");
		mvprintw(12, 19, "Make sure it exists in your current folder.");
		refresh();
		timeout(15000);
		getch();
		return 0;
	}
	Scores s;
	Scores score[80];

	//Citeste datele din fisier si le salveaza in structura.
	while(fscanf(f, "%s %d %*s %s %s\n", s.name, (&s.score), s.date1, s.date2) != EOF) {
		strcpy(score[k].name, s.name);
		score[k].score = s.score;
		strcpy(score[k].date1, s.date1);
		strcpy(score[k].date2, s.date2);
		k++;
	}

	//Sorteaza vectorul structura descrescator dupa scoruri
	while(ok != 1) {
		ok = 1;
		for(i = 0 ; i < k - 1 ; i++)
			if(score[i].score < score[i + 1].score) {	
				ok = 0;
				strcpy(auxn, score[i].name);
				strcpy(score[i].name, score[i + 1].name);
				strcpy(score[i + 1].name, auxn);

				strcpy(auxd1, score[i].date1);
				strcpy(score[i].date1, score[i + 1].date1);
				strcpy(score[i + 1].date1, auxd1);

				strcpy(auxd2, score[i].date2);
				strcpy(score[i].date2, score[i + 1].date2);
				strcpy(score[i + 1].date2, auxd2);

				auxs = score[i].score;
				score[i].score = score[i + 1].score;
				score[i + 1].score = auxs;
			}
	}
	//In caz ca sunt mai mult de 18 intrari, le ia pe primele 18 cele mai mari.
	if(k >= 18)
		k = 18;
	clear();
	attron(A_BOLD);
	mvprintw(1, 35, "High scores");
	attroff(A_BOLD);
	attron(A_UNDERLINE);
	mvprintw(3, 5, "Nr.");
	mvprintw(3, 10, "Name");
	mvprintw(3, 35, "Score");
	mvprintw(3, 49, "Date");
	attroff(A_UNDERLINE);
	for(i = 0 ; i < k ; i++) {
		mvprintw(4 + i, 5, "%d.", i + 1);
		mvprintw(4 + i, 10, "%s" , score[i].name);
		mvprintw(4 + i, 35, "%d" , score[i].score);
		mvprintw(4 + i, 49, "%s %s" , score[i].date1, score[i].date2);
	}
	refresh();
	timeout(80000);
	getch();
	return 0;
}

//Functie pentru pornirea jocului, activata in momentul alegerii optiunii "New Game" din meniu.
int startGame(int b[][4], int *exist, int *score, int aux[][4], int *scoreaux) {	
	int c, a, cond;				
	initBoard(b);
    printBoard(b);
	(*score) = 0;
	mvprintw(18, 33, "Score: %d", (*score));
	controls();
	timer();
	refresh();
	timeout(12000);
	//Preluarea inputului de la user (sagetile).
	//Va fi ERR in cazul time-outului, astfel
	//se va executa miscarea automata dupa 12 secunde de inactivitate.
    while((c = getch()) != 'q') {		
            int moves = 0, sums = 0;
            switch(c) {
            case KEY_UP:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
                moves = moveOnTop(b);
                sums = sumOnTop(b, score);
                moveOnTop(b);
                break;
            case KEY_DOWN:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
                moves = moveOnBottom(b);
                sums = sumOnBottom(b, score);
                moveOnBottom(b);
                break;
            case KEY_LEFT:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
                moves = moveToLeft(b);
                sums = sumOnLeft(b, score);
                moveToLeft(b);
                break;
            case KEY_RIGHT:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
                moves = moveToRight(b);
                sums = sumOnRight(b, score);
                moveToRight(b);
                break;
			case ERR:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
				a = whichIsBestMove(b);
				switch(a) {
					case 1:
						moves = moveOnTop(b);
                		sums = sumOnTop(b, score);
                		moveOnTop(b);
                		break;
					case 2:
						moves = moveOnBottom(b);
                		sums = sumOnBottom(b, score);
                		moveOnBottom(b);
						break;
					case 3:
                		moves = moveToLeft(b);
                		sums = sumOnLeft(b, score);
                		moveToLeft(b);
                		break;
					case 4:
						moves = moveToRight(b);
                		sums = sumOnRight(b, score);
                		moveToRight(b);
                		break;
				}
				break;
			case 'a':
				saveBoard(b, aux);
				(*scoreaux) = (*score);
				a = whichIsBestMove(b);
				switch(a) {
					case 1:
						moves = moveOnTop(b);
                		sums = sumOnTop(b, score);
                		moveOnTop(b);
                		break;
					case 2:
						moves = moveOnBottom(b);
                		sums = sumOnBottom(b, score);
                		moveOnBottom(b);
						break;
					case 3:
                		moves = moveToLeft(b);
                		sums = sumOnLeft(b, score);
                		moveToLeft(b);
                		break;
					case 4:
						moves = moveToRight(b);
                		sums = sumOnRight(b, score);
                		moveToRight(b);
                		break;
				}
				break;
			case 'u':
				if(checkIfUndone(b, aux) == 0) {
					doUndo(b, aux);
					(*score) = (*scoreaux);
					printBoard(b);
					mvprintw(18, 33, "Score: %d", (*score));
					controls();
					timer();
					refresh();
					(*exist) = 1;
				}
				break;
            }
            if(moves > 0 || sums > 0) {
                addRandomValue(b);
            	printBoard(b);
				mvprintw(18, 33, "Score: %d", (*score));
				controls();
				timer();
				refresh();
				(*exist) = 1;
				if(checkIfWin(b) == 1) {
					cond = 1;
					(*exist) = 0;
					return 2;
				}
						
			}
			else
				if(checkGameOver(b) == 1) {
					cond = 0;
					(*exist) = 0;
					return 1;
    			}
	
	}
	return 0;
}

//Functie apelata in cazul selectarii optiunii "Resume" din meniu.
//Similara cu startGame, dar nu contine initializarile.
int continueGame(int b[][4], int *exist,int *score,int aux[][4],int *scoreaux) {
	int c, a, cond;
	clear();
	printBoard(b);
	mvprintw(18, 33, "Score: %d", (*score));
	controls();
	timer();
	refresh();
	timeout(12000);
	while((c = getch()) != 'q') {
            int moves = 0, sums = 0;
            switch(c) {
            case KEY_UP:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
                moves = moveOnTop(b);
                sums = sumOnTop(b, score);
                moveOnTop(b);
                break;
            case KEY_DOWN:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
                moves = moveOnBottom(b);
                sums = sumOnBottom(b, score);
                moveOnBottom(b);
                break;
            case KEY_LEFT:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
                moves = moveToLeft(b);
                sums = sumOnLeft(b, score);
                moveToLeft(b);
                break;
            case KEY_RIGHT:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
                moves = moveToRight(b);
                sums = sumOnRight(b, score);
                moveToRight(b);
                break;
			case ERR:
				saveBoard(b, aux);
				(*scoreaux) = (*score);
				a = whichIsBestMove(b);
				switch(a) {
					case 1:
						moves = moveOnTop(b);
                		sums = sumOnTop(b, score);
                		moveOnTop(b);
                		break;
					case 2:
						moves = moveOnBottom(b);
                		sums = sumOnBottom(b, score);
                		moveOnBottom(b);
						break;
					case 3:
                		moves = moveToLeft(b);
                		sums = sumOnLeft(b, score);
                		moveToLeft(b);
                		break;
					case 4:
						moves = moveToRight(b);
                		sums = sumOnRight(b, score);
                		moveToRight(b);
                		break;
				}
				break;
			case 'a':
				saveBoard(b, aux);
				(*scoreaux) = (*score);
				a = whichIsBestMove(b);
				switch(a) {
					case 1:
						moves = moveOnTop(b);
                		sums = sumOnTop(b, score);
                		moveOnTop(b);
                		break;
					case 2:
						moves = moveOnBottom(b);
                		sums = sumOnBottom(b, score);
                		moveOnBottom(b);
						break;
					case 3:
                		moves = moveToLeft(b);
                		sums = sumOnLeft(b, score);
                		moveToLeft(b);
                		break;
					case 4:
						moves = moveToRight(b);
                		sums = sumOnRight(b, score);
                		moveToRight(b);
                		break;
				}
				break;
			case 'u':
				if(checkIfUndone(b, aux) == 0) {
					doUndo(b, aux);
					(*score) = (*scoreaux);
					printBoard(b);
					mvprintw(18, 33, "Score: %d", (*score));
					controls();
					timer();
					refresh();
					(*exist) = 1;
				}
				break;
            }
            if(moves > 0 || sums > 0) {
                addRandomValue(b);
            	printBoard(b);
				mvprintw(18, 33, "Score: %d", (*score));
				controls();
				timer();
				refresh();
				if(checkIfWin(b) == 1){
					cond = 1;
					(*exist) = 0;
					return 2;
				}
			}
			else
				if(checkGameOver(b) == 1){
					cond = 0;
					(*exist) = 0;
					return 1;
    			}
    }
	return 0;
}

int main() {
	int exist = 0, b[4][4], opt = 0, ok = 1, score = 0, a, c, cond;
	char menu_items[4][15] = {'\0'};
	int aux[4][4], scoreaux = 0;

	init();
	srand(time(NULL));
	menuCreate(&exist, menu_items);
	menuLoop(menu_items, &exist, &opt);
	while(ok == 1) {
		//opt=optiune, variabila care retine ce optiune s-a ales in momentul apasarii tastei ENTER.
		switch(opt) {	
			case 1:
				clear();
				a = startGame(b, &exist, &score, aux, &scoreaux);
				clear();
				if(a == 1) {
					cond = 0;
					endScreen(&cond, &score);
				}
				else
					if(a == 2) {
						cond = 1;
						endScreen(&cond, &score);
					}
				clear();
				menuCreate(&exist, menu_items);
				menuLoop(menu_items, &exist,&opt);
				break;
			case 2:
				if(exist==1) {
					c = continueGame(b, &exist, &score, aux, &scoreaux);
					clear();
					if(c == 1) {
						cond = 0;
						endScreen(&cond, &score);
					}
					else
						if(c == 2) {
							cond = 1;
							endScreen(&cond, &score);
						}
					menuCreate(&exist, menu_items);
					menuLoop(menu_items, &exist, &opt);
				}
				else {
					printHighscores();
					clear();
					menuCreate(&exist, menu_items);
					menuLoop(menu_items, &exist, &opt);
				}
			break;
			case 3:
				if(exist == 1) {
					printHighscores();
					clear();
					menuCreate(&exist, menu_items);
					menuLoop(menu_items, &exist,&opt);
				}
				else
					ok = 0;
				break;
			case 4:
				ok = 0;
				break;
		}
	}
	endwin();
	return 0;
}
