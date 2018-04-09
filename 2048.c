#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct { //Structura in care am stocat scorurile extrase din fisierul
	char name[60]; // "scores.txt".
	int score;
	char date1[50];
	char date2[50];
} Scores;

void init(){ //Initializare ncurses.
	initscr();
	cbreak();
	start_color();
	noecho();
	keypad(stdscr,TRUE);
	curs_set(0);
	init_pair(1,COLOR_WHITE,COLOR_CYAN); //Culoare background
	init_pair(2,COLOR_BLACK,COLOR_YELLOW); //Culoare pt celulele cu 0
	init_pair(3,COLOR_WHITE,COLOR_RED); //Culoare pt celulele cu 2
	init_pair(4,COLOR_WHITE,COLOR_GREEN); //Culoare pt celulele cu 4
	init_pair(5,COLOR_WHITE,COLOR_BLUE); //Culoare pt celulele cu 8
	init_pair(6,COLOR_WHITE,COLOR_BLACK); //Culoare pt celulele cu 16
	init_pair(7,COLOR_WHITE,COLOR_MAGENTA); //Culoare pt celulele cu 32
	init_pair(8,COLOR_BLACK,COLOR_WHITE); //Culoare pt celulele cu 64
	init_pair(9,COLOR_BLUE,COLOR_GREEN); //Culoare pt celulele cu 128
	init_pair(10,COLOR_YELLOW,COLOR_RED); //Culoare pt celulele cu 256
	init_pair(11,COLOR_BLACK,COLOR_MAGENTA); //Culoare pt celulele cu 512
	init_pair(12,COLOR_YELLOW,COLOR_BLACK); //Culoare pt celulele cu 1024
	init_pair(13,COLOR_BLUE,COLOR_YELLOW); //Culoare pt celulele cu 2048
	bkgd(COLOR_PAIR(1));
}

void ascii1(){ //Functia pentru afisarea primului ASCII, cel din meniu.
	mvprintw(1,18," .-----.   .----.      .---.    .-----.   ");
	mvprintw(2,18,"/ ,-.   \\ /  ..  \\    / .  |   /  .-.  \\  ");
	mvprintw(3,18,"'-'  |  |.  /  \\  .  / /|  |  |   \\_.' /  ");
	mvprintw(4,18,"   .'  / |  |  '  | / / |  |_  /  .-. '.  ");
	mvprintw(5,18," .'  /__ '  \\  /  '/  '-'    ||  |   |  | ");
	mvprintw(6,18,"|       | \\  `'  / `----|  |-' \\  '-'  /  ");
	mvprintw(7,18,"`-------'  `---''       `--'    `----''   ");
	mvprintw(19,22,"=====================================");
	mvprintw(20,37,"Made by");
	mvprintw(21,28,"Mirescu Andreea-Alexandra");
	mvprintw(22,38,"313CC");
}

void ascii2(){ //Functia pentru afisarea celui de al doilea ASCII, cel din joc.
	mvprintw(1,30," ___ ___ ___ ___ ");
	mvprintw(2,30,"|_  |   | | | . |");
	mvprintw(3,30,"|  _| | |_  | . |");
	mvprintw(4,30,"|___|___| |_|___|");
}

void menuCreate(int *exist, char menu_items[][15]){ //Functia pentru afisarea
	clear();	    //initiala a meniului si salvarea elementelor intr-un vector
	ascii1();		//in functie de existenta unei sesiuni anterior deschise.
	attron(A_UNDERLINE);
	mvprintw(10,36,"MENU");
	attroff(A_UNDERLINE);
	if((*exist)==0){
		attron(A_STANDOUT);
		mvprintw(12,35,"%s","New Game");
		attroff(A_STANDOUT);
		mvprintw(13,35,"%s","High Scores");
		mvprintw(14,35,"%s","Quit");
		strcpy(menu_items[0],"New Game");
		strcpy(menu_items[1],"High Scores");
		strcpy(menu_items[2],"Quit");
	}
	else {
		attron(A_STANDOUT);
		mvprintw(12,35,"%s","New Game");
		attroff(A_STANDOUT);
		mvprintw(13,35,"%s","Resume");
		mvprintw(14,35,"%s","High Scores");
		mvprintw(15,35,"%s","Quit");
		strcpy(menu_items[0],"New Game");
		strcpy(menu_items[1],"Resume");
		strcpy(menu_items[2],"High Scores");
		strcpy(menu_items[3],"Quit");
	}
	refresh();
}

