#include "Stream.h"

Stream::Stream():Print()
{
}

int Stream::read()
{
	uint8_t bt = 0;
	readBytes(&bt, 1);
	return bt;
}

rx_buffer_index_t Stream::readBytes(char *buffer, rx_buffer_index_t length)
{ 
	if (this->length == 0 || length == 0) return 0;
	rx_buffer_index_t readAmount = length;
	if (readAmount > this->length)  readAmount = this->length;

	for (rx_buffer_index_t i = 0; i < this->length; i++) {
		if (i < readAmount) {
			buffer[i] = this->buffer[i];
		}
		else {
			this->buffer[i - readAmount] = this->buffer[i];
		}
	}

	this->length -= readAmount;

	return readAmount;
};

void Stream::SetRXBuffer(uint8_t *byteArray, rx_buffer_index_t length)
{
	if (length > SERIAL_RX_BUFFER_SIZE) length = SERIAL_RX_BUFFER_SIZE;

	for (rx_buffer_index_t i = 0; i < length; i++) {
		buffer[i] = byteArray[i];
	}
	this->length = length;
}


void Stream::AddRXBuffer(uint8_t *byteArray, rx_buffer_index_t length)
{
	if (SERIAL_RX_BUFFER_SIZE - this->length < length) {
		length = SERIAL_RX_BUFFER_SIZE - this->length;
	}

	if (length == 0) return;

	for (rx_buffer_index_t i = 0; i < length; i++) {
		buffer[i + this->length] = byteArray[i];
	}
	this->length += length;
}

void Stream::SetRXBuffer(char *str, rx_buffer_index_t length)
{
	SetRXBuffer((uint8_t *)str, length);
}
void Stream::SetRXBuffer(char *str)
{
	SetRXBuffer((uint8_t *)str, (rx_buffer_index_t)strlen(str));
}

void Stream::AddRXBuffer(char *str, rx_buffer_index_t length)
{
	AddRXBuffer((uint8_t *)str, length);
}
void Stream::AddRXBuffer(char *str)
{
	AddRXBuffer((uint8_t *)str, (rx_buffer_index_t)strlen(str));
}

rx_buffer_index_t Stream::GetTXLength()
{
    return this->length;
}
