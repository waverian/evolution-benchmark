# Copyright (c) 2022 waverian. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
# LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

CC = cc
$(if $(shell which $(CC)),,$(error "No $(CC) in PATH"))

POSSIBLE_VALUES := ON OFF
# Check EB_USE_CPUID if defined and has a wrong value
ifeq ($(filter $(EB_USE_CPUID),POSSIBLE_VALUES),)
    ifneq ($(origin EB_USE_CPUID), undefined)
        $(error EB_USE_CPUID has a wrong value: $(EB_USE_CPUID))
    endif
endif

# Check EB_USE_POSIX_THREADS if defined and has a wrong value
ifeq ($(filter $(EB_USE_THREADS),POSSIBLE_VALUES),)
    ifneq ($(origin EB_USE_THREADS), undefined)
        $(error EB_USE_THREADS has a wrong value: $(EB_USE_THREADS))
    endif
endif

# Check EB_USE_UNISTD if defined and has a wrong value
ifeq ($(filter $(EB_USE_UNISTD),POSSIBLE_VALUES),)
    ifneq ($(origin EB_USE_UNISTD), undefined)
        $(error EB_USE_UNISTD has a wrong value: $(EB_USE_UNISTD))
    endif
endif


##### DETECT COMPILER COMPATIBILITIES: #####
EB_FLAGS=

ifndef EB_USE_CPUID
$(info Checking CPUID...)
ifeq ($(shell $(CC) compiler_probes/posix_cpuid.c -o /dev/null 2> /dev/null && echo 1), 1)
	EB_USE_CPUID=ON
	EB_FLAGS+=-DEB_USE_POSIX_CPUID
else
	EB_USE_CPUID=OFF
endif
endif

ifndef EB_USE_THREADS
$(info Checking PThreads...)
ifeq ($(shell $(CC) -lpthread compiler_probes/posix_threads.c -o /dev/null 2> /dev/null && echo 1), 1)
	EB_USE_THREADS=ON
	EB_FLAGS+=-DEB_USE_POSIX_THREADS
else
	EB_USE_THREADS=OFF
endif
endif

ifndef EB_USE_UNISTD
$(info Checking unistd...)
ifeq ($(shell $(CC) compiler_probes/unistd.c -o /dev/null 2> /dev/null && echo 1), 1)
	EB_USE_UNISTD=ON
	EB_FLAGS+=-DEB_USE_UNISTD
else
	EB_USE_UNISTD=OFF
endif
endif

BUILD_DIR?=build_make/

EB_VERSION=$(shell cat VERSION || echo "UNKNOWN")
EB_COMPILER=$(shell ${CC} --version | head -n1)

override CFLAGS += -DEB_VERSION="\"${EB_VERSION}\""\
                   -DEB_COMPILER="\"${EB_COMPILER}\""\
                   ${EB_FLAGS}


BENCHMARK_CORE_SRC=lib/src/core/core.c

BENCHMARK_CORE_OPT_OBJ=$(addprefix ${BUILD_DIR}, $(BENCHMARK_CORE_SRC:.c=.opt.o))
BENCHMARK_CORE_NONOPT_OBJ=$(addprefix ${BUILD_DIR}, $(BENCHMARK_CORE_SRC:.c=.nonopt.o))

BENCHMARK_CORE_OPT_LIB=${BUILD_DIR}core.opt.a
BENCHMARK_CORE_NOOPT_LIB=${BUILD_DIR}core.nonopt.a

BENCHMARK_CORE_DEPS=-lm

BENCHMARK_SRC=lib/src/platform.c\
              lib/src/thread.c\
              lib/src/constants.c\
              lib/src/printers/html.c\
              lib/src/printers/text.c\
              lib/src/evolution_benchmark.c

ifeq ($(EB_USE_THREADS), ON)
	BENCHMARK_DEPS=$(BENCHMARK_CORE_DEPS) -lpthread
else
	BENCHMARK_DEPS=$(BENCHMARK_CORE_DEPS)
endif

BENCHMARK_OBJ=$(addprefix ${BUILD_DIR}, $(BENCHMARK_SRC:.c=.o))

BENCHMARK_PUBLIC_INC=lib/inc/
BENCHMARK_PRIVATE_INC=lib/src/

BENCHMARK_LIB=${BUILD_DIR}evolution-benchmark.a

BENCHMARK_CLI_BIN=evolution-benchmark-cli
BENCHMARK_CLI_SRC=bin/src/app.c\
				  bin/main.c
BENCHMARK_CLI_INC=bin/src/


##### TARGETS: #####

build: clean $(BENCHMARK_CLI_BIN)

${BUILD_DIR}%.opt.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -O3 -c -DOPTIMIZATION=optimized -o $@ $^    \
		 -I${BENCHMARK_PUBLIC_INC}

${BUILD_DIR}%.nonopt.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -O0 -c -DOPTIMIZATION=nonoptimized -o $@ $^ \
		 -I${BENCHMARK_PUBLIC_INC}

${BUILD_DIR}%.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -O3 -c  -o $@ $^ \
		 -I${BENCHMARK_PUBLIC_INC} -I${BENCHMARK_PRIVATE_INC}

%.a:
	ar rcs $@ $^

$(BENCHMARK_LIB): $(BENCHMARK_CORE_NONOPT_OBJ) $(BENCHMARK_CORE_OPT_OBJ) $(BENCHMARK_OBJ)
$(BENCHMARK_CLI_BIN): $(BENCHMARK_LIB)
	${CC} ${CFLAGS} \
		${BENCHMARK_CLI_SRC}\
		-I${BENCHMARK_CLI_INC}\
		-I${BENCHMARK_PUBLIC_INC}\
		$^\
		${BENCHMARK_DEPS}\
		-o ${BUILD_DIR}$@
	cp ${BUILD_DIR}$@ $@
	@echo
	@echo "Waverian evolution benchmark executable is located ./evolution-benchmark-cli"

.build_dir: clean
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR}