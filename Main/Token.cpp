#pragma once
#include "Token.h"

inline bool isSingleToken(const char c) {
	return	c > 31 && c < 45 ||
		c > 57 && c < 65 ||
		c > 90 && c < 95 ||
		c > 95 && c < 97 ||
		c > 122 && c < 127 ||
		c == 46;
}

const char* Token::get_str()
{
	return (const char*)this->body;
}

uint32_t Token::get_size()
{
	return this->length;
}

Token::Token(const char* body, uint32_t length)
{
	this->body = body;
	this->length = length;
}

TokenVec::TokenVec(ISentence* sentence)
{
	this->reserve(30); //Reserve memory for at least n tokens
	const char* sentence_body = sentence->get_body();
	const uint32_t sentence_length = sentence->get_length();
	const char* tok_start = sentence_body;
	const char* last = sentence_body + sentence_length;

	for (const char* current = sentence_body; current < last; current++) {
		if (*current == ' ') {
			if (current - tok_start) {
				this->emplace_back(Token(tok_start, (current - tok_start)));
			}

			tok_start = current + 1;
			continue;
		}
		if (isSingleToken(*current)) {
			if (current - tok_start) {
				this->emplace_back(Token(tok_start, current - tok_start));
			}
			this->emplace_back(Token(current, sizeof(char)));

			tok_start = current + 1;
		}
	}
	if (last - tok_start) {
		this->emplace_back(Token(tok_start, last - tok_start));
	}
}

TokenVec::~TokenVec()
{
	//for (auto* tok : *this)
	//	delete tok;
}