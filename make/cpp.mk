CXX = clang++

CPPFLAGS =                                                          \
	$(patsubst %, -I%, $(shell find -L $(ROOTDIR) -name "include")) \
	-DGLFW_INCLUDE_VULKAN                                           \
	-DVK_ENABLE_BETA_EXTENSIONS                                     \
	-DVULKAN_HPP_ENABLE_DYNAMIC_LOADER_TOOL                         \
	-DVULKAN_HPP_USE_REFLECT

CXXFLAGS =       \
	-std=c++26   \
	-O3          \
	-Wall        \
	-Wextra      \
	-Wpedantic   \
	-Wshadow     \
	-Wconversion \
	-Werror


LDLIBS =

ifeq ($(BUILD_MODE), debug/build)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" -not -path "*/test/*")
	CXXFLAGS += -UNDEBUG -g
endif 

ifeq ($(BUILD_MODE), release/build)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" -not -path "*/test/*")
	CXXFLAGS += -DNDEBUG
endif

ifeq ($(BUILD_MODE), debug/test)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" -not -path "*/main.cpp")
	CPPFLAGS += `pkg-config --cflags gtest_main`
	CXXFLAGS += -UNDEBUG -g
	LDLIBS += `pkg-config --static --libs gtest_main`
endif

ifeq ($(BUILD_MODE), release/test)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" -not -path "*/main.cpp")
	CPPFLAGS += `pkg-config --cflags gtest_main`
	CXXFLAGS += -DNDEBUG
	LDLIBS += `pkg-config --static --libs gtest_main`
endif

ifeq ($(BUILD_MODE), compile_commands_release)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" )
	CPPFLAGS += `pkg-config --cflags gtest_main`
	CXXFLAGS += -DNDEBUG
	LDLIBS += `pkg-config --static --libs gtest_main`
endif

ifeq ($(BUILD_MODE), compile_commands_debug)
	SRCS := $(shell find -L $(ROOTDIR)/src -name "*.cpp" )
	CPPFLAGS += `pkg-config --cflags gtest_main`
	CXXFLAGS += -UNDEBUG -g
	LDLIBS += `pkg-config --static --libs gtest_main`
endif

define pkg_config_cflags
$(if $(wildcard $(1)/pkg-config.mk),$(shell pkg-config --cflags $(shell cat $(1)/pkg-config.mk)))
endef

define pkg_config_libs
$(if $(wildcard $(1)/pkg-config.mk),$(shell pkg-config --libs $(shell cat $(1)/pkg-config.mk)))
endef

define src_directory
$(ROOTDIR)/src/$(firstword $(subst /, ,$(patsubst $(ROOTDIR)/src/%,%,$(dir $(1)))))
endef

SRC_DIRS := $(sort $(foreach src,$(SRCS),$(call src_directory,$(src))))

LDLIBS += $(foreach dir,$(SRC_DIRS),$(call pkg_config_libs,$(dir)))

.PHONY: $(BUILD_MODE)
$(BUILD_MODE): $(ROOTDIR)/bin/$(BUILD_MODE)

$(ROOTDIR)/bin/$(BUILD_MODE): $(patsubst $(ROOTDIR)/src/%.cpp, $(ROOTDIR)/obj/$(BUILD_MODE)/%.o, $(SRCS))
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(call pkg_config_cflags,$(call src_directory,$<)) -o $@ $^ $(LDLIBS)

$(ROOTDIR)/obj/$(BUILD_MODE)/%.o: $(ROOTDIR)/src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(call pkg_config_cflags,$(call src_directory,$<)) $(CXXFLAGS) -o $@ -c $<

$(ROOTDIR)/dep/$(BUILD_MODE)/%.d: $(ROOTDIR)/src/%.cpp
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(call pkg_config_cflags,$(call src_directory,$<)) $(CXXFLAGS) $< -MM -MT $(patsubst $(ROOTDIR)/dep/$(BUILD_MODE)/%.d, $(ROOTDIR)/obj/$(BUILD_MODE)/%.o, $@) -o $@

-include $(patsubst $(ROOTDIR)/src/%.cpp, $(ROOTDIR)/dep/$(BUILD_MODE)/%.d, $(SRCS)) 
