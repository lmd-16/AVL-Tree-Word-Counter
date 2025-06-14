#include "Wordlist.h"
#include <iomanip>
#include <fstream>
using std::setprecision;
using std::fixed;
using std::ifstream;
using std::ofstream;

//Constructor for Wordlist
Wordlist::Wordlist() : root(nullptr) {}

//File constructor for Wordlist
//Takes filename as parameter

Wordlist::Wordlist(const std::string& fileName){
	root = nullptr;
	ifstream myFile;
	myFile.open(fileName.c_str());

	if(!myFile.is_open()){
		throw std::runtime_error("Unable to open file " + fileName);
	}
	if(myFile.peek() == EOF){
		throw std::invalid_argument("File is empty");
	}
	std::string next;
	while (myFile >> next){
		try{
			insert(next);
		}
		catch(const std::runtime_error& e){
			std::cerr << e.what() << ": "<< next << std::endl;
		}
	}
	

	myFile.close();
}

//Wordlist copy constructur 
//Takes wordlist object as parameter;


Wordlist::Wordlist(const Wordlist& other){
	size = 0;
	root = copyTree(other.root);
	

}

//Wordlist assignment operator
//Takes wordlist object as parameter
//returns assigned object

Wordlist& Wordlist::operator=(const Wordlist& other){
	if(this == &other){
		return *this;
	}
	deleteSubtree(root);

	root = copyTree(other.root);
	size = other.size;
	return *this;

}

//Copy constructor helper function
//Takes wordlist object and returns copied tree 
AVLTreeNode* Wordlist::copyTree(const AVLTreeNode* other){
	if(!other) return nullptr;
	AVLTreeNode* newnode = new AVLTreeNode(other->word);
	newnode->count = other->count;
	newnode->height = other->height;
	newnode->parent = nullptr;
	newnode->left = copyTree(other->left);
	newnode->right = copyTree(other->right);

	return newnode;
	

}

//Wordlist destructor
Wordlist::~Wordlist(){
	
	deleteSubtree(root);

}

//Wordlist destructor helper function
//Takes root as parameter and deletes subtrees
void Wordlist::deleteSubtree(AVLTreeNode* root){
	if(root == nullptr){
		return;
	}
	deleteSubtree(root->left);
	deleteSubtree(root->right);

	delete root;
}

//Wordlist helper function
//Traverses tree to find string passed as parameter
//return true if found
bool Wordlist::containsString(AVLTreeNode* root, std::string stringL)
{
	if(root == nullptr){
		return false;
	}
	if(root->word == stringL){
		return true;
	}
	if(root->word > stringL){
		return containsString(root->left, stringL); 
	}else{
		return containsString(root->right,stringL);
	}
}

//Wordlist height helper function
//Updates height of tree with the node passed as parameter

void Wordlist::updateHeight(AVLTreeNode* node){
	
	node->height = 1 + std::max(height(node->left), height(node->right));
	
}

//helper function for insertion
//Calculates the balance of the left and right subtrees for the node passed

int Wordlist::getBalance(AVLTreeNode* node)const {
	
	return height(node->left) - height(node->right);
	
}

//Helper function for insertion
//Carries out right rotation on the node passed as parameter
AVLTreeNode* Wordlist::rightRotate(AVLTreeNode* node){
	AVLTreeNode* x = node->left;
	node->left = x->right;

	if(x->right) x->right->parent =node;
	x->right = node;
	x->parent = node->parent;
	node->parent=x;

	updateHeight(node);
	updateHeight(x);
	return x;


}

//Helper function for insertion
//Carries out left rotation on the node passed as parameter
AVLTreeNode* Wordlist::leftRotate(AVLTreeNode* node){
	AVLTreeNode* y = node->right;
	node->right = y->left;
	if(y->left) y->left->parent = node;

	y->left = node;
	y->parent = node->parent;
	node->parent = y;

	updateHeight(node);
	updateHeight(y);
	return y;


}

//Helper function for insertion
//Carries out left right rotation on the node passed as parameter

AVLTreeNode* Wordlist:: leftrightRotate(AVLTreeNode* node){
	node->left = leftRotate(node->left);
	return rightRotate(node);

}

//Helper function for insertion
//Carries out right left rotation on the node passed as parameter

AVLTreeNode* Wordlist::rightleftRotate(AVLTreeNode* node){
	node->right = rightRotate(node->right);
	return leftRotate(node);

}

//Returns height of tree for node passed as parameter

int Wordlist::height(AVLTreeNode* node)const{
	if(node){
		return node->height;
	}
	return -1;

}

//Conducts in order traversal for printing values 
void Wordlist::inOrderTraversal(AVLTreeNode* root, int& counter) {
    if (root != nullptr) {
        inOrderTraversal(root->left, counter);  
        std::cout <<counter <<". "<< root->word << " " << root->count <<std::endl;  
		counter++;
        inOrderTraversal(root->right, counter); 
    }
}

//helper function for insertion of strings into tree while mainting BST property
//Takes root and string to be inserted as parameter 
AVLTreeNode* Wordlist::insertString(AVLTreeNode* root, const std::string& stringL) {
    if (root == nullptr) {
        return new AVLTreeNode(stringL);
    }

    if (stringL == root->word) {
        root->count++;  
        return root;  
    }else if(stringL < root->word) {
        root->left = insertString(root->left, stringL);
    }else {
        root->right = insertString(root->right, stringL);
    }


    updateHeight(root);

    int balance = getBalance(root);

    if (balance > 1 && stringL < root->left->word) {
        return rightRotate(root);
    }
    if (balance < -1 && stringL > root->right->word) {
        return leftRotate(root);
    }
    if (balance > 1 && stringL > root->left->word) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && stringL < root->right->word) {
		// std::cout<<" test 1" << std::endl;
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }


    return root;
}

