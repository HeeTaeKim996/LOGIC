#include <iostream>

#include "use/use_memCounter.h"

class TestClass
{
public:
    TestClass(int value = 0)
        : value(value)
    {
    }

private:
    int value;
};

int main()
{
    // new / delete
    TestClass* p1 = TrackNew<TestClass>(10);
    TestClass* p2 = TrackNew<TestClass>(20);

    TrackDelete(p1);
    TrackDelete(p2);


    // allocate / deallocate
    TestClass* memory = TrackAllocate<TestClass>(3);

    TrackDeallocate(memory);


    // construct / destruct
    TestClass* objectMemory = TrackAllocate<TestClass>(1);

    TrackConstruct(objectMemory, 30);
    TrackDestruct(objectMemory);

    TrackDeallocate(objectMemory);


    // 전체 상태 출력
    printf("\n========== Memory Tracker ==========\n");
    MemCounterManager::instance()->printAll();

    printf("\n========== New / Delete ==========\n");
    MemCounterManager::instance()->printNewState();

    printf("\n========== Allocate / Deallocate ==========\n");
    MemCounterManager::instance()->printAllocState();

    printf("\n========== Construct / Destruct ==========\n");
    MemCounterManager::instance()->printConstructState();

    return 0;
}