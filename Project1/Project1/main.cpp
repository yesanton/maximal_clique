 
#include "headerOfGeneration.h"
#include "graphGenerator.h"

class MainProcess {
	Graph graph;
	vector <bool> prev;
	bool breakPlaying;
	queue<chromosome> q;
	queue<chromosome> newQ;


	int numberOfPointsForMutation;
	int numberOfchromosome;
	int numberOfPointsForHybrid;
	//string NameFile;
	int ifWeMustPrintOutEveryTimeAllVector;
	int EpsilonForStoppingProcess;
	int percentageForMutation;
	int showProcessOfKillingVertices;
	int percentageForTryingToKillSomeVertices;

	bool showProcessOrNot;
	double time1;


	bool ifBustOrGenetical = true;


public:
	ofstream& getLogOut(){
		return graph.getLogOut();
	}
	
	void initialization(){
		//	breakPlaying = false;
		graph.loadSettings();
		ifBustOrGenetical = true;
	}
	

	void burstOrGenetical(bool t){
		ifBustOrGenetical = t;
	}


	void startAlgo(int threads = 1){

		graph.Initialise();
		time1 = 0;
		graph.logOut << "\nStart ";
		if (ifBustOrGenetical) graph.logOut << "genetic with optimization algorithm\n";
		else graph.logOut << "bust algorithm\n";


		graph.logOut << "Graph has been inputed with: " << graph.numberVertices << " vertices";
		if (ifBustOrGenetical)
			q = graph.process(threads);
		else q = graph.BUSTprocess();

	}

	void startAlgoSimpleThreads(int threads = 1){

		graph.Initialise();
		time1 = 0;
		graph.logOut << "\nStart ";
		if (ifBustOrGenetical) graph.logOut << "genetic with optimization algorithm\n";
		else graph.logOut << "bust algorithm\n";


		graph.logOut << "Graph has been inputed with: " << graph.numberVertices << " vertices";
		if (ifBustOrGenetical)
			q = graph.processSimpleThreads(threads);
		else q = graph.BUSTprocess();

	}
	void startAlgoComplexThreads(int threads = 1){

		graph.Initialise();
		time1 = 0;
		graph.logOut << "\nStart ";
		if (ifBustOrGenetical) graph.logOut << "genetic with optimization algorithm\n";
		else graph.logOut << "bust algorithm\n";


		graph.logOut << "Graph has been inputed with: " << graph.numberVertices << " vertices";
		if (ifBustOrGenetical)
			q = graph.processComplexThreads(threads);
		else q = graph.BUSTprocess();

	}
};	

void generateGraph(MainProcess *mp){
	//////// these variables 
	int VERTICES = 700;
	int RAREFICATION = 500;
	/////////
	generateGraph(VERTICES, RAREFICATION);

	mp->getLogOut() << "Graph successfully created with " << VERTICES << " vertices\n";
}
int main(){
	
	MainProcess mainProcess;

	mainProcess.initialization();

	//generateGraph(&mainProcess);
	cout << "Enter number of threads (-1 to exit) : ";

	int numberOfThreads; cin >> numberOfThreads;

	if (numberOfThreads == -1) return 0;
	

	//mainProcess.startAlgoComplexThreads(4);



	

	int t1 = clock();
	for (int i = 0; i < 1; i++){
		mainProcess.startAlgoSimpleThreads(numberOfThreads);
	}

	cout << "Average time with simple threads: " << (clock() - t1)  << endl;

	
	
	t1 = clock();
	for (int i = 0; i < 1; i++){
		mainProcess.startAlgo(numberOfThreads);
	}
	cout << "Average time without actual threads: " << (clock() - t1) << endl;
	
	
	t1 = clock();
	for (int i = 0; i < 1; i++){
		mainProcess.startAlgoComplexThreads(numberOfThreads);
	}
	cout << "Average time with complex multi-threading: " << (clock() - t1) << endl;

	

	system("pause");
	return 0;
}