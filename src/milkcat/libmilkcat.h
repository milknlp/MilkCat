//
// model_factory.h --- Created at 2014-02-03
// libmilkcat.h --- Created at 2014-02-06
//
// The MIT License (MIT)
//
// Copyright 2014 ling0322 <ling032x@gmail.com>
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

#ifndef LIBMILKCAT_H
#define LIBMILKCAT_H

#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include "utils/utils.h"
#include "utils/status.h"
#include "utils/readable_file.h"
#include "hmm_model.h"
#include "crf_model.h"
#include "trie_tree.h"
#include "static_array.h"
#include "static_hashtable.h"
#include "configuration.h"
#include "milkcat.h"
#include "segmenter.h"
#include "part_of_speech_tagger.h"
#include "tokenizer.h"
#include "milkcat_config.h"
#include "term_instance.h"
#include "part_of_speech_tag_instance.h"

// Model filenames
constexpr const char *UNIGRAM_INDEX = "unigram.idx";
constexpr const char *UNIGRAM_DATA = "unigram.bin";
constexpr const char *BIGRAM_DATA = "bigram.bin";
constexpr const char *HMM_PART_OF_SPEECH_MODEL = "ctb_pos.hmm";
constexpr const char *CRF_PART_OF_SPEECH_MODEL = "ctb_pos.crf";
constexpr const char *CRF_SEGMENTER_MODEL = "ctb_seg.crf";
constexpr const char *DEFAULT_TAG = "default_tag.cfg";
constexpr const char *OOV_PROPERTY = "oov_property.idx";

// Tokenizer ids for TokenizerFactory
const int kDefaultTokenizer = 0;

// Segmenter ids for SegmenterFactory
const int kBigramSegmenter = 0;
const int kCrfSegmenter = 1;
const int kMixedSegmenter = 2;

// Part-of-speech ids for PartOfSpeechTaggerFactory
const int kCrfPartOfSpeechTagger = 0;
const int kHmmPartOfSpeechTagger = 1;
const int kMixedPartOfSpeechTagger = 2;

class Cursor;
class ModelFactory;

struct milkcat_model_t {
  ModelFactory *model_factory;
};

struct milkcat_t {
  milkcat_model_t *model;

  Segmenter *segmenter;
  PartOfSpeechTagger *part_of_speech_tagger;
  
  std::vector<Cursor *> cursor_pool;
};


// A factory class that can obtain any model data class needed by MilkCat
// in singleton mode. All the getXX fucnctions are thread safe
class ModelFactory {
 public:
  ModelFactory(const char *model_dir_path);
  ~ModelFactory();

  // Get the index for word which were used in unigram cost, bigram cost
  // hmm pos model and oov property
  const TrieTree *Index(Status &status);

  void SetUserDictionary(const char *path) { user_dictionary_path_ = path; }

  // Load and set the user dictionary data specified by path
  void LoadUserDictionary(Status &status);
  bool HasUserDictionary() const { return user_dictionary_path_.size() != 0; }

  const TrieTree *UserIndex(Status &status);
  const StaticArray<float> *UserCost(Status &status);

  const StaticArray<float> *UnigramCost(Status &status);
  const StaticHashTable<int64_t, float> *BigramCost(Status &status);

  // Get the CRF word segmenter model
  const CRFModel *CRFSegModel(Status &status);

  // Get the CRF word part-of-speech model
  const CRFModel *CRFPosModel(Status &status);

  // Get the HMM word part-of-speech model
  const HMMModel *HMMPosModel(Status &status);

  // Get the character's property in out-of-vocabulary word recognition
  const TrieTree *OOVProperty(Status &status);

  const Configuration *DefaultTag(Status &status);

 private:
  std::string model_dir_path_;
  std::string user_dictionary_path_;
  pthread_mutex_t mutex;

  const TrieTree *unigram_index_;
  const TrieTree *user_index_;
  const StaticArray<float> *unigram_cost_;
  const StaticArray<float> *user_cost_;
  const StaticHashTable<int64_t, float> *bigram_cost_;
  const CRFModel *seg_model_;
  const CRFModel *crf_pos_model_;
  const HMMModel *hmm_pos_model_;
  const TrieTree *oov_property_;
  const Configuration *default_tag_;
};

// A factory function to create tokenizers
Tokenization *TokenizerFactory(int tokenizer_id);

// A factory function to create segmenters. On success, return the instance of 
// Segmenter, on failed, set status != Status::OK()
Segmenter *SegmenterFactory(ModelFactory *factory, int segmenter_id, Status &status);

// A factory function to create part-of-speech taggers. On success, return the 
// instance of part-of-speech tagger, on failed, set status != Status::OK()
PartOfSpeechTagger *PartOfSpeechTaggerFactory(ModelFactory *factory,
                                              int part_of_speech_tagger_id, 
                                              Status &status);


// Cursor class save the internal state of the analyzing result, such as
// the current word and current sentence. 
class Cursor {
 public:
  Cursor(milkcat_t *analyzer);
  ~Cursor();

  // Start to scan a text and use this->analyzer_ to analyze it
  // the result saved in its current state, use MoveToNext() to
  // iterate.
  void Scan(const char *text);

  // Move the cursor to next position, if end of text is reached
  // set end() to true
  void MoveToNext();

  // These function return the data of current position
  const char *word() const { return term_instance_->term_text_at(current_position_); }
  const char *part_of_speech_tag() const {
    if (analyzer_->part_of_speech_tagger != NULL)
      return part_of_speech_tag_instance_->part_of_speech_tag_at(current_position_);
    else
      return "NONE";
  }
  const int word_type() const { return term_instance_->term_type_at(current_position_); }

  // If reaches the end of text
  bool end() const { return end_; }

  milkcat_t *analyzer() const { return analyzer_; }
  
 private:
  milkcat_t *analyzer_;

  Tokenization *tokenizer_;
  TokenInstance *token_instance_;
  TermInstance *term_instance_;
  PartOfSpeechTagInstance *part_of_speech_tag_instance_;

  int sentence_length_;
  int current_position_;
  bool end_;
};

#endif