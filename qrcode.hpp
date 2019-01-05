#ifndef QRCODE_HPP
#define QRCODE_HPP


#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include "qrsegment.hpp"

namespace qrcodegen {


class QrCode final {

public:


	class Ecc final {
	public:
		const static Ecc LOW, MEDIUM, QUARTILE, HIGH;

	public:
		const int ordinal;
		const int formatBits;

	private:
		Ecc(int ord, int fb);
	};



public:


	static QrCode encodeText(const char *text, const Ecc &ecl);



	static QrCode encodeBinary(const std::vector<uint8_t> &data, const Ecc &ecl);



	static QrCode encodeSegments(const std::vector<QrSegment> &segs, const Ecc &ecl,
		int minVersion=1, int maxVersion=40, int mask=-1, bool boostEcl=true);



public:

	const int version;

	const int size;

	const Ecc &errorCorrectionLevel;

private:
	int mask;

private:
	std::vector<std::vector<bool>> modules;
	std::vector<std::vector<bool>> isFunction;



public:


	QrCode(int ver, const Ecc &ecl, const std::vector<uint8_t> &dataCodewords, int mask);



	QrCode(const QrCode &qr, int mask);



public:

	int getMask() const;



	int getModule(int x, int y) const;



	std::string toSvgString(int border) const;



private:

	void drawFunctionPatterns();



	void drawFormatBits(int mask);



	void drawVersion();


	void drawFinderPattern(int x, int y);


	void drawAlignmentPattern(int x, int y);



	void setFunctionModule(int x, int y, bool isBlack);


private:


	std::vector<uint8_t> appendErrorCorrection(const std::vector<uint8_t> &data) const;



	void drawCodewords(const std::vector<uint8_t> &data);



	void applyMask(int mask);



	int handleConstructorMasking(int mask);



	int getPenaltyScore() const;




private:


	static std::vector<int> getAlignmentPatternPositions(int ver);



	static int getNumRawDataModules(int ver);



	static int getNumDataCodewords(int ver, const Ecc &ecl);


private:

	static const int PENALTY_N1;
	static const int PENALTY_N2;
	static const int PENALTY_N3;
	static const int PENALTY_N4;

	static const int16_t NUM_ERROR_CORRECTION_CODEWORDS[4][41];
	static const int8_t NUM_ERROR_CORRECTION_BLOCKS[4][41];



private:


	class ReedSolomonGenerator final {

	private:


		std::vector<uint8_t> coefficients;


	public:


		ReedSolomonGenerator(int degree);


	public:


		std::vector<uint8_t> getRemainder(const std::vector<uint8_t> &data) const;


	private:


		static uint8_t multiply(uint8_t x, uint8_t y);

	};

};

}


#endif // QRCODE_HPP
