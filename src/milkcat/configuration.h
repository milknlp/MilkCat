//
// configuration.h --- Created at 2013-11-08
//
// The MIT License (MIT)
//
// Copyright (c) 2013 ling0322 <ling032x@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <map>
#include "utils/utils.h"

class Configuration {
 public:
  Configuration();

  // Load the configuration file of path
  static Configuration *New(const char *path, Status *status);

  // Get integer value by key, default is 0
  int GetInteger(const char *key) const;

  // Get string value by key, default is ""
  const char *GetString(const char *key) const;

  // Check if the key exists in configuration file
  bool HasKey(const char *key) const;

  // Set the integer value of key
  void SetInteger(const char *key, int value);

  // Set the string value of key
  void SetString(const char *key, const char * value);

  // Save the configuration into file
  bool SaveToPath(const char *path);

 private:
  std::map<std::string, std::string> data_;
};

#endif