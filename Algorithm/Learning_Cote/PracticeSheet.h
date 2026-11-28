#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef char* T;

#include <string.h>
#include <ctype.h>
int cmp(T* pointer_a, T* pointer_b)
{
    int na = strlen(*pointer_a);
    int nb = strlen(*pointer_b);

    char* a = (char*)malloc(sizeof(char) * (na + 1));
    memcpy(a, *pointer_a, na);
    char* b = (char*)malloc(sizeof(char) * (nb + 1));
    memcpy(b, *pointer_b, nb);

    int pa = 0;
    int pb = 0;

    while (a[pa] && isdigit(a[pa]) == false) { pa++; }
    while (b[pb] && isdigit(b[pb]) == false) { pb++; }


    na = pa;
    nb = pb;

    while (a[na] && isdigit(a[na])) { na++; }
    while (b[nb] && isdigit(b[nb])) { nb++; }

    if (a[na])
    {
        a[na] = '\0';
    }
    if (b[nb])
    {
        b[nb] = '\0';
    }

    int numA = atoi(&a[pa]);
    int numB = atoi(&b[pb]);

    a[pa] = '\0';
    b[pb] = '\0';

    
    for (int i = 0; i < pa; ++i)
    {
        if (islower(a[i]))
        {
            a[i] = toupper(a[i]);
        }
    }
    for (int i = 0; i < pb; ++i)
    {
        if (islower(b[i]))
        {
            b[i] = toupper(b[i]);
        }
    }



    int compString = strcmp(a, b);
    free(a); free(b);

    if (compString != 0) return compString;

    if (numA == numB)   return 0;
    if (numA > numB)    return 1;
                        return -1;
}



T* PS;

void merge(T* offset, int left, int right)
{
    if (left == right) return;

    int mid = (left + right) / 2;

    merge(offset, left, mid);
    merge(offset, mid + 1, right);

    int bufferCount = mid - left + 1;
    memcpy(PS, &offset[left], bufferCount * sizeof(T));

    mid++; // 이후 mid 는 right 의 포인터로 사용됨

    int bufferPointer = 0; // left 의 포인터

    while (bufferPointer < bufferCount && mid <= right)
    {
        offset[left++] = cmp(&PS[bufferPointer], &offset[mid]) <= 0 ?
            PS[bufferPointer++] : offset[mid++];
    }
    if (bufferPointer < bufferCount)
    {
        memcpy(&offset[left], &PS[bufferPointer],
            (bufferCount - bufferPointer) * sizeof(T));
    }
}

void mergeSort(T* offset, int n)
{
    merge(offset, 0, n - 1);
}





#include <string>
#include <vector>

using namespace std;

vector<string> solution(vector<string> files_cpp) 
{
    // to C
    int n = files_cpp.size();
    char** files = (char**)malloc(sizeof(char*) * n);

    for (int i = 0; i < files_cpp.size(); ++i)
    {
        const char* c = files_cpp[i].c_str();
        int n = files_cpp[i].size();

        files[i] = (char*)malloc(sizeof(char) * (n + 1));

        memcpy(files[i], c, n);
        files[i][n] = '\0';
    }



    // play
    PS = (char**)malloc(sizeof(char*) * n);
    mergeSort(files, n);



    // to C++
    vector<string> answer;
    answer.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        answer.push_back(files[i]);
    }

    return answer;
}