# localized_search_engine

This project is a simplified implementation of google's orignal search algorithm for only single word searches: http://infolab.stanford.edu/~backrub/google.html

This project excludes crawling the web and the google's page rank method. Instead we use the simple wiki english only dataset dump as simplified search pool for this algorithm 
Download link for wikipedia database dumps: https://dumps.wikimedia.org/simplewiki/20210501/

The first step is to convert the simplewiki dataset from xml format to .txt format. This can be easily done using the wp2txt utility as follows. Download the latest simplewikii dataset and run the following commands:
```linux
sudo apt-get install rubygems
gem install wp2txt
wp2txt --input-file  simplewiki-yourversion-pages-articles.xml.bz2
cat * > simple_wiki_articles.txt
```
Two different kinds of hash maps are used in this implementation because of their different kind of optimal points. Installation of sparse hash maps libraries and boost c++ libraries are required on the system.
```C++
google::sparse_hash_map
google::dense_hash_map
```
To build lexicon the sparse hashmaps are used because of it efficient space complexity whereas the title words short index of title words and hit list are stored in dense hashmaps because of it constant time lookup. 

Once the text file is generated, the program first parses the whole text file to generate a doclist and the lexicons(words list) against it. It the generates a hit list against each document and creates a forward index. The forward index  is then sorted to create inverted index for search queries.


