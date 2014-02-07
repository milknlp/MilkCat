//
// crf_segmenter.h --- Created at 2013-08-17
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

#ifndef CRF_SEGMENTER_H
#define CRF_SEGMENTER_H

#include "utils/utils.h"
#include "crf_tagger.h"
#include "token_instance.h"
#include "segmenter.h"

class SegmentFeatureExtractor;
class TermInstance;

class CRFSegmenter: public Segmenter {
 public:
  static CRFSegmenter *New(const CRFModel *model, Status &status);
  ~CRFSegmenter();

  // Segment a range [begin, end) of token 
  void SegmentRange(TermInstance *term_instance, TokenInstance *token_instance, int begin, int end);

  void Segment(TermInstance *term_instance, TokenInstance *token_instance) {
  	SegmentRange(term_instance, token_instance, 0, token_instance->size());
  }
 
 private:
  CRFTagger *crf_tagger_;

  SegmentFeatureExtractor *feature_extractor_;

  int S, B, B1, B2, M, E;

  CRFSegmenter();

  DISALLOW_COPY_AND_ASSIGN(CRFSegmenter);
};

#endif