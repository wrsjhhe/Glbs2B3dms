#include <fstream>
#include <string>
#include <filesystem>
#include "glbToB3dm.h"
namespace fs = std::filesystem;


void getAllGlbFiles(const std::string& dir,std::vector<std::string>& vecFilePath)
{
	for (const auto& entry : fs::directory_iterator(dir)) 
	{
		if (fs::is_directory(entry.path())) 
		{
			getAllGlbFiles(entry.path().string() + '/', vecFilePath);
		}
		else
		{
			std::string filePath = entry.path().string();
			std::string type = filePath.substr(filePath.size() - 3, filePath.size());
			if (type == "glb")
			{
				vecFilePath.emplace_back(entry.path().string());
			}
		}
	}
}

int main(int argc,char* argv[])
{

	if (argc < 2)
	{
		return -1;
	}

	std::string dir = argv[1];

	std::vector<std::string> vecFilePaths;
	getAllGlbFiles(dir, vecFilePaths);

	for (auto& file : vecFilePaths)
	{
		std::string& path = file;
		int fileSize = fs::file_size(path);
		std::ifstream ifs;
		ifs.open(path, std::ios::in | std::ios::binary);
		char* glbBuffer = new char[fileSize];

		ifs.read(glbBuffer, fileSize);

		std::string strFeatureTableJson = "{\"BATCH_LENGTH\":0}";

		GlbToB3dm g2b;
		g2b.Transform(glbBuffer, fileSize, strFeatureTableJson.data(), strFeatureTableJson.size());

		std::string outputPath = path.substr(0, path.size() - 3);
		outputPath += "b3dm";
		g2b.Write(outputPath);

		ifs.close();
		delete[] glbBuffer;
	}


	return 0;
}