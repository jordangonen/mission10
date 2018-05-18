//
//  main.cpp
//  mission10final
//
//  Created by Jordan Gonen and Kenny Breslow on 5/5/18.
//  Copyright © 2018 Jordan Gonen and Kenny Breslow. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <list>
#include <vector>
#include <ctime>


using namespace std;

// random number generator (takes in mod value)
int getRand(int mod){
    int randomNumber = rand() % mod;
    return randomNumber;
}
// obstacles class
class Obstacles {
public:
    string name;
    int value;
    int xCoordinate;
    int yCoordinate;
    char symbol;
    // define obstacle
    void createObstacle(string n, int v, int x, int y, char s) {
        xCoordinate = x;
        yCoordinate = y;
        name = n;
        value = v;
        symbol = s;
        
    }
    // add obstacle to obstacle vector
    void addObstacleToVector(vector<Obstacles> &allObstacles, Obstacles &obstacle) {
        allObstacles.push_back(obstacle);
    }
    
    
};

// compettiors class
class Competitors {
public:
    string name;
    vector<Obstacles> points;
    int xCoordinate;
    int yCoordinate;
    char symbol;
    int numBananas;
    
    
    // add player to all competitors vector
    void addPlayerToVector(vector<Competitors> &players, Competitors player) {
        players.push_back(player);
    }
    
    // make sure the move is valid
    int moveValid(char move, Competitors &player) {
        // bounds
        int maxCol = 14;
        int maxRow = 14;
        int minRow = 0;
        int minCol = 0;
        

        // invalid keypress
        if( move!='w' && move!='a' && move!='s' && move!='d') {
            return 0;
        } else {
            // out of bounds
            if(move == 'w') {
                if(player.xCoordinate-1<minRow){
                    return 1;
                }
            }
            // out of bounds
            if(move == 's') {
                if(player.xCoordinate+1>maxRow) {
                    return 1;
                }
            }
            // out of bounds
            if(move == 'a') {
                if(player.yCoordinate-1<minCol){
                return 1;
                }
            }
            // crossing finish line
            if(move == 'd') {
                if(player.yCoordinate+1>maxCol){
                    return 2;
            }
        }
            return 3;
        }
        
    }
    
    // actually make the move
    void move(vector<Obstacles> &obstacles, Competitors &player, char array[15][15], char direction) {
        int newRow = 0, newCol = 0 , oldRow = 0, oldCol = 0;
        
        oldRow = player.xCoordinate;
        oldCol = player.yCoordinate;
        
        if(direction == 'w') {
            newRow = player.xCoordinate-1;
            newCol = oldCol;
        }
        if(direction =='s') {
            newRow = player.xCoordinate+1;
            newCol = oldCol;
            
        }
        if(direction == 'a') {
            newRow = oldRow;
            newCol = player.yCoordinate-1;
            
        }
        if(direction == 'd') {
           newRow = oldRow;
           newCol = player.yCoordinate+1;
           
        }
        

        // calculate if hit obstacle
        // calculate overlap
        
        for(int i=0; i<obstacles.size(); i++) {
            if((obstacles.at(i).xCoordinate == newRow) &&
               (obstacles.at(i).yCoordinate == newCol)) {
                player.points.push_back(obstacles.at(i));
                obstacles.at(i).xCoordinate = -1;
                obstacles.at(i).yCoordinate = -1;
            }
        }
        // make the move
        array[oldRow][oldCol] = 'O';
        array[newRow][newCol] = player.symbol;
        player.xCoordinate = newRow;
        player.yCoordinate = newCol;
        
    }
    
    // total points calculation
    int calculateTotalPoints(Competitors &player, bool finish) {
        int totalSum = 0;
        for(int i=0; i<player.points.size(); i++) {
            totalSum += player.points.at(i).value;
            if(player.points.at(i).symbol == 'B') {
                player.numBananas = player.numBananas + 1;
            }
        }
        // if crossed finish line give a 5 point bonus
        if(finish == true && player.symbol == 'D') {
            totalSum += 5;
        }
        return totalSum;
    }
    
