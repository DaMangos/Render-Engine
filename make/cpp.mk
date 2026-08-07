CXX = clang++

CPP_FLAGS =                                                         \
	$(patsubst %, -I%, $(shell find -L $(ROOTDIR) -name "include")) \
	`pkg-config --cflags vulkan`                                    \
	`pkg-config --cflags glfw3`                                     \
	`pkg-config --cflags gtest_main`                                \
	-DGLFW_INCLUDE_VULKAN                                           \
	-DVK_ENABLE_BETA_EXTENSIONS                                     \
	-DVULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL                         \
	-DVULKAN_HPP_USE_REFLECT

CXX_FLAGS =      \
	-std=c++26   \
	-O3          \
	-Wall        \
	-Wextra      \
	-Wpedantic   \
	-Wshadow     \
	-Wconversion \
	-Werror

LDLIBS =                                \
	`pkg-config --static --libs vulkan` \
	`pkg-config --static --libs glfw3`  \

ifeq ($(BUILD_MODE), debug/build)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" -not -path "*/test/*")
	CXX_FLAGS += -UNDEBUG -g
endif 

ifeq ($(BUILD_MODE), release/build)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" -not -path "*/test/*")
	CXX_FLAGS += -DNDEBUG
endif

ifeq ($(BUILD_MODE), debug/test)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" -not -path "*/main.cpp")
	CXX_FLAGS += -UNDEBUG -g
	LDLIBS += `pkg-config --static --libs gtest_main`
endif

ifeq ($(BUILD_MODE), release/test)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" -not -path "*/main.cpp")
	CXX_FLAGS += -DNDEBUG
	LDLIBS += `pkg-config --static --libs gtest_main`
endif

ifeq ($(BUILD_MODE), compile_commands_release)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" )
	CXX_FLAGS += -DNDEBUG
endif

ifeq ($(BUILD_MODE), compile_commands_debug)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" )
	CXX_FLAGS += -UNDEBUG -g
endif

.PHONY: $(BUILD_MODE)
$(BUILD_MODE): $(ROOTDIR)/bin/$(BUILD_MODE)

$(ROOTDIR)/bin/$(BUILD_MODE): $(patsubst $(ROOTDIR)/src/%.cpp, $(ROOTDIR)/obj/$(BUILD_MODE)/%.o, $(SRCS))
	@mkdir -p $(@D)
	@$(CXX) $(CPP_FLAGS) -o $@ $^ $(LDLIBS)

$(ROOTDIR)/obj/$(BUILD_MODE)/%.o: $(ROOTDIR)/src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) -o $@ -c $<

$(ROOTDIR)/dep/$(BUILD_MODE)/%.d: $(ROOTDIR)/src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CPP_FLAGS) $(CXX_FLAGS) $< -MM -MT $(patsubst $(ROOTDIR)/dep/$(BUILD_MODE)/%.d, $(ROOTDIR)/obj/$(BUILD_MODE)/%.o, $@) -o $@

-include $(patsubst $(ROOTDIR)/src/%.cpp, $(ROOTDIR)/dep/$(BUILD_MODE)/%.d, $(SRCS)) 
