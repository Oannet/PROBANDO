#include <stdlib.h>
#include <stdio.h>
#include "connect4.h"


void connect4_init(connect4* game, int rows, int columns, int num_of_players, int win_size) {
    game->rows = rows;
    game->columns = columns;
    game->num_of_players = num_of_players;
    game->win_size = win_size;

    game->board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        game->board[i] = (int*)malloc(columns * sizeof(int));
        for (int j = 0; j < columns; j++) {
            game->board[i][j] = 0; 
        }
    }
}

void connect4_free(connect4* game) {
    for (int i = 0; i < game->rows; i++) {
        free(game->board[i]); 
    }
    free(game->board); 
    game->board = NULL; 
}

char* connect4_to_string(connect4* game) {
    int bufferSize = game->rows * (game->columns + 1) + 1; 
    char* buffer = (char*)malloc(bufferSize);
    char* ptr = buffer;

    for (int i = 0; i < game->rows; i++) {
        for (int j = 0; j < game->columns; j++) {
            *ptr++ = '0' + game->board[i][j]; 
        }
        if (i < game->rows - 1) {
            *ptr++ = '\n'; 
        }
    }
    *ptr = '\0'; 

    return buffer;
}


int connect4_make_play(connect4* game, int player, int column) {
    if (column < 0 || column >= game->columns) return INVALID_COLUMN;
    if (player < PLAYER_1 || player > game->num_of_players) return INVALID_PLAYER;

    for (int row = game->rows - 1; row >= 0; --row) {
        if (game->board[row][column] == 0) {
            game->board[row][column] = player;
            return game->rows - row - 1; 
        }
    }
    return FULL_COLUMN;
}

bool check_direction(connect4* game, int player, int startRow, int startCol, int dRow, int dCol) {
    int count = 0;
    for (int step = 0; step < game->win_size; ++step) {
        int newRow = startRow + step * dRow;
        int newCol = startCol + step * dCol;
        if (newRow < 0 || newRow >= game->rows || newCol < 0 || newCol >= game->columns || game->board[newRow][newCol] != player) {
            return false;
        }
        count++;
    }
    return count == game->win_size;
}


bool connect4_player_won(connect4* game, int player, int lastRow, int lastColumn) {
    const int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    for (int i = 0; i < 4; ++i) {
        int dRow = directions[i][0];
        int dCol = directions[i][1];
        for (int step = 0; step < game->win_size; ++step) {
            int startRow = lastRow - step * dRow;
            int startCol = lastColumn - step * dCol;
            if (startRow >= 0 && startCol >= 0 && check_direction(game, player, startRow, startCol, dRow, dCol)) {
                return true;
            }
        }
    }
    return false;
}


