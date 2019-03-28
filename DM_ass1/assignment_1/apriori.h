#include <bits/stdc++.h>
using namespace std;
class apriori{
private:
	vector<vector<vector<int>>> frequent_set,real_set,unreal_set;
	vector<vector<int>>transaction;
	vector<vector<int>>candidate_set,qualified_set;
	vector<tuple<vector<int>,vector<int>,double>>associationRules;
	vector<tuple<vector<int>,vector<int>,double>>redundantAssociationRules;
	vector<vector<int>>maximalFrequentItemSet;
	vector<vector<int>>closedFrequentItemSet;
	vector<vector<int>>notClosedFrequentItemSet;
	int level,minsup,min_conf,unique_cnt,N;
	void genFreqItemSet(){
		/* frequent item set generator all functions resembles to the ones used in the text book ntroduction to Data Mining
			Book by Michael Steinbach, Pang-Ning Tan, and Vipin Kumar*/
		// sorts the transactions based on id so that we can work with the dataset easily	
		for(auto&tupl:transaction){
			sort(tupl.begin(),tupl.end());
		}
		int clt=0;
		// loops until candidate set is empty or qualified candidate set is empty 
		while(1){
			// geneartes candidate set by calling genNextCandidateSet function and 
			candidate_set= genNextCandidateSet(qualified_set);
			if(candidate_set.size()==0)break;
			// generates qualified set based on support
			qualified_set= genQualifiedSet();
			if(qualified_set.size()==0)break;
			frequent_set.push_back(qualified_set);
			level++;
			clt++;
		}
	}
	void genAssociationRules(vector<vector<vector<int>>>ruleGenSet,vector<tuple<vector<int>,vector<int>,double>>&AssociationRules){
		// we use the apgen rules algorithm in the text book'
		// this is apgen rules calling function
		for(auto&it:ruleGenSet){
			for(auto&Fk:it){
				if(Fk.size()<2)break;
				vector<vector<int>>H;
				//if(Fk.size()>=3)cout<<"size "<<Fk.size()<<endl;
				for(int i=0;i<Fk.size();i++){
					H.push_back(vector<int>(1,Fk[i]));
				}
				level=0;
				apgenRules(Fk,H,AssociationRules);
			}
		}
	}
	void apgenRules(vector<int>Fk,vector<vector<int>>H,vector<tuple<vector<int>,vector<int>,double>>&AssociationRules){
		// implementation of ap gen function in text book
		int k=Fk.size();
		if(H.size()<1)return;
		int m = H[0].size();
		level++;
		if(k>m+1){
			vector<vector<int>>H1=genNextCandidateSet(H);
			vector<vector<int>>H2=H1;
			int combined_support=supportfinder(Fk);
			int p=0;
			//cout<<H1.size()<<"******\n";
			for(auto&it:H1){
				int consequent_support;
				double confidence;
				int rule_confidence;
				bool tempTruth = true;
				//cout<<rule_confidence<<"******\n";
				if(tempTruth){
					vector<int>precedent;
					int k=0;
					for(int i=0,j=0;i<it.size()&&j<Fk.size();j++){
						if(Fk[j]!=it[i]){
							precedent.push_back(Fk[j]);
						}
						else{
							i++;
						}
						k++;
					}
					for(int i=k;i<Fk.size();i++){
						precedent.push_back(Fk[i]);
					}
					consequent_support=supportfinder(precedent);
					confidence = (double)combined_support/consequent_support;
					rule_confidence=roundf(confidence*100);
					if(rule_confidence>min_conf){
						AssociationRules.push_back({precedent,it,confidence*100});
						tempTruth=false;
					}
				}
				else if(tempTruth){
					H2.erase(H2.begin()+p);
				}
				p++;
			}
			apgenRules(Fk,H2,AssociationRules);
		}
		else if(k==2){
			int combined_support=supportfinder(Fk);
			int conseq1=supportfinder(H[0]);
			int conseq2=supportfinder(H[1]);
			double confidence1 = (double)combined_support/conseq1;
			double confidence2 = (double)combined_support/conseq2;
			int rule_confidence1=roundf(confidence1*100);
			int rule_confidence2=roundf(confidence2*100);
			if(rule_confidence1>min_conf){
				AssociationRules.push_back({H[1],H[0],confidence1*100});
			}
			if(rule_confidence2>min_conf){
				AssociationRules.push_back({H[0],H[1],confidence2*100});
			}
		}
	}
	vector<vector<int>> genNextCandidateSet(vector<vector<int>>joiningSet){
		// we use F_k-1 X F_k-1 method in textbook
		vector<vector<int>>temp;
		level=joiningSet.size();
		if(level==0){
			//cout<<unique_cnt<<"***\n";
			for(int i=0;i<unique_cnt;i++){
				temp.push_back(vector<int>(1,i));
			}
			return temp;
		}
		level=joiningSet[0].size();
		for(int i=0;i<joiningSet.size()-1;i++){
			for(int j=i+1;j<joiningSet.size();j++){
				int check=0;
				for(int k=0;k<level-1;k++){
					if(joiningSet[i][k]!=joiningSet[j][k]){
						check=1;
						break;
					}
				}
				if(joiningSet[i][level-1]==joiningSet[j][level-1])check=1;
				if(check==0){
					int a,b,small,big;
					a=joiningSet[i][level-1];
					b=joiningSet[j][level-1];
					small=min(a,b);
					big=max(a,b);
					vector<int>curr;
					for(int k=0;k<level-1;k++){
						curr.push_back(joiningSet[i][k]);
					}
					curr.push_back(small);
					curr.push_back(big);
					temp.push_back(curr);
				}
			}
		}
		
		return temp;
	}
	vector<vector<int>> genQualifiedSet(){
		// return the qualified candidate set based on support count 
		vector<vector<int>>temp;
		for(auto&item:candidate_set){
			int item_freq=supportfinder(item);
			double val1=(double)item_freq/N;
			double item_support=roundf(val1*100);
			int val2=item_support;
			if(val2>=minsup)temp.push_back(item);
		}
		return temp;
	}
	int supportfinder(vector<int>item){
		//finds support for the item
		int cnt=0;
		for(auto&tid:transaction){
			bool check=false;
			if(item.size()>tid.size())continue;
			check=subset(item,tid);
			if(check)cnt++;
		}
		return cnt;
	}
	bool subset(vector<int>sub,vector<int>arr){
		// return true if subset
		for(int i=0,j=0;i<arr.size();i++){
			if(arr[i]==sub[j])j++;
			if(j==sub.size()){
				return true;
			}
		}
		return false;
	}
	void genMaximalAndClosedFrequentItemSet(){
		// geneartes maximal and closed frequnet set based on frequnet item set, also checks if there are any redundant association rules
		int freqSize=frequent_set.size();
		for(int i=0;i<frequent_set.size()-1;i++){
			vector<vector<int>>real_sub_set,unreal_sub_set;
			for(int j=0;j<frequent_set[i].size();j++){
				bool check=false;
				bool check_=false;
				bool check2=false;
				bool check_2=false;
				int lowerSupcount=supportfinder(frequent_set[i][j]);
				for(int k=0;k<frequent_set[i+1].size();k++){
					check=subset(frequent_set[i][j],frequent_set[i+1][k]);
					if(check){
						check_=true;
					}
					if(!check2&&check){
						int upperSupcount=supportfinder(frequent_set[i+1][k]);
						if(lowerSupcount==upperSupcount){
							check2=true;
						}
					}
					if(check2&&check_){
						break;
					}
				}
				if(!check_){
					maximalFrequentItemSet.push_back(frequent_set[i][j]);
				}
				if(!check2){
					real_sub_set.push_back(frequent_set[i][j]);
					closedFrequentItemSet.push_back(frequent_set[i][j]);
				}
				else{
					unreal_sub_set.push_back(frequent_set[i][j]);
					notClosedFrequentItemSet.push_back(frequent_set[i][j]);
				}
			}
			unreal_set.push_back(unreal_sub_set);
			real_set.push_back(real_sub_set);
		}
		vector<vector<int>>real_sub_set;
		for(int i=0;i<frequent_set[freqSize-1].size();i++){
			real_sub_set.push_back(frequent_set[freqSize-1][i]);
			maximalFrequentItemSet.push_back(frequent_set[freqSize-1][i]);
			closedFrequentItemSet.push_back(frequent_set[freqSize-1][i]);
		}
		real_set.push_back(real_sub_set);
	}
public:
	apriori(vector<vector<int>>Transaction,int minSup,int minConf,int unique_Cnt){
		// apriori constructor generates all the data at the time of object creation
		// calls frequent item set generator,Association rules generator
		level=0;
		transaction=Transaction;
		minsup=minSup;
		min_conf=minConf;
		unique_cnt=unique_Cnt;
		N=Transaction.size();
		genFreqItemSet();
		genMaximalAndClosedFrequentItemSet();
		genAssociationRules(real_set,associationRules);
		genAssociationRules(unreal_set,redundantAssociationRules);
	}
	vector<vector<vector<int>>> getFreqItemSet(){
		return frequent_set;
	}
	vector<tuple<vector<int>,vector<int>,double>> getAssociationRules(){
		return associationRules;
	}
	vector<vector<int>>getMaximalFrequentItemSet(){
		return maximalFrequentItemSet;
	}
	vector<vector<int>>getClosedFrequentItemSet(){
		return closedFrequentItemSet;
	}
	vector<vector<int>>getNotClosedFrequentItemSet(){
		return notClosedFrequentItemSet;
	}
	vector<tuple<vector<int>,vector<int>,double>> getRedundantAssociationRules(){
		return redundantAssociationRules;
	}
};