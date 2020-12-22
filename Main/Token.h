#pragma once
#include <cstdint>
#include <vector>

#include "Interfaces.h"

class Token : public IToken
{
protected:
	const char* body;
	uint32_t length;
public:
	const char* get_str(void) override;
	uint32_t get_size(void) override;
	Token(const char* body, uint32_t length);
};

class TokenVec : public std::vector<Token> {
public:
	TokenVec(ISentence* sentence);
	~TokenVec();
};