#include <vector>

using namespace std;

class Game{
public:
	Game():board{4,4,4,4,4,4,0,4,4,4,4,4,4,0},turn(0),over(0){
	}
	void move(int pos){
		int n=board[pos];
		board[pos]=0;
		int i=pos+1;
		while(n){
			if((i==6&&turn==1)||(i==13&&turn==0))i=(i+1)%14;
			board[i]++;
			n--;
			i=(i+1)%14;
		}
		i=(i+13)%14;
		if((i!=6)&&(i!=13)&&(board[i]==1)){
			if((turn==0&&i<6)||(turn==1&&i>6)){
				board[i]+=board[12-i];
				board[12-i]=0;
			}
		}
		bool zero=1;
		for(int i=0; i<6; ++i)if(board[i]){zero=0;break;}
		if(zero){
			for(int i=7; i<13; ++i){
				board[13]+=board[i];
				board[i]=0;
			}
			over=1;
		} else {
			zero=1;
			for(int i=7; i<13; ++i)if(board[i]){zero=0;break;}
			if(zero){
				for(int i=0; i<6; ++i){
					board[6]+=board[i];
					board[i]=0;
				}
				over=1;
			}
		}
		if((turn==0&&i==6) || (turn==1&&i==13))return;
		turn=!turn;
	}
	bool validMove(int p){
		if(turn==0&&(p>5||p<0))return 0;
		if(turn==1&&(p>12||p<7))return 0;
		if(board[p]==0)return 0;
		return 1;
	}
	bool getTurn(){
		return turn;
	}
	bool isOver(){
		return over;
	}
	bool getWinner(){
		return board[13]>=board[6];
	}
	vector<int> getTurnedBoard(){
		vector<int> res(14);
		if(turn){
			for(int i=7; i<14; ++i)res[i-7]=board[i];
			for(int i=0; i<7; ++i)res[i+7]=board[i];
		} else {
			for(int i=0; i<14; ++i)res[i]=board[i];
		}
		return res;
	}
private:
	int board[14];
	bool turn;
	bool over;
};