void menuLoop(char menu_items[][15], int *exist, int *opt){ //Functia care
	int i=0, a;		//creeaza bucla din meniu, pentru evidentierea optiunii
	while((a = getch()) != '\n'){		//selectate curent si face detectarea
		ascii1();						//optiunii selectate in variabila opt.
		attron(A_UNDERLINE);
		mvprintw(10,36,"MENU");
		attroff(A_UNDERLINE);
		mvprintw(12+i,35,"%s",menu_items[i]);
		switch(a){
			case KEY_UP:
				i--;
				if(i<0){
					if((*exist)==0)
						i=2;
					else
						i=3;
				}
				break;
			case KEY_DOWN:
				i++;
				if((*exist==0)){
					if(i>2)
						i=0;
				}
				else {
					if(i>3)
						i=0;
				}
				break;
		}
		attron(A_STANDOUT);
		mvprintw(12+i,35,"%s",menu_items[i]);
		attroff(A_STANDOUT);
		refresh();
	}
	(*opt)=i+1;
}

int randNum(){		//Functie pentru generarea aleatoare a unui numar din
	int num, ok=0;	//multimea {2, 4}.
	while(ok==0){
		num = rand() % 5;
		if(num%2 == 0 && num!=0)
			ok=1;
	}
	return num;
}

void randPos(int *pos1, int *pos2){ //Generarea unei pozitii in care sa apara
	(*pos1)=rand() % 4;		//piesa din intervalul {2, 4}, prin generarea
	(*pos2)=rand() % 4;		//aleatoare a doua coordonate.
}

void addRandomValue(int b[][4]){ //Functia care adauga o valoare aleatoare
	int pos1=0, pos2=0;		//din interval, la o pozitie aleatoare, pe
	do {					//tabla curenta.
		randPos(&pos1,&pos2);
	}while(b[pos1][pos2] != 0);
	b[pos1][pos2] = randNum();
}

void initBoard(int b[][4]){		//Initializarea tablei cu valori de 0 si
	int i, j;					//generarea celor 2 celule care contin
	for(i=0 ; i < 4 ; i++)		//valori din {2, 4}.
		for(j=0 ; j < 4 ; j++)
			b[i][j]=0;
	addRandomValue(b);
	addRandomValue(b);
}

void printBoard(int b[][4]){	//Functie pentru afisarea tablei si a altor
	int i, j, p_num;			//elemente care apar pe ecranul de joc.
	clear();					//(al doilea ASCII, panoul de control etc.)
	ascii2();
	for(i=0 ; i<5 ; i++)
		for(j=0 ; j<24 ; j++)
			mvprintw(6+2*i,27+j,"-");
	for(j=0 ; j<5 ; j++)
		for(i=0 ; i<7 ; i++)
			mvprintw(7+i,26+6*j,"|");
	for(i=0 ; i<5 ; i++)
		for(j=0 ; j<5 ; j++)
			mvprintw(6+2*i,26+6*j,"+");
	for(i=0 ; i<4 ; i++){			//Colorarea celulelor in functie
		for(j=0 ; j<4 ; j++){		//de valoare
			switch(b[i][j]){
				case 0:
					p_num=2;
					break;
				case 2:
					p_num=3;
					break;
				case 4:
					p_num=4;
					break;
				case 8:
					p_num=5;
					break;
				case 16:
					p_num=6;
					break;
				case 32:
					p_num=7;
					break;
				case 64:
					p_num=8;
					break;
				case 128:
					p_num=9;
					break;
				case 256:
					p_num=10;
					break;
				case 512:
					p_num=11;
					break;
				case 1024:
					p_num=12;
					break;
				case 2048:
					p_num=13;
					break;
			}	
			attron(COLOR_PAIR(p_num));
			mvprintw(7+2*i,27+6*j,"%d",b[i][j]);
			attroff(COLOR_PAIR(p_num));
		}
	}
	refresh();
}

int notEmptyLine(int b[][4], int i, int k){	//Verificare daca linia este goala.
	int notEmpty=0, j;
	for(j=k ; j<4 ; j++){
		if(b[i][j] != 0){
			notEmpty=1;
			break;
		}
	}
	return notEmpty;
}

int reverseNotEmptyLine(int b[][4], int i, int k){ //Aceasi functie, 
	int notEmpty=0, j;							//in sens invers.
	for(j=k ; j>=0 ; j--){
		if(b[i][j] != 0){
			notEmpty=1;
			break;
		}
	}
	return notEmpty;
}

