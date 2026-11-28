#pragma once

#include <functional>
#include <cstdint>
#include "Array.h"
#include <cstring>

//#define SWAP(a, b) do{auto c = a; a = b; b = c;}while(0)

// 매크로 사용은 지양하자. QuicSort에서 로 변수 네이밍 c를 넣었더니 스왑이 꼬이는 문제가 생겼었음



class Sort
{
public:
	static void test();

public:
	template<typename T>
	inline static void SWAP(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

public:
	template<typename T>
	static void bubbleSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare);

	template<typename T>
	static void selectionSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare);

	template<typename T>
	static void insertionSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare);

	template<typename T>
	static void shellSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare);

	template<typename T>
	static void quickSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare);

	template<typename T>
	static void mergeSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare);

	template<typename T>
	static void heapSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare);

	static void countingSort(Array<int>& arrs, int max);

	static void radixSort(Array<int>& arrs);
};

template<typename T>
inline void Sort::bubbleSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare)
{
#if 0	// V1
	for (int max = arrs.size() - 1; max >= 1; max--)
	{
		for (int i = 0; i < max; i++)
		{
			if (compare(arrs[i], arrs[i + 1]) > 0)
			{
				SWAP(arrs[i], arrs[i + 1])
			}

		}
	}
#elif 0	// V2 : 회전 때 교환이 한번도 일어나지 않았다면, 이미 모두 정렬된 것이기에, 바로 종료
	for (int max = arrs.size() - 1; max >= 1; max--)
	{
		bool exchanged = false;
		for (int i = 0; i < max; i++)
		{
			if (compare(arrs[i], arrs[i + 1]) > 0)
			{
				SWAP(arrs[i], arrs[i + 1]);
				exchanged = true;
			}
		}

		if (exchanged == false) return;
	}
#elif 1	
	// V3 : 왼쪽 -> 오른쪽으로의 버블 정렬 기준, 회전때마다 마지막으로 정렬된 위치를 기록.
	//		다음 회전시, 해당 위치까지만 회전 ( 마지막 정렬 기록이니, 그 이후는 이미 정렬이 완성 상태)
	//		회전 때 정렬이 아얘 없을시(k == 0), 종료
	int k = arrs.size() - 1;
	while (k > 0)
	{
		int last = 0;
		for (int i = 0; i < k; i++)
		{
			if (compare(arrs[i], arrs[i + 1]) > 0)
			{
				SWAP(arrs[i], arrs[i + 1]);
				last = i;
			}
		}

		k = last;
	}
#endif
}






template<typename T>
inline void Sort::selectionSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare)
{
	int size = static_cast<int>(arrs.size());
	// arrs.size() 는 size_t 이기에, arrs.empty() 일시,  
	// arrs.size() -1 는 unsigne 이기에 큰 양수가 되어, 크래시 발생.
	// 위처럼 size 를 미리 signed 인 int로 형변환하여 사용하여, 크래시 방지

	for (int i = 0; i < size - 1; i++)
	{
		int low = i;
		for (int j = i + 1; j < size; j++)
		{
			if (compare(arrs[j], arrs[low]) < 0)
			{
				low = j;
			}
		}

		SWAP(arrs[i], arrs[low]);
	}

}





template<typename T>
inline void Sort::insertionSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare)
{
	for (int i = 1; i < arrs.size(); i++)
	{
		T tmp = arrs[i];
		int j = i;
		for (; j > 0 && compare(tmp, arrs[j - 1]) < 0; j--)
		{
			arrs[j] = arrs[j - 1];
		}

		arrs[j] = tmp;
	}
}





template<typename T>
inline void Sort::shellSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare)
{
#if 0	// V1
	int n = arrs.size();
	for (int h = n / 2; h > 0; h /= 2)
	{
		for (int i = h; i < n; i++)
		{
			T tmp = arrs[i];
			int j = i;
			for (; j >= h && compare(tmp, arrs[j - h]) < 0; j -= h)
			{
				arrs[j] = arrs[j - h];
			}
			arrs[j] = tmp;
		}
	}
#elif 1	// V2 : h 값이 서로의 배수가 되지 않도록, h = 3 * h + 1 로 할당
	int n = arrs.size();
	int h = 1;
	for (; h < n; h = h * 3 + 1);
	for (; h > 0; h /= 3)
	{
		for (int i = h; i < n; i++)
		{
			T tmp = arrs[i];
			int j = i;
			for (; j >= h && compare(tmp, arrs[j - h]) < 0; j -= h)
			{
				arrs[j] = arrs[j - h];
			}
			arrs[j] = tmp;
		}
	}
#endif



}

