#include "alltests.h"

TEST(InitAccess, charint) {
  s21::map<char, int> dict;
  dict.insert(std::make_pair<char, int>('M', 1));
  dict.insert(std::make_pair<char, int>('R', 32));
  dict.insert(std::make_pair<char, int>('B', 1467));
  EXPECT_EQ(dict.size(), 3);
  EXPECT_EQ(dict.at('M'), 1);
  EXPECT_EQ(dict.at('R'), 32);
  EXPECT_EQ(dict.at('B'), 1467);
}

TEST(InitAccess, listinit) {
  s21::map<long, double> dict = {{5, 5.123}, {100, 100.3527}, {1001, 1000.567}};
  EXPECT_EQ(dict.size(), 3);
  EXPECT_DOUBLE_EQ(dict.at(5), 5.123);
  EXPECT_DOUBLE_EQ(dict.at(100), 100.3527);
  EXPECT_DOUBLE_EQ(dict.at(1001), 1000.567);
}

TEST(InitAccess, copy) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int> second(dict);
  EXPECT_EQ(dict.size(), 3);
  EXPECT_EQ(dict.at('M'), 1);
  EXPECT_EQ(dict.at('R'), 32);
  EXPECT_EQ(dict.at('B'), 1467);
  EXPECT_EQ(second.size(), 3);
  EXPECT_EQ(second.at('M'), 1);
  EXPECT_EQ(second.at('R'), 32);
  EXPECT_EQ(second.at('B'), 1467);
}

TEST(InitAccess, move) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int> second(std::move(dict));
  EXPECT_EQ(second.size(), 3);
  EXPECT_EQ(second.at('M'), 1);
  EXPECT_EQ(second.at('R'), 32);
  EXPECT_EQ(second.at('B'), 1467);
}

TEST(Operator, equal) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int> dict2 = {{'X', 3}, {'Y', 56}, {'Z', 67}};
  dict = std::move(dict2);
  EXPECT_TRUE(dict.contains('X') && dict.contains('Y') && dict.contains('Z'));
  EXPECT_TRUE(!dict.contains('M') && !dict.contains('R') &&
              !dict.contains('B'));
}

TEST(AccessAssign, at) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  dict.at('M') = 5;
  EXPECT_EQ(dict.size(), 3);
  EXPECT_EQ(dict.at('M'), 5);
  EXPECT_EQ(dict.at('R'), 32);
  EXPECT_EQ(dict.at('B'), 1467);
}

TEST(AccessAssign, brackets) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  dict['M'] = 5;
  dict['E'] = 404;
  EXPECT_EQ(dict.size(), 4);
  EXPECT_EQ(dict.at('M'), 5);
  EXPECT_EQ(dict.at('R'), 32);
  EXPECT_EQ(dict.at('B'), 1467);
  EXPECT_EQ(dict.at('E'), 404);
}

TEST(Iterators, LookupAscend) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int>::iterator it = dict.begin();
  EXPECT_EQ(it->first, 'B');
  EXPECT_EQ(it->second, 1467);
  ++it;
  std::pair<char, int> tmp = *it;
  EXPECT_EQ(tmp.first, 'M');
  EXPECT_EQ(tmp.second, 1);
  it++;
  EXPECT_EQ(it->first, 'R');
  EXPECT_EQ(it->second, 32);
  ++it;
  EXPECT_TRUE(it == dict.end());
}

TEST(Iterators, LookupDescend) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int>::iterator it = dict.end();
  --it;
  EXPECT_EQ(it->first, 'R');
  EXPECT_EQ(it->second, 32);
  it--;
  std::pair<char, int> tmp = *it;
  EXPECT_EQ(tmp.first, 'M');
  EXPECT_EQ(tmp.second, 1);
  --it;
  EXPECT_EQ(it->first, 'B');
  EXPECT_EQ(it->second, 1467);
}

TEST(Iterators, LookupAscendTree) {
  s21::map<int, int> dict = {{4, 4},   {2, 2}, {3, 3}, {1, 1}, {8, 8}, {10, 10},
                             {11, 11}, {9, 9}, {6, 6}, {7, 7}, {5, 5}};
  s21::map<int, int>::iterator it = dict.begin(), e = dict.end();
  std::pair<int, int> c(1, 1);
  while (it != e) {
    EXPECT_EQ(it->first, c.first);
    c.first++;
    EXPECT_EQ(it->second, c.second);
    c.second++;
    it++;
  }
  EXPECT_EQ(c.first, 12);
  EXPECT_EQ(c.second, 12);
}

