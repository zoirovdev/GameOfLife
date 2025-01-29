#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define WIDTH 100
#define HEIGHT 50

#define BACKGROUND '-'
#define CELL '#'
#define ALMOST_DEAD '*'

#define SPEED 250


typedef enum {
	DEAD,
	ALIVE,
	DYING
} State;

typedef struct {
	State state;
} Cell;

State gol[2][9] = {
	{DEAD, DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
	{DEAD, DEAD, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD},
};

State seeds[2][9] = {
	{DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
	{DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
};

State brain[3][9] = {
	{DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
	{DYING, DYING, DYING, DYING, DYING, DYING, DYING, DYING, DYING},
	{DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD}
};


Cell grid[HEIGHT][WIDTH] = {0};

void init_grid(int random){
	for(size_t i=0;i<HEIGHT;i++){
		for(size_t j=0;j<WIDTH;j++){
			if(rand()%2 == 0 && random == 1){
				grid[i][j].state = ALIVE;
				continue;
			}
			grid[i][j].state=DEAD;
		}
	}
}


void gen_next(State automaton[2][9]){
	Cell new_grid[HEIGHT][WIDTH] = {0};
	memcpy(new_grid, grid, sizeof(Cell) * HEIGHT * WIDTH);
	for(size_t i=0;i<HEIGHT;i++){
		for(size_t j=0;j<WIDTH;j++){
			int alive_count = 0;
			for(int k=-1; k<=1; k++){
				for(int l=-1; l<=1; l++){
					if(k==0 && l==0) continue;
					int row = (i+k+HEIGHT) % HEIGHT;
					int col = (j+l+WIDTH) % WIDTH;
					if(grid[row][col].state == ALIVE){
						alive_count++;
					}
				}
			}
			new_grid[i][j].state = automaton[grid[i][j].state][alive_count];
		}
	}
	memcpy(new_grid, grid, sizeof(Cell) * HEIGHT * WIDTH);
}


int print_grid(){
	int alive_count = 0;
	for(size_t i=0;i<HEIGHT;i++){
		for(size_t j=0;j<WIDTH;j++){
			switch(grid[i][j].state){
				case ALIVE:
					alive_count++;
					printf("%c", CELL);
					break;
				case DEAD:
					printf("%c", BACKGROUND);
					break;
				case DYING:
					printf("%c", ALMOST_DEAD);
			}
		}
		printf("\n");
	}

	return alive_count;
}


void init_glider(size_t offset){
	grid[offset+0][offset+1].state = ALIVE;
	grid[offset+1][offset+2].state = ALIVE;
	grid[offset+2][offset+0].state = ALIVE;
	grid[offset+2][offset+1].state = ALIVE;
	grid[offset+2][offset+2].state = ALIVE;
}

void init_oscillator(size_t offset){
	grid[offset/2+0][5].state = ALIVE;
	grid[offset/2+0][6].state = ALIVE;
	grid[offset/2+1][5].state = ALIVE;
	grid[offset/2+1][6].state = ALIVE;
	grid[offset/2+1][4].state = DYING;
	grid[offset/2+0][7].state = DYING;
	grid[offset/2-1][5].state = DYING;
	grid[offset/2+2][6].state = DYING;
}

int main(int argc, char *argv[]){
	char *program = argv[0];
	srand(time(NULL));
	system("clear");
	init_grid(0);
	init_oscillator(5);
	while(print_grid() != 0){
		usleep(SPEED*1000);
		gen_next(brain);
		system("clear");
	}
	return 0;
	if(argc < 2 || strcmp(argv[1], "default")==0){
	}else if(strcmp(argv[1], "glider") == 0){
		init_grid(0);
		init_glider(0);
		while(print_grid() != 0){
			usleep(SPEED*1000);
			gen_next(gol);
			system("clear");
		}
	}else if(strcmp(argv[1], "both") == 0){
		init_grid(1);
		init_glider(0);
		while(print_grid() != 0){
			usleep(SPEED*1000);
			gen_next(gol);
			system("clear");
		}
	}else{
		fprintf(stderr, "usage: %s <default || glider || both>\n", program);
		exit(1);
	}
	return 0;
}
