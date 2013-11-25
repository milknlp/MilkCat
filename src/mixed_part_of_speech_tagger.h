//
// mixed_part_of_speech_tagger.h --- Created at 2013-11-23
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

#ifndef MIXED_PART_OF_SPEECH_TAGGER_H
#define MIXED_PART_OF_SPEECH_TAGGER_H

#include "part_of_speech_tagger.h"

class CRFPartOfSpeechTagger;
class HMMPartOfSpeechTagger;
class PartOfSpeechTagInstance;
class TermInstance;

// Mixed HMM and CRF part-of-speech tagger
class MixedPartOfSpeechTagger: public PartOfSpeechTagger {
 public:
  ~MixedPartOfSpeechTagger();

  static MixedPartOfSpeechTagger *Create(
      const char *hmm_pos_model_path,
      const char *unigram_index_path,
      const char *default_tag_path,
      const char *crf_pos_model_path);

  void Tag(PartOfSpeechTagInstance *part_of_speech_tag_instance, TermInstance *term_instance);

 private:
  CRFPartOfSpeechTagger *crf_tagger_;
  HMMPartOfSpeechTagger *hmm_tagger_;
  PartOfSpeechTagInstance *one_instance_;

  MixedPartOfSpeechTagger();
};


#endif