#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "boost/tokenizer.hpp"
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <sparsehash/dense_hash_map>
#include <sparsehash/sparse_hash_map>
using google::sparse_hash_map;
using google::dense_hash_map;
using namespace std;

struct result{
	int docID;
	short score;
	result(int documID,int docscore){
		docID = documID;
		score = docscore;
	}
};
struct doc{
	string docnm;
	int docID;
	unsigned long loc;
	doc(){} 
	doc(string docname){
		docnm = docname;
	}
	doc(string docname,int docid,unsigned long loca){
		docnm = docname;
		docID = docid;
		loc = loca;
	}
};
struct hit{
	int doc_ID;
	int loc;
	hit(int lc,int doc){
		doc_ID = doc;
		loc = lc;
	}
};

struct hit_list_normal{
	int docID;
	short count;
	vector<short> locs;
};
struct hit_list{
	vector<hit_list_normal> normal_hits;
	vector<int> anchor_hits;
};
struct lexobj{
	vector<hit> hits;
	lexobj(hit n){
		hits.push_back(n);
	}
	lexobj(){};
void addhit(int loc,int doc){
	hits.push_back(hit(loc,doc));
	}
};


class Indexer
{
private:
	string article_loc;											// Location of text file
	unordered_map<string,bool> com;								// Stop words (and, the , or etc..)
	dense_hash_map<string,unsigned long> hitlocation;			// hitlist location of words
	sparse_hash_map<string,lexobj> Lexicon;						// inverted index map
	dense_hash_map<string,vector<int>> shortIndex;				// Short index of article names
	void loadcoms();											// method to load stopwords
public:
	vector<doc> doclist;										// DocID, docname and location vector
	void input();												// doclist generator
	string returnarticle(int docID);							// Return article with docID
	hit_list returnhitlist(string str);							// Return Hitlist with word
	void set_loc(string str);									// Set article location
	Indexer(string str);										// Overloaded constructor
	Indexer(void);
	~Indexer(void);
	void BuildLexicon();										// Method to build lexicon
	vector<int> search(string str);								// Search algorithm implementation
};


int fast_atoi( const char * str );
unsigned long fast_ltoi( const char * str );
bool sort_count(hit_list_normal a,hit_list_normal b);
bool sort_doc(hit_list_normal a,hit_list_normal b);
bool resultscorecomp(result a,result b);
bool resultIDcomp(result a, result b);
