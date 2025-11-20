#pragma once
#include <cstddef>
#include <iostream>

using namespace std;

template <class T>
class TMultistack
{
protected:
  size_t stacksCount;
  size_t totalCapacity;
  T* memory;
  size_t* tops;
  size_t* capacities;

public:
  TMultistack();
  TMultistack(size_t stacksCount_, size_t totalCapacity_);
  TMultistack(const TMultistack& other);
  TMultistack(TMultistack&& other);
  ~TMultistack();

  size_t GetStacksCount() const;
  size_t GetTotalCapacity() const;
  T* GetMemory() const;
  size_t* GetTops() const;
  size_t* GetCapacities() const;

  void SetStacksCount(size_t stacksCount_);
  void SetTotalCapacity(size_t totalCapacity_);
  void SetMemory(T* memory_);
  void SetTops(size_t* tops_);
  void SetCapacities(size_t* capacities_);

  void push(size_t stackIndex, const T& element);
  T pop(size_t stackIndex);
  bool IsEmpty(size_t stackIndex) const;
  bool IsFull(size_t stackIndex) const;
  size_t Size(size_t stackIndex) const;

  void repack();
  bool operator==(const TMultistack<T>& other) const;
  bool operator!=(const TMultistack<T>& other) const;

  class TIterator
  {
  protected:
    TMultistack<T>& p;
    size_t stack;
    size_t cur;
    size_t prev;
  public:
    TIterator(TMultistack<T>& multistack, size_t stackIndex, size_t start, size_t cnt);
    T& operator*();
    TIterator& operator++();
    TIterator operator++(int);

    bool operator==(const TIterator& other) const;
    bool operator!=(const TIterator& other) const;
  };

  TIterator begin(size_t stackIndex);
  TIterator end(size_t stackIndex);

  template <class I>
  friend istream& operator>>(istream& is, TMultistack<I>& multistack);

  template <class O>
  friend ostream& operator<<(ostream& os, const TMultistack<O>& multistack);
};

template <class T>
TMultistack<T>::TMultistack() : stacksCount(0), totalCapacity(0), memory(nullptr), tops(nullptr), capacities(nullptr) {}

template <class T>
TMultistack<T>::TMultistack(size_t stacksCount_, size_t totalCapacity_) : stacksCount(stacksCount_), totalCapacity(totalCapacity_)
{
  memory = new T[totalCapacity];
  tops = new size_t[stacksCount];
  capacities = new size_t[stacksCount];

  size_t baseCapacity = totalCapacity / stacksCount;
  for (size_t i = 0; i < stacksCount; ++i)
  {
    tops[i] = 0;
    capacities[i] = baseCapacity;
  }
  capacities[stacksCount - 1] = totalCapacity - baseCapacity * (stacksCount - 1);
}

template <class T>
TMultistack<T>::TMultistack(const TMultistack& other) : stacksCount(other.stacksCount), totalCapacity(other.totalCapacity)
{
  memory = new T[totalCapacity];
  tops = new size_t[stacksCount];
  capacities = new size_t[stacksCount];

  for (size_t i = 0; i < totalCapacity; ++i)
    memory[i] = other.memory[i];

  for (size_t i = 0; i < stacksCount; ++i)
  {
    tops[i] = other.tops[i];
    capacities[i] = other.capacities[i];
  }
}

template <class T>
TMultistack<T>::TMultistack(TMultistack&& other) : stacksCount(other.stacksCount), totalCapacity(other.totalCapacity),
                                                          memory(other.memory), tops(other.tops), capacities(other.capacities)
{
  other.memory = nullptr;
  other.tops = nullptr;
  other.capacities = nullptr;
  other.stacksCount = 0;
  other.totalCapacity = 0;
}

template <class T>
TMultistack<T>::~TMultistack()
{
  delete[] memory;
  delete[] tops;
  delete[] capacities;
}

template <class T>
size_t TMultistack<T>::GetStacksCount() const
{
  return stacksCount;
}

template <class T>
size_t TMultistack<T>::GetTotalCapacity() const
{
  return totalCapacity;
}

template <class T>
T* TMultistack<T>::GetMemory() const
{
  return memory;
}

template <class T>
size_t* TMultistack<T>::GetTops() const
{
  return tops;
}

template <class T>
size_t* TMultistack<T>::GetCapacities() const
{
  return capacities;
}

template <class T>
void TMultistack<T>::SetStacksCount(size_t stacksCount_)
{
  size_t* newTops = new size_t[stacksCount_];
  size_t* newCaps = new size_t[stacksCount_];

  for (size_t i = 0; i < min(stacksCount, stacksCount_); ++i)
  {
    newTops[i] = tops[i];
    newCaps[i] = capacities[i];
  }

  for (size_t i = stacksCount; i < stacksCount_; ++i)
  {
    newTops[i] = 0;
    newCaps[i] = 0;
  }

  delete[] tops;
  delete[] capacities;
  tops = newTops;
  capacities = newCaps;
  stacksCount = stacksCount_;
}

template <class T>
void TMultistack<T>::SetTotalCapacity(size_t totalCapacity_)
{
  T* newMem = new T[totalCapacity_];
  for (size_t i = 0; i < min(totalCapacity, totalCapacity_); ++i)
    newMem[i] = memory[i];

  delete[] memory;
  memory = newMem;
  totalCapacity = totalCapacity_;
}

template <class T>
void TMultistack<T>::SetMemory(T* memory_)
{
  delete[] memory;
  memory = memory_;
}

template <class T>
void TMultistack<T>::SetTops(size_t* tops_)
{
  delete[] tops;
  tops = tops_;
}