TEST(Iterators, LookupDescendTree) {
  s21::map<int, int> dict = {{4, 4},   {2, 2}, {3, 3}, {1, 1}, {8, 8}, {10, 10},
                             {11, 11}, {9, 9}, {6, 6}, {7, 7}, {5, 5}};
  s21::map<int, int>::iterator it = dict.begin(), e = dict.end();
  std::pair<int, int> c(11, 11);
  e--;
  while (it != e) {
    EXPECT_EQ(e->first, c.first);
    c.first--;
    EXPECT_EQ(e->second, c.second);
    c.second--;
    e--;
  }
  EXPECT_EQ(c.first, 1);
  EXPECT_EQ(c.second, 1);
  EXPECT_EQ(e->first, 1);
  EXPECT_EQ(e->second, 1);
}

TEST(MaxSize, maxsize) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  EXPECT_EQ(dict.max_size(), 18446744073709551615UL);
}

TEST(Insertions, pair) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  std::pair<s21::map<char, int>::iterator, bool> res =
      dict.insert(std::make_pair('C', 3));
  EXPECT_EQ(res.first->first, 'C');
  EXPECT_EQ(res.first->second, 3);
  EXPECT_EQ(res.second, true);
  EXPECT_EQ(dict.at('C'), 3);
}

TEST(Insertions, keyobj) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  std::pair<s21::map<char, int>::iterator, bool> res = dict.insert('C', 3);
  EXPECT_EQ(res.first->first, 'C');
  EXPECT_EQ(res.first->second, 3);
  EXPECT_EQ(res.second, true);
  EXPECT_EQ(dict.at('C'), 3);
}

TEST(Insertions, assign) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  std::pair<s21::map<char, int>::iterator, bool> res =
      dict.insert_or_assign('C', 3);
  EXPECT_EQ(res.first->first, 'C');
  EXPECT_EQ(res.first->second, 3);
  EXPECT_EQ(res.second, true);
  EXPECT_EQ(dict.at('C'), 3);
  res = dict.insert_or_assign('B', 55);
  EXPECT_EQ(res.first->first, 'B');
  EXPECT_EQ(res.first->second, 55);
  EXPECT_EQ(res.second, false);
  EXPECT_EQ(dict.at('B'), 55);
}

TEST(Insertions, many) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  std::vector<std::pair<s21::map<char, int>::iterator, bool>> res =
      dict.insert_many(std::make_pair('C', 3), std::make_pair('M', 100),
                       std::make_pair('A', 333), std::make_pair('C', 3));
  EXPECT_EQ(res[0].first->first, 'C');
  EXPECT_EQ(res[0].first->second, 3);
  EXPECT_EQ(res[0].second, true);
  EXPECT_EQ(res[1].first->first, 'M');
  EXPECT_EQ(res[1].first->second, 1);
  EXPECT_EQ(res[1].second, false);
  EXPECT_EQ(res[2].first->first, 'A');
  EXPECT_EQ(res[2].first->second, 333);
  EXPECT_EQ(res[2].second, true);
  EXPECT_EQ(res[3].first->first, 'C');
  EXPECT_EQ(res[3].first->second, 3);
  EXPECT_EQ(res[3].second, false);
}

TEST(Erase, base) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int>::iterator it = dict.begin();
  EXPECT_TRUE(dict.contains('B'));
  dict.erase(it);
  EXPECT_FALSE(dict.contains('B'));
  EXPECT_EQ(dict.size(), 2);
}

TEST(Erase, root) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int>::iterator it = dict.begin();
  it++;
  EXPECT_TRUE(dict.contains('M'));
  dict.erase(it);
  EXPECT_FALSE(dict.contains('M'));
  EXPECT_EQ(dict.size(), 2);
}

TEST(Swap, base) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int> dict2 = {{'X', 3}, {'Y', 56}, {'Z', 67}};
  dict.swap(dict2);
  EXPECT_TRUE(dict.contains('X') && dict.contains('Y') && dict.contains('Z') &&
              dict2.contains('M') && dict2.contains('R') &&
              dict2.contains('B'));
  EXPECT_TRUE(!dict2.contains('X') && !dict2.contains('Y') &&
              !dict2.contains('Z') && !dict.contains('M') &&
              !dict.contains('R') && !dict.contains('B'));
}

TEST(Merge, base) {
  s21::map<char, int> dict = {{'M', 1}, {'R', 32}, {'B', 1467}};
  s21::map<char, int> dict2 = {{'X', 3}, {'Y', 56}, {'Z', 67}};
  dict.merge(dict2);
  EXPECT_TRUE(dict.contains('X') && dict.contains('Y') && dict.contains('Z') &&
              dict.contains('M') && dict.contains('R') && dict.contains('B'));
}