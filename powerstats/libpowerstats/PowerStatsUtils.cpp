/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <pixelpowerstats/PowerStatsUtils.h>
namespace android
{
    namespace hardware
    {
        namespace google
        {
            namespace pixel
            {
                namespace powerstats
                {
                    namespace utils
                    {
                        bool extractStat(const char *line, const std::string &prefix, uint64_t &stat)
                        {
                            char const *prefixStart = strstr(line, prefix.c_str());
                            if (prefixStart == nullptr)
                            {
                                // Did not find the given prefix
                                return false;
                            }
                            stat = strtoull(prefixStart + prefix.length(), nullptr, 0);
                            return true;
                        }
                    } // namespace utils
                }     // namespace powerstats
            }         // namespace pixel
        }             // namespace google
    }                 // namespace hardware
} // namespace android