BAZEL_ARCHIVE=bazel-0.16.1-dist.zip
BAZEL_EXEC=bazel/output/bazel

$(BAZEL_EXEC): 
	wget https://github.com/bazelbuild/bazel/releases/download/0.16.1/bazel-0.16.1-dist.zip
	mkdir bazel
	unzip -d bazel $(BAZEL_ARCHIVE)
	bash ./bazel/compile.sh

.PHONY: build
build: $(BAZEL_EXEC)
	$(BAZEL_EXEC) build //shell:shell-exec

.PHONY: run
run: $(BAZEL_EXEC)
	$(BAZEL_EXEC) run //shell:shell-exec

