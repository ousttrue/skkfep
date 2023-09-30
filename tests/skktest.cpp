#include <ctrlcode.h>
#include <gtest/gtest.h>
#include <skk.h>

TEST(SkkTest, zenkaku)
{
  Skk skk;
  EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Normal);

  {
    auto res = skk.input(CTRL_J);
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Kana);
  }

  {
    auto res = skk.input('L');
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Zenkaku);
  }

  {
    auto res = skk.input('a');
    EXPECT_EQ(res.Output, "ａ");
  }
}

TEST(SkkTest, romkan)
{
  Skk skk;

  {
    auto res = skk.input(CTRL_J);
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Kana);
  }

  {
    auto res = skk.input('a');
    EXPECT_EQ(res.Output, "あ");
  }
}
