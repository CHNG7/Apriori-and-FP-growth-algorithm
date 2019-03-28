#include <bits/stdc++.h>
#include "input_reader.h"
#include "test.h"
using namespace std;
// generates frequent item set based on fptree algorithm
class FPTreeGrowth{
// generates frequne t item set based on fp growth algorithm
private:
    node*root;
    //unordered_map<int,int>itemMap;
    unordered_map<int,string>mp;
    vector<vector<int>>frequentItemSet;
    int uniqueCount,minSup;
    node* newNode(int data){
        node*temp= new node();
        temp->val=data;
        temp->cnt=0;
        temp->parent=NULL;
        temp->vis=false;
        return temp;
    }
    void builder(){
        // builder function which calls frequnetsetgen function
        for(int i=0;i<uniqueCount;i++){
           // test obj(root);
            node*copiedRoot=copyTree(root);
            set<int>possible;
            //test obj(copiedRoot);
           // cout<<"\n-----"<<endl;
            for(int j=0;j<i;j++)possible.insert(j);
            vector<int>prefix;
            FrequentSetGen(i,copiedRoot,prefix,possible);
           // cout<<"+++\n";
        }
    }
    void FrequentSetGen(int val,node*copiedRoot,vector<int>&prefix,set<int>possible){
        int supcnt=0;
        //cout<<"val = "<<val<<endl;
        bool found = prefixBuilder(copiedRoot,&supcnt,val);
        //test obj(copiedRoot);
       // cout<<val<<endl;
        if(!found)return;
        if(supcnt<minSup)return;
        prefix.push_back(val);
        frequentItemSet.push_back(prefix);
       // test obj(copiedRoot);
        
        conditionalFP(copiedRoot,possible,val);
        //cout<<"\n*****"<<val<<" size= "<<prefix.size()<<endl;
        //test obj(copiedRoot,mp);
        //cout<<endl;
        //set<int>iterPossible=possible;
        //copy copied Root here
        for(auto&it:possible){
            if(it>=val)continue;
            node*freqRoot=copyTree(copiedRoot);
            FrequentSetGen(it,freqRoot,prefix,possible);
        }
        prefix.pop_back();
    }
    node* copyTree(node*copiedRoot){
        // return root of a copied tree
        if(copiedRoot==NULL) return NULL;
        node*newRoot=newNode(copiedRoot->val);
        newRoot->cnt=copiedRoot->cnt;
        vector<node*>temp=copiedRoot->child;
        for(int i=0;i<temp.size();i++){
            node*copychild=copyTree(temp[i]);
            if(copychild==NULL)continue;
            newRoot->child.push_back(copychild);
        }
        return newRoot;
    }
    void conditionalFP(node*copiedRoot,set<int>&possible,int val){
        int leaf=val;
        unordered_map<int,int>supCount;
        updateSupport(copiedRoot,supCount);
        //cout<<"**++**\n";
        //test obj(copiedRoot,mp);
        removeInfrequent(copiedRoot,supCount,leaf,possible);
        //cout<<"**-------------------------**\n";
    }
    void removeInfrequent(node*copiedRoot,unordered_map<int,int>&supCount,int leaf,set<int>&possible){
        //preorderTraversal
        int val=copiedRoot->val;
        vector<node*>temp=copiedRoot->child;
        //cout<<"check3****"<<val<<endl;
		for(int i=0;i<temp.size();i++){
            int index=temp[i]->val;
            if(supCount[index]<minSup){
                //cout<<"check1"<<endl;
                copiedRoot->child.insert(copiedRoot->child.end(),temp[i]->child.begin(),temp[i]->child.end());
                temp.insert(temp.end(),temp[i]->child.begin(),temp[i]->child.end());
                copiedRoot->child.erase(copiedRoot->child.begin()+i);
                temp.erase(temp.begin()+i);
                possible.erase(index);
                i--;
                continue;
            }
            else if(index==leaf){
               // cout<<"check2\n";
                temp.erase(temp.begin()+i);
                copiedRoot->child.erase(copiedRoot->child.begin()+i);
                i--;
                continue;
            }
            removeInfrequent(temp[i],supCount,leaf,possible);
        }
    }
    void updateSupport(node*copiedRoot,unordered_map<int,int>&supCount){
        if(copiedRoot==NULL)return;
        vector<node*>temp=copiedRoot->child;
		for(int i=0;i<temp.size();i++){
            updateSupport(temp[i],supCount);
        }
        int sum=0;
        int val=copiedRoot->val;
        //if(val==-1)return;
        for(int i=0;i<temp.size();i++){
            sum+=temp[i]->cnt;
        }
        if(temp.size()!=0)copiedRoot->cnt=sum;
        
        if(supCount.find(val)==supCount.end())supCount[val]=0;
        supCount[val]+=copiedRoot->cnt;
    }
    bool prefixBuilder(node*copiedRoot,int*supcnt,int val){
        if(copiedRoot==NULL)return false;
        //cout<<"/n***"<<copiedRoot->val<<endl;
        int index=copiedRoot->val;
        vector<node*>temp=copiedRoot->child;
        if(index==val){
           // cout<<"check1"<<endl;
            copiedRoot->child.clear();
            *supcnt=*supcnt+copiedRoot->cnt;
            return true;
        }
        else if(index>val)return false;
        int check=0;
		for(int i=0;i<temp.size();i++){
            node*curr=temp[i];
            bool found = prefixBuilder(temp[i],supcnt,val);
            if(!found){
                //cout<<"check2"<<endl;
                copiedRoot->child.erase(copiedRoot->child.begin()+i);
                temp.erase(temp.begin()+i);
                delete curr;
                i--;
                continue;
            }
            if(found)check=1;
        }
        if(check==0)return false;
        return true;
    }
public:
    FPTreeGrowth(node*Root,int unique_cnt,int supcnt1,unordered_map<int,string>MP){
        mp=MP;
        root=Root;
        uniqueCount=unique_cnt;
        //itemMap= Itemmap;
        minSup=supcnt1;
        builder();
    }
    vector<vector<int>> getFrequentItemSet(){
        return frequentItemSet;
    }
};
class FPTreeConstruction{
// constructs the fp tree based on transactions
private:
    int unique;
    node*root;
    unordered_map<int,int>itemCount;
    vector<vector<int>>transaction;
    node* newNode(int data){
        node*temp= new node();
        temp->val=data;
        temp->cnt=0;
        temp->parent=NULL;
        temp->vis=false;
        return temp;
    }
    void constructTree(){
        for(auto&it:transaction){
            vector<int>temp=it;
            node*curr=root;
            for(int i=0;i<temp.size();i++){
                int val=temp[i];
                int check=0;
                if(itemCount.find(val)==itemCount.end())itemCount[val]=0;
                itemCount[val]++;
                vector<node*>child=curr->child;
                for(int j=0;j<child.size();j++){
                    if(child[j]->val==val){
                        check=1;
                        child[j]->cnt++;
                        curr=child[j];
                        break;
                    }
                }
                if(check==0){
                    node* createdNode=newNode(val);
                    createdNode->parent=curr;
                    createdNode->cnt++;
                    curr->child.push_back(createdNode);
                    curr=createdNode;
                }
            }
        }
    }
public:
    FPTreeConstruction(vector<vector<int>>Transaction,int unique_cnt){
        transaction=Transaction;
        for(auto&id:transaction){
            sort(id.begin(),id.end());
        }
        unique= unique_cnt;
        root = newNode(-1);
        constructTree();
    }
    node*getFPTree(){
        return root;
    }
    unordered_map<int,int> getitemCount(){
        return itemCount;
    }

};
int main(){
    // uses input reader class to handle input 
	input_reader inp("groceries.csv");
    int supcnt;
    cout<<"enter support count"<<endl;
    cin>>supcnt;
	vector<vector<int>>transaction=inp.getTransactions();
	int unique_cnt=inp.getCountofUniqueItems();
	unordered_map<int,string>MP=inp.getMappings();
	cout<<"unique items = "<<unique_cnt<<endl;
    // calls Fptree constuction to get fp tree of transactions
    FPTreeConstruction obj1(transaction,unique_cnt);
    node*root=obj1.getFPTree();
    //test obj4(root,MP);
    // calls fptree growth algo to get frequnet item sets
    FPTreeGrowth obj2(root,unique_cnt,supcnt,MP);
    vector<vector<int>>frequentItemSet=obj2.getFrequentItemSet();
    cout<<frequentItemSet.size()<<" = size"<<endl;
    // uses class test to ouput the frequen t item set on terminal
    test obj3(frequentItemSet,MP);
}
