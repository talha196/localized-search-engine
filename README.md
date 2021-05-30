# localized_search_engine

This project is a simplified implementation of google's orignal search algorihm http://infolab.stanford.edu/~backrub/google.html

This proeject excludes crawling the web and the google's page rank method. Instead we use the simple wiki english only dataset as a database to this search algorithm https://dumps.wikimedia.org/simplewiki/20210501/

The first step is to convert the simplewiki dataset from xml format to .txt format. This can be easily done using the wp2txt utility as follows. Download the latest simplewikii dataset and run the following commands:
```linux
sudo apt-get install rubygems
gem install wp2txt
wp2txt --input-file  simplewiki-yourversion-pages-articles.xml.bz2
cat * > simple_wiki_articles.txt
```
Two different kinds of hash maps are used in this implementation because of their different kind of optimal points. Installation of the following libraries as well as boost libraries is required.
```C++
google::sparse_hash_map
google::dense_hash_map
```
