{
  'target_defaults': {
    'cflags': [
      '-g',
      '-fPIC',
      '-O2',
    ],
    'cflags_cc': [
      '-std=c++11',
      '-fno-rtti',
      '-fno-exceptions',
    ],
    'xcode_settings': {
        'OTHER_CFLAGS': [
        '-g',
        '-fPIC',
        '-O2',
        '-std=c++11',
        '-fno-rtti',
      ],
    },
    'include_dirs': [
      'src',
    ],
  },
  'targets': [
    {
      'target_name': 'libmilkcat',
      'type': 'static_library',
      'defines': [
        'MODEL_PATH="<(model_path)"',
      ],
      'sources': [
        'src/milkcat/milkcat_config.h',
        'src/milkcat/crf_segmenter.cc',
        'src/milkcat/crf_segmenter.h',
        'src/milkcat/instance_data.h',
        'src/milkcat/instance_data.cc',
        'src/milkcat/out_of_vocabulary_word_recognition.cc',
        'src/milkcat/out_of_vocabulary_word_recognition.h',
        'src/milkcat/term_instance.h',
        'src/milkcat/term_instance.cc',
        'src/milkcat/part_of_speech_tag_instance.h',
        'src/milkcat/part_of_speech_tag_instance.cc',
        'src/milkcat/token_instance.h',
        'src/milkcat/token_instance.cc',
        'src/milkcat/token_lex.cc',
        'src/milkcat/token_lex.h',
        'src/milkcat/tokenizer.cc',
        'src/milkcat/tokenizer.h',
        'src/milkcat/libmilkcat.h',
        'src/milkcat/libmilkcat.cc',
        'src/milkcat/darts.h',
        'src/milkcat/milkcat.h',
        'src/milkcat/bigram_segmenter.h',
        'src/milkcat/bigram_segmenter.cc',
        'src/milkcat/feature_extractor.h',
        'src/milkcat/crf_part_of_speech_tagger.h',
        'src/milkcat/crf_part_of_speech_tagger.cc',
        'src/milkcat/crf_model.h',
        'src/milkcat/crf_model.cc',
        'src/milkcat/crf_tagger.h',
        'src/milkcat/crf_tagger.cc',
        'src/milkcat/configuration.h',
        'src/milkcat/configuration.cc',
        'src/milkcat/hmm_part_of_speech_tagger.h',
        'src/milkcat/hmm_part_of_speech_tagger.cc',
        'src/milkcat/mixed_part_of_speech_tagger.h',
        'src/milkcat/mixed_part_of_speech_tagger.cc',
        'src/milkcat/static_hash_table.h',
        'src/milkcat/segmenter.h',
        'src/milkcat/part_of_speech_tagger.h',
        'src/milkcat/mixed_segmenter.h',
        'src/milkcat/mixed_segmenter.cc',
        'src/milkcat/hmm_model.cc',
        'src/milkcat/hmm_model.h',
        'src/milkcat/trie_tree.h',
        'src/milkcat/trie_tree.cc',
        'src/milkcat/static_array.h',

        'src/utils/status.h',
        'src/utils/utils.cc',
        'src/utils/utils.h',
        'src/utils/strlcpy.cc',
        'src/utils/readable_file.cc',
        'src/utils/readable_file.h',
        'src/utils/writable_file.cc',
        'src/utils/writable_file.h',

        'src/neko/crf_vocab.h',      
        'src/neko/crf_vocab.cc',
        'src/neko/candidate.cc',
        'src/neko/candidate.h',
        'src/neko/maxent_classifier.cc',
        'src/neko/maxent_classifier.h',
        'src/neko/mutual_information.cc',
        'src/neko/mutual_information.h',
        'src/neko/final_rank.cc',
        'src/neko/final_rank.h',
        'src/neko/bigram_anal.cc',
        'src/neko/bigram_anal.h',
        'src/neko/utf8.h',
      ],
    },
    {
      'target_name': 'milkcat',
      'type': 'executable',
      'dependencies': [
        'libmilkcat',
      ],
      'xcode_settings': {
          'OTHER_CFLAGS!': [
          '-std=c++11',
          '-fno-rtti',
        ],
      },
      'sources': [
        'src/main.c', 
      ],
    },
    {
      'target_name': 'mk_model',
      'type': 'executable',
      'dependencies': [
        'libmilkcat',
      ],
      'sources': [
        'src/mk_model.cc', 
      ],
    },
    {
      'target_name': 'neko',
      'type': 'executable',
      'defines': [
        'MODEL_PATH="<(model_path)"',
      ],
      'dependencies': [
        'libmilkcat',
      ],
      'sources': [
        'src/neko_main.cc', 
      ],
    },
  ],
}