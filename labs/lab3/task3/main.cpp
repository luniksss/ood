#include <iostream>
#include "./InputStream/FileInputStream.h"
#include "./OutputStream/FileOutputStream.h"
#include "./InputStream/DecryptInputStream.h"
#include "./OutputStream/EncryptOutputStream.h"
#include "./InputStream/DecompressInputStream.h"
#include "./OutputStream/CompressOutputStream.h"

std::unique_ptr<IInputDataStream> CreateInputStream(const std::string& filename, const std::vector<std::string>& operations)
{
    std::unique_ptr<IInputDataStream> stream = std::make_unique<FileInputStream>(filename);

    for (const auto& op : operations)
    {
        if (op.rfind("decrypt:", 0) == 0)
        {
            uint64_t key = std::stoull(op.substr(8));
            stream = std::make_unique<DecryptInputStream>(std::move(stream), key);
        }
        else if (op == "decompress")
        {
            stream = std::make_unique<DecompressInputStream>(std::move(stream));
        }
    }
    return stream;
}

std::unique_ptr<IOutputDataStream> CreateOutputStream(const std::string& filename, const std::vector<std::string>& operations)
{
    std::unique_ptr<IOutputDataStream> stream = std::make_unique<FileOutputStream>(filename);

    for (const auto& op : operations)
    {
        if (op.rfind("encrypt:", 0) == 0)
        {
            uint64_t key = std::stoull(op.substr(8));
            stream = std::make_unique<EncryptOutputStream>(std::move(stream), key);
        }
        else if (op == "compress")
        {
            stream = std::make_unique<CompressOutputStream>(std::move(stream));
        }
    }
    return stream;
}

void PrintUsage()
{
    std::cout << "Usage: transform [options] <input-file> <output-file>\n";
    std::cout << "Options:\n";
    std::cout << "  --encrypt <key>   Add encryption step with key\n";
    std::cout << "  --decrypt <key>   Add decryption step with key\n";
    std::cout << "  --compress        Add compression step\n";
    std::cout << "  --decompress      Add decompression step\n";
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        PrintUsage();
        return 1;
    }

    try
    {
        const std::vector<std::string> args(argv + 1, argv + argc);
        std::string inputFile, outputFile;
        std::vector<std::string> outputOperations;
        std::vector<std::string> inputOperations;

        for (size_t i = 0; i < args.size(); )
        {
            if (args[i] == "--encrypt")
            {
                if (i + 1 >= args.size())
                {
                    throw std::runtime_error("Missing key for --encrypt");
                }
                outputOperations.push_back("encrypt:" + args[i + 1]);
                i += 2;
            }
            else if (args[i] == "--decrypt")
            {
                if (i + 1 >= args.size())
                {
                    throw std::runtime_error("Missing key for --decrypt");
                }
                inputOperations.push_back("decrypt:" + args[i + 1]);
                i += 2;
            }
            else if (args[i] == "--compress")
            {
                outputOperations.emplace_back("compress");
                i += 1;
            }
            else if (args[i] == "--decompress")
            {
                inputOperations.emplace_back("decompress");
                i += 1;
            }
            else
            {
                if (inputFile.empty())
                {
                    inputFile = args[i];
                }
                else if (outputFile.empty())
                {
                    outputFile = args[i];
                }
                else
                {
                    throw std::runtime_error("Unexpected argument: " + args[i]);
                }
                i += 1;
            }
        }

        if (inputFile.empty() || outputFile.empty())
        {
            PrintUsage();
            return 1;
        }

        auto inputStream = CreateInputStream(inputFile, inputOperations);
        auto outputStream = CreateOutputStream(outputFile, outputOperations);

        constexpr std::streamsize bufferSize = 4096;
        uint8_t buffer[bufferSize];

        while (!inputStream->IsEOF())
        {
            std::streamsize read = inputStream->ReadBlock(buffer, bufferSize);
            if (read > 0)
            {
                outputStream->WriteBlock(buffer, read);
            }
        }

        outputStream->Close();
        std::cout << "File transformed successfully: " << inputFile << " -> " << outputFile << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}