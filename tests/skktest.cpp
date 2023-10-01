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

TEST(SkkTest, henkan)
{
  Skk skk;

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Kana);
  }

  {
    auto output = skk.input('A');
    EXPECT_EQ(output.Unconfirmed, "あ");
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::KanjiInput);
  }

  {
    auto output = skk.input(' ');
    EXPECT_EQ(output.Unconfirmed, "亜");
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Selection);
  }

  {
    auto output = skk.input(CTRL_J);
    EXPECT_EQ(output.Confirmed, "亜");
    EXPECT_EQ(skk.currentKeymap().Type, KeymapTypes::Kana);
  }
}
