#include "Header.h"


// предикат
bool pred(pair<chromosome,int> a, pair<chromosome,int> b){
	return a.second > b.second;
}

elemGenotype Graph::process(){
	// chromosome and adaptability
	Genotype genotype;
	for (int i = 0 ; i < numberOfchromosome ; i++){
		pair<chromosome, int> temp;

		for (int j = 0 ; j < numberTops ; j++){
			temp.first.push_back(j);
		}
		random_shuffle(temp.first.begin(),temp.first.end());
		// definite the first set of chromosome
		genotype.push_back(temp);
		// calculate adaptability for the first generation
		genotype[i].second = functionAdaptability(genotype[i].first);
	}
	deque<int> conditionForExit;
	int i = 0;
	while (true)
	{
		Genotype newGenotype;
	
		Hybridization(newGenotype,genotype,2);
		
		Mutation(newGenotype,genotype,2);
		// вирішив залишити одну хромосому із старої популяції, щоб не ставало гірше (що можливо)
		newGenotype.push_back(*genotype.begin());

		for (int i = 0 ; i < newGenotype.size() ; i++){
			newGenotype[i].second = functionAdaptability(newGenotype[i].first);
		}

		sort(newGenotype.begin(), newGenotype.end(),pred);

		genotype.clear();

		double average = 0;
		for (int i = 0 ; i < newGenotype.size() / 2 ; i++)
		{
			genotype.push_back(newGenotype[i]);
			
		}
		//average += newGenotype[0].second + newGenotype[1].second;
		
		if (conditionForExit.size()>=eps) {
			int sum = 0;
			for (int i = 0 ; i < conditionForExit.size() ; i++){
				sum += conditionForExit[i];
			}
			if (sum/(double)conditionForExit.size() == newGenotype[0].second) break;
			else {
				//cout<<"Genotype: "<<newGenotype[0].second<<endl;
				conditionForExit.push_back(newGenotype[0].second);
				conditionForExit.pop_front();
			}
		}else conditionForExit.push_back(newGenotype[0].second);
		//if (average / 2 == genotype[0].second) break;
		//cout<<i<<"  value:  "<<newGenotype[0].second<<"\n"; i++;
		newGenotype.clear();
		
		// і можна починати з початку...
	}
	
	return genotype[0];

}
int Graph::functionAdaptability(chromosome a){
	int value = 0;
	for  (int i = 0 ; i < a.size()-1 ; i++){
		for (int j = 0 ; j < graph.size() ; j++){
			if (graph[j].first.first == a[i] && graph[j].first.second == a[i+1]){
				value += graph[j].second;
				break;
			}
		}
	}
	return value;
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
			/*int ind;
			if (from[i].first[from[i+1].first[elem2[elem2.size()-1]]] == from[i].first[from[i+1].first[k]]) {
			while (true)
			{
			k++;
			ind = from[i+1].first[k];
			chromosome::iterator iterChrom = find(elem2.begin(), elem2.end() , from[i].first[ind]);
			if (iterChrom == elem2.end()) break;


			}
			}
			else ind = from[i+1].first[elem2[elem2.size()-1]];

			elem2.push_back(from[i].first[ind]);*/
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
			for (int k = 0 ; k < pointsHybr[point-1] ; k++){
				if (point % 2 == 1) { //перевіряємо чи в нашому щойнозробленому хромосомі є числа, які ми намагаємося добавити з іншого шматка
					if (elem1[k] ==from[j].first[pointNow]) {ok = false;break;}
				}
				else {//враховуємо, якщо парна точка схрещування - вставляємо з одного - ні - з іншого
					if (elem1[k] ==from[i].first[pointNow]) {ok = false;break;}
				}
			}
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

		//for (int j = 0 ; j < from[0].first.size() ; j++){
			//int ifMutation = rand() % 10;
			//if (ifMutation == 5) {
		int k,j; 
		k = rand()%from[0].first.size();
		
		do 
		{
			j = rand()%from[0].first.size();
		} while (k==j);
				//int positionMutate = rand()%(from[0].first.size()-(j)) + j ;
				//if (j != positionMutate)
		swap(temp[k], temp[j]);
		
		
			//}
		//}

		pair<chromosome,int> chrom;
		chrom.first = temp;

		g.push_back(chrom);


	}
	return true;
}
bool Graph::Mutation(Genotype &g, Genotype &from, int numberOfPointsForMut){
	srand(time(0));
	int sizeFrom = from.size();
	for (int i = 0 ; i < sizeFrom ; i++){
		chromosome temp;

		temp = from[i].first;

		
		int k,j; 
		k = rand()%(from[0].first.size()-numberOfPointsForMut-numberOfPointsForMut);

		do 
		{
			j = rand()%(from[0].first.size()-numberOfPointsForMut+1);
		} while (k+numberOfPointsForMut > j);
		
		//swap(temp[k], temp[j]);

		for (int q = 0 ; q < numberOfPointsForMut ; q++){
			swap(temp[k], temp[j]); k++;j++;
		}
		

		pair<chromosome,int> chrom;
		chrom.first = temp;

		g.push_back(chrom);


	}
	return true;
}





elemGenotype Graph::processbust(){
	

	chromosome a;
	for (int i = 0 ; i < numberTops ; i++){
		a.push_back(i);
	}

	elemGenotype res;
	long long i = 0 ;
	do{
		if (functionAdaptability(a) > res.second){
			res.first = a;
			res.second = functionAdaptability(a);
		}
		i++;
		//if (i%100 == 0)cout<<i<<"Do`h\n"; i++;
	}while(next_permutation(a.begin(), a.end()));



	return res;
}






