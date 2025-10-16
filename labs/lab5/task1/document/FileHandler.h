#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <format>
#include <string>
#include <random>

class FileHandler
{
public:
	static std::string GetFileName(const std::string &path)
	{
		std::cout << path << std::endl;
		size_t slashPosition = path.find_last_of('/');
		if (slashPosition != std::string::npos && slashPosition != 0)
		{
			return path.substr(slashPosition + 1);
		}

		return "";
	}

	static std::string GetFileType(const std::string &path)
	{
		size_t dotPosition = path.find_last_of('.');
		if (dotPosition != std::string::npos && dotPosition != 0)
		{
			return path.substr(dotPosition + 1);
		}

		return "";
	}

	static std::string CreateFileName(const std::string &imageExtension)
	{
		const std::string chars =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";

		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<> distrib(0, chars.size() - 1);

		std::string result;
		result.reserve(12);

		for (size_t i = 0; i < 12; ++i)
		{
			result += chars[distrib(generator)];
		}
		return std::format("file_{}.{}", result, imageExtension);
	}

	static void CopyFile(const std::string &inputFilePath, const std::string &outputFilePath)
	{
		std::ifstream inputFile(inputFilePath);
		if (!inputFile.is_open())
		{
			throw std::runtime_error("Failed to open " + inputFilePath + " for reading.");
		}

		const std::filesystem::path outputDir = std::filesystem::path(outputFilePath).parent_path();
		if (!outputDir.empty() && !exists(outputDir))
		{
			create_directories(outputDir);
		}

		std::ofstream outputFile(outputFilePath);
		if (!outputFile.is_open())
		{
			throw std::runtime_error("Failed to open " + outputFilePath + " for writing.");
		}

		CopyStreams(inputFile, outputFile);
		if (!inputFile.eof())
		{
			throw std::runtime_error("Error reading input file.");
		}
	}

	static void DeleteFileIfExists(const std::string &filePath)
	{
		if (std::filesystem::exists(filePath))
		{
			std::filesystem::remove(filePath);
		}
	}

private:
	static void CopyStreams(std::istream &inputFile, std::ostream &outputFile)
	{
		char buffer[4096];
		while (inputFile)
		{
			inputFile.read(buffer, sizeof(buffer));
			outputFile.write(buffer, inputFile.gcount());

			if (!outputFile)
			{
				throw std::runtime_error("Failed to save data on disk.");
			}
		}
	}
};
#endif //FILEHANDLER_H
