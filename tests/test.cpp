#include <gtest.h>
#include "TMultistack.h"

TEST(TMultistackTest, DefaultConstructor)
{
  TMultistack<int> multistack;
  EXPECT_EQ(multistack.GetStacksCount(), 0);
  EXPECT_EQ(multistack.GetTotalCapacity(), 0);
  EXPECT_EQ(multistack.GetMemory(), nullptr);
  EXPECT_EQ(multistack.GetTops(), nullptr);
  EXPECT_EQ(multistack.GetCapacities(), nullptr);
}

TEST(TMultistackTest, ParameterizedConstructor)
{
  TMultistack<int> multistack(3, 15);
  EXPECT_EQ(multistack.GetStacksCount(), 3);
  EXPECT_EQ(multistack.GetTotalCapacity(), 15);

  size_t* capacities = multistack.GetCapacities();
  EXPECT_EQ(capacities[0], 5);
  EXPECT_EQ(capacities[1], 5);
  EXPECT_EQ(capacities[2], 5);

  for (size_t i = 0; i < 3; ++i)
  {
    EXPECT_TRUE(multistack.IsEmpty(i));
    EXPECT_FALSE(multistack.IsFull(i));
    EXPECT_EQ(multistack.Size(i), 0);
  }
}

TEST(TMultistackTest, CopyConstructor)
{
  TMultistack<int> multistack1(2, 10);
  multistack1.push(0, 1);
  multistack1.push(0, 2);
  multistack1.push(1, 3);
  multistack1.push(1, 4);

  TMultistack<int> multistack2(multistack1);

  EXPECT_EQ(multistack2.GetStacksCount(), 2);
  EXPECT_EQ(multistack2.GetTotalCapacity(), 10);
  EXPECT_EQ(multistack2.Size(0), 2);
  EXPECT_EQ(multistack2.Size(1), 2);

  EXPECT_EQ(multistack2.pop(0), 2);
  EXPECT_EQ(multistack2.pop(0), 1);
  EXPECT_EQ(multistack2.pop(1), 4);
  EXPECT_EQ(multistack2.pop(1), 3);
}

TEST(TMultistackTest, MoveConstructor)
{
  TMultistack<int> multistack1(2, 10);
  multistack1.push(0, 1);
  multistack1.push(1, 2);

  TMultistack<int> multistack2(std::move(multistack1));

  EXPECT_EQ(multistack2.GetStacksCount(), 2);
  EXPECT_EQ(multistack2.GetTotalCapacity(), 10);
  EXPECT_EQ(multistack2.Size(0), 1);
  EXPECT_EQ(multistack2.Size(1), 1);

  EXPECT_EQ(multistack1.GetStacksCount(), 0);
  EXPECT_EQ(multistack1.GetTotalCapacity(), 0);
  EXPECT_EQ(multistack1.GetMemory(), nullptr);
}

TEST(TMultistackTest, PushAndPop)
{
  TMultistack<int> multistack(2, 6);
  multistack.push(0, 10);
  multistack.push(0, 20);
  multistack.push(1, 30);
  multistack.push(1, 40);

  EXPECT_EQ(multistack.Size(0), 2);
  EXPECT_EQ(multistack.Size(1), 2);
  EXPECT_FALSE(multistack.IsEmpty(0));
  EXPECT_FALSE(multistack.IsEmpty(1));
  EXPECT_FALSE(multistack.IsFull(0));
  EXPECT_FALSE(multistack.IsFull(1));

  EXPECT_EQ(multistack.pop(0), 20);
  EXPECT_EQ(multistack.pop(0), 10);
  EXPECT_EQ(multistack.pop(1), 40);
  EXPECT_EQ(multistack.pop(1), 30);

  EXPECT_TRUE(multistack.IsEmpty(0));
  EXPECT_TRUE(multistack.IsEmpty(1));
}

TEST(TMultistackTest, PushWithRedistribution)
{
  TMultistack<int> multistack(2, 4);
  multistack.push(0, 1);
  multistack.push(0, 2);
  multistack.push(1, 3);
  multistack.push(1, 4);

  multistack.push(0, 5);

  EXPECT_GE(multistack.GetTotalCapacity(), 4);
  EXPECT_EQ(multistack.Size(0), 3);
  EXPECT_EQ(multistack.Size(1), 2);

  EXPECT_EQ(multistack.pop(0), 5);
  EXPECT_EQ(multistack.pop(0), 2);
  EXPECT_EQ(multistack.pop(0), 1);
  EXPECT_EQ(multistack.pop(1), 4);
  EXPECT_EQ(multistack.pop(1), 3);
}

