/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef ANDROID_LIBPERFMGR_FILENODE_H_
#define ANDROID_LIBPERFMGR_FILENODE_H_
#include <android-base/unique_fd.h>
#include <cstddef>
#include <string>
#include <vector>
#include "perfmgr/Node.h"
namespace android
{
  namespace perfmgr
  {
    // FileNode represents file
    class FileNode : public Node
    {
    public:
      FileNode(std::string name, std::string node_path,
               std::vector<RequestGroup> req_sorted, std::size_t default_val_index,
               bool reset_on_init, bool hold_fd = false);
      std::chrono::milliseconds Update(bool log_error) override;
      bool GetHoldFd() const;
      void DumpToFd(int fd) const override;
    private:
      FileNode(const Node &other) = delete;
      FileNode &operator=(Node const &) = delete;
      const bool hold_fd_;
      const std::chrono::milliseconds warn_timeout_;
      android::base::unique_fd fd_;
    };
  } // namespace perfmgr
} // namespace android
#endif // ANDROID_LIBPERFMGR_FILENODE_H_
