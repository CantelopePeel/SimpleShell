BAZEL_ARCHIVE=bazel-0.16.1-dist.zip
BAZEL_EXEC=bazel/output/bazel

.PHONY: shell_build
shell_build: $(BAZEL_EXEC)
	mkdir -p ./build
	$(BAZEL_EXEC) --output_user_root=./build build //shell:shell-exec
	mkdir -p ./bin
	cp ./bazel-bin/shell/shell-exec ./bin

.PHONY: shell_test
shell_test: $(BAZEL_EXEC)
	$(BAZEL_EXEC) --output_user_root=./build test //shell:shell-test

.PHONY: proc_build
proc_build:
	mkdir -p ./bin
	g++ -o ./bin/proc_info ./proc_info/proc_info.cc
	g++ -o ./bin/proc_info_rate ./proc_info/proc_info_rate.cc

.PHONY: run
run: $(BAZEL_EXEC)
	$(BAZEL_EXEC) --output_user_root=./build run //shell:shell-exec

.PHONY: clean
clean: $(BAZEL_EXEC)
	$(BAZEL_EXEC) --output_user_root=./build clean

.PHONY: full_clean
full_clean: $(BAZEL_EXEC)
	$(BAZEL_EXEC) --output_user_root=./build clean
	rm -rf bazel


$(BAZEL_EXEC): 
	wget -nv https://github.com/bazelbuild/bazel/releases/download/0.16.1/bazel-0.16.1-dist.zip
	mkdir bazel
	unzip -q -d bazel $(BAZEL_ARCHIVE)
	bash ./bazel/compile.sh
	rm $(BAZEL_ARCHIVE)
