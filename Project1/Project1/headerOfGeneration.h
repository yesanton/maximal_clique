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
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <bitset>
#include <unordered_set>
#include <queue>
#include <thread>
#include <future>

#define max_numberOfVerticesForDrawing 200
#pragma once
#define INF 60000
#define PI 3.14159265359
#define centerOfTheCircleX 340
#define centerOfTheCircleY 355
#define RADIUSforTheBigCircle 300
using namespace std;


typedef pair<int,int> edge;
typedef vector < bool > chromosome;

typedef pair<chromosome, int> elemGenotype;
typedef vector<elemGenotype> Genotype;

vector <vector < int> > funcCases(vector <vector < int> > a, int numberVertex);


// предикат
bool pred(pair<chromosome, int> a, pair<chromosome, int> b){
	return a.second > b.second;
}

void func(int a){

}

class Graph{

	//this extracted from process function for threads
	Genotype newGenotype;
	queue<chromosome> q;

public:
	ofstream logOut;
	//settings
	int numberOfchromosome ;
	int numberOfPointsForHybrid ;
    int numberOfPointsForMutation ;
	bool showProcessOfKillingVertices;
	string fileName;
	bool ifWeMustPrintOutEveryTimeAllVector;
	int eps;
	int percentageForMutation;
	int percentageForTryingToKillSomeVertices;
	int pauseBetweenAutomaticallyPlayingSteps;

	int restrictionToAmount;
	//set to find easier
	vector <pair <unordered_set< int > ,int > > graph;

	int numberVertices;
public:
	ofstream& getLogOut(){
		return logOut;
	}
	Graph(){
		logOut.open("logFile.txt");
	}
	void loadSettings();
	void Initialise();
	
	queue<chromosome> BUSTprocess();
	

	int functionAdaptability(chromosome a);	
	//ro the killing tops
	void processOfAddingVertices(Genotype &g, int percentage = 1);

	bool addAndTry(chromosome a);
	void printChromosomeLOG(elemGenotype a);
//////////////////////////////////////////////////////////////////////////
//наступні функції - перевантажені (викликаються у функції process)

	//повертаємо таку ж саму кількість хромосом як і було у генотипі from
	bool Hybridization(Genotype &g, Genotype &from);

	bool Hybridization(Genotype &g, Genotype &from, int num);

	pair<chromosome, int> HybrInOneChromosome(Genotype &from, int i, int j, int numberOfPointsForHybr);

	bool Mutation(Genotype &g, Genotype &from);
	bool Mutation(Genotype &g, Genotype &from, int numberOfPointsForMut);

	queue<chromosome> process(int);
	queue<chromosome> processSimpleThreads(int);

	elemGenotype processbust(bool);


	Genotype createFirstPopulation();
	void printChromosome(elemGenotype a);

	void HeuristicOpt(chromosome &a,queue<chromosome> &q,bool add);



	bool checkIfClique(chromosome &a);

	void execution_in_thread(int i, int j/*, Genotype &newGenotype, queue<chromosome> &q*/){
		 
		for (; i < j; i++){
			HeuristicOpt(newGenotype[i].first, q, i == 0);
			newGenotype[i].second = functionAdaptability(newGenotype[i].first);
		}

	}

	////////
	// here are the multicommunication parallel version

	queue<chromosome> processComplexThreads(int);
	void generateSomeIndividuals();
	Genotype genotype1;

	/// here is for parallel implementation flags

	bool stopAllThreadsExecution = false;
	deque<int> conditionForExitThreads;
};

