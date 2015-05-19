#pragma once
/*
 * Copyright (c) 2012 Aldebaran Robotics. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the COPYING file.
 */


#ifndef _SRC_PATH_CONF_HPP_
#define _SRC_PATH_CONF_HPP_

#include <set>
#include <string>

namespace qi {
  namespace path {
    namespace detail {

      /**
       * Recursively parse <sdk/share/qi/path.conf> files
       * Generated by qibuild, this makes it possible to find
       * files from the depencies build dir or sources
       */
      std::set<std::string> parseQiPathConf(const std::string &pathConf);

    } // detail
  } // path
} // qi

#endif  // _SRC_PATH_CONF_HPP_