template<typename T>
inline void Sort::quickSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare)
{

#if 0	// V1

	// ■ 이해 정리
	//	 ※ 아래 설명에서, x = arrs[mid]
	// 
	//	 □ 회전의 마지막 부분
	//	 1) pl = mid - 1, pr = mid + 1
	//		-> swap(mid-1, mid + 1) 
	//		-> pl++ 로 pl = mid, pr-- 로 pr = mid
	//		-> swap(mid, mid) (제라리 스왑)
	//		-> pl = mid + 1, pr = mid - 1
	//		-> pl <= pr 이 아니므로, 루프 종료. 이후 파티션은 partition(left, mid -1), partition(mid + 1, right)
	//		   로 mid 를 제외하여 파티션이 진행
	// 
	//   2) pl = mid, pr = mid + 1
	//		-> swap(mid, mid + 1)
	//		-> pl++ 로 pl = mid + 1, pr-- 로 pr = mid
	//		-> pl < pr 이 아니므로, 루프 종료. 이후 파티션은 partition(left, mid), partition(mid + 1, right)
	//		   로 mid 를 한 partition 에 포함하여 파티션이 진행됨
	//		
	//	 3) pl = mid - 1, pr = mid 
	//		-> 2) 와 같은 방식으로 하여, partition(left, mid - 1), partition(mid, right) 로 파티션 진행
	//
	//	 □ 파티션 중 mid 가 극단값일 때
	//		- mid가 left에 위치하게 될 극단값이라 하자. 
	//		  첫 회전에서 pl 은 무조건 compare(pl, x) > 0 이므로, pl 은 left 에 고정됨
	//		  첫 회전에서 pr 은 무조건 compare(pr, x) < 0 이므로, pr 은 mid 까지 내려옴
	//		-> swap(left, mid) 로 하여, mid 가 left에 위치하게 됨
	//		-> pl ++ 로 pl == left + 1, pr-- 로 pr = mid - 1
	//		-> 두번째 회전에서 pl 은 무조건 compare(pl, x) > 0 이므로, pl 은 left + 1 에 고정됨
	//		-> 두번째 회전에서 pr 은 무조건 compare(pr, x) < 0 이므로, pr 은 mid(현 left인) 까지 내려옴
	//		-> pl(left + 1) > pr(left) 이므로, 루프가 종료
	//		-> left == pr(left) 이므로, partition 하나는 진행 안되고,
	//		   pl < right 인 partition(left + 1, right)만 진행됨
	//
	if (arrs.size() == 0) return;


	function<void(int, int)> partition = [&](int left, int right)
		{
			int pl = left;
			int pr = right;
			T x = arrs[(left + right) / 2];
			do
			{
				while (compare(arrs[pl], x) < 0) pl++;
				while (compare(arrs[pr], x) > 0) pr--;

				if (pl <= pr)
				{
					SWAP(arrs[pl], arrs[pr]);
					pl++;
					pr--;
				}
			} while (pl <= pr);


			if (left < pr) partition(left, pr);
			if (pl < right) partition(pl, right);
		};


	partition(0, arrs.size() - 1);
#elif 1 // V2 

	// ■ 이해 정리
	// □ threeSort 의 필요성
	// 앞선 V1 은 mid 가 left 또는 right 값이면, 한 파티션은 단일값, 나머지 한 파티션은 그 외 값들로 재귀호출 된다.
	// 그럼 최악으로 O(n) = n*2 가 될 수 있다.
	// 위 threeSort 를 사용시, left, mid, right 로 간단하게 셋을 스왑함으로 써, 재귀 파티션이 단일값으로 호출되는 것을
	// 방지한다.
	// 
	// + threeSort 사용시, left 는 x 기준 무조건 낮은 값, right 는 x 기준 무조건 높은 값이 된다.
	//   QuickSort 는 파티션이 X(기준) 를 기준으로 한쪽은 모두 이하 값, 한쪽은 모두 이상 값으로 나누는 것에 기반한다.
	//   따라서 left, right 는 성질이 확정되므로, 생략하여 pl = left + 1; pr = right - 1; 에서부터 정렬을 
	//   시작할 수 있다.
	// 
	// □ SWAP(mid, right -1) 의 필요성
	// 앞선 V1 의 설명에서 보다시피, 정렬시 mid 는 최소 한번 스왑된다.
	// ( left = mid - 1, right = mid + 1 인 상황에서는 swap(mid, mid) 로 스왑,
	//  그 외 상황에서는 mid - 1 또는 mid + 1 과 스왑 )
	// QuickSort 는 기준점 X를 기준으로 왼쪽은 이하값, 오른쪽은 이상값이 오면 된다.
	// 그렇기에 mid 는 왼쪽 파티션이던, 오른쪽 파티션이던 어느 파티션에 위치하든 상관이 없다.
	// 따라서 mid 를 right -1 과 스왑하고, right - 1(기존 mid) 는 정렬에서 제외한다
	//  
	// ※ 실제로 아래 코드에서 right -1 과 mid 를 스왑하지 않고, 
	//   left + 1 과 mid를 스왑후 pl = left + 2; pr = right - 1;
	//   에서 시작하는 코드로 수정해도 동일하게 작동한다
	// 
	// 
	// ==> 위 두 방법을 사용시, left, right, right-1 를 정렬 검수에서 제외할 수 있기에 효율이 향상된다


	if (arrs.size() == 0) return;

	auto threeSort = [&](int a, int b, int c) -> void
		{
			if (compare(arrs[a], arrs[b]) > 0)	SWAP(arrs[a], arrs[b]);
			if (compare(arrs[b], arrs[c]) > 0)	SWAP(arrs[b], arrs[c]);
			if (compare(arrs[a], arrs[b]) > 0)	SWAP(arrs[a], arrs[b]);
		};

	function<void(int, int)> partition = [&](int left, int right)
		{
			int mid = (left + right) / 2;

			threeSort(left, mid, right);

			T x = arrs[mid];

			SWAP(arrs[mid], arrs[right - 1]);

			int pl = left + 1; // left 는 threeSort 에서 정렬됐기에 정렬 불필요
			int pr = right - 2; // right 는 threeSort 에서, right - 1 은 SWAP(mid, right - 1) 에서 정렬됐기에 불필요

			//int pl = left;
			//int pr = right;

			do
			{
				while (compare(arrs[pl], x) < 0) pl++;
				while (compare(arrs[pr], x) > 0) pr--;

				if (pl <= pr)
				{
					SWAP(arrs[pl], arrs[pr]);
					pl++;
					pr--;
				}
			} while (pl <= pr);

			if (left < pr) partition(left, pr);
			if (pl < right) partition(pl, right);
		};

	partition(0, arrs.size() - 1);
#endif
}




