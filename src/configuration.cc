//
// configuration.cc --- Created at 2013-11-08
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

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configuration.h"
#include "utils.h"

Configuration::Configuration() {}

Configuration *Configuration::LoadFromPath(const char *path) {
  char line[2048], 
       key[2048],
       value[2048],
       *p, 
       error_messge[1024];
  int line_number = 0;
  FILE *fp = fopen(path, "r");
  Configuration *self = new Configuration();

  if (fp == NULL) {
    sprintf(error_messge, "unable to open configuration file %s", path);
    set_error_message(error_messge);
    fclose(fp);
    delete self;
    return NULL;
  }

  while (NULL != fgets(line, 2048, fp)) {
    line_number++;
    trim(line);
    if (line[0] == '#') continue;

    p = line;
    while (*p != '=' && *p != '\0') p++;
    if (*p == '\0') {
      sprintf(error_messge, "missing '=' in %s line %d.\n", path, line_number);
      set_error_message(error_messge);
      fclose(fp);
      delete self;
      return NULL;
    }

    strlcpy(key, line, p - line + 1);
    strlcpy(value, p + 1, 1024);
    trim(key);
    trim(value);

    self->data_[key] = value;
  }
  
  fclose(fp);
  return self;
}

bool Configuration::SaveToPath(const char *path) {
  FILE *fp = fopen(path, "w");
  if (fp == NULL) return false;

  for (std::map<std::string, std::string>::iterator it = data_.begin(); it != data_.end(); ++it) {
    fprintf(fp, "%s = %s\n", it->first.c_str(), it->second.c_str());
  }

  fclose(fp);
  return true;
}

int Configuration::GetInteger(const char *key) const {
  return atoi(GetString(key));
}

bool Configuration::HasKey(const char *key) const {
  return data_.find(key) != data_.end();
}

const char *Configuration::GetString(const char *key) const {
  std::map<std::string, std::string>::const_iterator it = data_.find(std::string(key));
  if (it != data_.end()) {
    return it->second.c_str();
  } else {
    return "";
  }
}

void Configuration::SetInteger(const char *key, int value) {
  char buf[1024];
  sprintf(buf, "%d", value);
  data_[key] = buf;
}

void Configuration::SetString(const char *key, const char * value) {
  data_[key] = value;
}