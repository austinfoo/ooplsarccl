// ----------------------------
// projects/collatz/Collatz.c++
// Copyright (C) 2015
// Glenn P. Downing
// ----------------------------

// --------
// includes
// --------

#include <cassert>  // assert
#include <iostream> // endl, istream, ostream
#include <sstream>  // istringstream
#include <string>   // getline, string
#include <utility>  // make_pair, pair



using namespace std;

// ------------
// collatz_read
// ------------

pair<int, int> collatz_read (const string& s) {
    istringstream sin(s);
    int i;
    int j;
    sin >> i >> j;
    return make_pair(i, j);}

// ------------
// collatz_eval
// ------------

// The cycles cache
static const int cycles_cache_size = 1000000;
int cycles_cache[cycles_cache_size];

int collatz_eval (int i, int j)
{
  // Initialize variables
  int max_cycles = 0;
  for (int k = 0; k < cycles_cache_size; ++k) cycles_cache[k] = 0;
   
   // Tricky.  Make sure the inputs are ordered.
  if (i > j) std::swap(i, j);
  
  // Loop through the input range
  for (int k = i; k <= j; ++k) {
    int cycles = 1;
    int val = k;
      
    while (val != 1) 
    {
      // If val is in the cache then use the cached value and break out of the loop
	  if (val < cycles_cache_size && cycles_cache[val]) {
        cycles += cycles_cache[val] - 1;
        break;
      }
      
      // Compute one iteration of Collatz
      if (val & 1) {
	    val = 3 * val + 1;
      } else {
	    val = val / 2;
      }
      ++cycles;
    }
    
    // Save the number of cycles in the cache
    cycles_cache[k] = cycles;
    
    // Save the cycles if it is the max we have seen so far
    max_cycles = std::max (max_cycles, cycles);
  }
  
  return max_cycles;
}

// -------------
// collatz_print
// -------------

void collatz_print (ostream& w, int i, int j, int v) {
    w << i << " " << j << " " << v << endl;}

// -------------
// collatz_solve
// -------------

void collatz_solve (istream& r, ostream& w) {
    string s;
    while (getline(r, s)) {
        const pair<int, int> p = collatz_read(s);
        const int            i = p.first;
        const int            j = p.second;
        const int            v = collatz_eval(i, j);
        collatz_print(w, i, j, v);}}
// -------------------------------
// projects/collatz/RunCollatz.c++
// Copyright (C) 2015
// Glenn P. Downing
// -------------------------------

// --------
// includes
// --------

#include <iostream> // cin, cout



// ----
// main
// ----

int main () {
    using namespace std;
    collatz_solve(cin, cout);
    return 0;}

/*
% g++ -pedantic -std=c++11 -Wall Collatz.c++ RunCollatz.c++ -o RunCollatz



% cat RunCollatz.in
1 10
100 200
201 210
900 1000



% RunCollatz < RunCollatz.in > RunCollatz.out



% cat RunCollatz.out
1 10 1
100 200 1
201 210 1
900 1000 1



% doxygen -g
// That creates the file Doxyfile.
// Make the following edits to Doxyfile.
// EXTRACT_ALL            = YES
// EXTRACT_PRIVATE        = YES
// EXTRACT_STATIC         = YES



% doxygen Doxyfile
// That creates the directory html/.
*/
