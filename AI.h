#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

const int NL = 5; //number of layers
const int NN[NL] = {14,15,16,14,6}; //number of nodes per layer
const double WMAX = 1.5;

inline double sig(double x){
	return 1.0/(1.0+exp(-x));
}

mt19937 rng(42);
default_random_engine re;

double rdg(double min, double max){
	uniform_real_distribution<double> unif(min,max);
	return unif(re);
	//return ((max-min)*(double(rng())/double(numeric_limits<int>::max())) )+min;
}

bool mutate(){
	return (rng()%1024)==0;
}
bool mix(){
	return (rng()%256)==0;
}

class AI{
public:
	AI(){
		for(int i=0; i<NL-1; ++i){
			for(int k=0; k<NN[i+1]; ++k){
				for(int j=0; j<NN[i]; ++j){
					w[i][j][k]=rdg(-WMAX,WMAX);
				}
			}
		}
		for(int i=0; i<NL; ++i){
			for(int j=0; j<NN[i]; ++j){
				bias[i][j]=rdg(-WMAX,WMAX);
			}
		}
	}
	AI(const AI& a, const AI& b){
		for(int i=0; i<NL-1; ++i){
			for(int k=0; k<NN[i+1]; ++k){
				for(int j=0; j<NN[i]; ++j){
					if(mutate())w[i][j][k]=rdg(-WMAX,WMAX);
					else if(mix())w[i][j][k]=(a.w[i][j][k]+b.w[i][j][k])/2.0;
					else w[i][j][k]=(rng()%2)?a.w[i][j][k]:b.w[i][j][k];
				}
			}
		}
		for(int i=0; i<NL; ++i){
			for(int j=0; j<NN[i]; ++j){
				if(mutate())bias[i][j]=rdg(-WMAX,WMAX);
				else if(mix())bias[i][j]=(a.bias[i][j]+b.bias[i][j])/2.0;
				else bias[i][j]=(rng()%2)?a.bias[i][j]:b.bias[i][j];
			}
		}
	}
	vector<double> getMoves(const vector<int> v){
		load(v);
		prop();
		return getConfidences();
	}
	int fit;
	bool operator<(const AI& o) const {
		return fit>o.fit;
	}
	void printToFile(string filename){
		ofstream out;
		out.open(filename);
		out<<fixed<<setprecision(8);
		for(int i=0; i<NL-1; ++i){
			for(int k=0; k<NN[i+1]; ++k){
				for(int j=0; j<NN[i]; ++j){
					out<<w[i][j][k]<<' ';
				}
			}
		}
		out<<endl;
		for(int i=0; i<NL; ++i){
			for(int j=0; j<NN[i]; ++j){
				out<<bias[i][j]<<' ';
			}
		}cerr<<endl;
		out<<endl;
		out.close();
	}
private:
	double a[NL][16];
	double bias[NL][16];
	double w[NL-1][16][16];
	void load(const vector<int> v){
		for(int i=0; i<NN[0]; ++i){
			a[0][i]=double(v[i])/24.0;
		}
	}
	void prop(){
		for(int i=0; i<NL-1; ++i){
			for(int k=0; k<NN[i+1]; ++k){
				a[i+1][k]=bias[i+1][k];
				for(int j=0; j<NN[i]; ++j){
					a[i+1][k]+=a[i][j]*w[i][j][k];
				}
				a[i+1][k]=sig(a[i+1][k]);
			}
		}
	}
	vector<double> getConfidences(){
		vector<double> res(NN[NL-1]);
		for(int i=0; i<NN[NL-1]; ++i){
			res[i]=a[NL-1][i];
		}
		return res;
	}
};
