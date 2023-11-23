#include "gtest/gtest.h"
#include "utl.c"
TEST(TestAzzero, doubles) {
  double *buf = new double[10];
  for (int i=0; i<10; ++i) buf[i]=i+1;
  ASSERT_DOUBLE_EQ(buf[1], 2.0);
  ASSERT_DOUBLE_EQ(buf[5], 6.0);
  ASSERT_DOUBLE_EQ(buf[9], 10.0);

  azzero(buf,10);
  ASSERT_DOUBLE_EQ(buf[1], 0.0);
  ASSERT_DOUBLE_EQ(buf[5], 0.0);
  ASSERT_DOUBLE_EQ(buf[9], 0.0);
}