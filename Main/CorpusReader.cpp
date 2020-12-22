#include "CorpusReader.h"

#include <cstring>

class BufferSentence : public ISentence
{
	char* body;
	uint32_t length;
	uint32_t characters;
public:
	BufferSentence(char* b, uint32_t l, uint32_t c) { body = b; length = l; characters = c; }
	const char* get_body() const override { return this->body; }
	uint32_t get_length() const override { return this->length; }
	uint32_t get_characters() const override { return this->characters; }
};

CorpusReader::CorpusReader()
{
	file_stream = nullptr;
	buffer = (char*)malloc(sizeof(char) * READ_BUFFER);

	file_total_bytes = 0;
	buffer_current_byte = 0;
	buffer_end_byte = 0;
	buffer_start_byte = 0;
}

void CorpusReader::open_file(const char* filename)
{
	if (file_stream != nullptr)
		fclose(file_stream);

	file_stream = fopen(filename, "rb");

	if (file_stream == nullptr)
	{
		printf("Invalid filename provided to CorpusReader::open_file()\n");
		file_total_bytes = 0;
	}
	else
	{
		fseek(file_stream, 0, SEEK_END);
		file_total_bytes = ftell(file_stream);
		fseek(file_stream, 0, SEEK_SET);
	}

	//Set the start and end of the buffer to be the eof
	//To block the reading of the file if the file could not be open
	buffer_start_byte = file_total_bytes;
	buffer_end_byte = file_total_bytes;
	buffer_current_byte = file_total_bytes;
	read_from_pos();
}

int CorpusReader::read_from_pos(size_t pos)
{
	if (pos > file_total_bytes) return 1; //TODO check if the >= sign is needed

	buffer_current_byte = pos;
	buffer_start_byte = pos;
	buffer_end_byte = buffer_start_byte + fread(buffer, sizeof(char), READ_BUFFER, file_stream);
	return 0;
}

ISentence* CorpusReader::get_next()
{
	if (buffer_current_byte >= file_total_bytes) return nullptr;

	uint32_t sentence_characters = 0;
	const size_t buffer_index = buffer_current_byte - buffer_start_byte;
	char* current = buffer + buffer_index;
	char* sentence_body = current;
	const char* last = buffer + buffer_end_byte - buffer_start_byte;
	for (; current < last; current++)
	{
		if (*current == '\n')
		{
			ISentence* sentence = new BufferSentence(sentence_body, (uint32_t)(current - sentence_body) - (uint32_t)1 * (uint32_t)(*(current - 1) == '\r'), sentence_characters);
			buffer_current_byte += current - sentence_body + 1;
			return sentence;
		}
		sentence_characters += (*current & (unsigned char)192) != 128;
	}
	if (buffer_end_byte - buffer_start_byte != READ_BUFFER)
	{
		if (current - sentence_body > 1)
		{
			auto* sentence = new BufferSentence(sentence_body, current - sentence_body, sentence_characters);
			buffer_current_byte += current - sentence_body + 1;
			return sentence;
		}
	}
	if (read_from_pos(buffer_start_byte + buffer_index))
	{
		return get_next();
	}

	return nullptr;
}

uint32_t CorpusReader::get_sentence_count()
{
	//Save buffer and file info.
	const size_t save_current = buffer_current_byte;
	const size_t save_start = buffer_start_byte;
	const size_t save_end = buffer_end_byte;

	char* save_buf = (char*)malloc(sizeof(char) * READ_BUFFER);
	if (!save_buf) { printf("Could not allocate buffer\n"); return 0; }

	memcpy(save_buf, buffer, READ_BUFFER);

	//Count sentences
	uint32_t sentence_count = 0;
	buffer_end_byte = 0;
	while (buffer_end_byte < file_total_bytes)
	{
		read_from_pos(buffer_end_byte);
		const size_t bytes_to_read = buffer_end_byte - buffer_start_byte;
		const char* last = buffer + bytes_to_read;
		for (auto* current = buffer; current < last; current++)
		{
			sentence_count += (*current == '\n');
		}
	}

	//Restore buffer and file info.
	buffer_current_byte = save_current;
	buffer_start_byte = save_start;
	buffer_end_byte = save_end;
	memcpy(buffer, save_buf, READ_BUFFER);
	free(save_buf);
	return sentence_count;
}

CorpusReader::~CorpusReader()
{
	if (file_stream != nullptr)
		fclose(file_stream);
	free(buffer);
}