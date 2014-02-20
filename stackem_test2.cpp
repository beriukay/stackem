// stackem_test2.cpp
// Glenn G. Chappell
// 22 Nov 2013
//
// For CS 411/611 Fall 2013
// Test program for function stackEm
// Large Problems Added
// Used in Assignment 5, Exercise A

// Includes for code to be tested
#include "stackem.h"    // For function stackEm
#include "stackem.h"    // Double inclusion test

// Includes for testing package & code common to all test programs
#include <iostream>     // for std::cout, std::endl, std::cin
#include <string>       // for std::string
#include <stdexcept>    // for std::runtime_error

// Additional includes for this test program
#include <vector>       // for std::vector


// ************************************************************************
// Testing Package:
//     Class Tester - For Tracking Tests
// ************************************************************************


// class Tester
// For extremely simple unit testing.
// Keeps track of number of tests and number of passes.
// Use test (with success/failure parameter) to do a test.
// Get results with numTests, numPassed, numFailed, allPassed.
// Restart testing with reset.
// Invariants:
//     countTests_ == number of tests (calls to test) since last reset.
//     countPasses_ == number of times function test called with true param
//      since last reset.
//     0 <= countPasses_ <= countTests_.
//     tolerance_ >= 0.
class Tester {

// ***** Tester: ctors, dctor, op= *****
public:

    // Default ctor
    // Sets countTests_, countPasses_ to zero, tolerance_ to given value
    // Pre: None.
    // Post:
    //     numTests == 0, countPasses == 0, tolerance_ == abs(theTolerance)
    // Does not throw (No-Throw Guarantee)
    Tester(double theTolerance = 0.0000001)
        :countTests_(0),
         countPasses_(0),
         tolerance_(theTolerance >= 0 ? theTolerance : -theTolerance)
    {}

    // Compiler-generated copy ctor, copy op=, dctor are used

// ***** Tester: general public functions *****
public:

    // test
    // Handles single test, param indicates pass/fail
    // Pre: None.
    // Post:
    //     countTests_ incremented
    //     countPasses_ incremented if (success)
    //     Message indicating test name (if given)
    //      and pass/fail printed to cout
    // Does not throw (No-Throw Guarantee)
    //  - Assuming exceptions have not been turned on for cout.
    void test(bool success,
              const std::string & testName = "")
    {
        ++countTests_;
        if (success) ++countPasses_;

        std::cout << "    ";
        if (testName != "")
        {
            std::cout << "Test: "
                      << testName
                      << " - ";
        }
        std::cout << (success ? "passed" : "********** FAILED **********")
                  << std::endl;
    }

    // ftest
    // Does single floating-point test.
    // Tests passes iff difference of first two values is <= tolerance.
    // Pre: None.
    // Post:
    //     countTests_ incremented
    //     countPasses_ incremented if (abs(val1-val2) <= tolerance_)
    //     Message indicating test name (if given)
    //      and pass/fail printed to cout
    // Does not throw (No-Throw Guarantee)
    void ftest(double val1,
               double val2,
               const std::string & testName = "")
    { test(val1-val2 <= tolerance_ && val2-val1 <= tolerance_, testName); }

    // reset
    // Resets *this to default constructed state
    // Pre: None.
    // Post:
    //     countTests_ == 0, countPasses_ == 0
    // Does not throw (No-Throw Guarantee)
    void reset()
    {
        countTests_ = 0;
        countPasses_ = 0;
    }

    // numTests
    // Returns the number of tests that have been done since last reset 
    // Pre: None.
    // Post:
    //     return == countTests_
    // Does not throw (No-Throw Guarantee)
    int numTests() const
    { return countTests_; }

    // numPassed
    // Returns the number of tests that have passed since last reset
    // Pre: None.
    // Post:
    //     return == countPasses_
    // Does not throw (No-Throw Guarantee)
    int numPassed() const
    { return countPasses_; }

