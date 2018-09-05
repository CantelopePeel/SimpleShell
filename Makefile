BAZEL_ARCHIVE=bazel-0.16.1-dist.zip
BAZEL_EXEC=bazel/output/bazel

.PHONY: build
build: $(BAZEL_EXEC)
	$(BAZEL_EXEC) build //shell:shell-exec

.PHONY: run
run: $(BAZEL_EXEC)
	$(BAZEL_EXEC) run //shell:shell-exec

.PHONY: clean
clean: $(BAZEL_EXEC)
	$(BAZEL_EXEC) clean

.PHONY: full_clean
full_clean: $(BAZEL_EXEC)
	$(BAZEL_EXEC) clean
	rm -rf bazel


$(BAZEL_EXEC): 
	wget -nv https://github.com/bazelbuild/bazel/releases/download/0.16.1/bazel-0.16.1-dist.zip
	mkdir bazel
	unzip -q -d bazel $(BAZEL_ARCHIVE)
	bash ./bazel/compile.sh
	rm $(BAZEL_ARCHIVE)
