#include <iostream>
#include <fstream>

using namespace std;

int main ( int argc, char* argv[]) {
  if(argc < 4) { cerr << "usage: plus arg1 arg2 arg3" << endl; return 1; }

  ifstream larg(argv[1]), rarg(argv[2]);
  double x, y;

  ofstream dest(argv[3]);

  while((larg >> x) && (rarg >> y)) { 
#ifdef PLUS
    dest << (x+y) << endl;     
#endif
#ifdef MINUS
    dest << (x-y) << endl;     
#endif
#ifdef MULT
    dest << (x*y) << endl;     
#endif
#ifdef DIV
    dest << (x/y) << endl;     
#endif
  }
  
  return 0;
}
