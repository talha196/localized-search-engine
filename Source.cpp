
#include<iostream>
#include<boost/tokenizer.hpp>
#include<string>
#include <map>
#include <time.h>
#include "Indexer.h"
// #include <conio.h>
#include <stdio.h>
 using namespace std;
 

int main(){
	string asd;
	clock_t now = clock();
	Indexer news("C:\\SimpleWiki\\articles_in_plain_text.txt");  // Location of the simple text file
	cout << " TIME " << clock() - now ;
	vector<int> result;											// Result vector
	while(1){													// Interface for access
	system("cls");
	string art;
	cout << " Enter Search Term : " ;
	getline(cin,art);
	cout << art << endl;
	try{
		result = news.search(art);								// Searching method
		cout << " Results : " ;
		int lim = 20;
		int prevlim = 0;
		while(1){
			system("cls");
			cout << " Enter Search Term : " << art << "\n Results : \n";
			for (int x = prevlim ; x < lim && x < result.size(); x++)
				cout << "\t" << x+1 <<"#   "<< news.doclist[result[x]].docnm << endl;
			if ( lim == 20 ){
				if(20 < result.size())
				cout << " \n\n Press n to go to next page or x to enter the article ... " << endl;
				else
				cout << " Press x to continue ... " ;
				int c = getchar();
				if ( c == 'n'){
					prevlim +=20;
					lim += 20;
					continue;
				}
				else if(c == 'x')
					break;
				else
					continue;
			}
			else if ( lim > 20  ){
				if( lim <result.size())
				cout << " \n\n Press n to go to next page, p to go to previous one \n or x to enter the article ... " << endl;
				else
					cout << " \n\n Press p to go to previous one \n or x to enter the article ... " << endl;
				int c = getchar();
				if(lim < result.size())
				if ( c == 'n'){
					lim += 20;
					prevlim += 20;
					continue;
				}
				 if ( c == 'p'){
					lim -= 20;
					prevlim -=20;
					continue;
				}
				else if ( c == 'x')
					break;
			}

		}
		int docid;
		cout << "\n Enter the Id of the article : " ;
		cin >> docid;
		while(cin.fail() || docid < 1 || docid > result.size()){
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

			cout << "\n Enter valid ID : ";
			cin >> docid;
		}
		string temp  = news.returnarticle(result[docid-1]);
		system("cls");
		cout << temp;
		cout << "\n\n Press any key to continue ... ";
		std::getchar();
		getline(cin,temp);
	}
	catch(exception &excp){
		if (excp.what() == "Invalid ID"){
			cout << "Input is more than limit or less than 0 " << endl;
			continue;
			}
		else if(excp.what() == "Error word not found")
			cout << "Search item not found . Try a different search ";
			cout << " Press any key to continue ... " ;
			std::getchar();
			continue;
		}
	}
   cin.get();
   return 0;
}




