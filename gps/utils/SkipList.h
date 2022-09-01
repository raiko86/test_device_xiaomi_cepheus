/* Copyright (c) 2019 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef LOC_SKIP_LIST_H
#define LOC_SKIP_LIST_H

#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

namespace loc_util {

template <typename T,
          template <typename elem, typename Allocator = std::allocator<elem>>
          class container = list>
class SkipNode {
 public:
  typedef typename container<SkipNode<T, container>>::iterator NodeIterator;

  int mLevel;
  T mData;
  NodeIterator mNextInLevel;

  SkipNode(int level, T& data) : mLevel(level), mData(data) {}
};

template <typename T>
class SkipList {
  using NodeIterator = typename SkipNode<T>::NodeIterator;

 private:
  list<SkipNode<T>> mMainList;
  vector<NodeIterator> mHeadVec;
  vector<NodeIterator> mTailVec;

 public:
  SkipList(int totalLevels);
  void append(T& data, int level);
  void pop(int level);
  void pop();
  T front(int level);
  int size();
  void flush();
  list<pair<T, int>> dump();
  list<pair<T, int>> dump(int level);
};

template <typename T>
SkipList<T>::SkipList(int totalLevels)
    : mHeadVec(totalLevels, mMainList.end()),
      mTailVec(totalLevels, mMainList.end()) {}

template <typename T>
void SkipList<T>::append(T& data, int level) {
  if (level < 0 || level >= mHeadVec.size()) {
    return;
  }

  SkipNode<T> node(level, data);
  node.mNextInLevel = mMainList.end();
  mMainList.push_back(node);
  auto iter = --mMainList.end();
  if (mHeadVec[level] == mMainList.end()) {
    mHeadVec[level] = iter;
  } else {
    (*mTailVec[level]).mNextInLevel = iter;
  }
  mTailVec[level] = iter;
}

template <typename T>
void SkipList<T>::pop(int level) {
  if (mHeadVec[level] == mMainList.end()) {
    return;
  }

  if ((*mHeadVec[level]).mNextInLevel == mMainList.end()) {
    mTailVec[level] = mMainList.end();
  }

  auto tmp_iter = (*mHeadVec[level]).mNextInLevel;
  mMainList.erase(mHeadVec[level]);
  mHeadVec[level] = tmp_iter;
}

template <typename T>
void SkipList<T>::pop() {
  pop(mMainList.front().mLevel);
}

template <typename T>
T SkipList<T>::front(int level) {
  return (*mHeadVec[level]).mData;
}

template <typename T>
int SkipList<T>::size() {
  return mMainList.size();
}

template <typename T>
void SkipList<T>::flush() {
  mMainList.clear();
  for (int i = 0; i < mHeadVec.size(); i++) {
    mHeadVec[i] = mMainList.end();
    mTailVec[i] = mMainList.end();
  }
}

template <typename T>
list<pair<T, int>> SkipList<T>::dump() {
  list<pair<T, int>> li;
  for_each(mMainList.begin(), mMainList.end(), [&](SkipNode<T>& item) {
    li.push_back(make_pair(item.mData, item.mLevel));
  });
  return li;
}

template <typename T>
list<pair<T, int>> SkipList<T>::dump(int level) {
  list<pair<T, int>> li;
  auto head = mHeadVec[level];
  while (head != mMainList.end()) {
    li.push_back(make_pair((*head).mData, (*head).mLevel));
    head = (*head).mNextInLevel;
  }
  return li;
}

}  // namespace loc_util

#endif