    // numFailed
    // Returns the number of tests that have not passed since last reset
    // Pre: None.
    // Post:
    //     return + countPasses_ == numTests_
    // Does not throw (No-Throw Guarantee)
    int numFailed() const
    { return countTests_ - countPasses_; }

    // allPassed
    // Returns true if all tests since last reset have passed
    // Pre: None.
    // Post:
    //     return == (countPasses_ == countTests_)
    // Does not throw (No-Throw Guarantee)
    bool allPassed() const
    { return countPasses_ == countTests_; }

    // setTolerance
    // Sets tolerance_ to given value
    // Pre: None.
    // Post:
    //     tolerance_ = abs(theTolerance)
    // Does not throw (No-Throw Guarantee)
    void setTolerance(double theTolerance)
    { tolerance_ = (theTolerance >= 0 ? theTolerance : -theTolerance); }

// ***** Tester: data members *****
private:

    int countTests_;    // Number of tests done since last reset
    int countPasses_;   // Number of tests passed since last reset
    double tolerance_;  // Tolerance for floating-point near-equality tests

};  // end class Tester


// ************************************************************************
// Testing Package:
//     Class TypeCheck - Helper Class for Type Checking
// ************************************************************************


// class TypeCheck
// This class exists in order to have static member function check, which
// takes a parameter of a given type, by reference. Objects of type
// TypeCheck<T> cannot be created.
// Usage:
//     TypeCheck<MyType>::check(x)
//     returns true if the type of x is (MyType) or (const MyType),
//     otherwise false.
// Invariants: None.
// Requirements on Types: None.
template<typename T>
class TypeCheck {

private:

    // Uncopyable class. Do not define copy ctor, copy assn.
    TypeCheck(const TypeCheck &);
    TypeCheck<T> & operator=(const TypeCheck &);

    // Compiler-generated dctor is used (but irrelevant).

public:

    // check
    // The function and function template below simulate a single function
    // that takes a single parameter, and returns true iff the parameter has
    // type T or (const T).

    // check (reference-to-const T)
    // Pre: None.
    // Post:
    //     Return is true.
    // Does not throw (No-Throw Guarantee)
    static bool check(const T & param)
    { return true; }

    // check (reference-to-const non-T)
    // Pre: None.
    // Post:
    //     Return is false.
    // Requirements on types: None.
    // Does not throw (No-Throw Guarantee)
    template <typename OtherType>
    static bool check(const OtherType & param)
    { return false; }

};  // End class TypeCheck


// ************************************************************************
// Testing Package:
//     Class Counter - Helper Class for Counting Calls & Objects, Throwing
// ************************************************************************


// class Counter
// Item type for counting ctor, dctor, op= calls, counting existing
//  objects, and possibly throwing on copy. Has operator< (which always
//  returns false), allowing it to be the value type of a sorted container.
// If static member copyThrow_ is set, then copy ctor and copy assn throw
//  std::runtime_error. Exception object's "what" member is set to "C" by
//  the copy ctor and "A" by copy assn.
// Increments static data member ctorCount_ on default construction and
//  successful copy construction. Increments static data member assnCount_
//  on successful copy assignment. Increments static data member
//  dctorCount_ on destruction.
// Increments static data member existing_ on construction, and decrements
//  it on destruction.
// Static data member maxExisting_ is highest value of existing_ since last
//  reset, or start of program if reset has never been called.
// Invariants:
//     Counter::existing_ is number of existing objects of this class.
//     Counter::ctorCount_ is number of successful ctor calls since
//      most recent call to reset, or start of program if reset has never
//      been called.
//     Counter::dctorCount_ is (similarly) number of dctor calls.
//     Counter::assnCount_ is (similarly) number of copy assn calls.
//     Counter::maxExisting_ is (similarly) highest value existing_ has
//      assumed.
class Counter {

// ***** Counter: Ctors, dctor, op= *****
public:

