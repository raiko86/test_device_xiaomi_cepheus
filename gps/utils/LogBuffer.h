/* Copyright (c) 2019 The Linux Foundation. All rights reserved.
 *
 * SPDX-License-Identifier: Linux-OpenIB
 *
 */

#ifndef LOG_BUFFER_H
#define LOG_BUFFER_H

#include <loc_cfg.h>
#include <loc_pla.h>
#include <signal.h>
#include <time.h>

#include <fstream>
#include <functional>
#include <mutex>
#include <ostream>
#include <sstream>
#include <string>
#include <thread>

#include "SkipList.h"
#include "log_util.h"

// default error level time depth threshold,
#define TIME_DEPTH_THRESHOLD_MINIMAL_IN_SEC 60
// default maximum log buffer size
#define MAXIMUM_NUM_IN_LIST 50
// file path of dumped log buffer
#define LOG_BUFFER_FILE_PATH "/data/vendor/location/"

namespace loc_util {

class ConfigsInLevel {
 public:
  uint32_t mTimeDepthThres;
  uint32_t mMaxNumThres;
  int mCurrentSize;

  ConfigsInLevel(uint32_t time, int num, int size)
      : mTimeDepthThres(time), mMaxNumThres(num), mCurrentSize(size) {}
};

class LogBuffer {
 private:
  static LogBuffer* mInstance;
  static struct sigaction mOriSigAction[NSIG];
  static struct sigaction mNewSigAction;
  static mutex sLock;

  SkipList<pair<uint64_t, string>> mLogList;
  vector<ConfigsInLevel> mConfigVec;
  mutex mLock;

  const vector<string> mLevelMap{"E", "W", "I", "D", "V"};

 public:
  static LogBuffer* getInstance();
  void append(string& data, int level, uint64_t timestamp);
  void dump(std::function<void(stringstream&)> log, int level = -1);
  void dumpToAdbLogcat();
  void dumpToLogFile(string filePath);
  void flush();

 private:
  LogBuffer();
  void registerSignalHandler();
  static void signalHandler(const int code, siginfo_t* const si,
                            void* const sc);
};

}  // namespace loc_util

#endif
