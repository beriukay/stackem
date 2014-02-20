/* Paul Gentemann
 * CS 411
 * File Name : stackem.h
 * Description : Header for stackem.cpp
 */

#ifndef STACKEM_H
#define STACKEM_H

#include <algorithm>    // for std::sort
#include <utility>      // for std::pair
#include <vector>
using std::pair;    
using std::vector;

typedef vector<int> Block;
typedef vector<Block> Tower;
typedef pair<Block*, int > Pair; // For auto iterators and index lookup.
typedef vector <Pair> Matrix;

int stackEm(const Tower &);
int stackEmHelper(const Tower &);
bool canStack(const Block &, const Block &);
bool baseComp(const Block &, const Block &);

#endif 

