#pragma once
#include <string>
#include <iostream>
using std::string;
using std::cin;
using std::cout;
using std::endl;

class AVLTreeNode {
public:
	AVLTreeNode* parent;
	AVLTreeNode* left;
	AVLTreeNode* right;
	int height;
	int count;
	std::string word;

	AVLTreeNode(std::string treeWord) : word(treeWord), parent(nullptr), left(nullptr), 
	right(nullptr), count(1), height(0) {}

};

// Wordlist class
class Wordlist
{
private:
	AVLTreeNode* root; // DO NOT REMOVE
	AVLTreeNode* left; 
	AVLTreeNode* right; 
	int val;
	unsigned int size;

	AVLTreeNode* copyTree(const AVLTreeNode* other);
	void deleteSubtree(AVLTreeNode* root);
	bool containsString(AVLTreeNode* root, std::string stringL);
	void updateHeight(AVLTreeNode* node);
	int getBalance(AVLTreeNode* node)const;
	AVLTreeNode* rightRotate(AVLTreeNode* node);
	AVLTreeNode* leftRotate(AVLTreeNode* node);
	AVLTreeNode* leftrightRotate(AVLTreeNode* node);
	AVLTreeNode* rightleftRotate(AVLTreeNode* node);
	int height(AVLTreeNode* node)const;
	void inOrderTraversal(AVLTreeNode* root, int& counter);
	AVLTreeNode* insertString(AVLTreeNode* root, const std::string& stringL);
	AVLTreeNode* inOrderSuccessor(AVLTreeNode* root);
	AVLTreeNode* removeString(AVLTreeNode* root, const std::string& stringlist);
	int getStringCount(AVLTreeNode* root, const std::string& stringlist);
	int uniqueWords(AVLTreeNode* root)const;
	int sumWords(AVLTreeNode* root)const;
	std::string freqWords(AVLTreeNode* root, int& max, std::string& stringlist)const;
	int singletonsCount(AVLTreeNode* root)const;

public:
	Wordlist();
	Wordlist(const std::string& fileName);
	Wordlist(const Wordlist& other);
	Wordlist& operator=(const Wordlist& other);
	~Wordlist();
	void insert(std::string stringlist);
	bool remove(std::string stringlist);
	int getCount(const std::string& stringlist);
	bool contains(std::string stringlist);
	int differentWords() const;
	int totalWords()const;
	std::string mostFrequent()const;
	int singletons()const;
	void printWords();
	void printStatistics()const;
	AVLTreeNode* getRoot() const { return root; }; // DO NOT REMOVE

};