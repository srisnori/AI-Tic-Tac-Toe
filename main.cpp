#include <iostream>
#include "GameState.h"


using namespace std;

int main(){

    srand(time(0));
    int grid_size;
    int difficulty_level;
    int human_or_ai;

    while (true){
        while (true){
            cout << "Choose board size 3, 4, 5: "; 
            cin >> grid_size; 
            if (grid_size == 3 || grid_size == 4 || grid_size == 5){
                cout << "Choose difficulty level, Easy: 1 and Impossible: 2 --> ";
                cin >> difficulty_level;
                break;
            }
            cout << "Wrong, try again" << endl; 
            
        }

        system("clear");
        
        GameState game(grid_size);
        cout << game << endl;

        bool player_settings = true;
        int player;
 
        while (!game.done){
            system("clear");
            cout << game << endl;

            if (player_settings){
                cout << "Who you like to play first Human(0) or AI(1)" << endl;
                cin >> player;

                if (player == 1){
                    game.currentTurn = 1;
                }
                else if (player == 0){
                    game.currentTurn = 0;
                }
                else{
                    cout << "Invalid, try again" << endl;
                    cout << "Who you like to play first Human(0) or AI(1)" << endl;
                    cin >> player;
                }
                player_settings = false;
            }

            int x, y;
            if (game.currentTurn == 0){
                cout << "Enter coordinates (" << (game.currentTurn ? "O" : "X") << "): ";
                cin >> x >> y;
                if(x < 0|| x >= grid_size || y < 0 || y >= grid_size || !game.play(x, y)){
                    // if (!game.play(x, y)){
                        cout << "try again" << endl;
                        continue;
                }
            }
            if (difficulty_level == 1){
                Vec medium = game.average_ai();
                cout << "AI plays now" << endl;
                game.play(medium.x, medium.y);
            }

            else if(difficulty_level == 2){
                Vec impossible = game.unbeatable_ai(game);
                cout << "AI plays now" << endl;
                game.play(impossible.x, impossible.y);
            }
            // else if (difficulty_level != 1 || difficulty_level != 2){
            //     cout << "Invalid, try again" << endl;
            //     cin >> difficulty_level;
            // }
        }

        system("clear");
        cout << game << endl;

        if (game.hasWon(0)){
            cout << "X wins" << endl;
        }
        else if (game.hasWon(1)){
            cout << "O wins" << endl;
        }
        else {
            cout << "It's a tie" << endl;
        }
        cout << endl;

        
        char continue_game;
        cout << "Want to play again? (y/n): " << endl;
        cin >> continue_game;
        if (continue_game == 'y' || continue_game == 'Y'){
            continue;
        }
        else if (continue_game == 'n' || continue_game == 'N'){
            break;
        }
        else{
            cout << "Invalid input. Try again " << endl;
            cin >> continue_game;
        }
    }
    return 0;
}