int notEmptyColumn(int b[][4], int k, int j){//Verificare daca coloana e goala.
	int notEmpty=0, i;
	for(i=k+1 ; i<4 ; i++){
		if(b[i][j] != 0){
			notEmpty=1;
			break;
		}
	}
	return notEmpty;
}

int reverseNotEmptyColumn(int b[][4], int k, int j){ //Aceasi functie,
	int notEmpty=0, i;								//in sens invers.
	for(i=0 ; i<k ; i++){
		if(b[i][j] != 0){
			notEmpty=1;
			break;
		}
	}
	return notEmpty;
}

void permLineToLeft(int b[][4], int line, int col){	//Permutarea celulelor in
    int j;											//stanga.
	for(j=col ; j<3 ; j++){
		b[line][j] = b[line][j+1];
    }
    b[line][3]=0;
}

void permLineToRight(int b[][4], int line, int col){//Permutarea celulelor in
	int j;											//dreapta.
	for(j=col ; j>0 ; j--){
		b[line][j] = b[line][j-1];
	}
	b[line][0]=0;
}

void permLineOnTop(int b[][4], int line, int col){	//Permutarea celulelor in
	int i;											//sus.
	for(i=line ; i<3 ; i++){
		b[i][col] = b[i+1][col];
	}
	b[3][col] = 0;
}

void permLineOnBottom(int b[][4], int line, int col){//Permutarea celulelor in
	int i;											//jos.
	for(i=line ; i>0 ; i--){
	b[i][col] = b[i-1][col];
	}
	b[0][col] = 0;
}

int moveToLeft(int b[][4]){		//Mutarea efectiva la stanga.
	int perms=0;
	int i=0;
	while(i<4){
		int j=0;
		while(j<4){
			while(b[i][j] == 0 && notEmptyLine(b,i,j)){
				perms++;
				permLineToLeft(b,i,j);
			}
			j++;
		}
		i++;
	}
	return perms;
}

int moveToRight(int b[][4]){	//Mutarea efectiva la dreapt.
	int perms=0;
	int i=0;
	while(i<4){
		int j=3;
		while(j>0){
			while(b[i][j] == 0 && reverseNotEmptyLine(b,i,j)){
				perms++;
				permLineToRight(b,i,j);
			}
			j--;
		}
		i++;
	}
	return perms;
}

int moveOnTop(int b[][4]){	//Mutarea efectiva in sus.
	int perms=0;
	int j=0;
	while(j<4){
		int i=0;
		while(i<4){
			while(b[i][j] == 0 && notEmptyColumn(b,i,j)){
				perms++;
				permLineOnTop(b,i,j);
			}
			i++;
		}
		j++;
	}
	return perms;
}

int moveOnBottom(int b[][4]){	//Mutarea efectiva in jos.
	int perms=0;
	int j=3;
	while(j>=0){
		int i=3;
		while(i>=0){
			while(b[i][j] == 0 && reverseNotEmptyColumn(b,i,j)){
				perms++;
				permLineOnBottom(b,i,j);
			}
			i--;
		}
		j--;
	}
	return perms;
}

int sumOnLeft(int b[][4],int *score){	//Lipirea celulelor, la stanga
	int perms=0;						//si actualizarea scorului.
	int i, j;
	for(i=0 ; i<4 ; i++){
		for(j=0 ; j<3 ; j++){
			if(b[i][j]!=0 && b[i][j+1]!=0 && b[i][j]==b[i][j+1]){
				b[i][j]=2*b[i][j];
				(*score)=(*score)+b[i][j];
				b[i][j+1]=0;
				perms++;
			}
		}
	}
	return perms;
}

int sumOnRight(int b[][4], int *score){		//Lipirea celulelor, la dreapta.
	int perms=0;							//si actualizarea scorului.
	int i, j;
	for(i=0 ; i<4 ; i++){
		for(j=3 ; j>0 ; j--){
			if(b[i][j]!=0 && b[i][j-1]!=0 && b[i][j]==b[i][j-1]){
				b[i][j]=2*b[i][j];
				(*score)=(*score)+b[i][j];
				b[i][j-1]=0;
				perms++;
			}
		}
	}
	return perms;
}

int sumOnTop(int b[][4],int *score){		//Lipirea celulelor, in sus.
	int perms=0;							//si actualizarea scorului.
	int i, j;
	for(j=0 ; j<4 ; j++){
		for(i=0 ; i<3 ; i++){
			if(b[i][j]!=0 && b[i+1][j]!=0 && b[i][j]==b[i+1][j]){
				b[i][j]=2*b[i][j];
				(*score)=(*score)+b[i][j];
				b[i+1][j]=0;
				perms++;
			}
		}
	}
	return perms;
}

