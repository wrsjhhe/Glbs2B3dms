#pragma once
#include <string>



class GlbToB3dm
{
public:
	GlbToB3dm();

	~GlbToB3dm();


	void Transform(char* pGlbBuffer, size_t nGLength,
		char* pFeatureTableJson = nullptr, size_t nFJLength = 0,
		char* pFeatureTableBinary = nullptr, size_t nFBLength = 0,
		char* pBatchTableJson = nullptr, size_t nBJLength = 0,
		char* pBatchTableBinary = nullptr, size_t nBBLength = 0);

	void Write(std::string path);

private:
	int getBufferPadded(char* pBuffer,  int nBufferLength, char*& pNewBuffer, int byteOffset = 0);

	std::string getJsonBufferPadded(char* pString,  int nStringLength, int byteOffset = 0);

private:
	char* m_pB3dmBuffer;
	int   m_nB3dmLength;
};

