## SPDX-License-Identifier: GPL-2.0-only

test-help help::
	@echo  '*** coreboot test targets ***'
	@echo  '  what-jenkins-does    - Run platform build tests with junit output'
	@echo  '                         Skip tests by setting env vars JENKINS_SKIP_x_TESTS'
	@echo  '                         to 'y' where x is: CLANG, GCC, LINT, TOOL, or UNIT'
	@echo  '  lint / lint-stable   - Run coreboot lint tools (all / minimal)'
	@echo  '  test-basic           - Run standard build tests. All expected to pass.'
	@echo  '  test-lint            - Basic: Run stable and extended lint tests.'
	@echo  '  test-tools           - Basic: Tests a basic list of tools.'
	@echo  '  test-abuild          - Basic: Builds all platforms'
	@echo  '  test-payloads        - Basic: Builds internal payloads'
	@echo  '  test-cleanup         - Basic: Cleans coreboot directories'
	@echo

# junit.xml is a helper target to wrap builds that don't create junit.xml output
# BLD = The name of the build
# BLD_DIR = Top path from coreboot to the build subdirectory
# MAKETARGET = target to build
junit.xml:
	echo "Building $(BLD)"
	echo '<?xml version="1.0" encoding="utf-8"?><testsuite>' > $@.tmp
	echo "<testcase classname='$(BLD)' name='$(BLD)'>" >> $@.tmp
	-if [ -z "$(BLD_DIR)" ]; then                                          \
		projdir="";                                                    \
		xmlfile=junit_$(BLD).xml;                                      \
	else                                                                   \
		projdir="-C $(BLD_DIR)";                                       \
		xmlfile="$(BLD_DIR)/$@";                                       \
	fi;                                                                    \
	if [ -z "$(SKIP_DISTCLEAN)" ]; then                                    \
		$(MAKE) -j $(CPUS) $$projdir clean distclean > $@.tmp.2 2>&1 ; \
	fi;                                                                    \
	if $(MAKE) -j $(CPUS) $$projdir $(MAKETARGET) >> $@.tmp.2 2>&1; then   \
		type="system-out";                                             \
		echo "<$$type>" >> $@.tmp;                                     \
		echo "Building $(BLD) Succeeded";                              \
	else                                                                   \
		type="failure";                                                \
		echo "<failure type='buildFailed'>" >> $@.tmp;                 \
		echo "Building $(BLD) Failed";                                 \
	fi;                                                                    \
	cat $@.tmp.2;                                                          \
	echo '<![CDATA[' >> $@.tmp;                                            \
	cat $@.tmp.2 >> $@.tmp;                                                \
	echo "]]></$$type>" >>$@.tmp;                                          \
	rm -f $@.tmp.2;                                                        \
	echo "</testcase>" >> $@.tmp;                                          \
	echo "</testsuite>" >> $@.tmp;                                         \
	mv $@.tmp "$$xmlfile"
	echo

TOOLLIST= \
amdfwtool \
cbfstool \
cbmem \
ectool \
futility \
ifdtool \
intelmetool \
inteltool \
intelvbttool \
nvramtool \
post \
superiotool

TEST_PAYLOADLIST_INTERNAL= \
coreinfo \
nvramcui

JENKINS_PAYLOAD?=none
TEST_PAYLOAD?=$(JENKINS_PAYLOAD)
CPUS?=4

lint lint-stable lint-extended:
	util/lint/lint $@

ABUILD_OPTIONS=--blobs --junit --cpus $(CPUS) --payloads $(JENKINS_PAYLOAD)
ABUILD_OPTIONS+=$(if $(V),--verbose,)
ABUILD_OPTIONS+=$(JENKINS_ABUILD_OPT)

CCACHE_OPTIONS=$(if $(JENKINS_NOCCACHE),,--ccache)

COREBOOT_BUILD_DIR?=coreboot-builds

validate_sec_tools:
	if [ ! -f 3rdparty/intel-sec-tools/go.mod ]; then \
		echo "Please download 3rdparty/intel-sec-tools/:"; \
		echo "git submodule update --init 3rdparty/intel-sec-tools"; \
		exit 1; \
	fi

what-jenkins-does: test-cleanup validate_sec_tools
ifneq ($(JENKINS_SKIP_LINT_TESTS),y)
	JUNIT=--junit $(MAKE) test-lint
endif
	cd 3rdparty/intel-sec-tools/ ; go mod vendor
	cd util/goswid ; go mod vendor
	$(MAKE) test-abuild
ifneq  ($(JENKINS_SKIP_TOOL_TESTS),y)
	$(MAKE) test-tools
endif
ifneq  ($(JENKINS_SKIP_UNIT_TESTS),y)
	+$(MAKE) junit.xml-unit-tests COV=1
	+(cd payloads/libpayload; unset COREBOOT_BUILD_DIR; $(MAKE) junit.xml-unit-tests COV=1)
	+(cd payloads/libpayload; unset COREBOOT_BUILD_DIR; $(MAKE) coverage-report COV=1)
	+$(MAKE) coverage-report JUNIT_OUTPUT=y COV=1
	find . -name 'tests.info' -exec cat {} + >$(COREBOOT_BUILD_DIR)/coverage.info
endif

test-basic: test-lint test-tools test-abuild test-payloads test-cleanup

test-lint:
	util/lint/lint lint-stable $(JUNIT)
	util/lint/lint lint-extended $(JUNIT)

