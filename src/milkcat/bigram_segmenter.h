//
// hmm_segment_and_pos_tagger.h --- Created at 2013-08-14
// bigram_segmenter.h --- Created at 2013-10-21
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

#ifndef BIGRAM_SEGMENTER_H
#define BIGRAM_SEGMENTER_H

#include <stdint.h>
#include <unordered_set>
#include "milkcat_config.h"
#include "static_hashtable.h"
#include "darts.h"
#include "segmenter.h"
#include "static_array.h"

class TrieTree;
class TokenInstance;
class TermInstance;

class BigramSegmenter: public Segmenter {
 public:

  // A node in decode graph
  struct Node;

  // A Bucket contains several node
  class Bucket;

  // A pool to alloc and release nodes 
  class NodePool;

  BigramSegmenter(const TrieTree *index,
                  const TrieTree *user_index,
                  const StaticArray<float> *unigram_cost,
                  const StaticArray<float> *user_unigram_cost,
                  const StaticHashTable<int64_t, float> *bigram_cost);

  ~BigramSegmenter();

  // Segment a token instance into term instance
  void Segment(TermInstance *term_instance, TokenInstance *token_instance) override;

  // Get the recent segmentation cost
  double RecentSegCost() { return cost_; }

  // Get term-id by string. Return term-id if term exists, or a negative value 
  // if the term neither in system dictionary nor in user dictionry. 
  int GetTermId(const char *term_str);

  // Add a disabled term-id to segmenter. When the term-id is disabled, just like 
  // it doesn't exists in the dictionary for segmenter.
  void AddDisabledTermId(int term_id) {
    use_disabled_term_ids_ = true;
    disabled_term_ids_.insert(term_id);
  }

  // Clear all disabled term-ids in this segmenter
  void ClearAllDisabledTermIds() {
    disabled_term_ids_.clear();
    use_disabled_term_ids_ = false;
  }

 private:
  // Number of Node in each buckets_
  static const int kNBest = 3;

  // Buckets contain nodes for viterbi decoding
  Bucket *buckets_[kTokenMax + 1];

  // NodePool instance to alloc and release node
  NodePool *node_pool_;

  // Costs for unigram and bigram.
  const StaticArray<float> *unigram_cost_;
  const StaticArray<float> *user_cost_;
  const StaticHashTable<int64_t, float> *bigram_cost_;


  // Index for words in dictionary
  const TrieTree *index_;
  const TrieTree *user_index_;
  bool has_user_index_;

  // Stores the final cost of recent segmentation
  double cost_;

  // The disabled term-ids variables
  bool use_disabled_term_ids_;
  std::unordered_set<int> disabled_term_ids_;

  // Add an arc to the decode graph with the weight and term_id
  void AddArcToDecodeGraph(int from_position, int from_index, int to_position, double weight, int term_id);

  double CalculateBigramCost(int left_id, int right_id, double left_cost, double right_cost);
  int GetTermIdAndUnigramCost(const char *token_str, bool &system_flag, bool &user_flag, 
                              size_t &system_node, size_t &user_node, double &right_cost);
};

#endif