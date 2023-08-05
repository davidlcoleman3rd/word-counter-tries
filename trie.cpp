#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <string>


template <class T>                                          //Node class.  Semi-atomic "element" of the trie
class Node{
public:

//******
    Node() {
        parent = nullptr;
        depth = 0;
        count = 0;
    }

//******
	Node(T input) {                                         //Creates a new parent node of the super-structure
		data = input;
		parent = nullptr;
		depth = 0;
		count = 0;
	}

//******
	Node(T dataIn, Node* parentIn, int depthIn) {           //Creates a new node within the super-structure
		data = dataIn;
		SetParent(parentIn);
		parent->SetChild(this);
		SetDepth(depthIn);
		count = 0;
	}

//******
	void SetData(T input) {                                 //Sets the data of the node
		data = input;
	}

//******
	void NewChild(Node* nodeIn, T input) {                  //Creates a new child of the node
		nodeIn = new Node(input, this, (depth + 1));
	}

//******
	void SetChild(Node* inNode) {                           //Sets a new child of the node
		children.push_back(inNode);
	}

//******
	T GetData(){                                            //Retreive and return the data of the node to the calling function
		return data;
	}

//******
	Node* GetParent() {                                     //Gets the parent of the current node and returns to caller
		return parent;
	}

//******
	void SetParent(Node* inNode) {                          //Sets the parent of the current node
		parent = inNode;
	}

//******
	Node* GetChild(int inPos) {                             //Gets a particular child of the current node
		return children.at(inPos);
	}

//******
	int GetChildCount() {                                   //Gets the child count of the current node
		return children.size();
	}

//******
    void PopChild() {
        //std::cout << data << "\n\n";
        //std::cout << "\n\nTest again\n\n";
        //std::cout << "\n\nChild size:  " << children.size() << "\n\n";
        children.erase(children.end() - 1);
    }

//******                                                    //Returns all children in vector form to the caller
	std::vector<Node*> GetChildVector() {
		return children;
	}
//******
	int GetDepth(){                                         //Returns the depth of the node to the caller
		return depth;
	}

//******
    void IncrementCount() {                                 //Increments the node's count variable
        count++;
    }

//******
    int GetCount() {                                        //Returns the amount of times the node has been counted to the caller
        return count;
    }

//******
    void ResetCount() {                                     //Resets the count back to zero
        count = 0;
    }

//******
	void SetDepth(int input) {                              //Sets the depth of the child
		depth = input;
	}

//******
private:
	Node* parent;                       //The node's parent node in the super-structure
	std::vector<Node*> children;        //The node's children structure in the super-structure
	T data;                             //The value of the node
	int depth;                          //How deep the node is
	int count;                          //How many times this value occurs in the program
};

//******************************************

template <class T>                                      //Trie.  Is a tree of individual elements that form an overarching structure.  Can be counted to see how many times each item is iterated
class Trie {
public:

	//******
	Trie() {                                            //Creates a new trie
		root = new Node<T>;
		curr = root;
		next = root;
	}

    //******
    Trie(Trie& otherTrie) {                             //Creates a new trie by copying an old one
        curr = root;
        otherTrie.StartTraversal();
        root = new Node(otherTrie.GetCurr()->GetData());
        CopyLevel(otherTrie.GetCurr(), curr);

    }

    //******
    void operator=(const Trie& originalTrie) {          //Overload the '=' operator with the copy constructor above

    }

    //******
    Node<T>* GetCurr() {                                //Return the currently selected node to the caller
        return curr;
    }

    //******
    void StartTraversal() {                             //Starts traversal for the trie by setting curr to the root node
        curr = root;
    }

	//******
	void AddNode(T input) {                             //Adds a new node to the trie
		curr->NewChild(next, input);
		next = nullptr;
	}

	//******
	void AddNode(std::vector<T> input) {
        prev = curr;
        StartTraversal();
        for (int iter = 0; iter < input.size(); iter++) {
            for (int next = 0; next <= curr->GetChildCount(); next++) {
                if (next < curr->GetChildCount()) {
                    if (input.at(iter) == curr->GetChild(next)->GetData()) {
                        prev = curr;
                        curr = curr->GetChild(next);
                        next = curr->GetChildCount() + 1;
                        std::cout << "\n" << curr->GetData();
                    }
                } else {
                    Node<T>* tempPtr = new Node<T>(input.at(iter));
                    curr->SetChild(tempPtr);
                    prev = curr;
                    curr = curr->GetChild(next);
                    curr->SetParent(prev);
                    next = curr->GetChildCount() + 1;
                    std::cout << "\n" << curr->GetData();
                }
            }
        }
        curr->IncrementCount();
        prev = curr;
	}

    //******
    void IterateNode() {                                //Increments the current node's count by 1
        curr->IncrementCount();
    }

    //******
    int GetNodeCount() {                                //Returns the current node's count to the caller
        return curr->GetCount();
    }

	//******
	void MoveDown(int childPos) {                       //Move deeper into the tree by moving curr to a specific position deeper into the node
		curr = curr->GetChild(childPos);

	}

	//******
	void MoveUp() {                                     //Move shallower from the tree by moving curr to the parent node
		curr = curr->GetParent();
	}

    //******
    void DepthFirstPrint() {                                    //Prints the tree on a "string by string" basis by performing a depth-first search to find each word

        std::cout << "\n\n\n";
        Trie<T>* temp = this;
        for (int iter = root->GetChildCount() - 1; iter >= 0; iter--) {
            StartTraversal();
            std::vector<T> word;
            while (curr != nullptr) {
                word.push_back(GetNext(iter));

            }
            if (prev != root) {
                if (prev->GetCount() > 0) {
                        std::string outputStr = "";
                        for (auto characters : word) {
                            outputStr = outputStr + characters;
                        }
                        std::cout << outputStr << " - " << prev->GetCount();
                        std::cout << "\n";
                        prev->ResetCount();
                } else {
                    //std::cout << "\n\nData = " << prev->GetData();
                    //std::cout << "\nIter = " << iter << "\n\n";
                    Node<T>* tempPtr = prev->GetParent();
                    tempPtr->PopChild();
                    prev = nullptr;
                }
                iter++;
            }
        }
    }

    //******
    T GetNext(int iter) {                                        //Gets the next element of the tree and returns it to the caller - sets curr to the next value and sets prev to the previous curr value
        prev = curr;
        if (curr->GetChildCount() > 0) {
            curr = curr->GetChild(curr->GetChildCount() - 1);
        } else {
            curr = nullptr;
        }
        return prev->GetData();
    }


	//******
private:
	Node<T>* root;
	Node<T>* curr;
	Node<T>* prev;
	Node<T>* next;

};


//******************************************


int main() {

	std::string myString[5] = {"pizza", "pasta", "pizza", "apples", "soup"};

	Trie<char> myTrie;

	for (auto word : myString) {
        std::cout << "\n\n" << word << "\n\n";
        std::vector<char> temp;
        for (auto letter : word) {
            temp.push_back(letter);
        }
        myTrie.AddNode(temp);
	}

    myTrie.DepthFirstPrint();

	return 0;
}
