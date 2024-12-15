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
                // cout << "Do you want to play against human(1) to AI(0). Choose 1 or 0: " << endl;
                // cin >> human_or_ai;
                cout << "Choose difficulty level, Easy: 1, Medium: 2 and Impossible: 3 --> ";
                cin >> difficulty_level;
                break;
                
                // if(human_or_ai == 0) { 
                //     cout << "Choose difficulty level, Easy: 1, Medium: 2 and Impossible: 3 --> ";
                //     cin >> difficulty_level;
                // }
                
                // else if(human_or_ai!=1&&human_or_ai != 0) {
                //     cout<<"Invalid output, try again"<<endl;
                //     cin>>human_or_ai;
                // }
            cout << "Wrong, try again" << endl; 
            }
        }
        system("clear");
        

        GameState game(grid_size);
        cout<<game<<endl;

        bool player_settings = true;
        string player;
 
        while (!game.done){
            system("clear");
            cout << game << endl;

            int x, y;
            // if (human_or_ai == 0){
            //     if (player_settings){
            //         cout << "Who you like to play first? (H or AI)" << endl;
            //         cin >> player;

            //         if (player == "AI" || player == "ai"){
            //             game.currentTurn = 1;
            //         }
            //         player_settings = false;
            //     }
            // }

            if (game.currentTurn == 0){
                cout << "Enter coordinates (" << (game.currentTurn ? "O" : "X") << "): ";
                cin >> x >> y;
                // if(x<0||x>=size||){
                if (!game.play(x, y)){
                    cout << "try again" << endl;
                    continue;
                }
            }

            // else if (human_or_ai == 1){
            //     game.play(x, y);
            //     cout << "Enter coordinates (" << (game.currentTurn ? "O" : "X") << "): ";
            //     cin >> x >> y;
            //     if (!game.play(x, y)){
            //         cout << "try again" << endl;
            //         continue;
            //     }
            // }

            if (difficulty_level == 1){
                Vec easy = game.terrible_ai(grid_size);
                cout << "AI plays now" << endl;
                game.play(easy.x, easy.y);
            }
            else if (difficulty_level == 2){
                Vec medium = game.average_ai();
                cout << "AI plays now" << endl;
                game.play(medium.x, medium.y);
            }

            else if(difficulty_level==3){
                Vec impossible = game.unbeatable_ai(game);
                cout << "AI plays now" << endl;
                game.play(impossible.x, impossible.y);
            }
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

    
    

