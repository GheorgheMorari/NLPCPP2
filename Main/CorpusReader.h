#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "Interfaces.h"

#define READ_BUFFER ((size_t)262144) //256KiB

class CorpusReader
{
	size_t file_total_bytes;
	size_t buffer_start_byte;
	size_t buffer_end_byte;
	size_t buffer_current_byte;

	char* buffer;
	FILE* file_stream;

public:
	CorpusReader();
	void open_file(const char* filename);
	int read_from_pos(size_t pos = 0);
	ISentence* get_next();
	uint32_t get_sentence_count();

	~CorpusReader();
};
