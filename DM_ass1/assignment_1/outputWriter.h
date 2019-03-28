#include <bits/stdc++.h>
using namespace std;
// header file which contains class output writer works based oon the calling parameters
class outputWriter{
private:
    int cnt,support,confidence;
	unordered_map<int,string>mp;
public:
    outputWriter(unordered_map<int,string>MP,int supp,int conf){
        mp=MP;
        support=supp;
        confidence=conf;
    }
    void writeFrequentItemSet(vector<vector<vector<int>>>arr){
        string filename ="Frequent Item Set support = "+to_string(support)+"% .txt";
        ofstream fout(filename);
        cnt=1;
        for(auto&id1:arr){
			for(auto&id2:id1){
				cnt++;
			}
		}
        fout<<"No of frequent item set = "<<cnt<<endl;

        cnt=1;
		for(auto&id1:arr){
			fout<<"********"<<cnt++<<"set\n";
			for(auto&id2:id1){
				for(int i=0;i<id2.size();i++){
					fout<<mp[id2[i]]<<" ";
				}
				fout<<endl;
			}
		}
        fout.close();
    }
    void writeAssociationRules(string filename,vector<tuple<vector<int>,vector<int>,double>>arr){
        filename +="Association Rules support= "+to_string(support)+"% confidence = "+to_string(confidence)+"% .txt";
        ofstream fout(filename);
        fout<<"No of Association rules = "<<arr.size()<<endl;
        cnt=1;
		for(auto&id1:arr){
			fout<<cnt++<<".) ";
			vector<int>prec=get<0>(id1);
			vector<int>conseq=get<1>(id1);
			double confidence=get<2>(id1);
			fout<<"{ ";
			for(auto&id2:prec){
				fout<<mp[id2]<<", ";
				//cout<<endl;
			}
			fout<<"} -----> ";
			fout<<"{ ";
			for(auto&id2:conseq){
				fout<<mp[id2]<<", ";
				//cout<<endl;
			}
			fout<<"} confidence = "<<confidence<<endl;;
		}
        fout.close();
    }
    void writeSvector(vector<vector<int>>arr,string filename){
        string tempname=filename;
        filename+=" support = "+to_string(support)+"% .txt";
        ofstream fout(filename);
        fout<<tempname<<endl;
        for(int i=0;i<arr.size();i++){
			fout<<"{ ";
			for(int j=0;j<arr[i].size();j++){
				fout<<mp[arr[i][j]]<<", ";
			}
			fout<<" }";
			fout<<endl;
		}
        fout.close();
    }
};