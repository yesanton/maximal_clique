#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <fstream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
#include <list>
#include <iterator>
#include <deque>
#include <algorithm>
#include <set>


#define eps 10
using namespace std;
#define numberOfchromosome 150
typedef pair<int,int> topAnd;
typedef pair<topAnd, int> edge; 
typedef vector < int > chromosome;

typedef vector<pair<chromosome, int>> Genotype;
typedef pair<chromosome, int> elemGenotype;
class Graph{

	vector <edge> graph;
	int numberTops;
public:
	Graph(){
		ifstream fi("graph.txt");
		fi>>numberTops;
		for (int i = 0 ; i < (numberTops * (numberTops-1))/(double)2 ; i++)
		{
			edge a;
			fi>>a.first.first>>a.first.second>>a.second;
			edge b;
			b.first.first = a.first.second;
			b.first.second = a.first.first;
			b.second = a.second;
			graph.push_back(a);
			graph.push_back(b);
		}
	}
	int functionAdaptability(chromosome a);	
	//bool newGeneretion(vector<pair<chromosome, int>> & a, vector<pair<chromosome, int>> & newA,  int i, int j);

//////////////////////////////////////////////////////////////////////////
//наступні функції - перевантажені (викликаються у функції process)


	//повертаємо таку ж саму кількість хромосом як і було у генотипі from
	bool Hybridization(Genotype &g, Genotype &from);

	bool Hybridization(Genotype &g, Genotype &from, int num);
	pair<chromosome, int> HybrInOneChromosome(Genotype &from, int i, int j, int numberOfPointsForHybr);

	bool Mutation(Genotype &g, Genotype &from);
	bool Mutation(Genotype &g, Genotype &from, int numberOfPointsForMut);
	elemGenotype process();


	elemGenotype processbust();



	
};