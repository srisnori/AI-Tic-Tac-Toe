#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>
#include <type_traits>
#include "ArrayList.h"
#include "LinkedList.h"

struct Vec{
    int x;
    int y;

    Vec(){
        x = 0;
        y = 0;
    }

    Vec(int x, int y){
        this->x = x;
        this->y = y;
    }

    void set(int x, int y){
        this->x = x;
        this->y = y;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Vec& v){
    os << "(" << v.x << ", " << v.y << ")";

    return os;
}

struct GameState;
std::ostream& operator<<(std::ostream& os, const GameState& state);

struct GameState{
    int** grid;
    bool currentTurn;
    int size;
    int turnCount;

    bool done;
    Vec lastMove;

    GameState(int size = 3){
        this->size = size;
        currentTurn = 0;
        turnCount = 0;
        done = false;

        lastMove.set(-1, -1);

        grid = new int*[size];

        for (int i = 0; i < size; i++){
            grid[i] = new int[size];
            for (int j = 0; j < size; j++){
                grid[i][j] = -1;
            }
        }
    }

    GameState(const GameState& other){
        size = other.size;
        currentTurn = other.currentTurn;
        turnCount = other.turnCount;
        done = other.done;
        lastMove = other.lastMove;

        grid = new int*[size];

        for (int i = 0; i < size; i++){
            grid[i] = new int[size];
            for (int j = 0; j < size; j++){
                grid[i][j] = other.grid[i][j];
            }
        }
    }

    bool operator==(const GameState& other){
        bool sizeMatch = size == other.size;
        bool currentTurnMatch = currentTurn == other.currentTurn;
        bool turnCountMatch = turnCount == other.turnCount;
        bool doneMatch = done == other.done;
        bool lastMoveMatch = lastMove.x == other.lastMove.x && lastMove.y == other.lastMove.y;
        if (sizeMatch && currentTurnMatch && turnCountMatch && doneMatch && lastMoveMatch){

            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    if (grid[i][j] != other.grid[i][j]){
                        return false;
                    }
                }
            }