int sumOnBottom(int b[][4],int *score){		//Lipirea ceulelor, in jos.
	int perms=0;							//si actualizarea scorului.
	int i, j;
	for(j=0 ; j<4 ; j++){
		for(i=3 ; i>0 ; i--){
			if(b[i][j]!=0 && b[i-1][j]!=0 && b[i][j]==b[i-1][j]){
				b[i][j]=2*b[i][j];
				(*score)=(*score)+b[i][j];
				b[i-1][j]=0;
				perms++;
			}
		}
	}
	return perms;
}

void saveBoard(int b[][4], int aux[][4]){		//Salveaza tabla curenta
	int i, j;									//intr-un auxiliar.
	for(i=0 ; i<4 ; i++)
		for(j=0 ; j<4 ; j++)
			aux[i][j]=b[i][j];
}

void doUndo(int b[][4], int aux[][4]){			//Functia care face undo.
	int i, j;
	for(i=0 ; i<4 ; i++)
		for(j=0 ; j<4 ; j++)
			b[i][j]=aux[i][j];
}

int checkIfUndone(int b[][4], int aux[][4]){	//Verifica daca a fost deja
	int i, j, k=0;								//facut undo.
	for(i=0 ; i<4 ; i++)
		for(j=0 ; j<4 ;j++)
			if(b[i][j] != aux[i][j])		//Verifica daca toti termenii sunt
				k++;					//identici, cu exceptia a unuia singur
	if(k == 1)							//pentru ca salvarea tablei se face
		return 1;						//inainte de generarea celulei din
	return 0;							//intervalul {2, 4}
}

int whichIsBestMove(int b[][4]){ //Determinarea miscarii ideale, in cazul
	int aux1[4][4], aux2[4][4];  //automutarii, folosind functiile de mutare
	int	aux3[4][4], aux4[4][4];  //pe niste auxiliare si preluand valorile
	int	i, j, which=0;			//returnate. Functia returneaza urmatoarele
	int score2=0, max;			//valori: 1=sus, 2=jos, 3=stanga, 4=dreapta.
	int move[4]={0,0,0,0};		//move=cate miscari produce o anumita directie.
	int nrfcells[4]={0,0,0,0};	//nrfcells=numarul de celule eliberate de
	for(i=0 ; i<4 ; i++)		//fiecare directie in parte.
		for(j=0 ; j<4 ; j++){
			aux1[i][j]=b[i][j];
			aux2[i][j]=b[i][j];
			aux3[i][j]=b[i][j];
			aux4[i][j]=b[i][j];
		}
	move[0] = moveOnTop(aux1);
    nrfcells[0] = sumOnTop(aux1,&score2);
    move[0] = move[0] + moveOnTop(aux1);
	move[1] = moveOnBottom(aux2);
    nrfcells[1] = sumOnBottom(aux2,&score2);
    move[1] = move[1] + moveOnBottom(aux2);
	move[2] = moveToLeft(aux3);
    nrfcells[2] = sumOnLeft(aux3,&score2);
    move[2] = move[2] + moveToLeft(aux3);
	move[3] = moveToRight(aux4);
    nrfcells[3] = sumOnRight(aux4,&score2);
    move[3]= move[3] + moveToRight(aux4);
	max=nrfcells[0];
	which=1;
	if((nrfcells[0] == 0) && (nrfcells[1] == 0) && (nrfcells[2] == 0) && (nrfcells[3] == 0)){
		int i;
		for(i=0 ; i<4 ; i++){
			if(move[i] != 0){
				which=i+1;
				mvprintw(4,1,"%d",which);
				return which;
			}
		}
	}
	for(i=1 ; i<4 ; i++){
		if(nrfcells[i] > max && move[i] !=0){
			max=nrfcells[i];
			which=i+1;
		}
		
	}
	return which;
}

