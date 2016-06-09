#include <iostream>
#include <fstream>
using namespace std;
#include <vector>
#include <set>
//#define numberTops 50
#define  upperBound 100

/*
rarefaction - розрідження 1 - 10
1 - близький до повного

*/
void generateGraph(int numberTops, int rarefaction){
	
	ofstream fo("graph200.txt");

	
	
	/*for (int i = 0 ; i < numberTops ; i++){
		for (int j = i ; j < numberTops ; j++) {
			if (i!=j)
			fo<<i<<' '<<j<<' ' <<rand()%100<<endl;
		}
	}*/
	//numberTops = 50;
	//cout<<"Enter number of tops for graph\n";
	//cin>>numberTops;

	

	if (!(rarefaction == -1)) {

	fo<<numberTops<<endl;


	set<int> setForME;
	for (int i = 0 ; i < numberTops ; i++){
		fo<<i<<' '<<-10<<' '<<rand()%upperBound<<endl;
		int number = rand()%(rarefaction/2)  + rarefaction/2;
		setForME.clear();
		for (int j = 0 ; j < number ; j++){
			setForME.insert(rand()%numberTops);
		}
		setForME.erase(i);
		for (set<int>::iterator a = setForME.begin() ; a != setForME.end() ; a++){
			fo<< i << ' '<< *a<</*' '<< rand()%upperBound <<*/endl;
		}
	}
	fo<<-1<<endl;
	return;
	}

	fo<<numberTops<<endl;
	for (int i = 0 ; i < numberTops ; i++){
		fo<<i<<' '<<-10<<' '<<rand()%upperBound<<endl;
		for (int j = 0 ; j < numberTops ; j++){
			if (i!=j)
			fo<<i << ' ' << j<<endl;
		}
	}
	fo<<-1<<endl;
	return;
}