template <class T>
void TMultistack<T>::SetCapacities(size_t* capacities_)
{
  delete[] capacities;
  capacities = capacities_;
}

template <class T>
void TMultistack<T>::push(size_t stackIndex, const T& element)
{
  if (stackIndex >= stacksCount)
    throw("Invalid stack index");

  if (IsFull(stackIndex))
    repack();

  size_t start = 0;
  for (size_t i = 0; i < stackIndex; ++i)
    start += capacities[i];

  memory[start + tops[stackIndex]] = element;
  tops[stackIndex]++;
}

template <class T>
T TMultistack<T>::pop(size_t stackIndex)
{
  if (stackIndex >= stacksCount)
    throw("Invalid stack index");
  if (IsEmpty(stackIndex))
    throw("Empty stack");

  size_t start = 0;
  for (size_t i = 0; i < stackIndex; ++i)
    start += capacities[i];

  tops[stackIndex]--;
  return memory[start + tops[stackIndex]];
}

template <class T>
inline bool TMultistack<T>::IsEmpty(size_t stackIndex) const
{
  if (stackIndex >= stacksCount)
    throw("Invalid stack index");
  return tops[stackIndex] == 0;
}

template <class T>
inline bool TMultistack<T>::IsFull(size_t stackIndex) const
{
  if (stackIndex >= stacksCount)
    throw("Invalid stack index");
  return tops[stackIndex] >= capacities[stackIndex];
}

template <class T>
size_t TMultistack<T>::Size(size_t stackIndex) const
{
  return tops[stackIndex];
}

template <class T>
void TMultistack<T>::repack()
{
  size_t newTotalCapacity = totalCapacity * 2;
  T* newMemory = new T[newTotalCapacity];
  size_t newBaseCapacity = newTotalCapacity / stacksCount;

  size_t currentPos = 0;
  for (size_t i = 0; i < stacksCount; ++i)
  {
    size_t newCap = (i == stacksCount - 1) ? (newTotalCapacity - currentPos) : newBaseCapacity;
    for (size_t j = 0; j < tops[i]; ++j)
      newMemory[currentPos + j] = memory[capacities[i] * i + j];

    capacities[i] = newCap;
    currentPos += newCap;
  }

  delete[] memory;
  memory = newMemory;
  totalCapacity = newTotalCapacity;
}

template <class T>
bool TMultistack<T>::operator==(const TMultistack<T>& other) const
{
  if (stacksCount != other.stacksCount)
    return false;
  
  for (size_t i = 0; i < stacksCount; ++i)
  {
    if (tops[i] != other.tops[i] || capacities[i] != other.capacities[i])
      return false;
  }
  
  for (size_t i = 0; i < stacksCount; ++i)
  {
    size_t start = 0;
    size_t otherStart = 0;
    for (size_t j = 0; j < i; ++j)
    {
      start += capacities[j];
      otherStart += other.capacities[j];
    }

    for (size_t j = 0; j < tops[i]; ++j)
    {
      if (memory[start + j] != other.memory[otherStart + j])
        return false;
    }
  }
  return true;
}

template <class T>
bool TMultistack<T>::operator!=(const TMultistack<T>& other) const
{
  return !(*this == other);
}


template <class T>
TMultistack<T>::TIterator::TIterator(TMultistack<T>& multistack, size_t stackIndex, size_t start, size_t cnt)
        : p(multistack), stack(stackIndex), cur(start), prev(cnt) {}

template <class T>
T& TMultistack<T>::TIterator::operator*()
{
  size_t start = 0;
  for (size_t i = 0; i < stack; ++i)
    start += p.capacities[i];
  return p.memory[start + cur];
}

template <class T>
typename TMultistack<T>::TIterator& TMultistack<T>::TIterator::operator++()
{
  cur++;
  prev++;
  return *this;
}

template <class T>
typename TMultistack<T>::TIterator TMultistack<T>::TIterator::operator++(int)
{
  TIterator temp = *this;
  ++(*this);
  return temp;
}

template <class T>
bool TMultistack<T>::TIterator::operator==(const TIterator& other) const
{
  return &p == &other.p && stack == other.stack && prev == other.prev;
}

template <class T>
bool TMultistack<T>::TIterator::operator!=(const TIterator& other) const
{
  return !(*this == other);
}

template <class T>
typename TMultistack<T>::TIterator TMultistack<T>::begin(size_t stackIndex)
{
  return TIterator(*this, stackIndex, 0, 0);
}

template <class T>
typename TMultistack<T>::TIterator TMultistack<T>::end(size_t stackIndex)
{
  return TIterator(*this, stackIndex, tops[stackIndex], tops[stackIndex]);
}

template <class I>
istream& operator>>(istream& is, TMultistack<I>& multistack)
{
  size_t stacksCount, totalCapacity;
  is >> stacksCount >> totalCapacity;

  multistack.SetStacksCount(stacksCount);
  multistack.SetTotalCapacity(totalCapacity);

  for (size_t i = 0; i < stacksCount; ++i)
  {
    size_t stackSize;
    is >> stackSize;
    for (size_t j = 0; j < stackSize; ++j)
    {
      I element;
      is >> element;
      multistack.push(i, element);
    }
  }
  return is;
}

template <class O>
ostream& operator<<(ostream& os, const TMultistack<O>& multistack)
{
  for (size_t i = 0; i < multistack.stacksCount; ++i)
  {
    os << "[";
    size_t start = 0;
    for (size_t j = 0; j < i; ++j)
      start += multistack.capacities[j];

    for (size_t j = 0; j < multistack.tops[i]; ++j)
    {
      os << multistack.memory[start + j];
      if (j < multistack.tops[i] - 1)
        os << ", ";
    }
    os << "]\n";
  }
  return os;
}