test-abuild:
ifneq ($(JENKINS_SKIP_GCC_TESTS),y)
ifneq ($(JENKINS_SKIP_ALLTHREAD_TEST),y)
	NAME=emu_gcc_allthreads; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --name $${NAME} --target EMULATION_QEMU_X86_Q35 -b EMULATION_QEMU_X86_Q35 --exitcode --clean
	NAME=amd_gcc_allthreads; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --name $${NAME} --target GOOGLE_SKYRIM -b GOOGLE_SKYRIM --exitcode --clean
	NAME=intel_gcc_allthreads; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --name $${NAME} --target GOOGLE_REX0 -b GOOGLE_REX0 --exitcode --clean
	NAME=mediatek_gcc_allthreads; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --name $${NAME} --target GOOGLE_CHERRY -b GOOGLE_CHERRY --exitcode --clean
endif
	NAME=gcc-chromeos; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) -x --name $${NAME} --clean
	NAME=gcc; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --name $${NAME} --clean-somewhat
endif
ifneq ($(JENKINS_SKIP_CLANG_TESTS),y)
ifneq ($(JENKINS_SKIP_ALLTHREAD_TEST),y)
	NAME=emu_clang_allthreads; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --clang --name $${NAME} --target EMULATION_QEMU_X86_Q35 -b EMULATION_QEMU_X86_Q35 --exitcode --clean
	NAME=amd_clang_allthreads; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --clang --name $${NAME} --target GOOGLE_SKYRIM -b GOOGLE_SKYRIM --exitcode --clean
	NAME=intel_clang_allthreads; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --clang --name $${NAME} --target GOOGLE_REX0 -b GOOGLE_REX0 --exitcode --clean
	NAME=mediatek_clang_allthreads; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) --clang --name $${NAME} --target GOOGLE_CHERRY -b GOOGLE_CHERRY --exitcode --clean
endif
	NAME=clang-chromeos; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) -L -x --name $${NAME} --clean
	NAME=clang; util/abuild/abuild -o $(COREBOOT_BUILD_DIR)/$${NAME} $(ABUILD_OPTIONS) $(CCACHE_OPTIONS) -L --name $${NAME} --clean
endif

test-payloads:
	$(MAKE) -C payloads/libpayload test-configs -j $(CPUS) V=$(V) Q=$(Q) MFLAGS= MAKEFLAGS= $(if $(TEST_NOCCACHE),,CONFIG_LP_CCACHE=y)
	export MFLAGS=;export MAKEFLAGS=; \
	$(foreach payload, $(TEST_PAYLOADLIST_INTERNAL), \
		echo "*** Making payload $(payload) ***"; \
		$(MAKE) -C payloads/$(payload) distclean ;\
		$(MAKE) $(payload) -j $(CPUS) V=$(V) Q=$(Q)\
		|| exit 1; )

test-tools:
	@echo "Build testing $(TOOLLIST)"
	$(foreach tool, $(TOOLLIST),  echo "Building $(tool)";$(MAKE) CPUS=$(CPUS) V=$(V) Q=$(Q) BLD_DIR="util/$(tool)" BLD="$(tool)" MFLAGS= MAKEFLAGS= MAKETARGET= junit.xml; )
	unset COREBOOT_BUILD_DIR;$(MAKE) CPUS=$(CPUS) V=$(V) Q=$(Q) BLD_DIR=payloads/nvramcui BLD=nvramcui MFLAGS= MAKEFLAGS= MAKETARGET=all junit.xml
	unset COREBOOT_BUILD_DIR;$(MAKE) CPUS=$(CPUS) V=$(V) Q=$(Q) BLD_DIR=payloads/coreinfo BLD=coreinfo MFLAGS= MAKEFLAGS= MAKETARGET=defaultbuild junit.xml
	(cd payloads/libpayload; unset COREBOOT_BUILD_DIR; $(MAKE) $(if $(JENKINS_NOCCACHE),,CONFIG_LP_CCACHE=y) V=$(V) Q=$(Q) junit.xml)
	echo "The tegra builds need to be fixed.  Disabled until someone fixes them."
	# The tegra targets don't use the standard build system, so we need to provide an xcompile to them.
	# $(MAKE) xcompile=$(COREBOOT_BUILD_DIR)/xcompile $(COREBOOT_BUILD_DIR)/xcompile
	# $(MAKE) CPUS=$(CPUS) V=$(V) Q=$(Q) BLD_DIR=src/soc/nvidia/tegra124/lp0 BLD=tegra124_lp0 MFLAGS= MAKEFLAGS=xcompile=$(COREBOOT_BUILD_DIR)/xcompile MAKETARGET=all junit.xml
	# $(MAKE) CPUS=$(CPUS) V=$(V) Q=$(Q) BLD_DIR=src/soc/nvidia/tegra210/lp0 BLD=tegra120_lp0 MFLAGS= MAKEFLAGS=xcompile=$(COREBOOT_BUILD_DIR)/xcompile MAKETARGET=all junit.xml
	#
	echo "The gitconfig tests are flaky and are disabled until someone fixes them."
	# @echo "Running gitconfig tests"
	# @for test in $$(find util/gitconfig/test -maxdepth 1 \
	#	-type f -executable); do \
	#	echo "$${test}"; \
	#	"$${test}" || exit $${?}; \
	# done

test-cleanup:
	rm -rf $(COREBOOT_BUILD_DIR)/chromeos $(COREBOOT_BUILD_DIR)/default
	rm -rf $(COREBOOT_BUILD_DIR)/chromeos-clang $(COREBOOT_BUILD_DIR)/default-clang
	$(MAKE) clean
	$(foreach tool, $(TOOLLIST), $(MAKE) -C util/$(tool) clean ; )
	$(MAKE) -C src/soc/nvidia/tegra124/lp0 clean
	$(MAKE) -C src/soc/nvidia/tegra210/lp0 clean

.PHONY: test-basic test-lint test-abuild test-payloads
.PHONY: test-tools test-cleanup test-help
.PHONY: lint lint-stable what-jenkins-does
