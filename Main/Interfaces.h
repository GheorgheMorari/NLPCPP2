#pragma once
#include <cstdint>
#include <cstdlib>

class IToken
{
public:
	virtual uint32_t get_size(void) = 0;
	virtual const char* get_str(void) = 0;
	virtual ~IToken() = default;
};

class ISentence
{
public:
	virtual const char* get_body() const = 0;
	virtual uint32_t get_length() const = 0;
	virtual uint32_t get_characters() const = 0;
	//virtual void set(const char* body, uint32_t length, bool free_body) = 0;

	virtual ~ISentence() { free(this->free_body); free_body = nullptr; }

private:
	char* free_body = nullptr;
};

inline void print(ISentence* sentence)
{
	const uint32_t len = sentence->get_length();
	const char* body = sentence->get_body();
	for (uint32_t i = 0; i < len; i++) { printf("%c", body[i]); }
	printf("\n");
}