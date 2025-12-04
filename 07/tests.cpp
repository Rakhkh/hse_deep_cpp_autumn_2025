#include <gtest/gtest.h>
#include "AVL-Tree.hpp"
#include <vector>
#include <algorithm>

using avl_int_map = avl_ns::avl<int, int>;

// Тест на вставку, поиск и т.д.
TEST(AVLBasic, InsertAndFind) {
    avl_int_map tree;
    EXPECT_TRUE(tree.empty());

    auto [it1, inserted1] = tree.insert({5, 50});
    EXPECT_TRUE(inserted1);
    EXPECT_EQ(it1->second, 50);

    auto [it2, inserted2] = tree.insert({3, 30});
    EXPECT_TRUE(inserted2);
    EXPECT_EQ(it2->second, 30);

    auto [it3, inserted3] = tree.insert({7, 70});
    EXPECT_TRUE(inserted3);
    EXPECT_EQ(it3->second, 70);

    EXPECT_EQ(tree.size(), 3u);

    auto it_find = tree.find(3);
    ASSERT_NE(it_find, tree.end());
    EXPECT_EQ(it_find->second, 30);

    EXPECT_TRUE(tree.contains(5));
    EXPECT_FALSE(tree.contains(10));
}

// Тест на вставку дубликата
TEST(AVLBasic, InsertDuplicate) {
    avl_int_map tree;
    tree.insert({1, 10});
    auto [it, inserted] = tree.insert({1, 20});

    EXPECT_FALSE(inserted);
    EXPECT_EQ(it->second, 10); 
}

// Тест на оператор []
TEST(AVLBasic, OperatorAccess) {
    avl_int_map tree;

    tree[10] = 100;
    EXPECT_EQ(tree.size(), 1u);
    EXPECT_EQ(tree[10], 100);

    EXPECT_EQ(tree[5], 0);
    EXPECT_EQ(tree.size(), 2u);

    tree[5] = 55;
    EXPECT_EQ(tree[5], 55);
}

// Тест метода at
TEST(AVLBasic, AtMethod) {
    avl_int_map tree;
    tree.insert({2, 20});

    EXPECT_NO_THROW(tree.at(2));
    EXPECT_EQ(tree.at(2), 20);

    EXPECT_THROW(tree.at(99), std::out_of_range);
}

// Тест метода erase
TEST(AVLBasic, EraseElements) {
    avl_int_map tree;
    tree.insert({10, 100});
    tree.insert({5, 50});
    tree.insert({20, 200});

    EXPECT_EQ(tree.erase(5), 1u);
    EXPECT_EQ(tree.erase(999), 0u);

    EXPECT_FALSE(tree.contains(5));
    EXPECT_TRUE(tree.contains(10));

    EXPECT_EQ(tree.size(), 2u);
}

// Тест метода clear
TEST(AVLBasic, Clear) {
    avl_int_map tree;
    for (int i = 0; i < 10; i++)
        tree.insert({i, i * 10});

    EXPECT_FALSE(tree.empty());
    EXPECT_EQ(tree.size(), 10u);

    tree.clear();

    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.size(), 0u);

    EXPECT_EQ(tree.begin(), tree.end());
}

// Тест прямой итерации
TEST(AVLIterators, ForwardIterationSortedOrder) {
    avl_int_map tree;
    std::vector<int> keys = {7, 3, 9, 1, 5, 8, 10};

    for (int k : keys) tree.insert({k, k * 10});

    std::sort(keys.begin(), keys.end());

    std::vector<int> output;
    for (const auto& p : tree) {
        output.push_back(p.first);
    }

    EXPECT_EQ(output, keys);
}

// Тест обратной итерации
TEST(AVLIterators, ReverseIterationSortedOrder) {
    avl_int_map tree;
    std::vector<int> keys = {7, 3, 9, 1, 5, 8, 10};

    for (int k : keys) tree.insert({k, k * 10});

    std::sort(keys.begin(), keys.end(), std::greater<int>());

    std::vector<int> output;
    for (auto it = tree.rbegin(); it != tree.rend(); ++it) {
        output.push_back(it->first);
    }

    EXPECT_EQ(output, keys);
}

// Тест на работу инкремента и декремента итератора
TEST(AVLIterators, IteratorIncrementDecrementBasic) {
    avl_int_map tree;
    tree.insert({2, 20});
    tree.insert({1, 10});
    tree.insert({3, 30});

    auto it = tree.begin();
    EXPECT_EQ(it->first, 1);

    ++it;
    EXPECT_EQ(it->first, 2);

    ++it;
    EXPECT_EQ(it->first, 3);

    auto it2 = tree.end();
    --it2;
    EXPECT_EQ(it2->first, 3);
}

// Небольшой стресс-тест
TEST(AVLStress, Insert1000) {
    avl_int_map tree;
    const int N = 1000;

    for (int i = 0; i < N; i++)
        tree.insert({i, i});

    EXPECT_EQ(tree.size(), static_cast<size_t>(N));

    int expected = 0;
    for (const auto& p : tree) {
        EXPECT_EQ(p.first, expected);
        expected++;
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}