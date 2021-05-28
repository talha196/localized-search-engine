#include "Indexer.h"
#include <iostream>
#include <exception>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
using namespace boost;



Indexer::Indexer(string str)
{
	shortIndex.set_empty_key("");							//Requirement for Dense Hashmaps
	hitlocation.set_empty_key("");
	loadcoms();												// Loading stopwords
	article_loc = str;
	ifstream inp("doc_list.txt");							
	string temp;
	string temp2;
	int c= 0;
	char_separator<char> sep("%");
	char_separator<char> sep2("-/!#,.|:()\"\n' ");
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	tokenizer tok(temp,sep);
	tokenizer tok2(temp2,sep2);
	if(inp.good()){
		getline(inp,temp);
	temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
	doclist.resize(lexical_cast<int>(temp));
		while(getline(inp,temp)){								// Parsing and filling doclist
			tok.assign(temp,sep);
			doclist[c].docID = fast_atoi((*tok.begin()).c_str());
			temp2 = *(++tok.begin());
			doclist[c].docnm = temp2;
			tok2.assign(temp2,sep2);
			for (auto it2 = tok2.begin() ;it2 != tok2.end() ; it2++)
				shortIndex[*it2].push_back(c);					// Parsing and filling short index
			doclist[c].loc = fast_ltoi((*(++(++(tok.begin())))).c_str());
			c++;
		//	cout << c++ << endl;
		}
	}
	else{
		input();												// Building doclist if not available	
		BuildLexicon();											// Building lexicon
	}
	ifstream lexicon("lexicon.txt");
	char_separator<char> sep3(",");
	while(getline(lexicon,temp)){								// Parsing and filling lexicon
		tok.assign(temp,sep3);
		hitlocation[*tok.begin()] = fast_ltoi((*(++tok.begin())).c_str());
	}
	
}

Indexer::~Indexer(void){}

string Indexer::returnarticle(int docID){
	if(docID < 0 || docID >doclist.size()-1 )
		throw std::invalid_argument("Invalid ID");
	ifstream docinput(article_loc);
	string article;
	string str;
	if( docID > 0 )
	docinput.seekg(doclist[docID].loc-1000);
	while(str != "#article: "+doclist[docID].docnm ){
	getline(docinput,str);
	to_lower(str);
	}
	//cout << str << endl ;
	article = article + str + "\n";
		getline(docinput,str);
		while( str.substr(0,10) != "#Article: " && docinput.good() ){
	//	cout << str << endl ;d
		article = article +str + "\n";
		getline(docinput,str);
	}
	return article;
}

void Indexer::input(){
	string locartc = article_loc;
	ifstream document("doc_list.txt");
	if(document.good())
		return;
	ofstream docmlist("doc_list.txt");
	docmlist<<"              \n";
	document.open(locartc);
	if(!document.good()){
		cout << " ERROR FILE LOCATION INVALID " ;
		return;
	}
	string tempdata;
	char ch[2];
	char tmp[8];
	int c = 0;
	int loc;
	char_separator<char> sep("/!#,.|:()\"\n' ");
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	tokenizer tok(tempdata,sep);
	while(document.good()){
		if(document.peek() != '\n'){
		document.read(ch,2);
		if(ch[0] == 35 &&  ch[1] == 65){
			document.read(tmp,8);
			getline(document,tempdata);
			loc = document.tellg();
			to_lower(tempdata);
			doclist.push_back(doc(tempdata));
			doclist[c].docID = c;
			doclist[c].loc = loc;
			docmlist << doclist[c].docID <<"%" << doclist[c].docnm << "%" << doclist[c].loc <<endl;
			tok.assign(tempdata,sep);
			for (auto it = tok.begin() ; it != tok.end() ; it++)
				shortIndex[*it].push_back(c);
			c++;
			}
		}
		document.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	}
	docmlist.seekp(0);
	docmlist<<c;
	docmlist.close();
}


void Indexer::set_loc(string str){
	article_loc = str;
}

void Indexer::BuildLexicon(){
	ifstream inp("lexicon.txt");
	ifstream inp2("INDEX.txt");
	if(inp.good()&&inp2.good()){
		inp.close();
		inp2.close();
		return;
	}
	ofstream obj("lexicon.txt");
	ofstream obj2("INDEX.txt");
	char_separator<char> sep("-/!#,.|:()\"\n' ");
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	string tmp;
	tokenizer tok(tmp,sep);
	for(int i = 0 ; i < doclist.size() ; i++){
		cout << i << endl;
		string temp = returnarticle(i);
		temp.erase(remove(temp.begin(), temp.end(), '\t'), temp.end());
		to_lower(temp);
		if(temp.size()){
			tok.assign(temp,sep);
		int location = 0;
		for ( tokenizer::iterator it = tok.begin() ; it != tok.end() ; it++){
			location++;
			if((*it).size() <= 2 || (com.find(*it) != com.end()))
				continue;
			string str = *it;
			Lexicon[str].hits.push_back(hit(location,i));
			}
		}
	}
	int i = 0;
	for(sparse_hash_map<string,lexobj>::iterator it = Lexicon.begin() ; it != Lexicon.end() ; it++){
		obj<<it->first<<","<<obj2.tellp()<<endl;
		hitlocation[it->first] = obj2.tellp();
		obj2<<it->first<<endl;
		for(int x = 0 ; x < it->second.hits.size() ; x++)
			obj2<<it->second.hits[x].doc_ID << "," <<it->second.hits[x].loc <<",";
		obj2<<endl;
		cout << i++ <<endl;
	}
	obj.close();
	obj2.close();
}

