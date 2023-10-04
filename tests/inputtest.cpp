#include <ctrlcode.h>
#include <gtest/gtest.h>
#include <skk.h>

TEST(InputTest, ascii)
{
  skk::AsciiInput input;

  auto output = input.input('a');
  EXPECT_EQ(output.Confirmed, "a");
}

TEST(InputTest, zenkaku)
{
  skk::ZenkakuInput input;

  auto output = input.input('a');
  EXPECT_EQ(output.Confirmed, "ａ");
}

TEST(InputTest, kanaVowel)
{
  skk::KanaInput input;

  auto output = input.input('a');
  EXPECT_EQ(output.Confirmed, "あ");
}

TEST(InputTest, kanaConsonant)
{
  skk::KanaInput input;

  {
    auto output = input.input('k');
    skk::Output expected{ "", "k" };
    EXPECT_EQ(output, expected);
  }
  {
    auto output = input.input('a');
    skk::Output expected{ "か", "" };
    EXPECT_EQ(output, expected);
  }
}

// TEST(InputTest, code)
// {
//   skk::CodeInput input;
//
//   auto output = input.input("3042");
//   EXPECT_EQ(output.Confirmed, "あ");
// }
