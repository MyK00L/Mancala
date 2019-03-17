#include "AI.h"
#include "Game.h"
#include "minimax.cpp"
#include <iostream>

using namespace std;

const int POPSIZE = 42;

AI ais[POPSIZE];

bool matchvsmm(int i){
	Game game;
	vector<double> est;
	while(!game.isOver()){
		double ma=-2e9;
		int move=-1;
		if(game.getTurn()){
			move=mmf(game.getTurnedBoard())+7;
		} else {
			est=ais[i].getMoves(game.getTurnedBoard());
			for(int i=0; i<(int)est.size(); ++i){
				if(game.validMove(i)&&est[i]>ma){
					ma=est[i];
					move=i;
				}
			}
		}
		game.move(move);
	}
	return game.getWinner();
}

bool match(int i, int j){
	Game game;
	vector<double> est;
	while(!game.isOver()){
		double ma=-2e9;
		int move=-1;
		if(game.getTurn()){
			est=ais[j].getMoves(game.getTurnedBoard());
			for(int i=0; i<(int)est.size(); ++i){
				if(game.validMove(i+7)&&est[i]>ma){
					ma=est[i];
					move=i+7;
				}
			}
		} else {
			est=ais[i].getMoves(game.getTurnedBoard());
			for(int i=0; i<(int)est.size(); ++i){
				if(game.validMove(i)&&est[i]>ma){
					ma=est[i];
					move=i;
				}
			}
		}
		game.move(move);
	}
	return game.getWinner();
}

void cycle(){
	for(int i=0; i<POPSIZE; ++i)ais[i].fit=0;
	for(int i=0; i<POPSIZE; ++i){
		for(int j=0; j<POPSIZE; ++j){
			if(i==j)continue;
			if(match(i,j)){
				ais[j].fit++;
				//cerr<<j<<" won against "<<i<<endl;
			}
			else {
				ais[i].fit++;
				//cerr<<i<<" won against "<<j<<endl;
			}
		}
	}
	sort(ais,ais+POPSIZE);
	cerr<<ais[0].fit<<' '<<(POPSIZE-1)<<' '<<ais[POPSIZE-1].fit<<endl;
	for(int i=0; i< (POPSIZE>>1)-1; ++i){
		ais[i+(POPSIZE>>1)]=AI( (i<(POPSIZE>>2))?ais[0]:ais[i] ,ais[i+1]);
	}
}

int main(){
	for(int i=0; i<2; ++i){
		cout<<"generation #"<<i<<": ";
		cycle();
	}
	Game game;
	vector<double> est;
	vector<int> tmp;
	while(!game.isOver()){
		double ma=-2e9;
		int move=-1;

		if(game.getTurn()==0)cout<<endl<<endl<<"AI's turn"<<endl;
		else cout<<endl<<endl<<"MM's turn"<<endl;
		tmp=game.getTurnedBoard();
		cout<<"\n\t";
		for(int i=12;i>=7;--i)cout<<tmp[i]<<'\t';
		cout<<'\n';
		cout<<tmp[13]<<"\t\t\t\t\t\t\t"<<tmp[6]<<"\n\t";
		for(int i=0; i<6; ++i)cout<<tmp[i]<<'\t';
		cout<<endl;
		if(game.getTurn()==0){
			est=ais[0].getMoves(tmp);
			for(int i=0; i<(int)est.size(); ++i){
				cout<<est[i]<<' ';
				if(game.validMove(i)&&est[i]>ma){
					ma=est[i];
					move=i;
				}
			}
			cout<<endl;
			cout<<"AI's move: "<<move<<endl;
		} else {
			move=mmf(tmp)+7;
			cout<<"MM's move: "<<move<<endl;
		}
		game.move(move);
	}
	cout<<endl<<endl;
	ais[0].printToFile("bestAi.txt");
	if(game.getWinner()){
		cout<<"AI lost to MM"<<endl;
	} else {
		cout<<"MM lost to AI!??"<<endl;
	}
	return 0;
}