TEST(TMultistackTest, PopEmptyStack)
{
  TMultistack<int> multistack(2, 4);
  EXPECT_THROW(multistack.pop(0), const char*);
  EXPECT_THROW(multistack.pop(1), const char*);
}

TEST(TMultistackTest, InvalidStackIndex)
{
  TMultistack<int> multistack(2, 4);
  EXPECT_THROW(multistack.push(2, 10), const char*);
  EXPECT_THROW(multistack.pop(2), const char*);
  EXPECT_THROW(multistack.IsEmpty(2), const char*);
  EXPECT_THROW(multistack.IsFull(2), const char*);
}

TEST(TMultistackTest, EqualityOperator)
{
  TMultistack<int> multistack1(2, 6);
  multistack1.push(0, 1);
  multistack1.push(0, 2);
  multistack1.push(1, 3);

  TMultistack<int> multistack2(2, 6);
  multistack2.push(0, 1);
  multistack2.push(0, 2);
  multistack2.push(1, 3);

  TMultistack<int> multistack3(2, 6);
  multistack3.push(0, 4);
  multistack3.push(0, 5);
  multistack3.push(1, 6);

  EXPECT_TRUE(multistack1 == multistack2);
  EXPECT_FALSE(multistack1 == multistack3);
  EXPECT_TRUE(multistack1 != multistack3);
}

TEST(TMultistackTest, SetStacksCount)
{
  TMultistack<int> multistack(2, 6);
  multistack.push(0, 1);
  multistack.push(1, 2);

  multistack.SetStacksCount(3);

  EXPECT_EQ(multistack.GetStacksCount(), 3);
  EXPECT_EQ(multistack.Size(0), 1);
  EXPECT_EQ(multistack.Size(1), 1);
  EXPECT_EQ(multistack.Size(2), 0);
}

TEST(TMultistackTest, SetTotalCapacity)
{
  TMultistack<int> multistack(2, 4);
  multistack.push(0, 1);
  multistack.push(0, 2);
  multistack.push(1, 3);

  multistack.SetTotalCapacity(8);

  EXPECT_EQ(multistack.GetTotalCapacity(), 8);
  EXPECT_EQ(multistack.Size(0), 2);
  EXPECT_EQ(multistack.Size(1), 1);

  EXPECT_EQ(multistack.pop(0), 2);
  EXPECT_EQ(multistack.pop(0), 1);
  EXPECT_EQ(multistack.pop(1), 3);
}

TEST(TMultistackTest, Iterator)
{
  TMultistack<int> multistack(2, 6);
  multistack.push(0, 1);
  multistack.push(0, 2);
  multistack.push(0, 3);
  multistack.push(1, 4);
  multistack.push(1, 5);

  int expected0[] = {1, 2, 3};
  int i = 0;
  for (auto it = multistack.begin(0); it != multistack.end(0); ++it)
    EXPECT_EQ(*it, expected0[i++]);

  int expected1[] = {4, 5};
  i = 0;
  for (auto it = multistack.begin(1); it != multistack.end(1); ++it)
    EXPECT_EQ(*it, expected1[i++]);

  auto it = multistack.begin(0);
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);

  auto it2 = it++;
  EXPECT_EQ(*it2, 2);
  EXPECT_EQ(*it, 3);
}

TEST(TMultistackTest, IteratorEquality)
{
  TMultistack<int> multistack(2, 4);
  multistack.push(0, 1);
  multistack.push(0, 2);

  auto it1 = multistack.begin(0);
  auto it2 = multistack.begin(0);
  auto it3 = multistack.end(0);

  EXPECT_TRUE(it1 == it2);
  EXPECT_FALSE(it1 == it3);
  EXPECT_TRUE(it1 != it3);
}

TEST(IOStreamTest, MultistackOutput)
{
  TMultistack<int> multistack(2, 6);
  multistack.push(0, 1);
  multistack.push(0, 2);
  multistack.push(1, 3);
  multistack.push(1, 4);

  std::stringstream ss;
  ss << multistack;

  std::string output = ss.str();
  EXPECT_TRUE(output.find("[1, 2]") != std::string::npos);
  EXPECT_TRUE(output.find("[3, 4]") != std::string::npos);
}