//function to insert string into tree
void Wordlist::insert(std::string stringlist)
{

	root = insertString(root,stringlist);

	
}

//Helper function for removal 
//Calculates successor for removal of node passed
AVLTreeNode* Wordlist::inOrderSuccessor(AVLTreeNode* node){
	AVLTreeNode* curr = node->right;
	while(curr->left != nullptr){
		curr = curr->left;

	}
	return curr;

}

//Helper function for removal of string while mainting BST property
//Takes node and string to be removed as parameter

AVLTreeNode* Wordlist::removeString(AVLTreeNode* root, const std::string& stringlist){
	if(root == nullptr){
		return nullptr;
	}
	if(stringlist < root->word){
		root->left = removeString(root->left, stringlist);

	}else if(stringlist > root->word){
		root->right = removeString(root->right, stringlist);
	}else{
		if(root->left == nullptr && root->right == nullptr){
			delete root;
			return nullptr;

		}else if(root->left == nullptr){
			AVLTreeNode* temp = root->right;
			delete root;
			return temp;
		}else if(root->right == nullptr){
			AVLTreeNode* temp = root->left;
			delete root;
			return temp;
		}else{
			AVLTreeNode* successor = inOrderSuccessor(root);
			root->word = successor->word;
			root->right = removeString(root->right, successor->word);
		}
	}
	updateHeight(root);
	int balance = getBalance(root);




	if(balance > 1){
		if(getBalance(root->left) >= 0){
			return rightRotate(root);
		}else{
			root->left = leftRotate(root->left);
			return rightRotate(root);

		}
	}
	if(balance < -1){
		if(getBalance(root->right) <= 0){
			return leftRotate(root);

		}else{
			root->right = rightRotate(root->right);
			return leftRotate(root);
		}
	}

	return root;

}

//Removes string specified in parameter from tree
bool Wordlist::remove(std::string stringlist){
	if (!containsString(root, stringlist)) {
        return false; 
    }

    root = removeString(root, stringlist);

    return true;

}

//Helper function for getCount
//Returns count of string specified in parameter 

int Wordlist::getStringCount(AVLTreeNode* root, const std::string& stringlist) {
    if (root == nullptr) {
        return 0;
    }

    if (root->word == stringlist) {
        return root->count;
    }

   	if (stringlist < root->word) {
        return getStringCount(root->left, stringlist);
    } else {
        return getStringCount(root->right, stringlist);
    }
}

//Returns count of string passed as parameter
int Wordlist::getCount(const std::string& stringlist) {
    return getStringCount(root, stringlist);
}

//Checks if tree constains string specified as parameter
bool Wordlist::contains(std::string stringlist){
	if(getCount(stringlist) == 0){
		return false;
	}
		return true;
}

//Helper function for differentWords()

int Wordlist::uniqueWords(AVLTreeNode* root)const{
	if(root == nullptr){
		return 0;
	}
	int left = uniqueWords(root->left);
	int right = uniqueWords(root->right);
	return 1 + left + right;

}

//Returns count of unique words in tree
int Wordlist::differentWords()const{
	return uniqueWords(root);

}
//Helper function for totalWords()

int Wordlist::sumWords(AVLTreeNode* root)const{
	if (root == nullptr){
		return 0;
	}
	int left= sumWords(root->left);
	int right = sumWords(root->right);

	return root->count + left + right;
}

//Returns the total number of words in the tree
int Wordlist::totalWords()const{
	return sumWords(root);

}

//Helper function for mostFrequent()
std::string Wordlist::freqWords(AVLTreeNode* root, int& max, std::string& stringlist)const{
	if(root == nullptr){
		return "";
	}
	if(max < root->count){
		max = root->count;
		stringlist = root->word;
	}
	freqWords(root->left, max, stringlist);
	freqWords(root->right, max, stringlist);
	return stringlist;
	
}
//returns the count of the most frequent strings in the tree
std::string Wordlist::mostFrequent()const{
	if(root == nullptr){
		throw std::invalid_argument("Wordlist is empty");
	}

	int maxC = 0;
	std::string stringlist = " ";
	freqWords(root, maxC, stringlist);
	return stringlist + " " + std::to_string(maxC) ; 

}

//Helper function for singeltons()

int Wordlist::singletonsCount(AVLTreeNode* root)const{
	if(root == nullptr){
		return 0;
	}
	int left = singletonsCount(root->left);
	int right = singletonsCount(root->right);

	if(root->count == 1){
		return 1 + left + right;
	}
	return left + right;


}
//Returns the number of words with a count of 1 in the tree 
int Wordlist::singletons()const{
	return singletonsCount(root);

}

//Prints words in tree
void Wordlist::printWords(){
	int counter = 1;
	inOrderTraversal(root,counter);


}

void Wordlist::printStatistics() const
{
	std::cout << "Number of different words: " << differentWords() << endl;
	std::cout << "    Total number of words: " << totalWords() << endl;
	std::cout << "       Most frequent word: " << mostFrequent() << endl;
	std::cout << "     Number of singletons: " << singletons()
		<< setprecision(0) << fixed
		<< " (" << 100.0 * singletons() / differentWords() << "%)"
		<< endl;
}
