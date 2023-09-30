#include <ctrlcode.h>
#include <gtest/gtest.h>
#include <skk.h>

TEST(SkkTest, romkan)
{
  Skk skk;

  {
    auto res = skk.input(CTRL_J);
    // EXPECT_EQ(res.Mode, SkkModes::KANA_MODE);
  }

  {
    auto res = skk.input('a');
    EXPECT_EQ(res.Output, "あ");
  }
}
