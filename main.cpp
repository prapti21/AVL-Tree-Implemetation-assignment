// Implementation of AVL tree in CPP

#include<iostream>
#include<vector>
#include<string>
#include <fstream>

using namespace std;

//  Node structure for the AVL Tree
class AVLNode
{
	public:
	int key;
	AVLNode *left_Node;
	AVLNode *right_Node;
	int height;
};

// To obtain the maximum of two numbers.
int max(int number1, int number2){
	return (number1 > number2)? number1 : number2;
}


// To obtain the height of the tree rooted at node

int getHeightOfTree(AVLNode *node)
{
	if (node == NULL) return 0;
	return node->height;
}

//Function to create node with given key and make its children NULL
AVLNode* newNode(int key)
{
	AVLNode* new_node = new AVLNode();
	new_node->key = key;
	new_node->left_Node = NULL;
	new_node->right_Node = NULL;
	new_node->height = 1; // New node will be added to the leaf with the given key and its both child as NULL
	return(new_node);
}


// Function for performing the right rotation of subtree with its root rotateNode
AVLNode *rotateRight(AVLNode *rotateNode)
{
	AVLNode *leftNode = rotateNode->left_Node;
	AVLNode *Temp = leftNode->right_Node;

	// Performing the rotation
	leftNode->right_Node = rotateNode;
	rotateNode->left_Node = Temp;

	// Updating the heights
	rotateNode->height = max(getHeightOfTree(rotateNode->left_Node),getHeightOfTree(rotateNode->right_Node)) + 1;
	leftNode->height = max(getHeightOfTree(leftNode->left_Node),getHeightOfTree(leftNode->right_Node)) + 1;

	// Returning the new root of the subtree
	return leftNode;
}

// Function for performing the left rotation of subtree with its root rotateNode
AVLNode *rotateLeft(AVLNode *rotateNode)
{
	AVLNode *rightNode = rotateNode->right_Node;
	AVLNode *Temp = rightNode->left_Node;

	// Performing the rotation
	rightNode->left_Node = rotateNode;
	rotateNode->right_Node = Temp;

	// Updating the heights
	rotateNode->height = max(getHeightOfTree(rotateNode->left_Node),getHeightOfTree(rotateNode->right_Node)) + 1;
	rightNode->height = max(getHeightOfTree(rightNode->left_Node),getHeightOfTree(rightNode->right_Node)) + 1;

	// Returning the new root of the subtree
	return rightNode;
}

// To get the balance_factor factor of the node
int getBalanceFactor(AVLNode *Node)
{
	if (Node == NULL)
		return 0;
	return getHeightOfTree(Node->left_Node) - getHeightOfTree(Node->right_Node);
}


//Function to insert node into AVL tree rooted at node with given key
AVLNode* insert(AVLNode* root, int key_value)
{
	//Perform basic BST insertion 
	if (root == NULL)
		return(newNode(key_value));

	if (key_value < root->key)
		root->left_Node = insert(root->left_Node, key_value);
	else if (key_value > root->key)
		root->right_Node = insert(root->right_Node, key_value);
	else // Here the key is already present so no duplicate insertion is done
		return root;

	// Update heights of ancestor nodes 
	root->height = 1 + max(getHeightOfTree(root->left_Node), getHeightOfTree(root->right_Node));

	// Check whether tree has become unbalance using balance_factor factor of ancestor node
	int balance_factor = getBalanceFactor(root);


	// There are four possible cases in case the tree is unbalanced
	// Left Left Case (LL Rotation)
	if (balance_factor > 1 && key_value < root->left_Node->key)
		return rotateRight(root);

	// Left Right Case (LR Rotation)
	if (balance_factor > 1 && key_value > root->left_Node->key)
	{
		root->left_Node = rotateLeft(root->left_Node);
		return rotateRight(root);
	}

	// Right Right Case(RR Rotation)
	if (balance_factor < -1 && key_value > root->right_Node->key)
		return rotateLeft(root);

	
	// Right Left Case (RL Rotation)
	if (balance_factor < -1 && key_value < root->right_Node->key)
	{
		root->right_Node = rotateRight(root->right_Node);
		return rotateLeft(root);
	}

	// return the root node 
	return root;
}

// To find the smalllest node from the tree 
AVLNode * minValueNode(AVLNode* temp)
{
	AVLNode* current_node = temp;

	while (current_node->left_Node != NULL)
		current_node = current_node->left_Node;

	return current_node;
}

