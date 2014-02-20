/* Paul Gentemann
 * CS 411
 * File Name : stackem.cpp
 * Last Modified : Thu 20 Feb 2014 02:19:42 AM AKST
 * Description : Dynamic Programming version of stackem. Original code from
 * Erik Talvi's HW2, because it was faster than mine, and because I didn't 
 * really want to rework the bitmask stuff from my HW2.
 */

#include <future>   // for asynch
#include "stackem.h"

// Returns height of maximum stack.
int stackEm(const Tower & value)
{
    auto result = std::async(std::launch::async, stackEmHelper, value);
    return result.get();

}
int stackEmHelper(const Tower & test)
{
    // Handle trivial cases in one line.
    if (test.size() == 1) return test[0][4];

    Tower testCopy(test);
    std::sort(testCopy.begin(), testCopy.end(), [](const Block a, const Block b)
	    { return a[0] < b[0]; }); 

    // Make a stackability matrix, where (b_i,b_j) = true if j stacks on i
    vector<Matrix> stackables (testCopy.size());
    for (int i = 0; i < testCopy.size(); ++i)
    {
        for (int j = 0; j < testCopy.size(); ++j)
        {
            if (canStack(testCopy[j], testCopy[i])) 
            {
                Pair b(&testCopy[i], j);
                stackables[i].push_back(b);
            }
        }
    }

    // Vector of best heights using this piece. -1 means not yet computed.
    vector<int> computed(test.size(), -1);

    int base_index = 0;
    int bestHeight = 0;
    // Go through the list of stackables, find the best stack with given block.
    for (auto base : stackables)
    {
        int tempBest= 0;
        auto next = base.begin();
        while(next != base.end())
        {
            if (computed[next->second] != -1) // If already computed
            {
                // Get the best
                if (tempBest < computed[next->second])
                    tempBest = computed[next->second];
            }
            ++next;
        }
        // Otherwise, compute
        computed[base_index] = testCopy[base_index][4] + tempBest;
        if (bestHeight < computed[base_index])
            bestHeight = computed[base_index];
        ++base_index;
    }
    return bestHeight;
}

bool fitsOnTop(int top, int bottom)
{
    return top < bottom;
}

bool colorMatch(int topColor, int botColor)
{
    return topColor == botColor;
}

bool canStack(const Block & top, const Block & bot)
{
    if (fitsOnTop(top[0], bot[1]) && colorMatch(top[2], bot[3]))
        return true;
    return false;             
}