int checkGameOver(int b[][4]){	//Verificare daca jocul s-a terminat.
	int aux1[4][4], aux2[4][4]; //Acelasi principiu ca la functia precendenta.
	int aux3[4][4], aux4[4][4], i, j, score2=0;
	int move[4]={0,0,0,0};	 //move=cate miscari produce o anumita directie.
	for(i=0 ; i<4 ; i++)
		for(j=0 ; j<4 ; j++){
			aux1[i][j]=b[i][j];
			aux2[i][j]=b[i][j];
			aux3[i][j]=b[i][j];
			aux4[i][j]=b[i][j];
		}
	move[0] = moveOnTop(aux1);
    sumOnTop(aux1,&score2);
    move[0] = move[0] + moveOnTop(aux1);
	move[1] = moveOnBottom(aux2);
    sumOnBottom(aux2,&score2);
    move[1] = move[1] + moveOnBottom(aux2);
	move[2] = moveToLeft(aux3);
    sumOnLeft(aux3,&score2);
    move[2] = move[2] + moveToLeft(aux3);
	move[3] = moveToRight(aux4);
    sumOnRight(aux4,&score2);
    move[3]= move[3] + moveToRight(aux4);
	if((move[0] == 0) && (move[1] == 0) && (move[2] == 0) && (move[3] == 0))
		return 1;
	else
		return 0;
}

int checkIfWin(int b[][4]){		//Verificare daca jocul a fost castigat
	int i, j;					//(cauta piesa de 2048).
	for(i=0 ; i<4 ; i++)
		for(j=0 ; j<4 ; j++)
			if(b[i][j] == 2048)
				return 1;
	return 0;
}

int highScoreWrite(int *score){	//Functia care preia un nume si salveaza
	time_t t;				//scorul, numele dat si data terminarii jocului
	struct tm *c_tm;		//in fisierul scores.txt
	time(&t);
	c_tm=localtime(&t);
	int a, ok=0;
	char menu[2][12];
	strcpy(menu[0],"Yes");
	strcpy(menu[1],"No, retype");
	FILE *f;
	f=fopen("scores.txt","a");
	if(f == NULL){
		clear();
		mvprintw(11,21,"The scores.txt file could not be open!");
		mvprintw(12,19,"Make sure it exists in your current folder.");
		refresh();
		timeout(15000);
		getch();
		return 0;
	}
	char name[45];
	while(ok == 0){		//Bucla pentru interogarea utilizatorului
		int i=0;		//In cazul confirmarii cere nu nume si apoi il afisaza
		clear();		//pentru verificarea scrierii corecte.
		mvprintw(11,25,"Type your name and press ENTER:");
		refresh();
		curs_set(1);
		getnstr(name,45);
		curs_set(0);
		clear();
		mvprintw(11,27,"Is %s what you meant?",name);
		attron(A_STANDOUT);
		mvprintw(15,27,"%s","Yes");
		attroff(A_STANDOUT);
		mvprintw(15,41,"%s","No, retype");
		refresh();
		fflush(stdin);
		timeout(80000);
		while((a = getch()) != '\n'){
			mvprintw(15,30+20*i,"%s",menu[i]);
			switch(a){
				case KEY_LEFT:
					i--;
					if(i<0)
						i=1;
					break;
				case KEY_RIGHT:
					i++;
					if(i>1)
						i=0;
					break;
			}
			attron(A_STANDOUT);
			mvprintw(15,30+20*i,"%s",menu[i]);
			attroff(A_STANDOUT);
		}
		if(i == 0){
			ok=1;	//Scrierea in fisier a datelor.
			fprintf(f,"%s %d ",name,*score);
			fprintf(f,"on %d/%d/%d ",(*c_tm).tm_mday,((*c_tm).tm_mon)+1,((*c_tm).tm_year)+1900);
			fprintf(f,"%d:%d:%d\n",(*c_tm).tm_hour,(*c_tm).tm_min,(*c_tm).tm_sec);
		}
	}
	fclose(f);
	return 0;
}

int endScreen(int *cond, int *score){	//Functie pentru afisarea ecranului
	int a, i=0;						//final. In cazul in care jocul a fost
	char menu[2][4];				//castigat, utilizatorul este intrebat
	strcpy(menu[0],"Yes");			//daca vrea sa isi salveze scorul in "High
	strcpy(menu[1],"No");			//Scores".
	clear();
	if((*cond)==0){
		mvprintw(11,35,"Game over!");
		mvprintw(12,30,"Your score was: %d",(*score));
		timeout(20000);
		getch();
		return 0;
	}
	else
		mvprintw(11,35,"You won!");
	
	mvprintw(12,30,"Your score was: %d",(*score));
	mvprintw(13,15,"Do you want to save your score to the high scores?");
	attron(A_STANDOUT);
	mvprintw(15,30,"%s","Yes");
	attroff(A_STANDOUT);
	mvprintw(15,50,"%s","No");
	refresh();
	fflush(stdin);
	timeout(80000);
	while((a = getch()) != '\n'){
		mvprintw(15,30+20*i,"%s",menu[i]);
		switch(a){
			case KEY_LEFT:
				i--;
				if(i<0)
					i=1;
				break;
			case KEY_RIGHT:
				i++;
				if(i>1)
					i=0;
				break;
		}
		attron(A_STANDOUT);
		mvprintw(15,30+20*i,"%s",menu[i]);
		attroff(A_STANDOUT);
		refresh();
	}
	switch(i){
		case 0:
			highScoreWrite(score);
			break;
		case 1:
			break;
	}
	return 0;			
}