// Function to perform deletion of node with given key and return the root
AVLNode* deleteNode(AVLNode* root, int key_value)
{
	
	// Perform basic BST delete operation
	if (root == NULL)
		return root;

	// In case the key is less than root->key then perform search in left subtree
	if ( key_value < root->key )
		root->left_Node = deleteNode(root->left_Node, key_value);

	//In case the key is greater than root->key then perform search in right subtree
	else if( key_value > root->key )
		root->right_Node = deleteNode(root->right_Node, key_value);

	// else delete the node 
	else
	{
		// Node with either one child or no child
		if( (root->left_Node == NULL) || (root->right_Node == NULL) )
		{
			AVLNode *temp_node = root->left_Node ? root->left_Node : root->right_Node;

			// In case node has no children
			if (temp_node == NULL)
			{
				temp_node = root;
				root = NULL;
			}
			else // Node with just one child
			*root = *temp_node; // Copy the contents of the child 
			delete(temp_node);
		}
		else
		{
			// In case the node has two children 
			// Get the minimum possible node in Right subtree and delete it from its previous place
			AVLNode* temp_node = minValueNode(root->right_Node);
			root->key = temp_node->key;

			root->right_Node = deleteNode(root->right_Node, temp_node->key);
		}
	}

// In case the tree has just one node then delete the node
	if (root == NULL)
	return root;

	// Update height of the root
	root->height = 1 + max(getHeightOfTree(root->left_Node), getHeightOfTree(root->right_Node));

	// Check whether the tree has become unbalanced
	int balance_factor = getBalanceFactor(root);
	
	// In case of imbalane there are four cases

	// Left Left Case(LL Rotation)
	if (balance_factor > 1 && getBalanceFactor(root->left_Node) >= 0)
		return rotateRight(root);

	// Right Right Case(RR Rotation)
	if (balance_factor < -1 && getBalanceFactor(root->right_Node) <= 0)
		return rotateLeft(root);

	// Left Right Case(LR Rotation)
	if (balance_factor > 1 && getBalanceFactor(root->left_Node) < 0)
	{
		root->left_Node = rotateLeft(root->left_Node);
		return rotateRight(root);
	}
	
	// Right Left Case(RL Rotation)
	if (balance_factor < -1 && getBalanceFactor(root->right_Node) > 0)
	{
		root->right_Node = rotateRight(root->right_Node);
		return rotateLeft(root);
	}

	return root;
}

//To perform search key operation on the tree
string search( AVLNode* root, int targetkey)
{
    // continue traversing until root reaches the dead end
    while (root != NULL) {
        // passing the right subtree as new tree
        if (targetkey > root->key)
            root = root->right_Node;
 
        // passing the left subtree as new tree
        else if (targetkey < root->key)
            root = root->left_Node;
        else
            return to_string(root->key); // if the key is found return 1
    }
    return "NULL";
}

// To search the keys between range key1 and key2
void search(AVLNode *root, int key1, int key2, vector<int>& temp){
	if(root==NULL) return;
	search(root->left_Node,key1,key2,temp);
	if(key1<=root->key && root->key<=key2)
	{
		temp.push_back(root->key);
		return;
	}
	search(root->right_Node,key1,key2,temp);
}


// Main function
int main(int argc, char** argv)
{
	// Use command line arguements to get the input file
	ofstream output_handler;
	output_handler.open ("output_file.txt");
	AVLNode *root;
	ifstream input_handler;
	input_handler.open(argv[1]);
	string stringInput;
	if (input_handler.is_open()) {
	while (input_handler) { 
		getline (input_handler, stringInput);
		string op= stringInput.substr(0,stringInput.find('('));
		// Operations to be performed are initialize, insert, delete and search
		if(op=="Initialize"){
				root = NULL;
		}
		else if(op=="Insert"){
			string num = stringInput.substr(stringInput.find('(')+1,stringInput.find(')')-stringInput.find('(')-1);
			int number=stoi(num);
			root= insert(root,number);
		}
		else if(op=="Delete"){
			string num = stringInput.substr(stringInput.find('(')+1,stringInput.find(')')-stringInput.find('(')-1);
			int nr=stoi(num);
			root=deleteNode(root, nr);
		}
		else if(op=="Search"){
			string targetkey = stringInput.substr(stringInput.find('(')+1,stringInput.find(')')-stringInput.find('(')-1);
			if (op== "Search" && targetkey.find(',')==-1){
				int num=stoi(targetkey);
				output_handler<< search(root,num)<<"\n";	
			}
			if (op== "Search" && targetkey.find(',')!=-1){
						string num1= targetkey.substr(0,targetkey.find(','));
						int n1=stoi(num1);
						string num2= targetkey.substr(targetkey.find(',')+1);
						int n2=stoi(num2);
						vector<int> temp;
						string res="";
						search(root,n1,n2,temp);
						if(temp.size()==0){ 
							output_handler<<"NULL\n";
						}
						else{
						for(int i=0;i< temp.size();i++){
							if(i!=temp.size()-1){
								res+=to_string(temp[i])+",";
								}
							else {
								res+=to_string(temp[i]);
							}
							}
							output_handler<< res<<"\n";
						}
		}
		}
	}
	}
	else {
	output_handler << "There is no input file provided\n";
	}
	input_handler.close();
	output_handler.close();
	return 0;
}