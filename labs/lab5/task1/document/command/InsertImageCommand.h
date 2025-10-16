#ifndef INSERTIMAGECOMMAND_H
#define INSERTIMAGECOMMAND_H
#include <format>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include "EditorCommand.h"
#include "../CDocumentItem.h"
#include "../FileHandler.h"
#include "../image/CImage.h"

class InsertImageCommand: public EditorCommand
{
public:
	InsertImageCommand(
		std::string path,
		const unsigned width,
		const unsigned height,
		const std::optional<size_t> position,
		std::vector<CDocumentItem> &items
	): m_inputPath(std::move(path)),
	   m_width(width),
	   m_height(height),
	   m_position(position),
	   m_items(items)
	{
	}

	void DoExecute() override
	{
		if (!m_savedPath.has_value())
		{
			m_savedPath = SaveImage(m_inputPath);
		}

		const auto image = std::make_shared<CImage>(m_savedPath.value(), m_width, m_height);
		const CDocumentItem documentItem(image);
		if (!m_position.has_value())
		{
			m_items.push_back(documentItem);
			return;
		}

		const size_t position = m_position.value();
		if (position > m_items.size())
		{
			throw std::invalid_argument("Position out of range");
		}

		m_shouldDelete = false;
		m_items.insert(m_items.begin() + static_cast<std::vector<int>::difference_type>(position), documentItem);
	}

	void DoUnexecute() override
	{
		if (m_items.empty())
		{
			return;
		}

		m_shouldDelete = true;
		if (!m_position.has_value())
		{
			m_items.pop_back();
			return;
		}

		const size_t position = m_position.value();
		if (position < m_items.size())
		{
			m_items.erase(m_items.begin() + static_cast<std::vector<CDocumentItem>::iterator::difference_type>(position));
		}
	}

	~InsertImageCommand() override
	{
		Destroy();
	}

protected:
	bool m_shouldDelete = false;

private:
	void Destroy()
	{
		if (m_savedPath.has_value() && m_shouldDelete)
		{
			FileHandler::DeleteFileIfExists(m_savedPath.value());
		}
		m_shouldDelete = false;
	}

	static std::string SaveImage(const std::string &path)
	{
		const auto extension = FileHandler::GetFileType(path);
		const auto newFilePath = std::format("userImages/{}", FileHandler::CreateFileName(extension));

		FileHandler::CopyFile(path, newFilePath);
		return newFilePath;
	}

	std::string m_inputPath;
	unsigned m_width;
	unsigned m_height;
	std::optional<size_t> m_position;

	std::optional<std::string> m_savedPath = std::nullopt;
	std::vector<CDocumentItem> &m_items;
};

#endif //INSERTIMAGECOMMAND_H