void controls(){		//Afisarea panoului de control.
	int i, j;
	for(j=19 ; j<59 ; j++)
		mvprintw(16,j,"=");
	for(i=16 ; i<24 ; i++){
		mvprintw(i,19,"|");
		mvprintw(i,58,"|");
	}
	mvprintw(16,19,"+");
	mvprintw(16,58,"+");
	for(i=0 ; i<16 ; i++){
		mvprintw(i,58,"|");
		mvprintw(i,19,"|");
	}
	mvprintw(1,68,"UP");
	mvprintw(3,68,"^");
	mvprintw(4,60,"LEFT  < + >  RIGHT");
	mvprintw(5,68,"v");
	mvprintw(7,67,"DOWN");
	mvprintw(10,61,"q = Back to Main");
	mvprintw(11,65,"Menu");
	mvprintw(15,61,"a = Immediate");
	mvprintw(16,65,"auto move");
	mvprintw(20,61,"u = Undo last");
	mvprintw(21,65,"move");
	mvprintw(4,1,"Merge tiles with");
	mvprintw(6,1,"the same number");
	mvprintw(8,1,"to obtain one");
	mvprintw(10,1,"with double the");
	mvprintw(12,1,"value. Try to");
	mvprintw(14,1,"get to the 2048");
	mvprintw(16,1,"tile, if you can!");
}

void timer(){		//Afisarea ceasului.
	time_t t;
	struct tm *c_tm;
	time(&t);
	c_tm=localtime(&t);
	mvprintw(21,28,"%d/%d/%d ",(*c_tm).tm_mday,((*c_tm).tm_mon)+1,((*c_tm).tm_year)+1900);
	mvprintw(21,39,"%d:%d:%d",(*c_tm).tm_hour,(*c_tm).tm_min,(*c_tm).tm_sec);
}

int printHighscores(){		//Functie activata in momentul in care a fost
	FILE *f;			//selectata optiunea "High Scores" din meniu. Afisaza
	int k=0, i, ok=0;	//primele 18 cele mai mari scoruri din fisier,
	int auxs;			//sortate descrescator, numele jucatorului respectiv
	char auxn[60];		//si data jocului in cauza.
	char auxd1[60];
	char auxd2[60];
	f=fopen("scores.txt","r");
	if(f == NULL){
		clear();
		mvprintw(11,21,"The scores.txt file could not be open!");
		mvprintw(12,19,"Make sure it exists in your current folder.");
		refresh();
		timeout(15000);
		getch();
		return 0;
	}
	Scores s;
	Scores score[80];
	while(fscanf(f,"%s %d %*s %s %s\n",s.name,(&s.score),s.date1,s.date2) != EOF){
		strcpy(score[k].name,s.name);			//Citeste datele din fisier si
		score[k].score=s.score;					//le salveaza in structura.
		strcpy(score[k].date1,s.date1);
		strcpy(score[k].date2,s.date2);
		k++;
	}
	while(ok != 1){
		ok=1;
		for(i=0 ; i<k-1 ; i++)
			if(score[i].score < score[i+1].score){	//Sorteaza vectorul
				ok=0;				  //structura, descrescator, dupa scoruri.
				strcpy(auxn,score[i].name);
				strcpy(score[i].name,score[i+1].name);
				strcpy(score[i+1].name,auxn);
				strcpy(auxd1,score[i].date1);
				strcpy(score[i].date1,score[i+1].date1);
				strcpy(score[i+1].date1,auxd1);
				strcpy(auxd2,score[i].date2);
				strcpy(score[i].date2,score[i+1].date2);
				strcpy(score[i+1].date2,auxd2);
				auxs=score[i].score;
				score[i].score=score[i+1].score;
				score[i+1].score=auxs;
			}
	}
	if(k>=18)		//In caz ca sunt mai mult de 18 intrari, le ia pe primele
		k=18;		//18 cele mai mari.
	clear();
	attron(A_BOLD);
	mvprintw(1,35,"High scores");
	attroff(A_BOLD);
	attron(A_UNDERLINE);
	mvprintw(3,5,"Nr.");
	mvprintw(3,10,"Name");
	mvprintw(3,35,"Score");
	mvprintw(3,49,"Date");
	attroff(A_UNDERLINE);
	for(i=0 ; i<k ; i++){
		mvprintw(4+i,5,"%d.",i+1);
		mvprintw(4+i,10,"%s",score[i].name);
		mvprintw(4+i,35,"%d",score[i].score);
		mvprintw(4+i,49,"%s %s",score[i].date1,score[i].date2);
	}
	refresh();
	timeout(80000);
	getch();
	return 0;
}

