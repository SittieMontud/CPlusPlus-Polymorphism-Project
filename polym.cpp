#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
   Node* next;
   Node* prev;
   int value;
   int key;
   Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
   Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
   
   protected: 
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};

class LRUCache:public Cache {
    private:
        Node* n;
        void remove_node(Node* node){
            //A<->B<->C
            if (node->prev != NULL) node->prev->next = node->next;
            else head=node->next;
            if (node->next != NULL) node->next->prev=node->prev;
            else tail=node->prev;
        }
        void add_to_head(Node* node){
            if (head==NULL){
                head=node;
                tail=node;
                node->next=NULL;
                node->prev=NULL;
            } else {
                //node<->head<->node2
                node->next=head;
                head->prev=node;
                head=node;
            }
        }
        void remove_tail(){
            Node *old_tail = tail;
            if (tail==NULL) {
                return;
            }
            else if (head==tail) {
                head=NULL;
                tail=NULL;
            } else {
                n=tail;
                tail=n->prev;
                tail->next=NULL;
            }
            delete old_tail;
        }
    public:
        void set(int key, int value) override {
            map<int,Node*>::iterator it = mp.find(key);
            if (it!=mp.end()){ //key exists
                mp[key]->value=value; 
                remove_node(mp[key]);
                add_to_head(mp[key]);
            } else { //key does not exist
                if (mp.size()==cp) { //checking if at capacity
                    n=tail;
                    mp.erase(n->key); //remove from map
                    remove_node(n);
                    delete n;
                } 
                n = new Node (key,value);
                add_to_head(n);
                mp.insert(make_pair(key,n));      
            }
        }
        int get(int key) override {
            map<int,Node*>::iterator it = mp.find(key);
            if (it!=mp.end()){
                n = mp[key];
                remove_node(n);
                add_to_head(n);
                return n->value;
            } else {
                return -1;
            }
        }
        //constructor
        LRUCache(int cap){ cp=cap; head=NULL; tail=NULL;}
};

int main() {
   int n, capacity,i;
   cin >> n >> capacity;
   LRUCache l(capacity);
   for(i=0;i<n;i++) {
      string command;
      cin >> command;
      if(command == "get") {
         int key;
         cin >> key;
         cout << l.get(key) << endl;
      } 
      else if(command == "set") {
         int key, value;
         cin >> key >> value;
         l.set(key,value);
      }
   }
   return 0;
}