queue<chromosome> Graph::process(int threads = 1){
	//the result is a queue with all the chromosomes happened

	





	int time1 = clock();


	// chromosome and adaptability
	Genotype genotype = createFirstPopulation();
	ofstream fo("out.txt");
	deque<int> conditionForExit;
	int i = 0;
	int numberOfSteps = 0;

	while (restrictionToAmount == 0 || numberOfSteps < restrictionToAmount)
	{
		Hybridization(newGenotype, genotype, numberOfPointsForHybrid);

		Mutation(newGenotype, genotype, numberOfPointsForMutation);
		// вирішив залишити одну хромосому із старої популяції, щоб не ставало гірше (що можливо)
		newGenotype.push_back(*genotype.begin());

		for (int i = 0; i < threads; i++){
			execution_in_thread(i * newGenotype.size() / threads, (i + 1) * newGenotype.size() / threads - 1);
		}

		sort(newGenotype.begin(), newGenotype.end(),pred);


		logOut << "Step #: " << numberOfSteps << " fitness: " << newGenotype[0].second << " number of Vertices: " << count(newGenotype[0].first.begin(), newGenotype[0].first.end(), 1) << "\n";


		if (ifWeMustPrintOutEveryTimeAllVector){
			printChromosome(newGenotype[0]);
		}
		else { fo << endl; }

		printChromosomeLOG(newGenotype[0]);
		logOut << endl;
		numberOfSteps++;

		genotype.clear();
		double average = 0;


		int upperBoundForTHeLoop = newGenotype.size() / 3;
		genotype.assign(newGenotype.begin(), newGenotype.begin() + upperBoundForTHeLoop);

		int newGenotypeSizestart = (double(newGenotype.size() - 1) / 3) * 2;
		int newGenotypeSizeUpperBound = (double(newGenotype.size() - 1) / 6) * 5;


		// добавимо цю частину масиву ->      |--------++--| (там де плюси)
		genotype.insert(genotype.end(), newGenotype.begin() + newGenotypeSizestart, newGenotype.begin() + newGenotypeSizeUpperBound);


		/*for (int i = newGenotypeSizestart; i < newGenotypeSizeUpperBound ; i++){
		genotype.push_back(newGenotype[i]);
		}*/
		//average += newGenotype[0].second + newGenotype[1].second;




		if (conditionForExit.size() >= eps) {
			int sum = 0;
			for (int i = 0; i < conditionForExit.size(); i++){
				sum += conditionForExit[i];
			}
			if (sum / (double)conditionForExit.size() == newGenotype[0].second) break;
			else {
				//cout<<"Genotype: "<<newGenotype[0].second<<endl;
				conditionForExit.push_back(newGenotype[0].second);
				conditionForExit.pop_front();
			}
		}
		else conditionForExit.push_back(newGenotype[0].second);
		//if (average / 2 == genotype[0].second) break;
		//cout<<i<<"  value:  "<<newGenotype[0].second<<"\n"; i++;
		newGenotype.clear();

		// і можна починати з початку...
	}

	//logOut << "Time: " << (clock() - time1) / CLOCKS_PER_SEC << " seconds + fitness: " << genotype[0].second << "\n";
	logOut << "\nTime: " << (clock() - time1) / CLOCKS_PER_SEC;
	logOut << " seconds " << (clock() - time1) << " Millisecond" << " fitness: " << genotype[0].second << "\n";
	printChromosome(genotype[0]);

	q.push(genotype[0].first);

	graph.clear();
	newGenotype.clear();


	return q;//genotype[0];

}


