#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#define lint long unsigned int

//Custom class
struct Library {
  lint N; //number of books
  lint T; //signup days
  lint M; //ship rate

  vector<lint> books;
};

//Reads in data
void read_data(char* filename, lint& B, lint& L, lint& D,
                vector<lint>& books, vector<Library>& libs) {
  ifstream myfile (filename);
  string line;

  if (myfile.is_open()) {
    //Parameters
    getline(myfile,line);
    stringstream ss (line);
    ss >> B >> L >> D;

    //Books
    getline(myfile,line);
    ss << line;

    lint x;
    while (ss >> x) 
      books.push_back(x);

    //Libraries
    while (getline(myfile,line)) {
      Library lib;
      
      ss << line;
      ss >> lib.N >> lib.T >> lib.M;

      getline(myfile,line);

      ss << line;
      while (ss >> x)
        lib.books.push_back(x);
      
      libs.push_back(lib);
    }
  }
}


int main(int argc, char** argv) {
  //Initialize data
  char* filename = argv[1];
  lint B, L, D;
  vector<lint> books;
  vector<Library> libs;

  read_data(filename,B,L,D,books,libs);

  //B-number of different books
  //L-number of libraries
  //D-number of days

  return 0;
}