            return true;
        }
        else{
            return false;
        }
    }

    GameState& operator=(const GameState& other){
        currentTurn = other.currentTurn;
        turnCount = other.turnCount;
        done = other.done;
        lastMove = other.lastMove;

        if (size == other.size){
            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    grid[i][j] = other.grid[i][j];
                }
            }
        }
        else{
            size = other.size;
            for (int i = 0; i < size; i++){
                delete[] grid[i];
            }
            delete[] grid;

            grid = new int*[size];

            for (int i = 0; i < size; i++){
                grid[i] = new int[size];
                for (int j = 0; j < size; j++){
                    grid[i][j] = other.grid[i][j];
                }
            }
        }

        return *this;
    }

    bool hasWon(int player){
        for (int i = 0; i < size; i++){
            bool winRow = true;
            for (int j = 0; j < size; j++){
                if (grid[i][j] != player){
                    winRow = false;
                    break;
                }
            }
            if (winRow){
                return true;
            }
        }
        for (int i = 0; i < size; i++){
            bool winCol = true;
            for (int j = 0; j < size; j++){
                if (grid[j][i] != player){
                    winCol = false;
                    break;
                }
            }
            if (winCol){
                return true;
            }
        }

        bool winDiag = true;
        for (int i = 0; i < size; i++){
            if (grid[i][i] != player){
                winDiag = false;
                break;
            }
        }
        if (winDiag){
            return true;
        }
        
        bool winAntiDiag = true;
        for (int i = 0; i < size; i++){
            if (grid[i][size-1-i] != player){
                winAntiDiag = false;
                break;
            }
        }
        if (winAntiDiag){
            return true;
        }

        return false;
    }


    bool play(int x, int y){
        if (grid[x][y] != -1){
            return false;
        }

        grid[x][y] = currentTurn;
        currentTurn = !currentTurn;
        turnCount++;
        lastMove.set(x, y);

        if (turnCount == size * size){
            done = true;
        }
        else if (hasWon(0) || hasWon(1)){
            done = true;
        }

        return true;
    }

    ~GameState(){
        for (int i = 0; i < size; i++){
            delete [] grid[i];
        }
        delete [] grid;
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////
    int scoreFinder(GameState game){
        if (game.hasWon(1)){
            return 10;
        }
        if (game.hasWon(0)){
            return -10;
        }
        return 0;
    }
    
    int evaluate(GameState game, int depth, bool maximize){
        int get_score = scoreFinder(game);
        int limit_depth;
        if (size == 3){
            limit_depth = 9;
        }
        if (size == 4){
            limit_depth = 4;
        }
        if (size == 5){
            limit_depth = 2;
        }

        if (game.done || get_score != 0 || depth >= limit_depth){
            return get_score - depth;
        }
        int highest_score;

        if (maximize == true){ 
            highest_score = -1000;
            for (int i = 0; i < game.size; i++){
                for (int j = 0; j < game.size; j++){
                    if (game.grid[i][j] == -1){
                        game.grid[i][j] = 1;
                        int score = evaluate(game, depth + 1, false); 
                        if (highest_score < score){
                            highest_score = score;
                        }
                        game.grid[i][j] = -1;
                    }
                }
            }
            return highest_score;
        }
    

        else{
            highest_score = 1000;
            for (int i = 0; i < game.size; i++){
                for (int j = 0; j < game.size; j++){
                    if (game.grid[i][j] == -1){
                        game.grid[i][j] = 0;
                        int score = evaluate(game, depth + 1, true);
                        if (score < highest_score){
                            highest_score = score;
                        }
                        game.grid[i][j] = -1;
                    }
                }
            }
            return highest_score;
        }
    }

    Vec terrible_ai(int size){
        Vec move;
        while (true){
            int ran_x = rand() % (size);
            int ran_y = rand() % (size);
            if (grid[ran_x][ran_y] == -1){
                move.set(ran_x, ran_y);
                break;
            }
        }
        return Vec();
    }

    Vec average_ai(){
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                if (grid[i][j] == -1){
                    grid[i][j] = 1;
                    currentTurn = 0;
                    turnCount++;
                    lastMove.set(i, j);
                    return lastMove;
                }
                
            }
        }
        
        return Vec();
    }

    Vec unbeatable_ai(GameState& game){
        int bestScore = -1000;
        Vec move;
        for (int i = 0; i < game.size; i++){
            for (int j = 0; j < game.size; j++){
                if (game.grid[i][j] == -1){
                    game.grid[i][j] = 0;
                    if (game.hasWon(1)){
                        move.set(i, j);
                        game.grid[i][j] = -1;
                        return move;
                    }
                    game.grid[i][j] = -1;
                }
            }
        }

        
        for (int i = 0; i < game.size; i++){
            for (int j = 0; j < game.size; j++){
                if (game.grid[i][j] == -1){
                        game.grid[i][j] = 1;
                    if (game.hasWon(0)){
                        move.set(i, j);
                        game.grid[i][j] = -1;
                        return move;
                    }
                    game.grid[i][j] = -1;
                }
            }
        }
        

        for (int i = 0; i < game.size; i++){
            for (int j = 0; j < game.size; j++){
                if (game.grid[i][j] == -1){
                    game.grid[i][j] = 0;
                    int score = evaluate(game, 0, false);
                    game.grid[i][j] = -1;
                    if (score > bestScore){
                        bestScore = score;
                        move.set(i, j);
                    }
                }
            }
        }
        return move;
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::ostream& operator<<(std::ostream& os, const GameState& state){
    os << "   ";
    for (int j = 0; j < state.size; j++){
        os << " " << j << "  ";
    }
    os << std::endl;
    os << "   ";
    for (int j = 0; j < state.size; j++){
        os << "--- ";
    }
    os << std::endl;
    for (int i = 0; i < state.size; i++){
        os << i << " ";
        for (int j = 0; j < state.size; j++){
            char c = ' ';
            if (state.grid[i][j] == 0){
                c = 'X';
            }
            else if (state.grid[i][j] == 1){
                c = 'O';
            }
            os << "| " << c << " ";
            if (j == state.size - 1) os << "|";
        }
        os << std::endl << "   ";
        for (int j = 0; j < state.size; j++){
            os << "--- ";
        }
        os << std::endl;
    }

    return os;
}

#endif