queue<chromosome> Graph::processSimpleThreads(int threads = 1){
	//the result is a queue with all the chromosomes happened

	int time1 = clock();


	// chromosome and adaptability
	Genotype genotype = createFirstPopulation();
	ofstream fo("out.txt");
	deque<int> conditionForExit;
	int i = 0;
	int numberOfSteps = 0;

	while (restrictionToAmount == 0 || numberOfSteps < restrictionToAmount)
	{



		//richTextBox1->Text += "asdasd";



		Hybridization(newGenotype, genotype, numberOfPointsForHybrid);

		Mutation(newGenotype, genotype, numberOfPointsForMutation);
		// вирішив залишити одну хромосому із старої популяції, щоб не ставало гірше (що можливо)
		newGenotype.push_back(*genotype.begin());



		/*for (int i = 0; i < newGenotype.size() - 1; i++){
		HeuristicOpt(newGenotype[i].first, q, i == 0);
		newGenotype[i].second = functionAdaptability(newGenotype[i].first);
		}*/

		//std::thread test_th(&Graph::execution_in_thread,this,1,2,newGenotype, q);
		//test_th.join();
		vector <std::future<void>> handles;


		for (int i = 0; i < threads; i++){
			auto handle = std::async(std::launch::async, &Graph::execution_in_thread, this,
				i * newGenotype.size() / threads,
				(i + 1) * newGenotype.size() / threads - 1/*, newGenotype, q*/);

			

			handles.push_back(std::move(handle));
		}

			for (auto &h : handles)
		{
		h.get();
		}
		

		/*execution_in_thread(0 * newGenotype.size() / threads,
		(0 + 1) * newGenotype.size() / threads - 1, newGenotype, q);*/

		sort(newGenotype.begin(), newGenotype.end(), pred);


		logOut << "Step #: " << numberOfSteps << " fitness: " << newGenotype[0].second << " number of Vertices: " << count(newGenotype[0].first.begin(), newGenotype[0].first.end(), 1) << "\n";


		if (ifWeMustPrintOutEveryTimeAllVector){
			printChromosome(newGenotype[0]);
		}
		else { fo << endl; }

		printChromosomeLOG(newGenotype[0]);
		logOut << endl;
		numberOfSteps++;

		genotype.clear();
		double average = 0;


		int upperBoundForTHeLoop = newGenotype.size() / 3;
		genotype.assign(newGenotype.begin(), newGenotype.begin() + upperBoundForTHeLoop);

		int newGenotypeSizestart = (double(newGenotype.size() - 1) / 3) * 2;
		int newGenotypeSizeUpperBound = (double(newGenotype.size() - 1) / 6) * 5;


		// добавимо цю частину масиву ->      |--------++--| (там де плюси)
		genotype.insert(genotype.end(), newGenotype.begin() + newGenotypeSizestart, newGenotype.begin() + newGenotypeSizeUpperBound);


		/*for (int i = newGenotypeSizestart; i < newGenotypeSizeUpperBound ; i++){
		genotype.push_back(newGenotype[i]);
		}*/
		//average += newGenotype[0].second + newGenotype[1].second;




		if (conditionForExit.size() >= eps) {
			int sum = 0;
			for (int i = 0; i < conditionForExit.size(); i++){
				sum += conditionForExit[i];
			}
			if (sum / (double)conditionForExit.size() == newGenotype[0].second) break;
			else {
				//cout<<"Genotype: "<<newGenotype[0].second<<endl;
				conditionForExit.push_back(newGenotype[0].second);
				conditionForExit.pop_front();
			}
		}
		else conditionForExit.push_back(newGenotype[0].second);
		//if (average / 2 == genotype[0].second) break;
		//cout<<i<<"  value:  "<<newGenotype[0].second<<"\n"; i++;
		newGenotype.clear();

		// і можна починати з початку...
	}

	//logOut << "Time: " << (clock() - time1) / CLOCKS_PER_SEC << " seconds + fitness: " << genotype[0].second << "\n";
	logOut << "\nTime: " << (clock() - time1) / CLOCKS_PER_SEC;
	logOut << " seconds " << (clock() - time1) << " Millisecond" << " fitness: " << genotype[0].second << "\n";
	printChromosome(genotype[0]);

	q.push(genotype[0].first);

	graph.clear();
	newGenotype.clear();


	return q;//genotype[0];

}

queue<chromosome> Graph::processComplexThreads(int threadsN = 1){

	//the result is a queue with all the chromosomes happened

	int time1 = clock();


	// chromosome and adaptability
	genotype1 = createFirstPopulation();
	
	vector <std::thread> threads;
	for (int i = 0; i < threadsN; i++){
		threads.push_back(std::thread(&Graph::generateSomeIndividuals, this));
	}
	for (auto &th : threads){
		th.join();
		
	} cout << "b ";

	logOut << "\nTime: " << (clock() - time1) / CLOCKS_PER_SEC;
	logOut << " seconds " << (clock() - time1) << " Millisecond" << " fitness: " << genotype1[0].second << "\n";
	printChromosome(genotype1[0]);

	return queue<chromosome>() ;
}

std::mutex mutex_genotype1;

