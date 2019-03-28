#include <bits/stdc++.h>
using namespace std;
class test{
private:
	int cnt;
	unordered_map<int,string>mp;
public:
	test(unordered_map<int,string>MP){
		mp=MP;
	}
	void printSVector(vector<vector<int>>arr){
		for(int i=0;i<arr.size();i++){
			cout<<"{ ";
			for(int j=0;j<arr[i].size();j++){
				cout<<mp[arr[i][j]]<<" ";
			}
			cout<<" }\n";
			cout<<endl;
		}
	}
	void printVector(vector<int>arr){
		for(int i=0;i<arr.size();i++){
			cout<<arr[i]<<" ";
		}
		cout<<endl;
	}
	void printSSVector(vector<vector<vector<int>>>arr){
		cnt=1;
		for(auto&id1:arr){
			cout<<"********"<<cnt++<<"set\n";
			for(auto&id2:id1){
				for(int i=0;i<id2.size();i++){
					cout<<mp[id2[i]]<<", ";
				}
				cout<<endl;
			}
		}
	}
	void printTupleVector(vector<tuple<vector<int>,vector<int>,double>>arr){
		cnt=1;
		for(auto&id1:arr){
			cout<<cnt++<<".) ";
			vector<int>prec=get<0>(id1);
			vector<int>conseq=get<1>(id1);
			double confidence=get<2>(id1);
			cout<<"{ ";
			for(auto&id2:prec){
				cout<<mp[id2]<<", ";
				//cout<<endl;
			}
			cout<<"} -----> ";
			cout<<"{ ";
			for(auto&id2:conseq){
				cout<<mp[id2]<<", ";
				//cout<<endl;
			}
			cout<<"} confidence = "<<confidence<<endl;;
		}
	}
};