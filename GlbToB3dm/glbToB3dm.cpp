#include "glbToB3dm.h"
#include <fstream>

const int g_boundary = 8;

struct TileHeader
{
	char type[4];
	uint32_t  version;
	uint32_t  totalLength;
	uint32_t  featureTableJsonBufferLength;
	uint32_t  featureTableBinaryBufferLength;
	uint32_t  batchTableJsonBufferLength;
	uint32_t  batchTableBinaryBufferLength;
};

GlbToB3dm::GlbToB3dm():
	m_pB3dmBuffer(nullptr),
	m_nB3dmLength(0)
{

}

GlbToB3dm::~GlbToB3dm()
{

}

void GlbToB3dm::Transform(char* pGlbBuffer, size_t nGLength,
	char* pFeatureTableJson, size_t nFJLength,
	char* pFeatureTableBinary, size_t nFBLength,
	char* pBatchTableJson, size_t nBJLength,
	char* pBatchTableBinary, size_t nBBLength)
{
	const int headerByteLength = 28;
	char* featureTableJsonBuffer = nullptr;
	char* featureTableBinaryBuffer = nullptr;
	char* batchTableJsonBuffer = nullptr;
	char* batchTableBinaryBuffer = nullptr;

	std::string featureTableJson = getJsonBufferPadded(pFeatureTableJson, nFJLength, headerByteLength);
	int featureTableJsonBufferLength = featureTableJson.size();
	if (featureTableJsonBufferLength)
	{
		featureTableJsonBuffer = new char[featureTableJsonBufferLength];
		memcpy(featureTableJsonBuffer, featureTableJson.data(), featureTableJsonBufferLength);
	}


	int featureTableBinaryLength = getBufferPadded(pFeatureTableBinary, nFBLength, featureTableBinaryBuffer);

	std::string batchTableJson = getJsonBufferPadded(pBatchTableJson, nBJLength);
	int batchTableJsonLength = batchTableJson.size();
	if (batchTableJsonLength)
	{
		batchTableJsonBuffer = new char[batchTableJsonLength];
		memcpy(batchTableJsonBuffer, batchTableJson.data(), batchTableJsonLength);
	}

	int batchTableBinaryBufferLength = getBufferPadded(pBatchTableBinary,nBBLength,batchTableBinaryBuffer);

	m_nB3dmLength = headerByteLength + featureTableJsonBufferLength +
		featureTableBinaryLength + batchTableJsonLength +
		batchTableBinaryBufferLength + nGLength;

	TileHeader* pHeader = new TileHeader;
	const char* type = "b3dm";
	memcpy(pHeader->type, type, 4);
	pHeader->version = 1;
	pHeader->totalLength = m_nB3dmLength;
	pHeader->featureTableJsonBufferLength = featureTableJsonBufferLength;
	pHeader->featureTableBinaryBufferLength = featureTableBinaryLength;
	pHeader->batchTableJsonBufferLength = batchTableJsonLength;
	pHeader->batchTableBinaryBufferLength = batchTableBinaryBufferLength;
	
	int offset = 28;
	m_pB3dmBuffer = new char[m_nB3dmLength];
	memcpy(m_pB3dmBuffer, pHeader, offset);
	memcpy(m_pB3dmBuffer + offset, featureTableJsonBuffer, featureTableJsonBufferLength);
	offset += featureTableJsonBufferLength;
	if (featureTableBinaryBuffer != nullptr)
	{
		memcpy(m_pB3dmBuffer + offset, featureTableBinaryBuffer, featureTableBinaryLength);
	}
	
	offset += featureTableBinaryLength;
	if (batchTableJsonBuffer != nullptr)
	{
		memcpy(m_pB3dmBuffer + offset, batchTableJsonBuffer, batchTableJsonLength);
	}
	
	offset += batchTableJsonLength;
	if (batchTableBinaryBuffer != nullptr)
	{
		memcpy(m_pB3dmBuffer + offset, batchTableBinaryBuffer, batchTableBinaryBufferLength);
	}
	offset += batchTableBinaryBufferLength;

	memcpy(m_pB3dmBuffer + offset, pGlbBuffer, nGLength);

	delete[] featureTableJsonBuffer, featureTableBinaryBuffer, batchTableJsonBuffer, batchTableBinaryBuffer;

}

void GlbToB3dm::Write(std::string path)
{
	std::ofstream ofs;
	ofs.open(path, std::ios::out | std::ios::binary);
	ofs.write(m_pB3dmBuffer, m_nB3dmLength);
	ofs.close();
}

int GlbToB3dm::getBufferPadded(char* pBuffer, int nBufferLength,char*& pNewBuffer, int byteOffset)
{
	if (!pBuffer)
	{
		return 0;
	}

	int remainder = (byteOffset + nBufferLength) % g_boundary;
	int padding = (remainder == 0) ? 0 : g_boundary - remainder;
	int nTotalLength = nBufferLength + padding;
	pNewBuffer = new char[nTotalLength];
	memcpy(pNewBuffer, pBuffer, nBufferLength);

	return nTotalLength;
}

std::string GlbToB3dm::getJsonBufferPadded(char* pString, int nStringLength, int byteOffset)
{
	if (!pString)
	{
		return std::string();
	}
	std::string tempStr(pString);
	int remainder = (byteOffset + nStringLength) % g_boundary;
	int padding = (remainder == 0) ? 0 : g_boundary - remainder;

	for (int i = 0; i < padding; ++i) {
		tempStr += ' ';
	}

	return tempStr;
}