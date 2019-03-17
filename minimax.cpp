#include <iostream>
#include <cinttypes>
#include <string.h> //for memcpy

const int MAXD = 12; //Maximum Depth
const int INF  = 32768; //Infinity

using namespace std;

//typedef unsigned char byte;

#define byte uint8_t

//---Global variables---
const byte maxiInd[13]={13,0,1,2,3,4,5,7,8,9,10,11,12};//The indexes of the board the maxi player will loop through
byte board[14];//The board(bottom=player; top=machine)

/* This is what the board looks like(the numbers are the indexes of the array)
  12 11 10 09 08 07
13                 06
  00 01 02 03 04 05
*/


//---Calculations---
bool gameOver(){//Checks if the game is over(always gotta do this after a doMove)
    bool b=1;
    byte * i=board;
    byte * ed=&board[6];
    for(; i<ed; i++){
        if(*i){
            b=0;
            break;
        }
    }
    ed=&board[13];
    if(b){
        for(i=&board[7]; i<ed; i++){
            *ed+=*i;
            *i=0;
        }
    } else {
        b=1;
        for(i=&board[7]; i<ed; i++){
            if(*i){
                b=0;
                break;
            }
        }
        if(b){
            ed=&board[6];
            i=board;
            for(; i<ed; i++){
                *ed+=*i;
                *i=0;
            }
        }
    }
    return (board[6]+board[13]==48);
}

bool doMove(byte index){//Plays a given move(don't give it an invalid index)(returns who's gonna play next)
    byte i,stones;

    //---Player 1---
    if(index>6){
        stones = board[index];
        board[index]=0;
        for(i=1; i<=stones; i++){
            board[maxiInd[(index+i)%13]]++;
        }i--;
        byte t = (index+i)%13;
        if(maxiInd[t]==13)return 1;
        if(maxiInd[t]>6&&board[maxiInd[t]]==1){
            board[maxiInd[t]]+=board[12-maxiInd[t]];
            board[12-maxiInd[t]]=0;
        }
        return 0;

    //---Player 0---
    } else {
        stones = board[index];
        board[index]=0;
        for(i=1; i<=stones; i++){
            board[(index+i)%13]++;
        }i--;
        byte t = (index+i)%13;
        if(t==6)return 0;
        if(t<6&&board[t]==1){
            board[t]+=board[12-t];
            board[12-t]=0;
        }
        return 1;
    }
}

int heuristic(){//Calculates an heuristic value of the board(negative if human player is winning)
    return (int)((4*board[13]+3.5*board[7]+3*board[8]+2.5*board[9]+2*board[10]+1.5*board[11]+1*board[12])-(4*board[6]+3.5*board[5]+3*board[4]+2.5*board[3]+2*board[2]+1.5*board[1]+1*board[0]));
}

//--MiniMax--
int maxi(int alpha, int beta, int depth);//gotta declare it here so that mini can call it

int mini(int alpha, int beta, int depth){//minimizing player(returns the lowest value)
    if(board[13]>24)return INF;
    if(board[6]>24)return -INF;
    if(gameOver()){
        if(board[13]>24)return INF;
        if(board[6]>24)return -INF;
        return 0;
    }
    if(depth>MAXD)
        return heuristic();
    byte boardTemp[14];
    int best = INF;
    memcpy(boardTemp,board,14);
    for(byte i=0; i<6; i++){
        if(board[i]){
            int val;
            bool p = doMove(i);
            if(p){
                val = maxi(alpha, beta, depth+1);
            } else {
                val = mini(alpha, beta, depth+1);
            }
            if(val<=alpha||val<=-INF){
                return val;
            }
            if(val<beta)beta=val;
            if(val<best)best=val;
            memcpy(board,boardTemp,14);
        }

    }
    return best;
}

int maxi(int alpha, int beta, int depth){//maximizing player(returns the highest value)
    if(board[13]>24)return INF;
    if(board[6]>24)return -INF;
    if(gameOver()){
        if(board[13]>24)return INF;
        if(board[6]>24)return -INF;
        return 0;
    }
    if(depth>MAXD)
        return heuristic();
    byte boardTemp[14];
    int best = -INF;
    memcpy(boardTemp,board,14);
    for(byte i=7; i<13; i++){
        if(board[i]){
            int val;
            bool p = doMove(i);
            if(p){
                val = maxi(alpha, beta, depth+1);
            } else {
                val = mini(alpha, beta, depth+1);
            }
            if(val>=beta||val>=INF){
                return val;
            }
            if(val>alpha)alpha=val;
            if(val>best)best=val;
            memcpy(board,boardTemp,14);
        }

    }
    return best;
}
// highest value = most advantageous move for the machine
// lowest value = most advantageous move for the human player

int getAiMove(){//returns the index of the best move for the machine
    byte boardTemp[14];
    int bestVal = -INF;
    int bestMove=-1;
    memcpy(boardTemp,board,14);
    for(byte i=7; i<13; i++){
        if(board[i]){
            int val;
            bool p = doMove(i);
            if(p){
                val = maxi(bestVal, INF, 1);
            } else {
                val = mini(bestVal, INF, 1);
            }
            if(val>bestVal){
                bestVal=val;
                bestMove=i;
            }
            memcpy(board,boardTemp,14);
        }
    }
    cerr<<"MM's conf: "<<bestVal<<endl;
    if(bestMove==-1){ //If there's no decent move choose the first possible one
        for(byte i=7; i<13; i++){
            if(board[i]){
                bestMove=i;
                break;
            }
        }
    }
    return bestMove;
}


//---Main---
//int main()
int mmf(vector<int> T)
{
    for(int i=0; i<7; ++i)board[i+7]=T[i];
	for(int i=7; i<14; ++i)board[i-7]=T[i];
    /*if(board[0]==4&&board[1]==4&&board[2]==4&&board[3]==4&&board[4]==4&&board[5]==4&&board[6]==0&&board[7]==4&&board[8]==4&&board[9]==4&&board[10]==4&&board[11]==4&&board[12]==4&&board[13]==0){
        return 2;
    }
    if(board[0]==0&&board[1]==7&&board[2]==7&&board[3]==0&&board[4]==7&&board[5]==7&&board[6]==2&&board[7]==5&&board[8]==0&&board[9]==1&&board[10]==6&&board[11]==0&&board[12]==1&&board[13]==5&&player==1){
        return 5;
    }
    if(board[0]==1&&board[1]==10&&board[2]==0&&board[3]==0&&board[4]==0&&board[5]==0&&board[6]==5&&board[7]==1&&board[8]==5&&board[9]==2&&board[10]==12&&board[11]==0&&board[12]==8&&board[13]==4&&player==1){
        return 1;
    }
    if(board[0]==0&&board[1]==1&&board[2]==7&&board[3]==7&&board[4]==6&&board[5]==6&&board[6]==1&&board[7]==4&&board[8]==4&&board[9]==0&&board[10]==5&&board[11]==5&&board[12]==0&&board[13]==2&&player==1){
        return 3;
    }
    if(board[0]==0&&board[1]==7&&board[2]==7&&board[3]==2&&board[4]==0&&board[5]==7&&board[6]==3&&board[7]==6&&board[8]==1&&board[9]==2&&board[10]==7&&board[11]==0&&board[12]==1&&board[13]==5&&player==1){
        return 5;
    }*/
    return getAiMove()-7;
}