    // Default ctor
    // Pre: None.
    // Post:
    //     (ctorCount_ has been incremented.)
    //     (existing_ has been incremented.)
    // Does not throw (No-Throw Guarantee)
    Counter()
    {
        ++existing_;
        if (existing_ > maxExisting_)
            maxExisting_ = existing_;
        ++ctorCount_;
    }

    // Copy ctor
    // Throws std::runtime_error if copyThrow_.
    // Pre: None.
    // Post:
    //     (ctorCount_ has been incremented.)
    //     (existing_ has been incremented.)
    // May throw std::runtime_error
    // Strong Guarantee
    Counter(const Counter & other)
    {
        if (copyThrow_)
            throw std::runtime_error("C");
        ++existing_;
        if (existing_ > maxExisting_)
            maxExisting_ = existing_;
        ++ctorCount_;
    }

    // Copy assignment
    // Throws std::runtime_error if copyThrow_.
    // Pre: None.
    // Post:
    //     Return value is *this.
    //     (assnCount_ has been incremented.)
    // May throw std::runtime_error
    // Strong Guarantee
    Counter & operator=(const Counter & rhs)
    {
        if (copyThrow_)
            throw std::runtime_error("A");
        ++assnCount_;
        return *this;
    }

    // Dctor
    // Pre: None.
    // Post:
    //     (dctorCount_ has been incremented.)
    //     (existing_ has been decremented.)
    // Does not throw (No-Throw Guarantee)
    ~Counter()
    {
        --existing_;
        ++dctorCount_;
    }

// ***** Counter: Functions dealing with count *****
public:

    // reset
    // Pre: None.
    // Post:
    //     maxExisting_ == existing_.
    //     ctorCount_ == 0.
    //     dctorCount_ == 0.
    //     assnCount_ == 0.
    //     copyThrow_ == shouldThrow.
    // Does not throw (No-Throw Guarantee)
    static void reset(bool shouldThrow = false)
    {
        maxExisting_ = existing_;
        ctorCount_ = 0;
        dctorCount_ = 0;
        assnCount_ = 0;
        copyThrow_ = shouldThrow;
    }

    // getExisting
    // Pre: None.
    // Post:
    //     return == existing_.
    // Does not throw (No-Throw Guarantee)
    static int getExisting()
    { return existing_; }

    // getMaxExisting
    // Pre: None.
    // Post:
    //     return == maxExisting_.
    // Does not throw (No-Throw Guarantee)
    static int getMaxExisting()
    { return maxExisting_; }

    // getCtorCount
    // Pre: None.
    // Post:
    //     return == ctorCount_.
    // Does not throw (No-Throw Guarantee)
    static int getCtorCount()
    { return ctorCount_; }

    // getDctorCount
    // Pre: None.
    // Post:
    //     return == dctorCount_.
    // Does not throw (No-Throw Guarantee)
    static int getDctorCount()
    { return dctorCount_; }

    // getAssnCount
    // Pre: None.
    // Post:
    //     return == assnCount_.
    // Does not throw (No-Throw Guarantee)
    static int getAssnCount()
    { return assnCount_; }

    // setCopyThrow
    // Pre: None.
    // Post:
    //     copyThrow_ == shouldThrow
    // Does not throw (No-Throw Guarantee)
    static void setCopyThrow(bool shouldThrow)
    { copyThrow_ = shouldThrow; }

// ***** Counter: Data Members *****
private:

    static int existing_;     // # of existing objects
    static int maxExisting_;  // Max # of existing objects
    static int ctorCount_;    // # of successful (non-throwing) ctor calls
    static int dctorCount_;   // # of dctor calls
    static int assnCount_;    // # of successful (non-throwing) copy = calls
    static bool copyThrow_;   // true if copy operations (ctor, =) throw

};  // End class Counter

// Definition of static data member of class Counter
int Counter::existing_ = 0;
int Counter::maxExisting_ = 0;
int Counter::ctorCount_ = 0;
int Counter::dctorCount_ = 0;
int Counter::assnCount_ = 0;
bool Counter::copyThrow_ = false;


