PREFIX ?= /usr/local
MODEL_DIR ?= $(PREFIX)/share/milkcat
LIBRARY_DIR ?= $(PREFIX)/lib
BINARY_DIR ?= $(PREFIX)/bin
INCLUDE_DIR ?= $(PREFIX)/include
PYTHON ?= python
V ?= 0

PYTHONPATH := tools/gyp/pylib/
export PYTHONPATH

all: milkcat

gyp_build: milkcat.gyp
	if [ ! -d build ];then mkdir build; fi
	tools/gyp/gyp --depth=. milkcat.gyp -f make --generator-output=build -D model_path=$(MODEL_DIR)/
	make -C build

milkcat: gyp_build

clean:
	rm -rf build

install:
	if [ ! -d $(MODEL_DIR) ];then mkdir -p $(MODEL_DIR); fi
	cp data/* $(MODEL_DIR)
	cp build/out/Default/milkcat $(BINARY_DIR)
	cp build/out/Default/neko $(BINARY_DIR)
	cp build/out/Default/mk_model $(BINARY_DIR)
	cp build/out/Default/libmilkcat.* $(LIBRARY_DIR)
	cp src/milkcat/milkcat.h $(INCLUDE_DIR)

uninstall:
	rm $(MODEL_DIR)/*
	rm $(BINARY_DIR)/milkcat
	rm $(BINARY_DIR)/mk_model
	rm $(BINARY_DIR)/neko
	rm $(LIBRARY_DIR)/libmilkcat.*


CPPLINT_EXCLUDE ?=
CPPLINT_EXCLUDE += src/milkcat/token_lex.cc
CPPLINT_EXCLUDE += src/milkcat/token_lex.h
CPPLINT_EXCLUDE += src/milkcat/darts.h
CPPLINT_EXCLUDE += src/neko/utf8.h

ALL_CPP_FILES = $(wildcard src/*.cc \
                           src/*.h \
                           src/milkcat/*.cc \
                           src/milkcat/*.h \
                           src/utils/*.cc \
                           src/utils/*.h \
                           src/neko/*.cc \
                           src/neko/*.h)
CPPLINT_FILES = $(filter-out $(CPPLINT_EXCLUDE), $(ALL_CPP_FILES))

lint:
	$(PYTHON) tools/cpplint.py $(CPPLINT_FILES)
