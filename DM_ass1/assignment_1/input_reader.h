#include <bits/stdc++.h>
using namespace std;
// header file which contains class input_reader which takes care of input and their mappings
class input_reader{
private:
	ifstream fin;
	string file_name;
	vector<vector<int>>transaction;
	unordered_map<string,int>mp;
	unordered_map<int,string>mp2;
	int unique_cnt;
	void parser(){
		string s;
		// parser function takes evry line in the file and seperates each string based on sepeartor ',' and maps each string'
		// to a unique id.
		// also modifies each transaction to unique ids
		while(getline(fin,s)){
			vector<int>temp;
			int start=0;
			int tool=0;
			//cout<<s<<endl;
			for(int i=0;i<s.length();i++){
				int end=i;
				if(s[i]!=','&&i!=s.length()-1)continue;
				if(i==s.length()-1){
					end++;
				}
				string item=s.substr(start,end-start);
				//cout<<item<<" ";
				start=i+1;
				if(mp.find(item)==mp.end()){
					mp[item]=unique_cnt;
					mp2[unique_cnt]=item;
					//cout<<item<<" "<<unique_cnt;
					temp.push_back(unique_cnt);
					unique_cnt++;
				}
				else{
					temp.push_back(mp[item]);
				}
			}
			transaction.push_back(temp);

			temp.clear();
		}
	}
public:
	input_reader(string fileName){
		// constructor which opens the file and calls parser
		unique_cnt=0;
		file_name=fileName;
		fin.open(file_name);
		if(fin.fail()){
			cout<<"Could not open the file";
			exit(0);
		}
		parser();
	}
	vector<vector<int>> getTransactions(){
		return transaction;
	}
	unordered_map<int,string> getMappings(){
		return mp2;
	}
	int getCountofUniqueItems(){
		return unique_cnt;
	}
};