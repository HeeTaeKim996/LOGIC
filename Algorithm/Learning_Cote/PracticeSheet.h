#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>



#include <string>
#include <vector>
using namespace std;

typedef long long int64;



long long solution(int n, vector<int> times_cpp) 
{
    // to C
    int64 size = times_cpp.size();
    int* times = (int*)malloc(sizeof(int) * size);
    memcpy(times, &times_cpp[0], sizeof(int) * size);


    int64 left = times[0];
    int64 right = (int64)times[size - 1] * n;


    while (left <= right)
    {
        int64 mid = (left + right) / 2;

        int64 sum = 0;
        for (int i = 0; i < size; ++i)
        {
            sum += mid / times[i];
        }


        if(sum >= n)
        {
            right = mid - 1;
        }
        else
        {
            left = mid + 1;
        }
    }

    long long answer = 0;
    return left;
}