    // instantiate player
    void definePlayer(string n, int x, int y, char s, vector<Obstacles> totalPoints, int b) {
        name = n;
        xCoordinate = x;
        yCoordinate = y;
        symbol = s;
        points = totalPoints;
        numBananas = b;
    }
    

};
// check if macho / chicken killed dave
int checkOverlap(Competitors dave, Competitors macho, Competitors chicken) {
    int daveRow = dave.xCoordinate;
    int daveCol = dave.yCoordinate;
    
    if(daveRow == chicken.xCoordinate && daveCol == chicken.yCoordinate) {
        return 1;
    } else
    if(daveRow == macho.xCoordinate && daveCol == macho.yCoordinate) {
        return 2;
    }
    return 3;
}
// create all obstacles and give them coordinates (random chance per obstacle)
void createAllObstacles(char array[15][15], vector<Obstacles> &allObstacles) {
    // create all the obstacles and assign them coordinates, place them into vector all obstacles
    // do not let them start at coordinate 0, 0 or any around them.
    for(int i=0; i<15; i++) {
        for(int j=0; j<15; j++) {
            if(i==0 && j==0) {
                continue;
            }
            int rand = getRand(100);
            if(rand > 0 && rand < 65) {
                Obstacles empty;
                empty.createObstacle("empty", 0, i, j, 'O');
                empty.addObstacleToVector(allObstacles, empty);
            } else
            if(rand > 65 && rand <90) {
                // place banana
                Obstacles banana;
                banana.createObstacle("banana", 1, i, j, 'B');
                banana.addObstacleToVector(allObstacles, banana);
            } else
            if(rand > 90 && rand <96) {
                // place trash can
                Obstacles trashCan;
                trashCan.createObstacle("trash", -1, i, j, 'T');
                trashCan.addObstacleToVector(allObstacles, trashCan);
            } else
            if(rand > 96 && rand <99) {
                // place old lady
                Obstacles oldLady;
                oldLady.createObstacle("oldLady", -2, i, j, 'L');
                oldLady.addObstacleToVector(allObstacles, oldLady);
            }
        }
    }
}
// actually distribute the players/obstacles
void fillInBoard(vector<Competitors> &competitors, vector<Obstacles> &obstacles, char array[15][15]){
    int x, y;
    char symbol;
//    cout<<obstacles.size()<<endl;
    for(int i=0; i<obstacles.size(); i++) {
        x = obstacles.at(i).xCoordinate;
        y = obstacles.at(i).yCoordinate;
//        cout<<obstacles.at(i).symbol<<endl;
        symbol = obstacles.at(i).symbol;
        array[x][y] = symbol;
    }
    for(int i=0; i<competitors.size(); i++) {
        x = competitors.at(i).xCoordinate;
        y = competitors.at(i).yCoordinate;
        symbol = competitors.at(i).symbol;
        array[x][y] = symbol;
    }
}



// print out the board
void printBase(char array[15][15]) {
    for(int i=0; i<15; i++) {
        for(int j=0; j<15; j++) {
            cout<<array[i][j]<<" ";
        }
        cout<<endl;
    }
}


