#------------------------------------------------------------------------------
# @author 	Daniel Kopiecki
# @date   	2023-01-20
# @brief  	Common automation tasks
#------------------------------------------------------------------------------

project   := ctetris
cores     := $(shell nproc)
root_dir  := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
build_dir := _build
bin_dir   := $(build_dir)/app
debug     ?= y
top       := $(abspath $(dir $(firstword $(MAKEFILE_LIST))))

cmake_conf_flags += -DPROJECT_NAME=$(project)
ifeq ($(debug),y)
cmake_conf_flags += -DCMAKE_BUILD_TYPE=DEBUG
else
cmake_conf_flags += -DCMAKE_BUILD_TYPE=RELEASE
endif

cmake_conf_flags += "-DCMAKE_TOOLCHAIN_FILE=$(top)/cmake/toolchain/native.cmake"
cmake_conf_flags += "-GNinja"

cmake_configure  := cmake $(cmake_conf_flags) ..
cmake_build      := cmake --build . $(cmake_build_flags) -- -j$(cores)

default: build
.PHONY: default

build: | .mk-build-dir
	@ cd $(build_dir) && \
	        [ -f CMakeCache.txt ] \
	            && echo "Building project $(project)" \
	            || $(cmake_configure) && \
	        $(cmake_build)
.PHONY: build

run:
	@ ./$(bin_dir)/$(project)
.PHONY: run

.mk-build-dir:
	@ mkdir -p $(build_dir)
.PHONY: .mk-build-dir

clean:
	rm -rf "$(build_dir)"
.PHONY: clean
