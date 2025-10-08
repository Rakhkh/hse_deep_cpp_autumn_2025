#include <gtest/gtest.h>
#include "allocator.cpp" 

class AllocatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        alloc.makeAllocator(100);
    }
    
    void TearDown() override { }
    
    Allocator alloc;
};

// Тест на выделение и запись
TEST_F(AllocatorTest, AllocationAndWrite) {
    char* block = alloc.alloc(10);
    ASSERT_NE(block, nullptr);

    strcpy(block, "test");
    EXPECT_STREQ(block, "test");
}

// Тест на несколько выделений подряд
TEST_F(AllocatorTest, MultipleAllocations) {
    char* block1 = alloc.alloc(20);
    char* block2 = alloc.alloc(30);
    
    ASSERT_NE(block1, nullptr);
    ASSERT_NE(block2, nullptr);
    EXPECT_EQ(block2 - block1, 20); 
}

// Тест на переполнение
TEST_F(AllocatorTest, Overflow) {
    char* block1 = alloc.alloc(80);
    char* block2 = alloc.alloc(30);  
    
    ASSERT_NE(block1, nullptr);
    ASSERT_EQ(block2, nullptr);
}

// Тест метода reset
TEST_F(AllocatorTest, MethodReset) {
    char* block1 = alloc.alloc(80);
    ASSERT_NE(block1, nullptr);

    alloc.reset();
    char* block2 = alloc.alloc(90);
    ASSERT_NE(block2, nullptr);
}

// Тест на выделение нулевого размера
TEST_F(AllocatorTest, ZeroSizeAllocation) {
    char* block = alloc.alloc(0);
    ASSERT_EQ(block, nullptr);
}

// Тест метода makeAllocator (обнуляет ли память)
TEST_F(AllocatorTest, MethodmakeAllocator) {
    char* block1 = alloc.alloc(80);
    ASSERT_NE(block1, nullptr);
    
    alloc.makeAllocator(200);
    char* block2 = alloc.alloc(150);
    ASSERT_NE(block2, nullptr);
}

// Тест на граничный случай
TEST_F(AllocatorTest, EdgeCase) {
    char* block = alloc.alloc(100);
    ASSERT_NE(block, nullptr);
    
    char* block2 = alloc.alloc(1);
    ASSERT_EQ(block2, nullptr);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}