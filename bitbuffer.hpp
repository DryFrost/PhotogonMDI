#ifndef BITBUFFER_HPP
#define BITBUFFER_HPP


#pragma once

#include <cstdint>
#include <vector>
#include "QrSegment.hpp"


namespace qrcodegen {


class BitBuffer final {

private:

	std::vector<uint8_t> data;
	int bitLength;



public:

	BitBuffer();



public:

	int getBitLength() const;


	std::vector<uint8_t> getBytes() const;

	void appendBits(uint32_t val, int len);


	void appendData(const QrSegment &seg);

};

}

#endif // BITBUFFER_HPP
