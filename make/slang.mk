SLANG := slangc

SRCS     := $(shell find -L $(ROOTDIR)/src -name "*.slang")
GENERATE := $(foreach src,$(SRCS),$(dir $(src))generate/$(notdir $(src:.slang=.cpp)))
SPV      := $(foreach src,$(SRCS),$(subst /src/,/spv/,$(dir $(src)))generate/$(notdir $(src:.slang=.spv)))

.PHONY: shaders
shaders: $(GENERATE)

$(GENERATE): $(SPV)
	@mkdir -p $(@D)
	@xxd -i -n $(notdir $(@:.cpp=_spv)) $< $@
	@{ printf '%s\n\n' '#include <cstdint>'; printf 'alignas(std::uint32_t) '; cat $@; } > $@.tmp && mv $@.tmp $@

$(SPV): $(SRCS)
	@mkdir -p $(@D)
	@$(SLANG) $< -target spirv -profile spirv_1_4 -emit-spirv-directly -fvk-use-entrypoint-name -entry vert_main -entry frag_main -o $@