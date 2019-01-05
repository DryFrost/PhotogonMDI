#include <cstddef>
#include "BitBuffer.hpp"


qrcodegen::BitBuffer::BitBuffer() :
	data(),
	bitLength(0) {}


int qrcodegen::BitBuffer::getBitLength() const {
	return bitLength;
}


std::vector<uint8_t> qrcodegen::BitBuffer::getBytes() const {
	return data;
}


void qrcodegen::BitBuffer::appendBits(uint32_t val, int len) {
	if (len < 0 || len > 32 || (len < 32 && (val >> len) != 0))
		throw "Value out of range";
	size_t newBitLen = bitLength + len;
	while (data.size() * 8 < newBitLen)
		data.push_back(0);
	for (int i = len - 1; i >= 0; i--, bitLength++)
		data.at(bitLength >> 3) |= ((val >> i) & 1) << (7 - (bitLength & 7));
}


void qrcodegen::BitBuffer::appendData(const QrSegment &seg) {
	size_t newBitLen = bitLength + seg.bitLength;
	while (data.size() * 8 < newBitLen)
		data.push_back(0);
	for (int i = 0; i < seg.bitLength; i++, bitLength++) {
		int bit = (seg.data.at(i >> 3) >> (7 - (i & 7))) & 1;
		data.at(bitLength >> 3) |= bit << (7 - (bitLength & 7));
	}
}
