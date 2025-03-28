#include "alltests.h"

TEST(SInit, inserting) {
  s21::set<char> dict;
  dict.insert('M');
  dict.insert('R');
  dict.insert('B');
  EXPECT_EQ(dict.size(), 3);
  EXPECT_TRUE(dict.contains('M'));
  EXPECT_TRUE(dict.contains('R'));
  EXPECT_TRUE(dict.contains('B'));
}

TEST(SInit, listinit) {
  s21::set<int> dict = {5, 100, 1001};
  EXPECT_EQ(dict.size(), 3);
  EXPECT_TRUE(dict.contains(5));
  EXPECT_TRUE(dict.contains(100));
  EXPECT_TRUE(dict.contains(1001));
}

TEST(SInit, copy) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char> second(dict);
  EXPECT_EQ(dict.size(), 3);
  EXPECT_TRUE(dict.contains('M'));
  EXPECT_TRUE(dict.contains('R'));
  EXPECT_TRUE(dict.contains('B'));
  EXPECT_EQ(second.size(), 3);
  EXPECT_TRUE(second.contains('M'));
  EXPECT_TRUE(second.contains('R'));
  EXPECT_TRUE(second.contains('B'));
}

TEST(SInit, move) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char> second(std::move(dict));
  EXPECT_EQ(second.size(), 3);
  EXPECT_TRUE(second.contains('M'));
  EXPECT_TRUE(second.contains('R'));
  EXPECT_TRUE(second.contains('B'));
}

TEST(SOperator, equal) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char> dict2 = {'X', 'Y', 'Z'};
  dict = std::move(dict2);
  EXPECT_TRUE(dict.contains('X') && dict.contains('Y') && dict.contains('Z'));
  EXPECT_TRUE(!dict.contains('M') && !dict.contains('R') &&
              !dict.contains('B'));
}

TEST(SIterators, LookupAscend) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char>::iterator it = dict.begin();
  EXPECT_EQ(*it, 'B');
  ++it;
  EXPECT_EQ(*it, 'M');
  it++;
  EXPECT_EQ(*it, 'R');
  ++it;
  EXPECT_TRUE(it == dict.end());
}

TEST(SIterators, LookupDescend) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char>::iterator it = dict.end();
  it--;
  EXPECT_EQ(*it, 'R');
  --it;
  EXPECT_EQ(*it, 'M');
  it--;
  EXPECT_EQ(*it, 'B');
  EXPECT_TRUE(it == dict.begin());
}

TEST(SMaxSize, maxsize) {
  s21::set<char> dict = {'M', 'R', 'B'};
  EXPECT_EQ(dict.max_size(), 18446744073709551615UL);
}

TEST(SErase, base) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char>::iterator it = dict.begin();
  EXPECT_TRUE(dict.contains('B'));
  dict.erase(it);
  EXPECT_FALSE(dict.contains('B'));
  EXPECT_EQ(dict.size(), 2);
}

TEST(SErase, root) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char>::iterator it = dict.begin();
  it++;
  EXPECT_TRUE(dict.contains('M'));
  dict.erase(it);
  EXPECT_FALSE(dict.contains('M'));
  EXPECT_EQ(dict.size(), 2);
}

TEST(SSwap, base) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char> dict2 = {'X', 'Y', 'Z'};
  dict.swap(dict2);
  EXPECT_TRUE(dict.contains('X') && dict.contains('Y') && dict.contains('Z') &&
              dict2.contains('M') && dict2.contains('R') &&
              dict2.contains('B'));
  EXPECT_TRUE(!dict2.contains('X') && !dict2.contains('Y') &&
              !dict2.contains('Z') && !dict.contains('M') &&
              !dict.contains('R') && !dict.contains('B'));
}

TEST(SMerge, base) {
  s21::set<char> dict = {'M', 'R', 'B'};
  s21::set<char> dict2 = {'X', 'Y', 'Z'};
  dict.merge(dict2);
  EXPECT_TRUE(dict.contains('X') && dict.contains('Y') && dict.contains('Z') &&
              dict.contains('M') && dict.contains('R') && dict.contains('B'));
}

TEST(SEmpty, all) {
  s21::set<char> dict = {'M'};
  s21::set<char>::iterator it = dict.begin();
  EXPECT_FALSE(dict.empty());
  dict.erase(it);
  EXPECT_TRUE(dict.empty());
}

TEST(Find, general) {
  s21::set<char> dict = {'M', 'R', 'B', 'X', 'Y', 'Z'};
  s21::set<char>::iterator it = dict.find('X');
  EXPECT_EQ(*it, 'X');
  it++;
  EXPECT_EQ(*it, 'Y');
  it--;
  --it;
  EXPECT_EQ(*it, 'R');
}

TEST(SInsertions, many) {
  s21::set<char> dict = {'M', 'R', 'B'};
  std::vector<std::pair<s21::set<char>::iterator, bool>> res =
      dict.insert_many('C', 'M', 'A', 'C');
  EXPECT_EQ(*(res[0].first), 'C');
  EXPECT_EQ(res[0].second, true);
  EXPECT_EQ(*(res[1].first), 'M');
  EXPECT_EQ(res[1].second, false);
  EXPECT_EQ(*(res[2].first), 'A');
  EXPECT_EQ(res[2].second, true);
  EXPECT_EQ(*(res[3].first), 'C');
  EXPECT_EQ(res[3].second, false);
}
