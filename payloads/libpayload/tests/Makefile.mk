# SPDX-License-Identifier: GPL-2.0-only

testsrc := $(top)/tests

# Place the build output in one of two places depending on COV, so that code
# built with code coverage never mixes with code built without code coverage.
ifeq ($(COV),1)
testobj := $(obj)/coverage
else
testobj := $(obj)/tests
endif
coverage-dir := $(testobj)/coverage_reports

cmockasrc := $(coreboottop)/3rdparty/cmocka
cmockaobj := $(objutil)/cmocka
CMOCKA_LIB := $(cmockaobj)/src/libcmocka.so

CMAKE := cmake

TEST_DEFAULT_CONFIG := $(top)/configs/config.unit-tests
TEST_DOTCONFIG := $(testobj)/.config
TEST_KCONFIG_AUTOHEADER := $(testobj)/libpayload-config.src.h
TEST_KCONFIG_AUTOCONFIG := $(testobj)/auto.conf
TEST_KCONFIG_DEPENDENCIES := $(testobj)/auto.conf.cmd
TEST_KCONFIG_SPLITCONFIG := $(testobj)/config/
TEST_KCONFIG_TRISTATE := $(testobj)/tristate.conf
TEST_KCONFIG_NEGATIVES := 1
TEST_KBUILD_KCONFIG := $(top)/Kconfig
TEST_CONFIG_ := CONFIG_LP_


# Default includes
TEST_CFLAGS := -include include/kconfig.h
TEST_CFLAGS += -include $(coreboottop)/src/commonlib/bsd/include/commonlib/bsd/compiler.h
TEST_CFLAGS += -Iinclude -Iinclude/mock
TEST_CFLAGS += -I$(coreboottop)/src/commonlib/bsd/include
TEST_CFLAGS += -I$(coreboottop)/src/commonlib/include
TEST_CFLAGS += -I$(dir $(TEST_KCONFIG_AUTOHEADER))
TEST_CFLAGS += -I$(VBOOT_SOURCE)/firmware/include

# Test specific includes
TEST_CFLAGS += -I$(testsrc)/include
TEST_CFLAGS += -I$(cmockasrc)/include

# Minimal subset of warnings and errors. Tests can be less strict than actual build.
TEST_CFLAGS += -Wall -Wundef -Wstrict-prototypes -Wvla
TEST_CFLAGS += -Wwrite-strings -Wno-address-of-packed-member -Wimplicit-fallthrough
TEST_CFLAGS += -Wstrict-aliasing -Wshadow -Werror
TEST_CFLAGS += -Wno-unknown-warning-option -Wno-source-mgr -Wno-main-return-type

TEST_CFLAGS += -std=gnu23 -ffunction-sections -fdata-sections -fno-builtin

ifneq ($(filter-out 0,$(DEBUG)),)
TEST_CFLAGS += -Og -ggdb3
else
TEST_CFLAGS += -Os
endif

# Make unit-tests detectable by the code
TEST_CFLAGS += -D__TEST__

# Link against CMocka
TEST_LDFLAGS := -L$(dir $(CMOCKA_LIB)) -lcmocka -Wl,-rpath=$(dir $(CMOCKA_LIB))

TEST_LDFLAGS += -Wl,--gc-sections

# Disable userspace relocations
TEST_CFLAGS += -fno-pie -fno-pic
TEST_LDFLAGS += -no-pie

ifeq ($(COV),1)
TEST_CFLAGS += --coverage
TEST_LDFLAGS += --coverage
endif


# Extra attributes for unit tests. Declated per each test. Only `srcs` is required.
attributes := cflags config mocks srcs

alltests :=
subdirs := tests/crypto tests/curses tests/drivers tests/gdb tests/libc tests/libcbfs
subdirs += tests/liblz4 tests/liblzma tests/libpci

define tests-handler
alltests += $(1)$(2)
$(foreach attribute,$(attributes), \
	$(eval $(1)$(2)-$(attribute) += $($(2)-$(attribute))))
$(foreach attribute,$(attributes), \
	$(eval $(2)-$(attribute) := ))
endef