TEST(IOStreamTest, MultistackInput)
{
  TMultistack<int> multistack;
  std::stringstream ss("2 10\n2 1 2\n3 3 4 5");

  ss >> multistack;

  EXPECT_EQ(multistack.GetStacksCount(), 2);
  EXPECT_EQ(multistack.GetTotalCapacity(), 20);
  EXPECT_EQ(multistack.Size(0), 2);
  EXPECT_EQ(multistack.Size(1), 3);

  EXPECT_EQ(multistack.pop(0), 2);
  EXPECT_EQ(multistack.pop(0), 1);
  EXPECT_EQ(multistack.pop(1), 5);
  EXPECT_EQ(multistack.pop(1), 4);
  EXPECT_EQ(multistack.pop(1), 3);
}

TEST(TemplateTest, MultistackWithDouble)
{
  TMultistack<double> multistack(2, 6);
  multistack.push(0, 1.5);
  multistack.push(0, 2.7);
  multistack.push(1, 3.14);

  EXPECT_DOUBLE_EQ(multistack.pop(0), 2.7);
  EXPECT_DOUBLE_EQ(multistack.pop(0), 1.5);
  EXPECT_DOUBLE_EQ(multistack.pop(1), 3.14);
}

TEST(TemplateTest, MultistackWithString)
{
  TMultistack<std::string> multistack(2, 6);
  multistack.push(0, "hello");
  multistack.push(0, "world");
  multistack.push(1, "test");

  EXPECT_EQ(multistack.pop(0), "world");
  EXPECT_EQ(multistack.pop(0), "hello");
  EXPECT_EQ(multistack.pop(1), "test");
}

TEST(EdgeCaseTest, MultistackSingleStack)
{
  TMultistack<int> multistack(1, 5);
  multistack.push(0, 1);
  multistack.push(0, 2);
  multistack.push(0, 3);

  EXPECT_EQ(multistack.Size(0), 3);
  EXPECT_EQ(multistack.GetStacksCount(), 1);
  EXPECT_EQ(multistack.GetTotalCapacity(), 5);

  EXPECT_EQ(multistack.pop(0), 3);
  EXPECT_EQ(multistack.pop(0), 2);
  EXPECT_EQ(multistack.pop(0), 1);
}

TEST(EdgeCaseTest, MultistackManyStacks)
{
  TMultistack<int> multistack(5, 25);
  for (size_t i = 0; i < 5; ++i)
  {
    multistack.push(i, static_cast<int>(i * 10 + 1));
    multistack.push(i, static_cast<int>(i * 10 + 2));
  }

  for (size_t i = 0; i < 5; ++i)
  {
    EXPECT_EQ(multistack.Size(i), 2);
    EXPECT_EQ(multistack.pop(i), static_cast<int>(i * 10 + 2));
    EXPECT_EQ(multistack.pop(i), static_cast<int>(i * 10 + 1));
  }
}

TEST(EdgeCaseTest, MultistackMultipleRedistribution)
{
  TMultistack<int> multistack(2, 4);

  for (int i = 0; i < 20; ++i)
  {
    multistack.push(0, i);
    multistack.push(1, i * 2);
  }

  EXPECT_EQ(multistack.Size(0), 20);
  EXPECT_EQ(multistack.Size(1), 20);

  for (int i = 19; i >= 0; --i)
  {
    EXPECT_EQ(multistack.pop(0), i);
    EXPECT_EQ(multistack.pop(1), i * 2);
  }
}

TEST(EdgeCaseTest, MultistackUnevenUsage)
{
  TMultistack<int> multistack(3, 12);

  for (int i = 0; i < 10; ++i)
    multistack.push(0, i);

  multistack.push(1, 100);
  multistack.push(1, 200);

  EXPECT_EQ(multistack.Size(0), 10);
  EXPECT_EQ(multistack.Size(1), 2);
  EXPECT_EQ(multistack.Size(2), 0);

  for (int i = 9; i >= 0; --i)
    EXPECT_EQ(multistack.pop(0), i);

  EXPECT_EQ(multistack.pop(1), 200);
  EXPECT_EQ(multistack.pop(1), 100);
}