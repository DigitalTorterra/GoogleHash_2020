#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

#define lint long unsigned int

//Custom class
struct Library {
  lint N; //number of books
  lint T; //signup days
  lint M; //ship rate

  vector<lint> books;
  vector<lint> books_scanned;
};

bool sortbysec(const pair<lint,lint> &a, const pair<lint,lint> &b) {
  return (a.second > b.second);
}

//Reads in data
void read_data(char* filename, lint& B, lint& L, lint& D,
                vector<lint>& books, vector<Library>& libs) {
  ifstream myfile (filename);
  string line;

  if (myfile.is_open()) {
    //Parameters
    getline(myfile,line);
    stringstream ss1 (line);
    ss1 >> B >> L >> D;
    
    //Books
    getline(myfile,line);
    stringstream ss2 (line);

    lint x;
    while (ss2 >> x) {
      books.push_back(x);
    } 

    //Libraries
    while (getline(myfile,line)) {
      Library lib;
      
      stringstream ss3 (line);

      ss3 >> lib.N >> lib.T >> lib.M;


      getline(myfile,line);

      stringstream ss4 (line);

      while (ss4 >> x)
        lib.books.push_back(x);


      libs.push_back(lib);

    }
  }
}

//Calculate score for a given library
//If time, add sort
lint find_score(lint days_left, Library& lib, vector<bool>& shipped, vector<lint>& books) {
  //lib.books-list of book numbers
  vector<pair<lint,lint>> scores; //first is number, second is score
  for (lint book: lib.books) {
    pair<lint,lint> tmp(book,books[book]*(1-shipped[book]));
    scores.push_back(tmp);
  }


  sort(scores.begin(), scores.end(), sortbysec);
  lint num_books = (days_left-lib.T)*lib.M;

  lint score = 0;
  for (lint i=0; (i<num_books)&&(i<lib.books.size()); i++) {
    score += scores[i].second;
    shipped[scores[i].first] = 1;
  }


  return score;
}

//returns library #
long int find_next(lint days_left, lint& score, vector<bool>& shipped, 
  vector<bool>& used, vector<lint>& books, vector<Library>& libs) {
  
  lint max_score = 0;
  long int best_lib = -1;
  vector<bool> best_shipped = shipped;

  //iterate through libraries
  for (lint lib=0; lib<libs.size(); lib++) {
    if (used[lib])
      continue;


    //calculate the score
    vector<bool> current_shipped = shipped;
    lint score = find_score(days_left,libs[lib],current_shipped,books);

    if (score > max_score) {
      max_score = score;
      best_lib = lib;
      best_shipped = current_shipped;
    }
  }



  if (best_lib != -1) {
    vector<lint> final_used;
    for (lint i=0; i<best_shipped.size(); i++) {
      if (best_shipped[i]==1 && shipped[i] == 0) {
        final_used.push_back(i);
      }
    }

    libs[best_lib].books_scanned = final_used;
    shipped = best_shipped;
  }

  //update best_lib
  score += max_score;
  return best_lib;          
}


int main(int argc, char** argv) {
  //Initialize data
  char* filename = argv[1];
  lint B, L, D;
  vector<lint> books;   //all book scores
  vector<Library> libs; //all libraries
  vector<lint> log;     //order of libraries

  read_data(filename,B,L,D,books,libs);

  //B-number of different books
  //L-number of libraries
  //D-number of daysbn 
  lint queue = 0;
  vector<bool> shipped (B, 0);
  vector<bool> used (L, 0);
  lint score = 0;

  for (lint d=0; d<D; d++) {
    if (queue==0) {
      //call find_next
      lint days_left = D-d;
      long int next_lib = find_next(days_left,score,shipped,used,books,libs);

      if (next_lib==-1) 
        break;

      log.push_back(next_lib);
      used[next_lib] = 1;

      queue += libs[next_lib].T;
      
    }

    queue--;
  }

  //Write the data
  ofstream out_file (argv[2]);

  if (out_file.is_open()) {
    out_file << log.size() << endl;


    for (lint lib_num : log) {
      out_file << lib_num << " " << libs[lib_num].books_scanned.size() << endl;

      for (lint i : libs[lib_num].books_scanned)
        out_file << i << " ";
      out_file << endl;
    }
  }

  return 0;
}