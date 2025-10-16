#include <gtest/gtest.h>
#include "../document/image/CImage.h"
#include "../document/paragraph/CParagraph.h"
#include "../document/CConstDocumentItem.h"
#include "../document/CDocumentItem.h"

TEST(CImageTests, GettersReturnCorrectValues)
{
    CImage img("image.png", 100, 200);
    EXPECT_EQ(img.GetPath(), "image.png");
    EXPECT_EQ(img.GetWidth(), 100u);
    EXPECT_EQ(img.GetHeight(), 200u);
}

TEST(CImageTests, ResizeChangesDimensions)
{
    CImage img("image.png", 100, 200);
    img.Resize(150, 300);
    EXPECT_EQ(img.GetWidth(), 150u);
    EXPECT_EQ(img.GetHeight(), 300u);
}

TEST(CParagraphTests, GetTextReturnsText)
{
    CParagraph p("Hello world");
    EXPECT_EQ(p.GetText(), "Hello world");
}

TEST(CParagraphTests, SetTextChangesText)
{
    CParagraph p("Hello");
    p.SetText("New text");
    EXPECT_EQ(p.GetText(), "New text");
}

TEST(CConstDocumentItemTests, HoldsImage)
{
    auto img = std::make_shared<CImage>("path.png", 10, 20);
    CConstDocumentItem item(img);
    EXPECT_NE(item.GetImage(), nullptr);
    EXPECT_EQ(item.GetImage()->GetPath(), "path.png");
    EXPECT_EQ(item.GetParagraph(), nullptr);
}

TEST(CConstDocumentItemTests, HoldsParagraph)
{
    auto p = std::make_shared<CParagraph>("text");
    CConstDocumentItem item(p);
    EXPECT_NE(item.GetParagraph(), nullptr);
    EXPECT_EQ(item.GetParagraph()->GetText(), "text");
    EXPECT_EQ(item.GetImage(), nullptr);
}

TEST(CDocumentItemTests, HoldsImage)
{
    auto img = std::make_shared<CImage>("pic.png", 30, 40);
    CDocumentItem item(img);
    EXPECT_NE(item.GetImage(), nullptr);
    EXPECT_EQ(item.GetImage()->GetPath(), "pic.png");
    EXPECT_EQ(item.GetParagraph(), nullptr);
}

TEST(CDocumentItemTests, HoldsParagraph)
{
    auto p = std::make_shared<CParagraph>("para");
    CDocumentItem item(p);
    EXPECT_NE(item.GetParagraph(), nullptr);
    EXPECT_EQ(item.GetParagraph()->GetText(), "para");
    EXPECT_EQ(item.GetImage(), nullptr);
}