template<typename T>
inline void Sort::mergeSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare)
{
	if (arrs.size() == 0) return;

	Array<T> buffer(arrs.size() / 2 + 1);
	// 아래 bufferCount 할당 구문을 먼저 보고 여기 주석을 보자.
	// 아래 정의에 의해, 
	// 짝수일시, size == 4 -> bufferCount = 3 / 2 + 1 = 1 + 1 = 2. 따라서 4(size) / 2 = 2 로 할당해도 문제 없음
	// 홀수일시 size == 3 -> bufferCount = 2 / 2 + 1 = 1 + 1 = 2. 3(size) / 2 = 1. 부족함.
	// 따라서 size / 2 + 1 만큼 할당해야 함


	function<void(int, int)> merge = [&](int left, int right) -> void
		{
			if (left == right) return;

			int mid = (left + right) / 2;

			merge(left, mid);
			merge(mid + 1, right);



			// 이후로 mid 는 우측 파티션의 포인터 역할을 하며, 
			// 아래 bufferCount 설정에 의해, left 부터 mid 까지 모두 좌측 파티션이 된다. 
			// [위 merge(left, mid) 에 의해 left ~ mid(포함) 까지 정렬됐으므로, L 을 통째로 버퍼로 옮겨야 함]
			++mid;
			
			int bufferCount = mid - left;
			memcpy(&buffer[0], &arrs[left], bufferCount * sizeof(T)); // 좌측 파티션 할당




			// 이후로 left는 left ~ right 까지의 배열의 포인터 역할을 한다

			int bufferPointer = 0;

			while (bufferPointer < bufferCount && mid <= right)
				// 우측 파티션 또는 좌측 파티션이 모두 소진될 때까지 비교하여 삽입한다
			{
				arrs[left++] = compare(buffer[bufferPointer], arrs[mid]) <= 0 ?
					buffer[bufferPointer++] : arrs[mid++];
			}
			if (bufferPointer < bufferCount)
				// 좌측 파티션이 남은 경우, 잔여 공간에 일괄 복사한다
			{
				memcpy(&arrs[left], &buffer[bufferPointer],
					(bufferCount - bufferPointer) * sizeof(T));
			}
			// 우측 파티션이 남은 경우에는, 아무 것도 처리할 필요가 없다. 
			// 우측 파티션 자체가 arrs 의 mid + 1 <= .. <= right 이기 때문 
		};

	merge(0, arrs.size() - 1);
}








