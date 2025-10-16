#ifndef IDOCUMENT_H
#define IDOCUMENT_H
#include <optional>
#include "CConstDocumentItem.h"
#include "CDocumentItem.h"

class IDocument
{
public:
    virtual size_t GetItemsCount() const = 0;
	virtual CConstDocumentItem GetItem(size_t index) const = 0;
	virtual CDocumentItem GetItem(size_t index) = 0;
	virtual void DeleteItem(size_t index) = 0;

	virtual std::string GetTitle() const = 0;
	virtual void SetTitle(const std::string &title) = 0;

	virtual void InsertParagraph(const std::string &text, std::optional<size_t> position) = 0;
	virtual void InsertImage(
		const std::string &path,
		unsigned width,
		unsigned height,
		std::optional<size_t> position) = 0;
	virtual void Save(const std::string &path) const = 0;

	virtual void ReplaceText(const std::string &text, size_t position) = 0;
	virtual void ResizeImage(size_t position, unsigned width, unsigned height) = 0;

	virtual bool CanBeUndo() const = 0;
	virtual bool CanRedo() const = 0;
	virtual void Undo() = 0;
	virtual void Redo() = 0;

	virtual ~IDocument() = default;
};

#endif //IDOCUMENT_H