int startGame(int b[][4], int *exist, int *score, int aux[][4], int *scoreaux){	
	int c, a, cond;				//Functie pentru pornirea
	initBoard(b);				//jocului propriu zis, activata in momentul
    printBoard(b);				//alegerii optiunii "New Game" din meniu.
	(*score)=0;
	mvprintw(18,33,"Score: %d",(*score));
	controls();
	timer();
	refresh();
	timeout(12000);
    while((c=getch())!='q'){		//Preluarea inputului de la user, sagetile,
            int moves=0, sums=0;	//iar ERR in cazul time-outului, astfel
            switch(c){				//executandu-se miscarea automata dupa
            case KEY_UP:			//12 secunde de inactivitate.
				saveBoard(b,aux);
				(*scoreaux)=(*score);
                moves = moveOnTop(b);
                sums = sumOnTop(b,score);
                moveOnTop(b);
                break;
            case KEY_DOWN:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
                moves = moveOnBottom(b);
                sums = sumOnBottom(b,score);
                moveOnBottom(b);
                break;
            case KEY_LEFT:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
                moves = moveToLeft(b);
                sums = sumOnLeft(b,score);
                moveToLeft(b);
                break;
            case KEY_RIGHT:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
                moves = moveToRight(b);
                sums = sumOnRight(b,score);
                moveToRight(b);
                break;
			case ERR:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
				a=whichIsBestMove(b);
				switch(a){
					case 1:
						moves = moveOnTop(b);
                		sums = sumOnTop(b,score);
                		moveOnTop(b);
                		break;
					case 2:
						moves = moveOnBottom(b);
                		sums = sumOnBottom(b,score);
                		moveOnBottom(b);
						break;
					case 3:
                		moves = moveToLeft(b);
                		sums = sumOnLeft(b,score);
                		moveToLeft(b);
                		break;
					case 4:
						moves = moveToRight(b);
                		sums = sumOnRight(b,score);
                		moveToRight(b);
                		break;
				}
				break;
			case 'a':
				saveBoard(b,aux);
				(*scoreaux)=(*score);
				a=whichIsBestMove(b);
				switch(a){
					case 1:
						moves = moveOnTop(b);
                		sums = sumOnTop(b,score);
                		moveOnTop(b);
                		break;
					case 2:
						moves = moveOnBottom(b);
                		sums = sumOnBottom(b,score);
                		moveOnBottom(b);
						break;
					case 3:
                		moves = moveToLeft(b);
                		sums = sumOnLeft(b,score);
                		moveToLeft(b);
                		break;
					case 4:
						moves = moveToRight(b);
                		sums = sumOnRight(b,score);
                		moveToRight(b);
                		break;
				}
				break;
			case 'u':
				if(checkIfUndone(b,aux) == 0){
					doUndo(b,aux);
					(*score) = (*scoreaux);
					printBoard(b);
					mvprintw(18,33,"Score: %d",(*score));
					controls();
					timer();
					refresh();
					(*exist)=1;
				}
				break;
            }
            if(moves>0 || sums>0){
                addRandomValue(b);
            	printBoard(b);
				mvprintw(18,33,"Score: %d",(*score));
				controls();
				timer();
				refresh();
				(*exist)=1;
				if(checkIfWin(b) == 1){
					cond=1;
					(*exist)=0;
					return 2;
				}
						
			}
			else
				if(checkGameOver(b) == 1){
					cond=0;
					(*exist)=0;
					return 1;
    			}
	
	}
	return 0;
}