void Graph::generateSomeIndividuals(){
	int i{ 0 };
	while (stopAllThreadsExecution != true)
	{ 

		//here is the first synchronized block
		mutex_genotype1.lock();
		Genotype aGenotype(genotype1.begin(), genotype1.end());
		mutex_genotype1.unlock();

		// do something here

		ofstream fo("out.txt");
		
		int numberOfSteps = 0;
		int restrictionForOneGlobalIteration = 5;


		Genotype newStepGenotype;
		queue<chromosome> qq;

		while (numberOfSteps < restrictionForOneGlobalIteration && !stopAllThreadsExecution)
		{
			Hybridization(newStepGenotype, aGenotype, numberOfPointsForHybrid);
		
			Mutation(newStepGenotype, aGenotype, numberOfPointsForMutation);
			
			newStepGenotype.push_back(*aGenotype.begin());
			
			
			for (int i = 0; i < newStepGenotype.size(); i++){
				
				HeuristicOpt(newStepGenotype[i].first, qq, i == 0);
				newStepGenotype[i].second = functionAdaptability(newStepGenotype[i].first);
			}
			
			sort(newStepGenotype.begin(), newStepGenotype.end(), pred);

			logOut << "Step #: " << numberOfSteps << " fitness: " << newStepGenotype[0].second << " number of Vertices: " << count(newStepGenotype[0].first.begin(), newStepGenotype[0].first.end(), 1) << "\n";

			/*if (ifWeMustPrintOutEveryTimeAllVector){
				printChromosome(newGenotype[0]);
			}
			else { fo << endl; }*/
			//printChromosomeLOG(newGenotype[0]);
			//logOut << endl;
			
			numberOfSteps++;

			aGenotype.clear();
			double average = 0;


			int upperBoundForTHeLoop = newStepGenotype.size() / 3;
			aGenotype.assign(newStepGenotype.begin(), newStepGenotype.begin() + upperBoundForTHeLoop);

			int newGenotypeSizestart = (double(newStepGenotype.size() - 1) / 3) * 2;
			int newGenotypeSizeUpperBound = (double(newStepGenotype.size() - 1) / 6) * 5;

			aGenotype.insert(aGenotype.end(), newStepGenotype.begin() + newGenotypeSizestart, newStepGenotype.begin() + newGenotypeSizeUpperBound);

			

			newStepGenotype.clear();

		}
		//logOut << "\nTime: " << (clock() - time1) / CLOCKS_PER_SEC;
		//logOut << " seconds " << (clock() - time1) << " Millisecond" << " fitness: " << genotype[0].second << "\n";
		//printChromosome(genotype[0]);

		/*q.push(genotype[0].first);*/

		//graph.clear();
		newStepGenotype.clear();






		// after this statement everything is synchronized in the block
		//user to write to the pull of chonosomes
		std::lock_guard<std::mutex> lock(mutex_genotype1);
		

		//genotype1.resize(genotype1.size() * 2 / 3);
		//here we add 33% of best individuals to the main pull
		//genotype1.insert(genotype1.begin(), 
		//	aGenotype.begin(), aGenotype.begin() + aGenotype.size() / 3);

		genotype1.resize(genotype1.size() * 2 / 3);
		genotype1.insert(genotype1.end(), aGenotype.begin(), aGenotype.begin() + aGenotype.size() / 3);


		sort(genotype1.begin(), genotype1.end(), pred);


		
		if (conditionForExitThreads.size() >= eps && !stopAllThreadsExecution) {
			int sum = 0;
			for (int i = 0; i < conditionForExitThreads.size(); i++){
				sum += conditionForExitThreads[i];
			}
			if (sum / (double)conditionForExitThreads.size() == genotype1[0].second)
			{

				stopAllThreadsExecution = true;

				//cout << "We just stop here\n";
			}
			else {
				//cout<<"Genotype: "<<newGenotype[0].second<<endl;
				conditionForExitThreads.push_back(genotype1[0].second);
				conditionForExitThreads.pop_front();
			}
		}
		else conditionForExitThreads.push_back(genotype1[0].second);

	//	cout << std::this_thread::get_id() << "  ";

		//start again
	}
	
}




