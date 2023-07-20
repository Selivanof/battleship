#include <iostream>
#include <cstdlib>

using namespace std;

/**
 * Shuffles an array using an algorithm that initially iterates an index i from the last to second element.
 * And then a second index j is used to select an element randomly between the first element and the i-th element.
 * And finally the elements in positions i and j are swapped.
 *
 * @param myArray the array to be shuffled.
 * @param myArraySize the size of the array to be shuffled.
 */
template <class X> void shuffle(X** myArray, int myArraySize)
{
     // TODO: Implement here the shuffle algorithm
     for(int i=myArraySize-1;i>0;i--)
     {   int j=0;
         j=rand()%i+1;
         X* temp=myArray[j];
         myArray[j]=myArray[i];
         myArray[i]=temp;



     }
}