hit_list Indexer::returnhitlist(string str){
	hit_list temp;
	char_separator<char> sep(",");
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	string strtemp;
	if ( shortIndex.find(str) != shortIndex.end())
		temp.anchor_hits = shortIndex[str];
	if(hitlocation.find(str) != hitlocation.end()){
		ifstream input("INDEX.txt");
		input.seekg(hitlocation[str]);
		getline(input,strtemp);
		getline(input,strtemp);
		input.close();
		tokenizer tok(strtemp,sep);
		for ( auto it = tok.begin(); it != tok.end() ; it++){
			if(!temp.normal_hits.size()){
			hit_list_normal temp1;
			temp1.docID = fast_atoi(it->c_str());
			temp1.count = 1;
			temp1.locs.push_back(fast_atoi((++it)->c_str()));
			temp.normal_hits.push_back(temp1);
			}
			else
				if(fast_atoi(it->c_str()) == temp.normal_hits.back().docID){
					temp.normal_hits.back().count++;
					temp.normal_hits.back().locs.push_back(fast_atoi((++it)->c_str()));
				}
				else if(fast_atoi(it->c_str()) != temp.normal_hits.back().docID){
					hit_list_normal temp1;
					temp1.docID = fast_atoi(it->c_str());
					temp1.count = 1;
					temp1.locs.push_back(fast_atoi((++it)->c_str()));
					temp.normal_hits.push_back(temp1);
				}
		}
	}
	return temp;
}

void Indexer::loadcoms(){
	com.insert(pair<string,int>("the",1));
	com.insert(pair<string,int>("and",1));
	com.insert(pair<string,int>("that",1));
	com.insert(pair<string,int>("have",1));
	com.insert(pair<string,int>("for",1));
	com.insert(pair<string,int>("not",1));
	com.insert(pair<string,int>("with",1));
	com.insert(pair<string,int>("you",1));
	com.insert(pair<string,int>("this",1));
	com.insert(pair<string,int>("but",1));
	com.insert(pair<string,int>("from",1));
	com.insert(pair<string,int>("they",1));
	com.insert(pair<string,int>("saw",1));
	com.insert(pair<string,int>("she",1));
	com.insert(pair<string,int>("would",1));
	com.insert(pair<string,int>("there",1));
	com.insert(pair<string,int>("their",1));
	com.insert(pair<string,int>("what",1));
	com.insert(pair<string,int>("about",1));
	com.insert(pair<string,int>("which",1));
	com.insert(pair<string,int>("when",1));
	com.insert(pair<string,int>("like",1));
	com.insert(pair<string,int>("know",1));
	com.insert(pair<string,int>("take",1));
	com.insert(pair<string,int>("other",1));
	com.insert(pair<string,int>("than",1));
	com.insert(pair<string,int>("then",1));
	com.insert(pair<string,int>("only",1));
	com.insert(pair<string,int>("also",1));
	com.insert(pair<string,int>("after",1));
	com.insert(pair<string,int>("back",1));
	com.insert(pair<string,int>("how",1));
	com.insert(pair<string,int>("even",1));
	com.insert(pair<string,int>("because",1));
	com.insert(pair<string,int>("any",1));
	com.insert(pair<string,int>("these",1));
	com.insert(pair<string,int>("of",1));
	com.insert(pair<string,int>("it",1));
	com.insert(pair<string,int>("in",1));
	com.insert(pair<string,int>("on",1));
	com.insert(pair<string,int>("as",1));
	com.insert(pair<string,int>("it",1));
	com.insert(pair<string,int>("do",1));
	com.insert(pair<string,int>("at",1));
	com.insert(pair<string,int>("be",1));
}

