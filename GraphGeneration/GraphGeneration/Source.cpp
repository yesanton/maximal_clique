#include <iostream>
#include <fstream>
using namespace std;
#include <vector>
#include <set>
#define numberTops 50

int main (){
	
	ofstream fo("graph.txt");

	fo<<numberTops<<endl;
	
	/*for (int i = 0 ; i < numberTops ; i++){
		for (int j = i ; j < numberTops ; j++) {
			if (i!=j)
			fo<<i<<' '<<j<<' ' <<rand()%100<<endl;
		}
	}*/
	set<int> setForME;
	for (int i = 0 ; i < numberTops ; i++){
		int number = rand()%(numberTops/10);
		setForME.clear();
		for (int j = 0 ; j < number ; j++){
			setForME.insert(rand()%numberTops);
		}
		setForME.erase(i);
		for (set<int>::iterator a = setForME.begin() ; a != setForME.end() ; a++){
			fo<< i << ' '<< *a<<endl;
		}
	}



	system("pause");
	return 0;
}