# Copy attributes of one test to another
# $1 - input test name
# $2 - output test name
copy-test = $(foreach attribute,$(attributes), \
		$(eval $(strip $(2))-$(attribute) := $($(strip $(1))-$(attribute))))

$(call add-special-class,tests)
$(call evaluate_subdirs)

# Create actual targets for unit test binaries
# $1 - test name
define TEST_CC_template

# Generate custom config.h redefining given config symbols, and declaring mocked
# functions weak. It is important that the compiler already sees that they are
# weak (and they aren't just turned weak at a later stage) to prevent certain
# optimizations that would break if the function gets replaced. (For clang this
# file needs to be marked `system_header` to prevent it from warning about
# `#pragma weak` entries without a matching function declaration, since there is
# no -Wno-xxx commandline for that.)
$(1)-config-file := $(testobj)/$(1)/libpayload-config.h
$$($(1)-config-file): $(TEST_KCONFIG_AUTOHEADER)
	mkdir -p $$(dir $$@);
	printf '// File generated by tests/Makefile.mk\n// Do not change\n' > $$@;
	printf '#ifndef TEST_LIBPAYLOAD_CONFIG_H_\n' >> $$@;
	printf '#define TEST_LIBPAYLOAD_CONFIG_H_\n' >> $$@;
	printf '#include <%s>\n\n' "$(notdir $(TEST_KCONFIG_AUTOHEADER))" >> $$@;
	for kv in $$($(1)-config); do \
		key="`echo $$$$kv | cut -d '=' -f -1`"; \
		value="`echo $$$$kv | cut -d '=' -f 2-`"; \
		printf '#undef %s\n' "$$$$key" >> $$@; \
		printf '#define %s %s\n\n' "$$$$key" "$$$$value" >> $$@; \
	done
	printf '#ifdef __clang__\n' >> $$@;
	printf '#pragma clang system_header\n' >> $$@;
	printf '#endif\n\n' >> $$@;
	printf '#ifdef __TEST_SRCOBJ__\n' >> $$@;
	for m in $$($(1)-mocks); do \
		printf '#pragma weak %s\n' "$$$$m" >> $$@; \
	done
	printf '#endif\n\n' >> $$@;
	printf '#endif\n' >> $$@;

$($(1)-objs): TEST_CFLAGS += -I$$(dir $$($(1)-config-file)) \
	-D__TEST_NAME__=\"$(subst /,_,$(1))\"

# Give us a way to distinguish between libpayload source files and test files in the code.
$($(1)-srcobjs): TEST_CFLAGS += -D__TEST_SRCOBJ__

# Compile sources and apply mocking/wrapping for selected symbols.
# For each listed mock add new symbol with prefix `__real_`,
# pointing to the same section:address. This will keep original
# function accessible if required.
$($(1)-objs): $(testobj)/$(1)/%.o: $$$$*.c $$($(1)-config-file)
	mkdir -p $$(dir $$@)
	$(HOSTCC) $$(TEST_CFLAGS) $($(1)-cflags) -MMD \
		-MF $$(basename $$@).d -MT $$@ -c $$< -o $$@.orig
	objcopy_wrap_flags=''; \
	for sym in $$($(1)-mocks); do \
		sym_line="$$$$($(HOSTOBJDUMP) -t $$@.orig \
			| grep -E "[0-9a-fA-F]+\\s+w\\s+F\\s+.*\\s+$$$$sym$$$$")"; \
		if [ ! -z "$$$$sym_line" ] ; then \
			addr="$$$$(echo "$$$$sym_line" | awk '{ print $$$$1 }')"; \
			section="$$$$(echo "$$$$sym_line" | awk '{ print $$$$(NF - 2) }')"; \
			objcopy_wrap_flags="$$$$objcopy_wrap_flags --add-symbol __real_$$$${sym}=$$$${section}:0x$$$${addr},function,global"; \
		fi \
	done ; \
	$(HOSTOBJCOPY) $$@.orig $$$$objcopy_wrap_flags $$@

$($(1)-bin): $($(1)-objs) $(CMOCKA_LIB)
	$(HOSTCC) $$^ $($(1)-cflags) $$(TEST_LDFLAGS) -o $$@