vector<int> Indexer::search(string str){
	char_separator<char> sep("/!#,.|:()\"\n' ");
	typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
	to_lower(str);
	tokenizer tok(str,sep);
	vector<string> words;
	vector<hit_list> list;
	bool not_in_shortindex = 0;
	bool not_in_index = 0;
	for(auto it = tok.begin() ; it != tok.end() ; it++){
		if(it->size() >= 2 &&  (com.find(*it) == com.end()) )
		words.push_back(*it);
	}
	vector<result> title_score;
	vector<int> temptitle;
	int cnt = 0;
	for(int i = 0 ; i < words.size() ; i++){
		if(shortIndex.find(words[i]) != shortIndex.end()){
				vector<int> temp(shortIndex[words[i]]);
				for (int i = 0 ; i < temp.size() ; i++)
					temptitle.push_back(temp[i]);
		}
		else
			cnt++;
	}
	if(cnt == words.size())
		not_in_shortindex = 1;
	cnt = 0;
	sort(temptitle.begin() , temptitle.end());
	for(int i = 0 ; i < temptitle.size(); i++){
		if(title_score.empty() || title_score.back().docID != temptitle[i])
			title_score.push_back(result(temptitle[i],20));
		else if(title_score.back().docID == temptitle[i])
			title_score.back().score+=20;
	}
	temptitle.clear();
	sort(title_score.begin(),title_score.end(),resultscorecomp);
	dense_hash_map<int,vector<int>> docscore;
	docscore.set_empty_key(-1);
	int total_hits = 0;
	for ( int i = 0 ; i < words.size() ; i++){
			list.push_back(returnhitlist(words[i]));
			if(list.back().normal_hits.size() == 0 )
				cnt++;
			for(int x = 0 ; x < list.back().normal_hits.size() ; x++){
				if(list.back().normal_hits.size() > 1000){
					if(list.back().normal_hits[x].locs.size() > 2 )
				docscore[list.back().normal_hits[x].docID].push_back(i);
				}
				else
					docscore[list.back().normal_hits[x].docID].push_back(i);
			}
	}
	if(cnt == words.size())
		not_in_index = 1;
	if(not_in_index && not_in_shortindex){
		throw std::invalid_argument("Error word not found");
	}
	for(dense_hash_map<int,vector<int>>::iterator it = docscore.begin() ; it != docscore.end() ; it++){
		int score= 0;
		int lastlistloc = 0;
		int lastloc;
		for ( int i = 0 ; i < it->second.size() ; i++){
			int loc = 0;
			int listloc = it->second[i];
			while(list[listloc].normal_hits[loc].docID!= it->first)
				loc++;
			if(it->second.size() == 1){
				if(words.size() == 1){
					score = min((int)list[listloc].normal_hits[loc].count,35)*2; //30
					break;
				}
				else{
					score = min((int)list[listloc].normal_hits[loc].count,15);//20
					break;
				}
			}
			if(i == 0){
				lastlistloc = listloc;
				lastloc = loc;
				score += list[listloc].normal_hits[loc].count*1.5;//new
				continue;
			}
			else{
				score += list[listloc].normal_hits[loc].count*1.5;//new
				int size1 = list[listloc].normal_hits[loc].locs.size();
				int size2 = list[lastlistloc].normal_hits[lastloc].locs.size();
				int maxsize = max(size1,size2);
				float pointscore = 0;
				for ( int  x = 0 ; x < maxsize ; x++){
					if(x < size1 && x < size2)
						pointscore = pointscore+(25.0/(float)(abs(list[listloc].normal_hits[loc].locs[x]-list[lastlistloc].normal_hits[lastloc].locs[x])));//5.0/15/20/30
					else{
						score += (maxsize-x)*0.2;//0.2
						break;
					}
				}
				score += (int)pointscore;
			}
			lastlistloc = listloc;
			lastloc = loc;
		}
		title_score.push_back(result(it->first,score));
	}
	sort(title_score.begin(),title_score.end(),resultIDcomp);
	 vector<result> temp;
	 temp.push_back(*title_score.begin());
	 for(int i = 1 ; i < title_score.size() ; i++){
		 if(title_score[i].docID == temp.back().docID)
			 temp.back().score += title_score[i].score;
		 else
			 temp.push_back(title_score[i]);
	 }
	 sort(temp.rbegin(),temp.rend(),resultscorecomp);
	 vector<int> totalresult;
	 for(int x = 0 ; x < temp.size() ; x++)
		 totalresult.push_back(temp[x].docID);
	 /*for(int x =0 ; x <100 && x < totalresult.size(); x++)
		 cout << doclist[totalresult[x]].docnm << endl;*/
	 return totalresult;
}


int fast_atoi( const char * str )
{
    int val = 0;
    while( *str ) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}
unsigned long fast_ltoi( const char * str )
{
    unsigned long val = 0;
    while( *str ) {
        val = val*10 + (*str++ - '0');
    }
    return val;
}

bool sort_count(hit_list_normal a,hit_list_normal b){
	return a.count < b.count ;
}
bool sort_doc(hit_list_normal a,hit_list_normal b){
	return a.docID < b.docID;
}
bool resultscorecomp(result a,result b){
	return a.score < b.score;
}
bool resultIDcomp(result a, result b){
	return a.docID < b.docID;
}