// operator< (Counter)
// Dummy-ish operator<, forming a strict weak order for Counter class
// Returns false (which is legal for a strict weak order; all objects of
//  type Counter are equivalent).
// Pre: None.
// Post:
//     Return value == false.
// Does not throw (No-Throw Guarantee)
bool operator<(const Counter & a,
               const Counter & b)
{ return false; }


// ************************************************************************
// Definitions for this test suite
// ************************************************************************


// Type for a block; must have size 5
typedef std::vector<int> Bk;

// Indices within a block
const int BS = 0;
const int TS = 1;
const int BC = 2;
const int TC = 3;
const int HT = 4;

// makeb
// Make a block with the given values
Bk makeb(int bs, int ts, int bc, int tc, int ht)
{
    //return Bk { bs, ts, bc, tc, ht };
    Bk b;
    b.push_back(bs);
    b.push_back(ts);
    b.push_back(bc);
    b.push_back(tc);
    b.push_back(ht);
    return b;
}


// ************************************************************************
// Test Suite Functions
// ************************************************************************


// test_stackEm_small
// Test suite for function stackEm: small examples
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_stackEm_small(Tester & t)
{
    std::cout << "Test Suite: function stackEm - small examples"
              << std::endl;

    std::vector<Bk> blox;
    const std::vector<Bk> & cb(blox);

    blox.clear();
    blox.push_back(makeb(10, 1, 3, 6, 27));
    t.test(stackEm(cb) == 27, "One block #1");

    blox.clear();
    blox.push_back(makeb(49, 23, 36, 84, 581));
    t.test(stackEm(cb) == 581, "One block #2");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 12));
    blox.push_back(makeb(4, 2, 2, 3, 13));
    t.test(stackEm(cb) == 25, "Two blocks, stackable #1");

    blox.clear();
    blox.push_back(makeb(4, 2, 2, 3, 13));
    blox.push_back(makeb(7, 5, 1, 2, 12));
    t.test(stackEm(cb) == 25, "Two blocks, stackable #2");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 12));
    blox.push_back(makeb(4, 2, 3, 1, 13));
    t.test(stackEm(cb) == 13, "Two blocks, not stackable - color #1");

    blox.clear();
    blox.push_back(makeb(4, 2, 3, 1, 13));
    blox.push_back(makeb(7, 5, 1, 2, 12));
    t.test(stackEm(cb) == 13, "Two blocks, not stackable - color #2");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 13));
    blox.push_back(makeb(4, 2, 3, 1, 12));
    t.test(stackEm(cb) == 13, "Two blocks, not stackable - color #3");

    blox.clear();
    blox.push_back(makeb(4, 2, 3, 1, 12));
    blox.push_back(makeb(7, 5, 1, 2, 13));
    t.test(stackEm(cb) == 13, "Two blocks, not stackable - color #4");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 13));
    blox.push_back(makeb(6, 2, 2, 1, 17));
    t.test(stackEm(cb) == 17, "Two blocks, not stackable - bigger size #1");

    blox.clear();
    blox.push_back(makeb(6, 2, 2, 1, 17));
    blox.push_back(makeb(7, 5, 1, 2, 13));
    t.test(stackEm(cb) == 17, "Two blocks, not stackable - bigger size #2");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 17));
    blox.push_back(makeb(6, 2, 2, 1, 13));
    t.test(stackEm(cb) == 17, "Two blocks, not stackable - bigger size #3");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 17));
    blox.push_back(makeb(6, 2, 2, 1, 13));
    t.test(stackEm(cb) == 17, "Two blocks, not stackable - bigger size #4");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 13));
    blox.push_back(makeb(5, 2, 2, 1, 17));
    t.test(stackEm(cb) == 17, "Two blocks, not stackable - equal size #1");

    blox.clear();
    blox.push_back(makeb(5, 2, 2, 1, 17));
    blox.push_back(makeb(7, 5, 1, 2, 13));
    t.test(stackEm(cb) == 17, "Two blocks, not stackable - equal size #2");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 17));
    blox.push_back(makeb(5, 2, 2, 1, 13));
    t.test(stackEm(cb) == 17, "Two blocks, not stackable - equal size #3");

    blox.clear();
    blox.push_back(makeb(7, 5, 1, 2, 17));
    blox.push_back(makeb(5, 2, 2, 1, 13));
    t.test(stackEm(cb) == 17, "Two blocks, not stackable - equal size #4");

    blox.clear();
    blox.push_back(makeb(9, 7, 1, 2, 1));
    blox.push_back(makeb(6, 4, 2, 1, 2));
    blox.push_back(makeb(3, 1, 1, 2, 3));
    t.test(stackEm(cb) == 6, "Three blocks, stackable #1");

    blox.clear();
    blox.push_back(makeb(9, 7, 1, 2, 1));
    blox.push_back(makeb(3, 1, 1, 2, 3));
    blox.push_back(makeb(6, 4, 2, 1, 2));
    t.test(stackEm(cb) == 6, "Three blocks, stackable #2");

    blox.clear();
    blox.push_back(makeb(6, 4, 2, 1, 2));
    blox.push_back(makeb(9, 7, 1, 2, 1));
    blox.push_back(makeb(3, 1, 1, 2, 3));
    t.test(stackEm(cb) == 6, "Three blocks, stackable #3");

    blox.clear();
    blox.push_back(makeb(6, 4, 2, 1, 2));
    blox.push_back(makeb(3, 1, 1, 2, 3));
    blox.push_back(makeb(9, 7, 1, 2, 1));
    t.test(stackEm(cb) == 6, "Three blocks, stackable #4");

    blox.clear();
    blox.push_back(makeb(3, 1, 1, 2, 3));
    blox.push_back(makeb(9, 7, 1, 2, 1));
    blox.push_back(makeb(6, 4, 2, 1, 2));
    t.test(stackEm(cb) == 6, "Three blocks, stackable #5");

    blox.clear();
    blox.push_back(makeb(3, 1, 1, 2, 3));
    blox.push_back(makeb(6, 4, 2, 1, 2));
    blox.push_back(makeb(9, 7, 1, 2, 1));
    t.test(stackEm(cb) == 6, "Three blocks, stackable #6");

    blox.clear();
    blox.push_back(makeb(3, 1, 2, 1, 3));
    blox.push_back(makeb(6, 4, 2, 1, 2));
    blox.push_back(makeb(9, 7, 1, 2, 1));
    t.test(stackEm(cb) == 4, "Three blocks, partially stackable - color");

    blox.clear();
    blox.push_back(makeb(3, 1, 1, 1, 3));
    blox.push_back(makeb(7, 4, 1, 1, 2));
    blox.push_back(makeb(9, 7, 1, 1, 1));
    t.test(stackEm(cb) == 5, "Three blocks, partially stackable - size");

    blox.clear();
    blox.push_back(makeb(3, 1, 1, 1, 3));
    blox.push_back(makeb(7, 4, 1, 2, 2));
    blox.push_back(makeb(9, 7, 1, 2, 1));
    t.test(stackEm(cb) == 3, "Three blocks, not stackable");
}