endef

$(foreach test,$(alltests), \
	$(eval $(test)-srcobjs := $(addprefix $(testobj)/$(test)/, \
		$(patsubst %.c,%.o,$(filter-out tests/%,$($(test)-srcs))))) \
	$(eval $(test)-objs := $(addprefix $(testobj)/$(test)/, \
		$(patsubst %.c,%.o,$($(test)-srcs)))) \
	$(eval $(test)-bin := $(testobj)/$(test)/run))
$(foreach test,$(alltests), \
	$(eval $(call TEST_CC_template,$(test))))
$(foreach test,$(alltests), \
	$(eval all-test-objs += $($(test)-objs)) \
	$(eval test-bins += $($(test)-bin)))

DEPENDENCIES += $(addsuffix .d,$(basename $(all-test-objs)))
-include $(DEPENDENCIES)

# Build CMocka
$(CMOCKA_LIB):
	echo "*** Building CMOCKA ***"
	mkdir -p $(cmockaobj)
	cd $(cmockaobj) && $(CMAKE) $(abspath $(cmockasrc))
	$(MAKE) -C $(cmockaobj)

# Kconfig targets
$(TEST_DOTCONFIG):
	mkdir -p $(dir $@)
	cp $(TEST_DEFAULT_CONFIG) $(TEST_DOTCONFIG)

$(TEST_KCONFIG_AUTOHEADER): TEST_KCONFIG_FLAGS := DOTCONFIG=$(TEST_DOTCONFIG) \
	KCONFIG_AUTOHEADER=$(TEST_KCONFIG_AUTOHEADER) \
	KCONFIG_AUTOCONFIG=$(TEST_KCONFIG_AUTOCONFIG) \
	KCONFIG_DEPENDENCIES=$(TEST_KCONFIG_DEPENDENCIES) \
	KCONFIG_SPLITCONFIG=$(TEST_KCONFIG_SPLITCONFIG) \
	KCONFIG_TRISTATE=$(TEST_KCONFIG_TRISTATE) \
	KCONFIG_NEGATIVES=$(TEST_KCONFIG_NEGATIVES) \
	KBUILD_KCONFIG=$(TEST_KBUILD_KCONFIG) \
	KBUILD_DEFCONFIG=$(TEST_DEFAULT_CONFIG) \
	CONFIG_=$(TEST_CONFIG_)

$(TEST_KCONFIG_AUTOHEADER): $(TEST_DOTCONFIG) $(objk)/conf
	mkdir -p $(dir $@)
	$(MAKE) $(TEST_KCONFIG_FLAGS) olddefconfig V=$(V)
	$(MAKE) $(TEST_KCONFIG_FLAGS) syncconfig V=$(V)

$(TEST_KCONFIG_AUTOCONFIG): $(TEST_KCONFIG_AUTOHEADER)
	true

.PHONY: $(alltests) $(addprefix clean-,$(alltests)) $(addprefix try-,$(alltests))
.PHONY: $(addprefix build-,$(alltests)) $(addprefix run-,$(alltests))
.PHONY: unit-tests build-unit-tests run-unit-tests clean-unit-tests
.PHONY: junit.xml-unit-tests clean-junit.xml-unit-tests

ifeq ($(JUNIT_OUTPUT),y)
$(addprefix run-,$(alltests)): export CMOCKA_MESSAGE_OUTPUT=xml
$(addprefix run-,$(alltests)): export CMOCKA_XML_FILE=$(testobj)/junit-libpayload-%g.xml
endif

$(addprefix run-,$(alltests)): run-%: $$(%-bin)
	rm -f $(testobj)/junit-libpayload-$(subst /,_,$(patsubst $(testobj)/%/,%,$(dir $^)))\(*\).xml
	rm -f $(testobj)/$(subst /,_,$^).failed
	-$^ || echo failed > $(testobj)/$(subst /,_,$^).failed

$(addprefix build-,$(alltests)): build-%: $$(%-bin)

$(alltests): run-$$(@)

