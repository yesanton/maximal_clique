#include "Header.h"
void printChromosome(elemGenotype a){
	cout<<"Chromosome: (";
	for (int i = 0 ; i < a.first.size() ; i++){
		cout<< a.first[i] << ' ';
	} cout<<")\n";
}

int main (){
	srand(time(0));
	
	int time1 =  clock();

	Graph g;
	/*elemGenotype a = g.process();

	printChromosome(a);
	cout<<a.second<<endl;
	cout<<"Time: "<< clock() - time1<<endl;
	time1 = clock();*/
	cout<<"Bust\n";
	elemGenotype b = g.processbust();
	printChromosome(b);
	cout<<b.second<<endl;
	cout<<"Time: "<< clock() - time1<<endl;
	system("pause");
	return 0;
}