template<typename T>
inline void Sort::heapSort(Array<T>& arrs, function<int(const T& a, const T& b)> compare)
{
	auto heapDown = [&](int parent, int lastChild) -> void
		{
			T mover = arrs[parent];
			int child;
			for (; parent * 2 + 1 <= lastChild; parent = child)
			{
				int cl = parent * 2 + 1;
				int cr = cl + 1;

				child = (cr <= lastChild && compare(arrs[cr], arrs[cl]) > 0) ? cr : cl;

				if (compare(mover, arrs[child]) >= 0) break;

				arrs[parent] = arrs[child];
			}

			arrs[parent] = mover;
		};

	int n = arrs.size();
	for (int i = n / 2 - 1; i >= 0; i--)
		// 마지막 자식은 n-1. 마지막 자식의 부모는 (n-1 -1) / 2 = n/2 - 1
	{
		heapDown(i, n - 1);
		// 마치 i 부터 priority_queue 에 삽입하는 것처럼 하여, arrs 를 힙 정렬
	}

	for (int i = n - 1; i > 0; i--)
	{
		SWAP(arrs[0], arrs[i]);
		heapDown(0, i - 1);
		// top 을 i 위치로 옮기고(priority_queue 에서 pop하고), 
		// 자식이 i-1 까지만 있는 다는 가정하에, priority_queue 정렬
	}
}

inline void Sort::countingSort(Array<int>& arrs, int max)
{
	Array<int> imos(max + 1, 0);
	Array<int> buffer(arrs.size());

	for (int i = 0; i < arrs.size(); i++) imos[arrs[i]]++;

	for (int i = 1; i <= max; i++) imos[i] += imos[i - 1];

	for (int i = arrs.size() - 1; i >= 0; i--) buffer[--imos[arrs[i]]] = arrs[i];

	memcpy(&arrs[0], &buffer[0], arrs.size() * sizeof(int));
}


// □ 기수정렬(RadixSort) 
//	도수 정렬의 안정정렬 특성을 활용하여, 도수 정렬을 반복하는 정렬
//	도수 정렬, 기수 정렬 모두 양의 정수를 대상으로 한다.
//	정수값의 최대값, 최소값의 편차가 작다면 도수정렬, 크다면 기수정렬을 활용하자
//
inline void Sort::radixSort(Array<int>& arrs)
{
#if 0 // V1
	int n = arrs.size();

	Array<int> buffer(n);
	

	int max = 0;
	for (int i = 0; i < n; i++)
	{
		if (arrs[i] > max) max = arrs[i];
	}

	int level = 1;
	for (; max / level > 0; level *= 10)
	{
		int imos[10] = {};

		for (int i = 0; i < n; i++)
		{
			int digit = (arrs[i] / level) % 10;
			imos[digit]++;
		}

		for (int i = 1; i <= 9; i++)
		{
			imos[i] += imos[i - 1];
		}

		for (int i = n - 1; i >= 0; i--)
		{
			int digit = (arrs[i] / level) % 10;

			buffer[--imos[digit]] = arrs[i];
		}

		memcpy(&arrs[0], &buffer[0], n * sizeof(int));
	}
#elif 1 // V2 : 
		// 1) (10 제곱수 나누기) 가 아닌 (N 배수 우측 시프트) 로 수정하여, 효율 개선
		// 2) 루프 때마다 buffer -> arrs 복사를 하지 않고, buffer -> arrs || arrs -> buffer 를 번갈아 해서, 효율 개선
	
#define SHIFTING_CHUNK 4
#define READING_CHUNK 0x0F
	

	int n = arrs.size();
	
	Array<int> buffer(n);
	bool copyOnArrs = false;

	int max = 0;
	for (int i = 0; i < n; i++)
	{
		if (arrs[i] > max) max = arrs[i];
	}

	
	for (int shift = 0; (max >> shift) > 0; shift += SHIFTING_CHUNK)
	{
		int imos[READING_CHUNK + 1] = {};

		if (copyOnArrs)
		{
			for (int i = 0; i < n; i++)
			{
				int index = (buffer[i] >> shift) & READING_CHUNK;
				imos[index]++;
			}
		}
		else
		{
			for (int i = 0; i < n; i++)
			{
				int index = (arrs[i] >> shift) & READING_CHUNK;
				imos[index]++;
			}
		}


		for (int i = 1; i <= READING_CHUNK; i++)
		{
			imos[i] += imos[i - 1];
		}

		if (copyOnArrs)
		{
			for (int i = n - 1; i >= 0; i--)
			{
				int index = (buffer[i] >> shift) & READING_CHUNK;
				arrs[--imos[index]] = buffer[i];
			}
		}
		else
		{
			for (int i = n - 1; i >= 0; i--)
			{
				int index = (arrs[i] >> shift) & READING_CHUNK;
				buffer[--imos[index]] = arrs[i];
			}
		}


		copyOnArrs = !copyOnArrs;
	}

	if (copyOnArrs)
	{
		memcpy(&arrs[0], &buffer[0], n * sizeof(int));
	}

#endif
}

