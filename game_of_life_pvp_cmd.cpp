#include <cstdio>
#include <cstdlib>
//#include <conio.h>
//#include <unistd.h>
#include <time.h>

const char *
init_cells = 
".........."
".......#.."
"......#..."
"......###."
".........."
".........."
".........."
".........."
".........."
"..........";

const int n_cells_x = 80;
const int n_cells_y = 20;
const int n_cells = n_cells_x * n_cells_y;

char DEAD='.';
char ALIVE='#';

char arr1[n_cells + 1];
char arr2[n_cells + 1];

char *cells = arr1;
char *tmp = arr2;

int count_neighbours(int x, int y) {
    int cnt = 0;
    for(int i = x - 1; i <= x + 1; ++i) {
        for (int j = y - 1; j <= y + 1; ++j) {
            cnt += ((i == x && j == y) || i < 0 || j < 0 || i >= n_cells_x || j >= n_cells_y) ? 0 : cells[j*n_cells_x + i] == ALIVE;
        }
    }
    return cnt;
}

void step() {
    for (int y = 0; y < n_cells_y; ++y) {
        for (int x = 0; x < n_cells_x; ++x) {
            int neighbours = count_neighbours(x, y);
            // printf("%d, ", neighbours);
            if (cells[y*n_cells_x + x] == DEAD) {
                tmp[y*n_cells_x + x] = (neighbours == 3) ? ALIVE : DEAD;
            } else {
                tmp[y*n_cells_x + x] = (neighbours == 2 || neighbours == 3) ? ALIVE : DEAD;
            }
        }
        // puts("");
    }
    // puts("");
    char *swapper = cells;
    cells = tmp;
    tmp = swapper;
}

int main() {
    srand(time(0));
    const int steps_per_sec = 30;
    const int time_between_steps = (int)(1000000.0/steps_per_sec);
    for (int i = 0; i < n_cells; ++i) {
        cells[i] = rand() % 10 > 2 ? DEAD : ALIVE;
        // cells[i] = init_cells[i];
    }
    char output[(n_cells_x + 3) * (n_cells_y + 1)];
    for (int i = 0; i < (n_cells_x + 3) * (n_cells_y + 1); ++i){
        output[i] = 'E';
    }
    output[(n_cells_x + 3) * n_cells_y + 0] = '*';
    output[(n_cells_x + 3) * n_cells_y + n_cells_x + 1] = '*';
    for (int ox = 1; ox < n_cells_x + 1; ++ox) {
        output[(n_cells_x + 3) * n_cells_y + ox] = '-';
    }
    for (int oy = 0; oy < n_cells_y; ++oy) {
        output[oy * (n_cells_x + 3) + 0] = '|';
        output[oy * (n_cells_x + 3) + (n_cells_x + 1)] = '|';
        output[oy * (n_cells_x + 3) + (n_cells_x + 2)] = '\n';
    }
    output[(n_cells_x + 3) * (n_cells_y + 1) - 1] = '\0';
    for (;;) {
        for(int x = 0; x < n_cells_x; ++x) {
            for (int y = 0; y < n_cells_y; ++y) {
                output[y*(n_cells_x+3)+(x+1)] = cells[y * n_cells_x + x];
            }
        }
        system("CLS");
        puts(output);
        /*
        while(!kbhit()) {
          usleep(100);
        }
        */
        getchar();
        //usleep(time_between_steps);
        step();
    }
    return 0;
}