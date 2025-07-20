#include "pch.h"

namespace TestBasics {
	TEST(TestBasics, TestEq) {
		EXPECT_EQ(1, 1);
	}


	TEST(TestBasics, TestTrue) {
		EXPECT_TRUE(true);
	}


	TEST(TestBasics, TestNe) {
		EXPECT_NE("a", "b");
	}

}