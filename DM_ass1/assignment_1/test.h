#include <bits/stdc++.h>
using namespace std;
struct node{
    int val,cnt;
	bool vis;
    vector<node*>child;
    node*parent;
};
class test{
private:
	int cnt;
	unordered_map<int,string>mp;
	void postorder(node*root){
		if(root==NULL)return;
		vector<node*>temp=root->child;
		for(int i=0;i<temp.size();i++){
			postorder(temp[i]);
		}
		cout<<mp[root->val]<<" ( "<<root->cnt<<" ) ";
	}
public:
	test(vector<vector<int>>arr,unordered_map<int,string>MP){
		mp=MP;
		for(int i=0;i<arr.size();i++){
			cout<<"{ ";
			for(int j=0;j<arr[i].size();j++){
				cout<<mp[arr[i][j]]<<", ";
			}
			cout<<" }"<<endl;
		}
	}
	test(vector<int>arr){
		for(int i=0;i<arr.size();i++){
			cout<<arr[i]<<" ";
		}
		cout<<endl;
	}
	test(vector<vector<vector<int>>>arr,unordered_map<int,string>MP){
		cnt=1;
		mp=MP;
		for(auto&id1:arr){
			cout<<"********"<<cnt++<<"set\n";
			for(auto&id2:id1){
				for(int i=0;i<id2.size();i++){
					cout<<mp[id2[i]]<<" ";
				}
				cout<<endl;
			}
		}
	}
	test(vector<tuple<vector<int>,vector<int>,double>>arr,unordered_map<int,string>MP){
		cnt=1;
		mp=MP;
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
	test(node*root,unordered_map<int,string>MP){
		mp=MP;
		postorder(root);
	}
};