int continueGame(int b[][4], int *exist,int *score,int aux[][4],int *scoreaux){
	int c, a, cond;	//Functie apelata in
	clear();		//cazul selectarii optiunii "Resume" din meniu.
	printBoard(b);	//Foarte similara cu startGame, nu contine initializarile.
	mvprintw(18,33,"Score: %d",(*score));
	controls();
	timer();
	refresh();
	timeout(12000);
	while((c=getch())!='q'){
            int moves=0, sums=0;
            switch(c){
            case KEY_UP:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
                moves = moveOnTop(b);
                sums = sumOnTop(b,score);
                moveOnTop(b);
                break;
            case KEY_DOWN:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
                moves = moveOnBottom(b);
                sums = sumOnBottom(b,score);
                moveOnBottom(b);
                break;
            case KEY_LEFT:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
                moves = moveToLeft(b);
                sums = sumOnLeft(b,score);
                moveToLeft(b);
                break;
            case KEY_RIGHT:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
                moves = moveToRight(b);
                sums = sumOnRight(b,score);
                moveToRight(b);
                break;
			case ERR:
				saveBoard(b,aux);
				(*scoreaux)=(*score);
				a=whichIsBestMove(b);
				switch(a){
					case 1:
						moves = moveOnTop(b);
                		sums = sumOnTop(b,score);
                		moveOnTop(b);
                		break;
					case 2:
						moves = moveOnBottom(b);
                		sums = sumOnBottom(b,score);
                		moveOnBottom(b);
						break;
					case 3:
                		moves = moveToLeft(b);
                		sums = sumOnLeft(b,score);
                		moveToLeft(b);
                		break;
					case 4:
						moves = moveToRight(b);
                		sums = sumOnRight(b,score);
                		moveToRight(b);
                		break;
				}
				break;
			case 'a':
				saveBoard(b,aux);
				(*scoreaux)=(*score);
				a=whichIsBestMove(b);
				switch(a){
					case 1:
						moves = moveOnTop(b);
                		sums = sumOnTop(b,score);
                		moveOnTop(b);
                		break;
					case 2:
						moves = moveOnBottom(b);
                		sums = sumOnBottom(b,score);
                		moveOnBottom(b);
						break;
					case 3:
                		moves = moveToLeft(b);
                		sums = sumOnLeft(b,score);
                		moveToLeft(b);
                		break;
					case 4:
						moves = moveToRight(b);
                		sums = sumOnRight(b,score);
                		moveToRight(b);
                		break;
				}
				break;
			case 'u':
				if(checkIfUndone(b,aux) == 0){
					doUndo(b,aux);
					(*score) = (*scoreaux);
					printBoard(b);
					mvprintw(18,33,"Score: %d",(*score));
					controls();
					timer();
					refresh();
					(*exist)=1;
				}
				break;
            }
            if(moves>0 || sums>0){
                addRandomValue(b);
            	printBoard(b);
				mvprintw(18,33,"Score: %d",(*score));
				controls();
				timer();
				refresh();
				if(checkIfWin(b) == 1){
					cond=1;
					(*exist)=0;
					return 2;
				}
			}
			else
				if(checkGameOver(b) == 1){
					cond=0;
					(*exist)=0;
					return 1;
    			}
    }
	return 0;
}

int main(){
	int exist=0, b[4][4], opt=0, ok=1, score=0, a, c, cond;
	char menu_items[4][15]={'\0'};
	int aux[4][4], scoreaux=0;

	init();
	srand(time(NULL));
	menuCreate(&exist,menu_items);
	menuLoop(menu_items,&exist,&opt);
	while(ok==1){
		switch(opt){	//opt=optiune, variabila care retine ce optiune s-a
			case 1:		//ales in momentul apasarii tastei ENTER.
				clear();
				a = startGame(b,&exist,&score,aux,&scoreaux);
				clear();
				if(a == 1){
					cond=0;
					endScreen(&cond,&score);
				}
				else
					if(a == 2){
						cond=1;
						endScreen(&cond,&score);
					}
				clear();
				menuCreate(&exist,menu_items);
				menuLoop(menu_items,&exist,&opt);
				break;
			case 2:
				if(exist==1){
					c = continueGame(b,&exist,&score,aux,&scoreaux);
					clear();
					if(c == 1){
						cond=0;
						endScreen(&cond,&score);
					}
					else
						if(c == 2){
							cond=1;
							endScreen(&cond,&score);
						}
					menuCreate(&exist,menu_items);
					menuLoop(menu_items,&exist,&opt);
				}
				else {
					printHighscores();
					clear();
					menuCreate(&exist,menu_items);
					menuLoop(menu_items,&exist,&opt);
				}
			break;
			case 3:
				if(exist == 1){
					printHighscores();
					clear();
					menuCreate(&exist,menu_items);
					menuLoop(menu_items,&exist,&opt);
				}
				else
					ok=0;
				break;
			case 4:
				ok=0;
				break;
		}
	}
	endwin();
	return 0;
}
