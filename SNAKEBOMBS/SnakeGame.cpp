#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MAX 100

typedef struct {
    int x, y;
} Ponto;

Ponto cobra[MAX];
Ponto comida;
Ponto bombas[MAX];
int tam;
int dir;
int score;
int gameOver; // Variável para controlar o estado do jogo
int hardMode; // Variável para controlar o modo de jogo

void drawMenu() {
    cleardevice();
    setcolor(WHITE);
    char snakeGame[] = "SNAKE VS BOMBS!";
    char normalGame[] = "1. Normal Game";
    char hardGame[] = "2. Hard Game";
    char quitGame[] = "3. Quit Game";
    char tutorialLine1[] = "WASD para se mover - ";
    char tutorialLine2[] = "Os quadrados brancos sao comida que te fazem crescer. ";
    char tutorialLine3[] = "Fuja dos quadrados roxos, são bombas que podem custar sua vida!";
    outtextxy(getmaxx() / 2 - 60, getmaxy() / 5 - 30, snakeGame);
    outtextxy(getmaxx() / 2 - 50, getmaxy() / 2 - 50, normalGame);
    outtextxy(getmaxx() / 2 - 50, getmaxy() / 2 - 30, hardGame);
    outtextxy(getmaxx() / 2 - 50, getmaxy() / 2 - 10, quitGame);
    outtextxy(getmaxx() / 2 - 110, getmaxy() / 2 + 30, tutorialLine1);
    outtextxy(getmaxx() / 2 - 200, getmaxy() / 2 + 50, tutorialLine2);
    outtextxy(getmaxx() / 2 - 250, getmaxy() / 2 + 70, tutorialLine3);
}

int getMenuChoice() {
    char choice;
    while (1) {
        if (kbhit()) {
            choice = getch();
            if (choice == '1' || choice == '2' || choice == '3') {
                return choice - '0';
            }
        }
    }
}

void inicializa() {
    tam = 5;
    dir = 3;
    score = 0;
    gameOver = 0; // Inicializa o estado do jogo como não acabado
    cobra[0].x = getmaxx() / 2;
    cobra[0].y = getmaxy() / 2;
    for (int i = 1; i < tam; i++) {
        cobra[i].x = cobra[i - 1].x + 10;
        cobra[i].y = cobra[i - 1].y;
    }
    comida.x = 20 + rand() % (getmaxx() - 60);
    comida.y = 50 + rand() % (getmaxy() - 90);
    for (int i = 0; i < MAX; i++) {
        bombas[i].x = -1;
        bombas[i].y = -1;
    }
}

void gerarComida() {
    comida.x = 20 + rand() % (getmaxx() - 60);
    comida.y = 50 + rand() % (getmaxy() - 90);
}

void gerarBomba() {
    int index = 0;
    for (int i = 0; i < MAX; i++) {
        if (bombas[i].x == -1 && bombas[i].y == -1) {
            index = i;
            break;
        }
    }
    bombas[index].x = 20 + rand() % (getmaxx() - 40);
    bombas[index].y = 20 + rand() % (getmaxy() - 40);
}

void desenha() {
    cleardevice();
    setcolor(WHITE);
    rectangle(10, 10, getmaxx() - 10, getmaxy() - 10);
    setfillstyle(SOLID_FILL, RED);
    for (int i = 0; i < tam; i++) {
        bar(cobra[i].x, cobra[i].y, cobra[i].x + 10, cobra[i].y + 10);
    }
    setfillstyle(SOLID_FILL, WHITE);
    bar(comida.x, comida.y, comida.x + 10, comida.y + 10);
    for (int i = 0; i < MAX; i++) {
        if (bombas[i].x != -1 && bombas[i].y != -1) {
            setfillstyle(SOLID_FILL, MAGENTA);
            bar(bombas[i].x, bombas[i].y, bombas[i].x + 10, bombas[i].y + 10);
            line(bombas[i].x + 2, bombas[i].y + 2, bombas[i].x + 8, bombas[i].y + 8);
            line(bombas[i].x + 2, bombas[i].y + 8, bombas[i].x + 8, bombas[i].y + 2);
        }
    }
    char* score_text = (char*)malloc(20 * sizeof(char));
    sprintf(score_text, "Score: %d", score);
    outtextxy(20, 15, score_text);
    free(score_text);
    char gameMessage[] = "Aperte 'p' para pausar ou 'r' para recomeçar";
    outtextxy(20, 30, gameMessage);
    line(0, 50, getmaxx(), 50);
}