$(addprefix try-,$(alltests)): try-%: clean-% $(CMOCKA_LIB) $(TEST_KCONFIG_AUTOCONFIG)
	mkdir -p $(testobj)/$*
	echo "<testcase classname='libpayload_build_unit_test' name='$*'>" >> $(testobj)/$*.tmp; \
	$(MAKE) V=$(V) Q=$(Q) COV=$(COV) JUNIT_OUTPUT=y "build-$*" >> $(testobj)/$*.tmp.2 2>&1 \
		&& type="system-out" || type="failure"; \
	if [ $$type = "failure" ]; then \
		echo "<failure type='buildFailed'>" >> $(testobj)/$*.tmp; \
	else \
		echo "<$$type>" >> $(testobj)/$*.tmp; \
	fi; \
	echo '<![CDATA[' >> $(testobj)/$*.tmp; \
	cat $(testobj)/$*.tmp.2 >> $(testobj)/$*.tmp; \
	echo "]]></$$type>" >> $(testobj)/$*.tmp; \
	rm -f $(testobj)/$*.tmp.2; \
	echo "</testcase>" >> $(testobj)/$*.tmp; \
	if [ $$type != 'failure' ]; then \
		$(MAKE) V=$(V) Q=$(Q) COV=$(COV) JUNIT_OUTPUT=y "run-$*"; \
	fi


TESTS_BUILD_XML_FILE := $(testobj)/junit-libpayload-tests-build.xml

$(TESTS_BUILD_XML_FILE): clean-junit.xml-unit-tests $(addprefix try-,$(alltests))
	mkdir -p $(dir $@)
	echo '<?xml version="1.0" encoding="utf-8"?><testsuite>' > $@
	for tst in $(alltests); do \
		cat $(testobj)/$$tst.tmp >> $@; \
	done
	echo "</testsuite>" >> $@

junit.xml-unit-tests: $(TESTS_BUILD_XML_FILE)

clean-junit.xml-unit-tests:
	rm -f $(TESTS_BUILD_XML_FILE)


# Build a code coverage report by collecting all the gcov files into a single
# report. If COV is not set, this might be a user error, and they're trying
# to generate a coverage report without first having built and run the code
# with code coverage. absence of COV=1 will be corrected.

.PHONY: coverage-report clean-coverage-report

ifeq ($(COV),1)
coverage-report:
	lcov -o $(testobj)/tests.info -c -d $(testobj) --exclude '$(testsrc)/*'
	genhtml -q -o $(coverage-dir) -t "coreboot unit tests" -s $(testobj)/tests.info

clean-coverage-report:
	rm -Rf $(coverage-dir)
else
coverage-report:
	COV=1 V=$(V) $(MAKE) coverage-report

clean-coverage-report:
	COV=1 V=$(V) $(MAKE) clean-coverage-report
endif

unit-tests: build-unit-tests run-unit-tests

build-unit-tests: $(test-bins)

run-unit-tests: $(alltests)
	if [ `find $(testobj) -name '*.failed' | wc -l` -gt 0 ]; then \
		echo "**********************"; \
		echo "     TESTS FAILED"; \
		echo "**********************"; \
		exit 1; \
	else \
		echo "**********************"; \
		echo "   ALL TESTS PASSED"; \
		echo "**********************"; \
		exit 0; \
	fi

$(addprefix clean-,$(alltests)): clean-%:
	rm -rf $(testobj)/$*

clean-unit-tests:
	rm -rf $(testobj)

list-unit-tests:
	@echo "unit-tests:"
	for t in $(sort $(alltests)); do \
		echo "  $$t"; \
	done

help-unit-tests help::
	@echo  '*** libpayload unit-tests targets ***'
	@echo  '  Use "COV=1 make [target]" to enable code coverage for unit tests'
	@echo  '  unit-tests            - Run all unit-tests from tests/'
	@echo  '  clean-unit-tests      - Remove unit-tests build artifacts'
	@echo  '  list-unit-tests       - List all unit-tests'
	@echo  '  <unit-test>           - Build and run single unit-test'
	@echo  '  clean-<unit-test>     - Remove single unit-test build artifacts'
	@echo  '  coverage-report       - Generate a code coverage report'
	@echo  '  clean-coverage-report - Remove the code coverage report'
	@echo
