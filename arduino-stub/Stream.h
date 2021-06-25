#pragma once
#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include "Print.h"

#define SERIAL_RX_BUFFER_SIZE 2048

#if  (SERIAL_RX_BUFFER_SIZE>256)
typedef uint16_t rx_buffer_index_t;
#else
typedef uint8_t rx_buffer_index_t;
#endif


class Stream: public Print
{
protected:
	rx_buffer_index_t length = 0;
	uint8_t buffer[SERIAL_RX_BUFFER_SIZE];
public:
	Stream();

	int read();
    void begin(long speed) {}
    void end() {}

	int available() { return length; }
	void flush() { length = 0; }

	rx_buffer_index_t readBytes(char *buffer, rx_buffer_index_t length); // read chars from stream into buffer
	rx_buffer_index_t readBytes(const char *buffer, rx_buffer_index_t length) { return readBytes((char *)buffer, length); }
	rx_buffer_index_t readBytes(uint8_t *buffer, rx_buffer_index_t length) { return readBytes((char *)buffer, length); }

    void SetRXBuffer(uint8_t *byteArray, rx_buffer_index_t length);
	void AddRXBuffer(uint8_t *byteArray, rx_buffer_index_t length);

	void SetRXBuffer(char *byteArray, rx_buffer_index_t length);
	void SetRXBuffer(char *byteArray);

	void AddRXBuffer(char *byteArray, rx_buffer_index_t length);
	void AddRXBuffer(char *byteArray);

    rx_buffer_index_t GetTXLength();
};