void atualiza() {
    if (gameOver) return; // Se o jogo acabou, não atualize

    for (int i = tam - 1; i > 0; i--) {
        cobra[i] = cobra[i - 1];
    }
    switch (dir) {
        case 1:
            cobra[0].y -= 10;
            break;
        case 2:
            cobra[0].y += 10;
            break;
        case 3:
            cobra[0].x -= 10;
            break;
        case 4:
            cobra[0].x += 10;
            break;
        default:
            break;
    }

// Verificar score negativo
	if (score < 0) {
    	gameOver = 1;
    	char gameOverMessage[] = "Game Over. Deseja jogar novamente? (s/n)";
    	outtextxy(getmaxx() / 2 - 120, getmaxy() / 2, gameOverMessage);
    	while (1) {
        	if (kbhit()) {
            	char resposta = getch();
            	if (resposta == 's' || resposta == 'S') {
                	inicializa(); // Reiniciar o jogo
                	break;
            	} else if (resposta == 'n' || resposta == 'N') {
                	drawMenu(); // Voltar para o menu principal
                	break;
            	}
        	}
    	}
	}
	
    if (abs(cobra[0].x - comida.x) < 10 && abs(cobra[0].y - comida.y) < 10) {
        score += 10;
        gerarComida();
        tam++;
        if (hardMode || (score % 20 == 0)) { // Adiciona bomba a cada comida no modo hard ou a cada 20 pontos no modo normal
            gerarBomba();
        }
    }

    for (int i = 0; i < MAX; i++) {
        if (bombas[i].x != -1 && bombas[i].y != -1) {
            if (abs(cobra[0].x - bombas[i].x) < 10 && abs(cobra[0].y - bombas[i].y) < 10) {
                if (hardMode) {
                    gameOver = 1; // No modo hard, colidir com a bomba encerra o jogo
                } else {
                    score -= 30;
                    tam -= 3; // No modo normal, reduz o tamanho da cobra em 3 unidades
                    bombas[i].x = -1;
                    bombas[i].y = -1;
                }
            }
        }
    }
}

int colisao() {
    if (cobra[0].x <= 0 || cobra[0].x >= getmaxx() - 10 || cobra[0].y <= 40 || cobra[0].y >= getmaxy() - 10)
        return 1;

    for (int i = 1; i < tam; i++) {
        if (cobra[0].x == cobra[i].x && cobra[0].y == cobra[i].y)
            return 1;
    }

    return 0;
}

int main() {
    int gd = DETECT, gm;
    char* empty_string = (char*)malloc(1 * sizeof(char));
    empty_string[0] = '\0';
    initgraph(&gd, &gm, empty_string);
    free(empty_string);
    srand(time(NULL));
    int choice;
    char gameOverMessage[] = "Game Over. Deseja jogar novamente? (s/n)";
    char pauseMessage[] = "Jogo pausado. Aperte qualquer tecla para continuar";
    while (1) {
        drawMenu();
        choice = getMenuChoice();
        switch (choice) {
			case 1:  // Normal game
    			inicializa();
    			hardMode = 0; // Desativa o modo hard
    			gameOver = 0;
    			while (!gameOver) {
        			desenha();
        			if (kbhit()) {
            			switch (getch()) {
                			case 'w':
                   			 dir = 1;
                    		 break;
                			case 's':
                    dir = 2;
                    break;
                case 'a':
                    dir = 3;
                    break;
                case 'd':
                    dir = 4;
                    break;
                case 'p':  // Pause game
                    outtextxy(getmaxx() / 2 - 50, getmaxy() / 2, pauseMessage);
                    getch();
                    break;
                case 'r':  // Restart game
                    inicializa();
                    break;
            }
        }
        atualiza();
        if (colisao()) {
            gameOver = 1;
            outtextxy(getmaxx() / 2 - 120, getmaxy() / 2, gameOverMessage);
            while (1) {
                if (kbhit()) {
                    char resposta = getch();
                    if (resposta == 's' || resposta == 'S') {
                        break;
                    } else if (resposta == 'n' || resposta == 'N') {
                        drawMenu(); // Voltar para o menu principal
                        break;
                    }
                }
            }
        }
        delay(100);
    }
    break;
			case 2:  // Hard game
    			inicializa();
    			hardMode = 1; // Ativa o modo hard
    			gameOver = 0;
    			while (!gameOver) {
        			desenha();
        			if (kbhit()) {
            			switch (getch()) {
                			case 'w':
                    			dir = 1;
                    			break;
                			case 's':
                    			dir = 2;
                    			break;
                			case 'a':
                    			dir = 3;
                    			break;
                			case 'd':
                    			dir = 4;
                    			break;
                			case 'p':  // Pause game
                    			outtextxy(getmaxx() / 2 - 50, getmaxy() / 2, pauseMessage);
                    			getch();
                    			break;
               				case 'r':  // Restart game
                    			inicializa();
                    			break;
            }
        }
        atualiza();
        if (colisao()) {
            gameOver = 1;
            outtextxy(getmaxx() / 2 - 120, getmaxy() / 2, gameOverMessage);
            while (1) {
                if (kbhit()) {
                    char resposta = getch();
                    if (resposta == 's' || resposta == 'S') {
                        break;
                    } else if (resposta == 'n' || resposta == 'N') {
                        drawMenu(); // Voltar para o menu principal
                        break;
                    }
                }
            }
        }
        delay(100);
    }
    break;

            case 3:  // Quit game
                closegraph();
                printf("Game Over! Score: %d\n", score);
                return 0;
        }
    }
}

