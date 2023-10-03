#include <ctrlcode.h>
#include <gtest/gtest.h>
#include <skk.h>

TEST(SkkTest, zenkaku)
{
  skk::Skk skk;
  EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Direct);
  EXPECT_EQ(skk.inputmode(), skk::InputType::Ascii);

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Direct);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Kana);
  }

  {
    auto output = skk.input('L');
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Direct);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Zenkaku);
  }

  {
    auto output = skk.input('a');
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Direct);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Zenkaku);
    EXPECT_EQ(output.Confirmed, "ａ");
  }
}

TEST(SkkTest, romkan)
{
  skk::Skk skk;

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Direct);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Kana);
  }

  {
    auto output = skk.input('a');
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Direct);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Kana);
    EXPECT_EQ(output.Confirmed, "あ");
  }
}

TEST(SkkTest, henkan)
{
  skk::Skk skk;
  skk.open_dictionary();

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Direct);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Kana);
  }

  {
    auto output = skk.input('A');
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Entry);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Kana);
    EXPECT_EQ(output.Unconfirmed, "あ");
  }

  {
    auto output = skk.input(' ');
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Selection);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Kana);
    EXPECT_EQ(output.Unconfirmed, "亜");
  }

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk.conversionmode(), skk::ConversionType::Direct);
    EXPECT_EQ(skk.inputmode(), skk::InputType::Kana);
    EXPECT_EQ(output.Confirmed, "亜");
  }
}