// test_stackEm_medium
// Test suite for function stackEm: medium examples
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_stackEm_medium(Tester & t)
{
    std::cout << "Test Suite: function stackEm - medium examples"
              << std::endl;

    std::vector<Bk> blox;
    const std::vector<Bk> & cb(blox);

    {
    blox.clear();
    const int levels = 3;
    for (int i = 0; i < levels; ++i)
    {
        blox.push_back(makeb(i*20+2,  i*20+1,  1, 1, 1));
        blox.push_back(makeb(i*20+5,  i*20+3,  1, 1, 1));
        blox.push_back(makeb(i*20+6,  i*20+4,  1, 1, 2));
        blox.push_back(makeb(i*20+9,  i*20+7,  1, 1, 1));
        blox.push_back(makeb(i*20+10, i*20+8,  1, 1, 2));
        blox.push_back(makeb(i*20+12, i*20+11, 1, 1, 1));
    }
    t.test(stackEm(cb) == 6*levels, "Many choices #1");
    }

    {
    blox.clear();
    const int levels = 3;
    for (int i = 0; i < levels; ++i)
    {
        blox.push_back(makeb(i*20+2,  i*20+1,  1, 1, 1));
        blox.push_back(makeb(i*20+5,  i*20+3,  1, 1, 1));
        blox.push_back(makeb(i*20+6,  i*20+4,  1, 1, 2));
        blox.push_back(makeb(i*20+9,  i*20+7,  1, 1, 3));
        blox.push_back(makeb(i*20+10, i*20+8,  1, 1, 2));
        blox.push_back(makeb(i*20+12, i*20+11, 1, 1, 1));
    }
    t.test(stackEm(cb) == 7*levels, "Many choices #2");
    }

    {
    blox.clear();
    const int levels = 4;
    for (int i = 0; i < levels; ++i)
    {
        blox.push_back(makeb(i*20+2,  i*20+1,  i*2+2, i*2+1, 1));
        blox.push_back(makeb(i*20+5,  i*20+3,  i*2+2, i*2+2, 1));
        blox.push_back(makeb(i*20+6,  i*20+4,  i*2+2, i*2+2, 2));
        blox.push_back(makeb(i*20+9,  i*20+7,  i*2+2, i*2+2, 1));
        blox.push_back(makeb(i*20+10, i*20+8,  i*2+2, i*2+2, 2));
        blox.push_back(makeb(i*20+12, i*20+11, i*2+3, i*2+2, 1));
    }
    t.test(stackEm(cb) == 6*levels, "Many choices #3");
    }

    {
    blox.clear();
    const int levels = 6;
    for (int i = 0; i < levels; ++i)
    {
        blox.push_back(makeb(i*20+2,  i*20+1,  i*4+2, i*4+1, 2));
        blox.push_back(makeb(i*20+17, i*20+3,  i*4+4, i*4+2, 2));
        blox.push_back(makeb(i*20+18, i*20+4,  i*4+4, i*4+2, 3));
        blox.push_back(makeb(i*20+6,  i*20+3,  i*4+3, i*4+2, 1));
        blox.push_back(makeb(i*20+18, i*20+15, i*4+4, i*4+3, 1));
        blox.push_back(makeb(i*20+14, i*20+7,  i*4+3, i*4+3, 4));
        blox.push_back(makeb(i*20+20, i*20+19, i*4+5, i*4+4, 2));
    }
    t.test(stackEm(cb) == 10*levels, "Many choices #4");
    }

    {
    // Dijkstra's Algorithm graph from text p. 336
    // Finding longest a-e walk using at most 4 edges
    //vector<int> data {
    int data[21] = {
        1, 2, 3,  // Edge a-b: 3
        1, 4, 7,  // Edge a-d: 7
        2, 4, 2,  // Edge b-b: 2
        2, 3, 4,  // Edge b-c: 4
        4, 3, 5,  // Edge d-c: 5
        4, 5, 4,  // Edge d-e: 4
        3, 5, 6   // Edge c-e: 3
    };
    blox.clear();
    for (int i = 0; i < 7; ++i)
    {
        for (int sz = 2; sz <= 5; ++sz)
        {
            int x = data[i*3+0];
            int y = data[i*3+1];
            int wt = data[i*3+2];
            blox.push_back(makeb(sz, sz, x, y, wt));
            blox.push_back(makeb(sz, sz, y, x, wt));
        }
    }
    // Start vertex: a
    blox.push_back(makeb(1, 1, 1, 1, 1000));
    // End vertex: e
    blox.push_back(makeb(99, 99, 5, 5, 1000));
    // Longest walk: a-d-a-d-e; 7+7+7+4 = 25
    t.test(stackEm(cb) == 2000+25, "Longest-walk encoding");
    }

    {
    // TSP graph from text p. 447
    // Solving Generlized TSP
    //vector<int> data {
    int data[30] = {
        1, 2, 4,   // Edge a-b:  4
        1, 3, 8,   // Edge a-c:  8
        1, 4, 9,   // Edge a-d:  9
        1, 5, 12,  // Edge a-e: 12
        2, 3, 6,   // Edge b-c:  6
        2, 4, 8,   // Edge b-d:  8
        2, 5, 9,   // Edge b-e:  9
        3, 4, 10,  // Edge c-d: 10
        3, 5, 11,  // Edge c-e: 11
        4, 5, 7,   // Edge d-e:  7
    };
    blox.clear();
    // Encode edges
    for (int i = 0; i < 10; ++i)
    { 
        for (int sz = 2; sz <= 6; ++sz)
        {
            int x = data[i*3+0];
            int y = data[i*3+1];
            int wt = data[i*3+2];
            blox.push_back(makeb(sz, sz, x, y, 100-wt));
            blox.push_back(makeb(sz, sz, y, x, 100-wt));
        }
    }
    // Start vertex: a
    blox.push_back(makeb(1, 1, 1, 1, 1000));
    // End vertex: a
    blox.push_back(makeb(99, 99, 1, 1, 1000));
    // Shortest walk: a-c-b-a-b-a 8+6+4+4+4=26
    t.test(stackEm(cb) == 2000+500-26, "Generalized TSP encoding");
    }

    // Random-ish weights
    {
    //vector<int> data {
    int data[38] = {
        1, 6, 3, 5, 2, 4, 6, 2, 5, 5, 3, 4, 1, 1, 2, 6, 5, 3, 4, 2,
        4, 4, 3, 5, 2, 3, 6, 5, 2, 5, 1, 5, 4, 1, 3, 3, 3, 1
    };
    blox.clear();
    for (size_t i = 0; i < 38/*data.size()*/; ++i)
    {
        blox.push_back(makeb(int(i)+5, int(i)+1, 1, 1, data[i]));
    }
    t.test(stackEm(cb) == 39, "Random-ish weights");
    }
}