void Graph::Initialise(){
	
	queue<chromosome> a;
	swap(a, q);

	long long sumWeight = 0;
	int number = 0;
	ifstream fi(fileName);

	fi>>numberVertices; 

	graph.resize(numberVertices);

	for (int i = 0 ; fi ; i++)
	{
		int index1,index2;
		fi>>index1>>index2;
		int weight;
		if (index2 == -10) {
			fi>>weight; 
			graph[index1].second = weight;
		} else if (index1 == -1 || index2 == -1) 
			break;
		else {
			number++;
			graph[index1].first.insert(index2);
			graph[index2].first.insert(index1);

		}

	}




}



void Graph::loadSettings(){
	ifstream fi("settings.txt");
	srand(time(0));
	string mark;
	int temp;

	//standart settings

	numberOfPointsForMutation =3; 
	numberOfchromosome = 150;
	numberOfPointsForHybrid =1;
	fileName ="graph200.txt";
	ifWeMustPrintOutEveryTimeAllVector =0;
	eps = 4;
	percentageForTryingToKillSomeVertices = 1;
	restrictionToAmount = 0;
	bool ok = true;
	do 
	{
		fi>>mark;
		if (mark == "numberOfchromosome"){
			fi>>temp;
			numberOfchromosome = temp;
		}else if (mark == "numberOfPointsForHybrid"){
			fi>>temp;
			numberOfPointsForHybrid = temp;
		}else if (mark == "numberOfPointsForMutation"){
			fi>>temp;
			numberOfPointsForMutation = temp;
		}else if (mark == "nameFile"){
			fi>>fileName;
		} else if (mark == "ifWeMustPrintOutEveryTimeAllVector"){
			fi>>ifWeMustPrintOutEveryTimeAllVector;
		} else if (mark == "EpsilonForStoppingProcess"){
			fi>>eps;
		} else if (mark == "percentageForMutation"){
			fi>>percentageForMutation;
		} else if (mark == "showProcessOfKillingTops"){
			fi>>showProcessOfKillingVertices;
		} else if (mark == "percentageForTryingToKillSomeTops"){
			fi>>percentageForTryingToKillSomeVertices;
		} else if (mark == "restrictionToAmount"){
			fi>>restrictionToAmount;
		} else if (mark == "pauseBetweenAutomaticallyPlayingSteps")
			fi>>pauseBetweenAutomaticallyPlayingSteps;


		else {
			ok = !ok;
		}


	} while (ok);
 
}



queue<chromosome> Graph::BUSTprocess(){
	queue<chromosome> q;
	chromosome theBest;
	int fitnessValueBest = 0;
	long long numberOfSteps = 0;
	int time1 =  clock();
	int numberOfTrue = 1;

	do 
	{
		numberOfSteps++;
		chromosome result(graph.size(), false);
		if (graph.size() == numberOfTrue)
			result.assign(numberOfTrue,true);
		else 
			for (int i = graph.size() - 1 ; i >= graph.size() - numberOfTrue ; i--){
				result[i] = true;
			}
		do
		{
			if (checkIfClique(result)) {
			int FA = functionAdaptability(result);
			if (fitnessValueBest < FA) {
				theBest = result;
				fitnessValueBest = FA;
				q.push(result);
			}}
		}while (next_permutation(result.begin(),result.end()));

		numberOfTrue++;

	} while (numberOfTrue <= graph.size());

	elemGenotype R;
	R.first = theBest;
	R.second = fitnessValueBest;
	
	printChromosome(R);
	return q;
}




//additional function

int Graph::functionAdaptability(chromosome a){


	long long f2 = 0;

	for (int i = 0 ; i < a.size() ; i++){
		if (a[i])
			f2 += graph[i].second;
	}
	return /*f1+*/f2 ;
}

