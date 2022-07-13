# Copyright (c) 2021 Waverian
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


LFK_VERSION=$(shell cat VERSION || echo "UNKNOWN")
LFK_COMPILER=$(shell ${CC} --version | head -n1)
BUILD_DIR = build_dir/


CFLAGS = -DLFK_VERSION="\"${LFK_VERSION}\"" -DLFK_COMPILER="\"${LFK_COMPILER}\""

LKF_LIB=${BUILD_DIR}lfk.a
LFK_CORE_OPT_LIB=${BUILD_DIR}lfk-core-optimized.a
LFK_CORE_NOOPT_LIB=${BUILD_DIR}lfk-core-nonoptimized.a


LFK_CORE_SRC=lfk_benchmark/core/src/posix/platform.c\
 			 lfk_benchmark/core/src/lfk_core.c

LFK_CORE_OPT_OBJ=$(addprefix ${BUILD_DIR}, $(LFK_CORE_SRC:.c=.opt.o))
LFK_CORE_NONOPT_OBJ=$(addprefix ${BUILD_DIR}, $(LFK_CORE_SRC:.c=.nonopt.o))

LFK_CORE_PRIVATE_INCLUDE=lfk_benchmark/core/src/
LFK_CORE_PUBLIC_INCLUDE=lfk_benchmark/core/inc/
LFK_CORE_DEPS=-lm

LFK_SRC=lfk_benchmark/src/posix/platform.c\
        lfk_benchmark/src/posix/thread.c\
        lfk_benchmark/src/constants.c\
        lfk_benchmark/src/lfk.c

LFK_OBJ=$(addprefix ${BUILD_DIR}, $(LFK_SRC:.c=.o))

LFK_PUBLIC_INC=lfk_benchmark/inc/
LFK_PRIVATE_INC=lfk_benchmark/src/


${BUILD_DIR}%.opt.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -O3 -c -DOPTIMIZATION=optimized -o $@ $^    \
		-I${LFK_CORE_PRIVATE_INCLUDE} -I${LFK_CORE_PUBLIC_INCLUDE} -I${LFK_PUBLIC_INC}

${BUILD_DIR}%.nonopt.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -O0 -c -DOPTIMIZATION=nonoptimized -o $@ $^ \
		-I${LFK_CORE_PRIVATE_INCLUDE} -I${LFK_CORE_PUBLIC_INCLUDE} -I${LFK_PUBLIC_INC}

${BUILD_DIR}%.o : %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -O3 -c  -o $@ $^ \
		-I${LFK_CORE_PUBLIC_INCLUDE} -I${LFK_PUBLIC_INC} -I${LFK_PRIVATE_INC}


$(LFK_CORE_OPT_LIB): $(LFK_CORE_OPT_OBJ)
	ar rcs $@ $^

$(LFK_CORE_NOOPT_LIB): $(LFK_CORE_NONOPT_OBJ)
	ar rcs $@ $^

$(LKF_LIB): $(LFK_CORE_OPT_LIB) $(LFK_CORE_NOOPT_LIB) $(LFK_OBJ)
	ar rcs $@ $^


LFK_CONSOLE_SRC=lfk_console/src/utils/html_report_generator.c\
				lfk_console/src/utils/text_report_generator.c\
				lfk_console/src/app.c\
				lfk_console/main.c

LF_CONSOLE_INC=lfk_console/src/

lfk_console: $(LKF_LIB)
	${CC} $(LFK_CONSOLE_SRC) -I${LF_CONSOLE_INC} -I${LFK_CORE_PUBLIC_INCLUDE} -I${LFK_PUBLIC_INC}\
		-s ${LKF_LIB}\
		-s ${LFK_CORE_OPT_LIB}\
		-s ${LFK_CORE_NOOPT_LIB}\
		-lm -lpthread\
		-o ${BUILD_DIR}$@

.build_dir: clean
	mkdir -p ${BUILD_DIR}

clean:
	rm -rf ${BUILD_DIR}