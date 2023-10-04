#include <ctrlcode.h>
#include <gtest/gtest.h>
#include <skk.h>

TEST(InputTest, ascii)
{
  skk::AsciiInput input;

  auto res = input.putc('a');
  EXPECT_EQ("a", res.Output.Confirmed);
}

TEST(InputTest, zenkaku)
{
  skk::ZenkakuInput input;

  auto res = input.putc('a');
  EXPECT_EQ("ａ", res.Output.Confirmed);
}

TEST(InputTest, kanaVowel)
{
  skk::KanaInput input;

  auto res = input.putc('a');
  EXPECT_EQ("あ", res.Output.Confirmed);
}

TEST(InputTest, kanaConsonant)
{
  skk::KanaInput input;

  // https://zenn.dev/uga_rosa/articles/ec5281d5a95a57#%E3%83%86%E3%82%B9%E3%83%88
  {
    auto res = input.putc('k');
    skk::Output expected{ "", "k" };
    EXPECT_EQ(expected, res.Output);
  }
  {
    auto res = input.putc('a');
    skk::Output expected{ "か", "" };
    EXPECT_EQ(expected, res.Output);
  }

  {
    auto res = input.puts(u8"ohayou");
    skk::Output expected{ "おはよう" };
    EXPECT_EQ(expected, res.Output);
  }

  {
    auto res = input.puts(u8"amenbo");
    skk::Output expected{ "あめんぼ" };
    EXPECT_EQ(expected, res.Output);
  }

  {
    auto res = input.puts(u8"uwwwa");
    skk::Output expected{ "うwっわ" };
    EXPECT_EQ(expected, res.Output);
  }

  {
    auto res = input.puts(u8"rkakyra");
    skk::Output expected{ "から" };
    EXPECT_EQ(expected, res.Output);
  }
}

// TEST(InputTest, code)
// {
//   skk::CodeInput input;
//
//   auto output = input.input("3042");
//   EXPECT_EQ("あ", output.Confirmed);
// }
