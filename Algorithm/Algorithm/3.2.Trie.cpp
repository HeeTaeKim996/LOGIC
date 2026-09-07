#include "pch.h"
#include "3.2.Trie.h"
#include <functional>


void Trie::test()
{
	TrieNode<int> root;


	STRING str1 = L"APPLE";
	int val1 = 1;

	STRING str2 = L"APPLY";
	int val2 = 2;

	STRING str3 = L"APP";
	int val3 = 3;

	root.insert(str1.c_str(), &val1);
	root.insert(str2.c_str(), &val2);
	root.insert(str3.c_str(), &val3);


	auto debugSearch = [&](const STRING& search) -> void
		{
			TrieNode<int>* trie1 = root.find(search.c_str());
			if (trie1 != nullptr)
			{
				printf("find Completed : %d\n", *trie1->getValue());
			}
			else 
			{
				printf("find Failed \n");
			}
		};

	debugSearch(str1.c_str());
	debugSearch(str2.c_str());
	debugSearch(str3.c_str());
	debugSearch(L"APPLICATION");

	printf("\nstr3 removed : \n");
	root.remove(str3.c_str());

	debugSearch(str1.c_str());
	debugSearch(str2.c_str());
	debugSearch(str3.c_str());


	printf("\nstr2 removed : \n");
	root.remove(str2.c_str());

	debugSearch(str1.c_str());
	debugSearch(str2.c_str());
	debugSearch(str3.c_str());

	printf("\nstr1 removed : \n");
	root.remove(str1.c_str());

	debugSearch(str1.c_str());
	debugSearch(str2.c_str());
	debugSearch(str3.c_str());

}