bool Graph::Hybridization(Genotype &g, Genotype &from){
	srand(time(0));
	
	for (int i = 0 ; i < from.size() ; i+=2){
		

		int k = 0;

		int pointHybr = rand()%from[0].first.size();

		chromosome elem1(from[i].first.begin(),from[i].first.begin()+pointHybr);

		set<int> whatWeWillPush;
		for (int i = 0 ; i < from[0].first.size() ; i++){
			whatWeWillPush.insert(i);
		}

		int pointNow = pointHybr;
		while (elem1.size() < from[0].first.size()){
			bool ok = true;
			for (int k = 0 ; k < pointHybr ; k++){
				if (elem1[k] ==from[i+1].first[pointNow]) {ok = false;break;}
			}
			
			if (ok)
			{
				elem1.push_back(from[i+1].first[pointNow]);
			} else
			{
				elem1.push_back(-1);
			}
			pointNow++;

		}
		//з множини идаляємо які вже заповнили
		for (int i = 0 ; i < from[0].first.size() ; i++){
			whatWeWillPush.erase(elem1[i]);
		} 
		for (int i = 0 ; i < from[0].first.size() ; i++){
			if (elem1[i] == -1){
				elem1[i] = *whatWeWillPush.begin();
				whatWeWillPush.erase(*whatWeWillPush.begin());
			}
		}

		k = 0;

		for (int i = 0 ; i < from[0].first.size() ; i++){
			whatWeWillPush.insert(i);
		}
		pointNow = pointHybr;

		chromosome elem2(from[i+1].first.begin(),from[i+1].first.begin()+pointHybr);

		while (elem2.size() < from[0].first.size()){
			bool ok = true;
			for (int k = 0 ; k < pointHybr ; k++){
				if (elem2[k] ==from[i].first[pointNow]) {ok = false;break;}
			}

			if (ok)
			{
				elem2.push_back(from[i].first[pointNow]);
			} else
			{
				elem2.push_back(-1);
			}
			pointNow++;

		}
		//з множини идаляємо які вже заповнили
		for (int i = 0 ; i < from[0].first.size() ; i++){
			whatWeWillPush.erase(elem2[i]);
		} 
		for (int i = 0 ; i < from[0].first.size() ; i++){
			if (elem2[i] == -1){
				elem2[i] = *whatWeWillPush.begin();
				whatWeWillPush.erase(*whatWeWillPush.begin());
			}
		}
		pair<chromosome, int> c1,c2;
		c1.first = elem1;
		c2.first = elem2;

		g.push_back(c1);
		g.push_back(c2);
	}
	return true;
}
bool Graph::Hybridization(Genotype &g, Genotype &from,int num){
	srand(time(0));

	for (int i = 0 ; i < from.size() ; i+=2){
		g.push_back(HybrInOneChromosome(from,i,i+1,num));
		g.push_back(HybrInOneChromosome(from,i+1,i,num));
	}
	return true;
}
pair<chromosome, int> Graph::HybrInOneChromosome(Genotype &from, int i, int j, int numberOfPointsForHybr){

	int pointHybr;  
	set <int> pH;// спочатку використовуємо множину - щоб легко проводити пошук, також вставку - він не допускає ключів дублікатів
	while (pH.size() != numberOfPointsForHybr)
	{
		//створюємо множину неповторюваних елементів - точок 
		//дані вставляються за зростанням - дуже зручно
		pointHybr = rand()%from[0].first.size();
		pH.insert(pointHybr);//якщо такої точки схрещування ще немає, вставляємо її в множину
	}
	vector<int>  pointsHybr(pH.begin(),pH.end());

	pointsHybr.push_back(from[0].first.size());




	chromosome elem1(from[i].first.begin(),from[i].first.begin()+pointsHybr[0]);//перший з першого

	set<int> whatWeWillPush;
	for (int i = 0 ; i < from[0].first.size() ; i++){
		whatWeWillPush.insert(i);
	}

	int pointNow = pointsHybr[0];
	for (int point = 1 ; point < pointsHybr.size() ; point++){
		//pointNow = pointsHybr[point];
		while (elem1.size() < /*from[0].first.size()*/pointsHybr[point]){
			bool ok = true;
			if (ok)
			{
				if (point % 2 == 1) { 
					elem1.push_back(from[j].first[pointNow]);
				}
				else {
					elem1.push_back(from[i].first[pointNow]);
				}

			} else
			{//якщо вже наявне таке значення вставляємо -1(потім заповнимо)
				elem1.push_back(-1);
			}
			pointNow++;

		}
	}

	pair<chromosome, int> chrom; // приводимо до потрібного нам вигляду

	chrom.first = elem1;

	return chrom;
}