// test_stackEm_large
// Test suite for function stackEm: large examples
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_stackEm_large(Tester & t)
{
    std::cout << "Test Suite: function stackEm - large examples"
              << std::endl;

    std::vector<Bk> blox;
    const std::vector<Bk> & cb(blox);

    {
    blox.clear();
    const int levels = 400;
    for (int i = 0; i < levels; ++i)
    {
        blox.push_back(makeb(i*20+2,  i*20+1,  1, 1, 1));
        blox.push_back(makeb(i*20+5,  i*20+3,  1, 1, 1));
        blox.push_back(makeb(i*20+6,  i*20+4,  1, 1, 2));
        blox.push_back(makeb(i*20+9,  i*20+7,  1, 1, 1));
        blox.push_back(makeb(i*20+10, i*20+8,  1, 1, 2));
        blox.push_back(makeb(i*20+12, i*20+11, 1, 1, 1));
    }
    t.test(stackEm(cb) == 6*levels, "Many choices #1");
    }

    {
    blox.clear();
    const int levels = 400;
    for (int i = 0; i < levels; ++i)
    {
        blox.push_back(makeb(i*20+2,  i*20+1,  1, 1, 1));
        blox.push_back(makeb(i*20+5,  i*20+3,  1, 1, 1));
        blox.push_back(makeb(i*20+6,  i*20+4,  1, 1, 2));
        blox.push_back(makeb(i*20+9,  i*20+7,  1, 1, 3));
        blox.push_back(makeb(i*20+10, i*20+8,  1, 1, 2));
        blox.push_back(makeb(i*20+12, i*20+11, 1, 1, 1));
    }
    t.test(stackEm(cb) == 7*levels, "Many choices #2");
    }

    {
    blox.clear();
    const int levels = 45;
    for (int i = 0; i < levels; ++i)
    {
        blox.push_back(makeb(i*20+2,  i*20+1,  i*2+2, i*2+1, 1));
        blox.push_back(makeb(i*20+5,  i*20+3,  i*2+2, i*2+2, 1));
        blox.push_back(makeb(i*20+6,  i*20+4,  i*2+2, i*2+2, 2));
        blox.push_back(makeb(i*20+9,  i*20+7,  i*2+2, i*2+2, 1));
        blox.push_back(makeb(i*20+10, i*20+8,  i*2+2, i*2+2, 2));
        blox.push_back(makeb(i*20+12, i*20+11, i*2+3, i*2+2, 1));
    }
    t.test(stackEm(cb) == 6*levels, "Many choices #3");
    }

    {
    blox.clear();
    const int levels = 33;
    for (int i = 0; i < levels; ++i)
    {
        blox.push_back(makeb(i*20+2,  i*20+1,  i*4+2, i*4+1, 2));
        blox.push_back(makeb(i*20+17, i*20+3,  i*4+4, i*4+2, 2));
        blox.push_back(makeb(i*20+18, i*20+4,  i*4+4, i*4+2, 3));
        blox.push_back(makeb(i*20+6,  i*20+3,  i*4+3, i*4+2, 1));
        blox.push_back(makeb(i*20+18, i*20+15, i*4+4, i*4+3, 1));
        blox.push_back(makeb(i*20+14, i*20+7,  i*4+3, i*4+3, 4));
        blox.push_back(makeb(i*20+20, i*20+19, i*4+5, i*4+4, 2));
    }
    t.test(stackEm(cb) == 10*levels, "Many choices #4");
    }

    // Random-ish weights
    {
    //vector<int> data {
    const size_t DATASIZE = 100;
    int data[DATASIZE] = {
        1, 6, 3, 5, 2, 4, 6, 2, 5, 5, 3, 4, 1, 1, 2, 6, 5, 3, 4, 2,
        4, 4, 3, 5, 2, 3, 6, 5, 2, 5, 1, 5, 4, 1, 3, 3, 3, 1, 2, 3,
        5, 4, 6, 2, 5, 3, 4, 2, 3, 4,
        4, 5, 2, 6, 7, 3, 4, 1, 3, 5, 1, 5, 4, 6, 3, 4, 4, 3, 4, 1,
        3, 3, 2, 5, 7, 1, 4, 3, 7, 7, 5, 4, 2, 3, 1, 4, 3, 2, 3, 4,
        3, 2, 3, 4, 5, 5, 5, 5, 2, 1
    };
    blox.clear();
    for (size_t i = 0; i < 50*DATASIZE/*data.size()*/; ++i)
    {
        blox.push_back(makeb(int(i)+10, int(i)+1,
                             1, 1,
                             data[i%DATASIZE]));
    }
    t.test(stackEm(cb) == 2552, "Random-ish weights");
    }
}


// test_stackEm
// Test suite for function stackEm
// Uses other test-suite functions
// Pre: None.
// Post:
//     Pass/fail status of tests have been registered with t.
//     Appropriate messages have been printed to cout.
// Does not throw (No-Throw Guarantee)
void test_stackEm(Tester & t)
{
    // Do all the test suites
    std::cout << "TEST SUITES FOR FUNCION stackEm" << std::endl;
    test_stackEm_small(t);
    test_stackEm_medium(t);
    test_stackEm_large(t);
}


// ************************************************************************
// Main program
// ************************************************************************


// main
// Runs function stackEm test suite, prints results to cout.
int main()
{
    Tester t;
    test_stackEm(t);

    std::cout << std::endl;
    if (t.allPassed())
    {
        std::cout << "All tests successful" 
                  << std::endl;
    }
    else
    {
        std::cout << "Tests ********** UNSUCCESSFUL **********"
                  << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Press ENTER to quit ";
    while (std::cin.get() != '\n') ;

    return 0;
}

