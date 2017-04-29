/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"
#include<fstream>
#include<iostream>
#include<unistd.h>
#include<cstring>
using namespace std;
vector<int> p;	//profit
vector<int> w;	//widht
vector<int> c;	//class
int n, d, B;	//number of items, Capacity of the knapsack

int tMax;	//Maximum execution time, in seconds
int aType;	//Algorithm type, 0:bt, 1:bnb
vector <int> sol;
int main(int argc, char* argv[]){
	tMax = 600;
	aType = 0;
	int verbose = 0;
	string input_file;
	int option, i_f = 0;
	if ( argc == 1 ){
		show_usage();
		return 0;
	}
	while ((option = getopt(argc, argv, "t:i:vb:"))!=-1)
		switch(option){
			case 'b':
				if (strcmp(optarg,"t")==0)
					aType = 0;
				else if (strcmp(optarg,"b")==0)
					aType = 1;
			case 't':
				tMax=atoi(optarg);
				break;
			case 'i':
				i_f = 1;
				input_file.assign(optarg);
				break;
			case 'v':
				verbose=1;
				break;
			default:
				break;
		}

	if ( i_f == 0 ){
		cout << "-i  mandatory argument" << endl;
		return 1;
	}

	if ( !read_knapsack(input_file) ) {
		cout << "Couldn't read";
		return 1;
	}

	if ( verbose )
		show_input();

	if ( aType == 0 )
		bt(n,d,B,p,w,c,sol,tMax);
	else if ( aType == 1 )
		bnb(n,d,B,p,w,c,sol,tMax);

	if ( !is_feasible_solution() && verbose==1 )
		cout << "Infeasible solution" << endl;
	//std output
	for(int i=0; i<n; i++)
		cout << sol[i] << " ";
	cout << endl;
	return 0;
}

int read_knapsack(string input_file){
	ifstream kinput;
	kinput.open(input_file.c_str()); if (!kinput) return 0;
	kinput >> n >> d >> B;
	for(int i=0; i<n; i++){
		int x, y, z;
		kinput >> x >> y >> z;
		p.push_back(x); w.push_back(y); c.push_back(z);
		sol.push_back(0);	//The trivial solution, empty knapsack
	}
	return 1;
}

int show_usage(){
	cout << "Uso: ./knapsack -i <input_file> [ -bt | -bb ] -t <max_time>"  << endl 
	     << "onde -bt indica backtracking e -bb indica branch-and-bound. max_time eh o tempo maximo de execucao." << endl;
	return 0;
}

int show_input(){
//	cout << "Input file: " << input_file;
	cout << n << " " << d << " " << B << endl;
	for(int i=0; i<n; i++)
		cout<<p[i]<<" "<<w[i]<<" "<<c[i]<<endl;
	cout << "Using " << (aType==0?"backtracking":"branch-and-bound") << endl;
	return 0;
}

///
// returns true if sol is feasible
bool is_feasible_solution(){
	set<int> nc;
	int weight = 0;
	for(int i=0; i<n; i++){
		if ( sol[i] == 1) 
			nc.insert(c[i]);
		weight += (w[i]*sol[i]);
	}
	if ( weight > B )
		return false;
	int used_classes = nc.size();
	if ( weight+(used_classes-1)*d > B )
		return false;
	return true;
}