bool Graph::Mutation(Genotype &g, Genotype &from){
	srand(time(0));
	int sizeFrom = from.size();
	for (int i = 0 ; i < sizeFrom ; i++){
		chromosome temp;

		temp = from[i].first;

		int k,j; 
		k = rand()%from[0].first.size();
		
		do 
		{
			j = rand()%from[0].first.size();
		} while (k==j);
			
		swap(temp[k], temp[j]);
		

		pair<chromosome,int> chrom;
		chrom.first = temp;

		g.push_back(chrom);


	}
	return true;
}
bool Graph::Mutation(Genotype &g, Genotype &from, int numberOfPointsForMut){
	srand(time(0));
	int sizeFrom = from.size() ;//* percentageForMutation / 100.0;
	for (int i = 0 ; i < sizeFrom ; i++){

		chromosome temp;

		temp = from[i].first;

		/*
		int k,j; 
		k = rand()%(from[0].first.size()-numberOfPointsForMut-numberOfPointsForMut);

		do 
		{
			j = rand()%(from[0].first.size()-numberOfPointsForMut+1);
		} while (k+numberOfPointsForMut > j);
		

		for (int q = 0 ; q < numberOfPointsForMut ; q++){
			swap(temp[k], temp[j]); k++;j++;
		}
		
		*/

		for (int i = 0 ; i < numberOfPointsForMut ; i++){
			int k = rand() % temp.size();

			temp[k] = !temp[k];

		}



		pair<chromosome,int> chrom;
		chrom.first = temp;

		g.push_back(chrom);


	}
	return true;
}


Genotype Graph::createFirstPopulation(){
	//we define empty population (the process adds vertexes itself)
	srand(time(0));
	Genotype genotype;
	for (int i = 0 ; i < numberOfchromosome ; i++){
		pair<chromosome, int> temp;
		//bool choice;
		chromosome temp1(numberVertices,0);
		/*for (int j = 0 ; j < numberTops ; j++){
			choice = rand()%2;
			temp.first.push_back(choice);
		}*/
		for (int i = 0 ; i < temp1.size() ; i++)
		{
			if (rand()%2 == 0) {
				temp1[i] = true;
			}
		}

		temp.first = temp1;
		// definite the first set of chromosome
		genotype.push_back(temp);
		// calculate adaptability for the first generation
		


		//values will be 0, because the graph is empty
		genotype[i].second = 0; /*functionAdaptability(genotype[i].first);*/
	}
	
	//sort(genotype.begin(), genotype.end(),pred);
	return genotype;
}


vector < vector < int > > funcCases(vector <vector < int> > a, int numberVertex){
	vector<vector<int>> out;

	for (int i = 0; i < a.size(); i++){
		vector < int > temp = a[i];
		temp.push_back(0);
		for (int j = 0 ; j < numberVertex ; j++){
			temp[temp.size() - 1] = j;
			out.push_back(temp);
		}

	}
	return out;
}

bool Graph::addAndTry(chromosome a){
	int qqq = 0;
	
	int m = false;
	if (showProcessOfKillingVertices){
		cout<<"   Number Of Tops before killing: ";
		for (int i = 0 ; i < a.size() ; i++) qqq+=int(a[i]);
		cout<<qqq<<" fitness: "<< functionAdaptability(a)<<endl;
	}
	


	for (int i = 0 ; i < a.size() ; i++){
		// if we find top that is in smallest domination set
		// kill it and check if we didn't add to fitness value
		
		if (a[i] == false){
			int fitnessValueOfOlfChromosome = functionAdaptability(a);
			a[i] = true;
			int fitnessValueOfNewChromosome = functionAdaptability(a);
			if (fitnessValueOfNewChromosome > fitnessValueOfOlfChromosome) {
				m = true;
			}else {
				a[i] = false;
			}
		}	

	}

	qqq=0;
	if(showProcessOfKillingVertices){
		cout<<"   Number Of Tops after killing: ";
		for (int i = 0 ; i < a.size() ; i++) qqq+=int(a[i]);
		cout<<qqq<<" fitness: "<< functionAdaptability(a)<<endl;


	}

	
	return m;

}

