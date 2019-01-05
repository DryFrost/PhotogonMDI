#ifndef QRSEGMENT_HPP
#define QRSEGMENT_HPP


#pragma once

#include <cstdint>
#include <vector>


namespace qrcodegen {


class QrSegment final {

public:
	class Mode final {

	public:

		static const Mode NUMERIC;
		static const Mode ALPHANUMERIC;
		static const Mode BYTE;
		static const Mode KANJI;



	public:
		const int modeBits;

	private:
		int numBitsCharCount[3];



	private:
		Mode(int mode, int cc0, int cc1, int cc2);

	public:
		int numCharCountBits(int ver) const;

	};



public:


	static QrSegment makeBytes(const std::vector<uint8_t> &data);


	static QrSegment makeNumeric(const char *digits);

	static QrSegment makeAlphanumeric(const char *text);

	static std::vector<QrSegment> makeSegments(const char *text);


public:


	static bool isAlphanumeric(const char *text);

	static bool isNumeric(const char *text);



public:

	const Mode mode;

	const int numChars;

	const std::vector<uint8_t> data;

	const int bitLength;


public:


	QrSegment(const Mode &md, int numCh, const std::vector<uint8_t> &b, int bitLen);


	static int getTotalBits(const std::vector<QrSegment> &segs, int version);


private:

	static const int8_t ALPHANUMERIC_ENCODING_TABLE[59];

};

}


#endif // QRSEGMENT_HPP
