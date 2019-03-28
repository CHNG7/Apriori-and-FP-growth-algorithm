#include <bits/stdc++.h>
#include "apriori.h"
#include "input_reader.h"
#include "test1.h"
#include "outputWriter.h"
using namespace std;
int main(){
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	int support,confidence;
	cout<<"enter support and confidence "<<endl;
	cin>>support>>confidence;
	input_reader inp("groceries.csv");
	vector<vector<int>>transaction=inp.getTransactions();
	int unique_cnt=inp.getCountofUniqueItems();
	unordered_map<int,string>MP=inp.getMappings();
	cout<<"unique items = "<<unique_cnt<<endl;
	apriori obj1(transaction,support,confidence,unique_cnt);
	vector<vector<vector<int>>>frequent_set=obj1.getFreqItemSet();
	vector<tuple<vector<int>,vector<int>,double>>associationRules=obj1.getAssociationRules();
	vector<tuple<vector<int>,vector<int>,double>>redudantAssociationRules=obj1.getRedundantAssociationRules();
	vector<vector<int>>maximalFrequentSet=obj1.getMaximalFrequentItemSet();
	vector<vector<int>>closedFrequentSet=obj1.getClosedFrequentItemSet();
	vector<vector<int>>notClosedFrequentSet=obj1.getNotClosedFrequentItemSet();
	//test printobj(MP);
	outputWriter outputobj(MP,support,confidence);
	//frequentset
	outputobj.writeFrequentItemSet(frequent_set);
	//printobj.printSSVector(frequent_set);

	//association rules
	cout<<"Interesting association rules "<<associationRules.size()<<endl;
	outputobj.writeAssociationRules("",associationRules);
	

	//maximal frequent item set
	//cout<<"\nmaximal frequent item set\n";
	outputobj.writeSvector(maximalFrequentSet,"Maximal frequent item set");
	//printobj.printSVector(maximalFrequentSet);

	//maximal closed frequent items set
	cout<<"\nClosed frequent item set\n";
	outputobj.writeSvector(closedFrequentSet,"Closed frequent item set");
	//printobj.printSVector(closedFrequentSet);


	//cout<<"\nnot Closed frequent item set\n";
	outputobj.writeSvector(notClosedFrequentSet,"Not closed frequent item set");

	//getRedundantRules
	outputobj.writeAssociationRules("Redudndant ",redudantAssociationRules);
}