#include <ctrlcode.h>
#include <gtest/gtest.h>
#include <skk.h>

TEST(SkkTest, zenkaku)
{
  Skk skk;
  EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Normal);

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Kana);
  }

  {
    auto output = skk.input('L');
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Zenkaku);
  }

  {
    auto output = skk.input('a');
    EXPECT_EQ(output.Confirmed, "ａ");
  }
}

TEST(SkkTest, romkan)
{
  Skk skk;

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Kana);
  }

  {
    auto output = skk.input('a');
    EXPECT_EQ(output.Confirmed, "あ");
  }
}