void Graph::processOfAddingVertices(Genotype &g, int percentage /* = 1 */){
	if (percentage == 1){
		addAndTry(g[0].first);


	} else {
		int numberForTrying = g.size() * (percentage / 100.0);
		addAndTry(g[0].first);
		numberForTrying--;
		while (numberForTrying > 0)
		{
			//try to kill tops in random chromosome
			addAndTry(g[rand()%g.size()].first);
			numberForTrying--;
		}


	}
}

void Graph:: printChromosome(elemGenotype a){
	ofstream fo("resulTS.txt");

//	cout<<"Chromosome: (";
	int number1 = 0;
	for (int i = 0 ; i < a.first.size() ; i++){
		if (a.first[i] == true) number1++;
		if (a.first[i]) {
			fo<<int(i)<< '\n';
			}
		//cout<< a.first[i] << ' ';
	} //cout<<")\n";
	
}
void Graph:: printChromosomeLOG(elemGenotype a){
	logOut<< "Chromosome (vertices included): (";

	int number1 = 0;
	for (int i = 0 ; i < a.first.size() ; i++){
		if (a.first[i] == true) number1++;
		if (a.first[i]) {
			logOut<<int(i)<<' ';
			}
		
	} 
	logOut <<") + number Vertices: " << number1 <<"\n";

}



bool Graph::checkIfClique(chromosome &a){
	bool ok = true;

	for (int i = 0 ; i < a.size() ; i++){
		if (a[i])
			for (int j = 0 ; j  < a.size() ; j++){
				if (a[j] && i!=j)	
					if (graph[i].first.find(j) == graph[i].first.end()) ok = false;
			}
	}


	return ok;

}



void Graph::HeuristicOpt(chromosome &a,queue<chromosome> &q, bool add){
	int verticesToBeAdded = log(a.size());
	
	//enlarge stage
	
	//if (add)  q.push(a);
	/*
	for (int i = 0 ; i < verticesToBeAdded ; i++){
		//if the node is already true, just ignore it
		// try again
		int node = rand() % a.size();

		a[node] = true;
	}
	if (add)  q.push(a);*/
	//extract a clique stage

	int idx = rand() % a.size();

	for (int i = idx ; i < a.size() ; i++){
		if (a[i])
			if (bool(rand()%2)) {
				a[i] = false;
			} else {
				for (int j = i + 1 ; j < a.size() ; j++){
					
					//case that we will not encounter the idx's vertex in the 
					//adjacency list of i's
					if (graph[j].first.find(i) == graph[j].first.end()) {
						a[j] = false;
					}
					
				} 
				
				for (int j = 0 ; j < i - 1 ; j++){
					//the same procedure
					if (graph[j].first.find(i) == graph[j].first.end()) {
						a[j] = false;
					}
				}
			}

	}

	for (int i = idx - 1 ; i >= 0 ; i--){
		if (a[i]){
			if (bool(rand()%2)) {
				a[i] = false;
			} else {
				for (int j = i-1 ; j >= 0 ; j--){
					if(graph[j].first.find(i) == graph[j].first.end()){
						a[j] = false;
					}
				}
				for (int j = i+1 ; j < a.size() ; j++){
					if(graph[j].first.find(i) == graph[j].first.end()){
						a[j] = false;
					}
				}


			}
		}
	}
	//if (add)  q.push(a);

	 
	//extend subgraph
	
	idx = rand() & a.size();

	bool ok;

	for (int i = idx ; i < a.size() ; i++){
		if (!a[i]) {
			ok = true;
			for (int j = 0 ; j < a.size() ; j++){
				if (a[j]) {
					if (graph[i].first.find(j) == graph[i].first.end()) {
						ok = false;
						break;
					}
				}
			}
			if (ok) a[i] = true;
		}

	} for (int i = 0 ; i < idx - 1;  i++){
		if (!a[i]) {
			ok = true;
			for (int j = 0 ; j < a.size() ; j++){
				if (a[j]) {
					if (graph[i].first.find(j) == graph[i].first.end()) {
						ok = false;
						break;
					}
				}
			}
			if (ok) a[i] = true;
		}

	}
		//if (add)  q.push(a);

}




