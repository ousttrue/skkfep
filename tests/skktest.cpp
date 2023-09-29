#include <gtest/gtest.h>
#include <skk.h>

TEST(SkkTest, romkan)
{
  Skk skk;

  auto res = skk.input('a');
  EXPECT_EQ(res.Output, "あ");
}
