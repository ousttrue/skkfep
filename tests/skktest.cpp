#include <ctrlcode.h>
#include <gtest/gtest.h>
#include <skk.h>

TEST(SkkTest, zenkaku)
{
  skk::Skk skk;
  EXPECT_EQ(skk::ConversionType::Direct, skk.conversionmode());
  EXPECT_EQ(skk::InputType::Ascii, skk.inputmode());

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk::ConversionType::Direct, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Kana, skk.inputmode());
  }

  {
    auto output = skk.input('L');
    EXPECT_EQ(skk::ConversionType::Direct, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Zenkaku, skk.inputmode());
  }

  {
    auto output = skk.input('a');
    EXPECT_EQ(skk::ConversionType::Direct, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Zenkaku, skk.inputmode());
    EXPECT_EQ("ａ", output.Confirmed);
  }
}

TEST(SkkTest, romkan)
{
  skk::Skk skk;

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk::ConversionType::Direct, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Kana, skk.inputmode());
  }

  {
    auto output = skk.input('a');
    EXPECT_EQ(skk::ConversionType::Direct, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Kana, skk.inputmode());
    EXPECT_EQ("あ", output.Confirmed);
  }
}

TEST(SkkTest, henkan)
{
  skk::Skk skk;
  skk.open_dictionary();

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk::ConversionType::Direct, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Kana, skk.inputmode());
  }

  {
    auto output = skk.input('A');
    EXPECT_EQ(skk::ConversionType::Entry, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Kana, skk.inputmode());
    EXPECT_EQ("あ", output.Unconfirmed);
  }

  {
    auto output = skk.input(' ');
    EXPECT_EQ(skk::ConversionType::Selection, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Kana, skk.inputmode());
    EXPECT_EQ("亜", output.Unconfirmed);
  }

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk::ConversionType::Direct, skk.conversionmode());
    EXPECT_EQ(skk::InputType::Kana, skk.inputmode());
    EXPECT_EQ("亜", output.Confirmed);
  }
}