int main() {
    srand(time(0));
    

    
    // create empty board
    char plaza[15][15] = {{}};
    for(int i=0;i<15;i++) {
        for(int j=0;j<15;j++) {
            plaza[i][j] =  'O';
        }
    }
    // vectors
    vector<Competitors> allCompetitors;
    vector<Obstacles> allObstacles;

    vector<Obstacles> davePoints;
    vector<Obstacles> machoPoints;
    vector<Obstacles> chickenPoints;
    int daveB =0 , machoB = 0, chickenB = 0;
    // instantiate competitors
    Competitors dave, macho, chicken;
    // define dave and add him to vector
    dave.definePlayer("Dave & Stewart", 0, 0, 'D', davePoints, daveB);
    dave.addPlayerToVector(allCompetitors, dave);
    // define macho and add him to vector

    macho.definePlayer("Macho", 0, 14, 'M', machoPoints, machoB);
    macho.addPlayerToVector(allCompetitors, macho);
    // define chicken and add him to vector

    chicken.definePlayer("Crazy Chicken", 14, 13, 'C', chickenPoints, chickenB);
    chicken.addPlayerToVector(allCompetitors, chicken);

    // create all obstacles (assign their coordinates)
    createAllObstacles(plaza, allObstacles);
    // fill in board with BOTH players and obstacles
    fillInBoard(allCompetitors, allObstacles, plaza);
    
    cout<<"The timer has started! You have 15 seconds!" << endl << flush;

    // print board
    printBase(plaza);

    
    
    // implement timer (can adjust here)
    int seconds = 15;
    time_t start_time = time(nullptr);
    bool crossedFinishLine = false;
       while((time(nullptr) - start_time)<seconds) {
        char playerMove;
        // prompt direction
        cout<<"Which direction would you like to move? (w, a, s, d)"<<endl;
        cin>>playerMove;
        int val;
           // make sure move is valid
        val = dave.moveValid(playerMove, dave);
            if(val==0) {
                cout<<"Please enter a w, a, s or d"<<endl;
            } else
            if(val==1) {
                cout<<"Dave went out of bounds, try again."<<endl;
                // crossed finish line
            } else if(dave.moveValid(playerMove, dave)==2) {
                cout<<"Dave crossed the finish line!"<<endl;
                crossedFinishLine = true;
                cout<<"Dave's Final Score is "<<dave.calculateTotalPoints(dave, crossedFinishLine)<<" with "<<dave.numBananas<<" bananas."<<endl;
                cout<<"El Macho's Final Score is "<<macho.calculateTotalPoints(macho, crossedFinishLine)<<" with "<<macho.numBananas<<" bananas."<<endl;
                cout<<"Crazy Chicken's Final Score is "<<chicken.calculateTotalPoints(chicken, crossedFinishLine)<<" with "<<chicken.numBananas<<" bananas."<<endl;
                break;
                // valid move, now move chicken adn macho
            } else if(dave.moveValid(playerMove, dave)==3) {
                int rand = getRand(2);
                if(rand == 0) {
                    macho.move(allObstacles, macho, plaza, 's');
                }
                if(rand == 1) {
                    chicken.move(allObstacles, chicken, plaza, 'w');
                }
                dave.move(allObstacles, dave, plaza, playerMove);
                // check if overlap between chicken macho and dave
                int overlapVal = checkOverlap(dave, macho, chicken);
                
                if(overlapVal== 1) {
                    cout<<"The Crazy Chicken Killed You. Game Over"<<endl;
                    crossedFinishLine = true;
                    break;
                }  else if(overlapVal == 2) {
                        cout<<"El Macho Killed You. Game Over"<<endl;
                        crossedFinishLine = true;
                        break;
                    }
                }
       
                printBase(plaza);
            }
       
      
    // time is up
    if(crossedFinishLine == false) {
        cout<<"Time's Up"<<endl;
        cout<<"Dave's Final Score is "<<dave.calculateTotalPoints(dave, crossedFinishLine)<<" with " << dave.numBananas <<" banana(s)"<<" bananas."<<endl;
        cout<<"El Macho's Final Score is "<<macho.calculateTotalPoints(macho, crossedFinishLine)<<" with "<<macho.numBananas<<" bananas."<<endl;
        cout<<"Crazy Chicken's Final Score is "<<chicken.calculateTotalPoints(chicken, crossedFinishLine)<<" with "<<chicken.numBananas<<" bananas."<<endl;
    }

}
