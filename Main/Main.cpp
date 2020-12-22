#include <iostream>
#include <vector>

#include "CorpusReader.h"
#include "Token.h"
using namespace std;
int main()
{
	system("chcp 65001");
	CorpusReader test;
	test.open_file("test.txt");
	ISentence* sentence;
	while ((sentence = test.get_next()) != nullptr)
	{
		print(sentence);
		delete sentence;
	}
}