.PHONY: all
all: compile_commands_debug format shaders release debug test_release test_debug

.PHONY: help
help :
	@echo compile_commands_release
	@echo compile_commands_debug
	@echo format
	@echo release
	@echo debug
	@echo test_release
	@echo test_debug
	@echo shaders
	@echo clean

.PHONY: compile_commands_release
compile_commands_release :
	@rm -f $(CURDIR)/compile_commands.json
	@bear -- make -f $(CURDIR)/make/cpp.mk --warn-undefined-variables ROOTDIR="$(CURDIR)" BUILD_MODE="compile_commands_release"
	@rm -f $(CURDIR)/bin/compile_commands_release 
	@rm -rf $(CURDIR)/obj/compile_commands_release
	@echo finished compile commands release

.PHONY: compile_commands_debug
compile_commands_debug :
	@rm -f $(CURDIR)/compile_commands.json
	@bear -- make -f $(CURDIR)/make/cpp.mk --warn-undefined-variables ROOTDIR="$(CURDIR)" BUILD_MODE="compile_commands_debug"
	@rm -f $(CURDIR)/bin/compile_commands_debug 
	@rm -rf $(CURDIR)/obj/compile_commands_debug
	@echo finished compile commands debug

.PHONY: format
format:
	@clang-format $(shell find -L $(CURDIR) -name "*.cpp" -o -name "*.hpp" -o -name "*.ipp" -o -name "*.frag" -o -name "*.vert" -o -name "*.glsl") -style=file -i
	@echo finished format

.PHONY: release
release: shaders
	@make -f $(CURDIR)/make/cpp.mk --warn-undefined-variables ROOTDIR="$(CURDIR)" BUILD_MODE="release/build"
	@echo finished release build

.PHONY: debug
debug: shaders
	@make -f $(CURDIR)/make/cpp.mk --warn-undefined-variables ROOTDIR="$(CURDIR)" BUILD_MODE="debug/build"
	@echo finished debug build

.PHONY: test_release
test_release: shaders
	@make -f $(CURDIR)/make/cpp.mk --warn-undefined-variables ROOTDIR="$(CURDIR)" BUILD_MODE="release/test"
	@echo finished release test

.PHONY: test_debug
test_debug: shaders
	@make -f $(CURDIR)/make/cpp.mk --warn-undefined-variables ROOTDIR="$(CURDIR)" BUILD_MODE="debug/test"
	@echo finished debug test

.PHONY: shaders
shaders :
	@make -f $(CURDIR)/make/glsl.mk --warn-undefined-variables ROOTDIR="$(CURDIR)"
	@echo finished shaders

.PHONY: clean
clean:
	@rm -rf $(CURDIR)/spv $(CURDIR)/bin $(CURDIR)/dep $(CURDIR)/obj $(CURDIR)/log $(CURDIR)/compile_commands.json
	@echo finished clean
