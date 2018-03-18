_GUI = $(if $(NOGUI),,-D GUI)
PRE_ = $(if $(DEBUG),$(VALGRIND) ,)
CC = gcc
VALGRIND = valgrind --leak-check=full --error-exitcode=1

# DIRECTORIES
D_SRC		= src
D_INCLUDE	= include
D_DATA		= data
D_SCRIPTS	= scripts
D_CONF		= conf
D_DOC		= doc
D_BUILD		= build
D_BIN		= bin
D_TESTS		= tests
D_VALGRIND	= valgrind

#EXECUTABLES
TARGETS = clash-of-particles snow read-file write-fact
EXECUTABLES = $(TARGETS:%=$(D_BIN)/%)
TEST-TARGETS = heap-correctness heap-complexity particle loader
TEST-EXECUTABLES = $(TEST-TARGETS:%=$(D_TESTS)/%)

# FLAGS
DFLAGS = -I $(D_INCLUDE)/ -I $(D_INCLUDE)/tests
CFLAGS = -g -std=c99 -Wall -Werror $(DFLAGS) $(_GUI)$(if $(DEBUG),, -D NDEBUG -O3)
LDFLAGS = -lm -lSDL
LDFLAGS-T = $(LDFLAGS)
VALGOPT = D_BUILD=$(D_VALGRIND)/$(D_BUILD) \
          D_BIN=$(D_VALGRIND)/$(D_BIN) \
          D_TESTS=$(D_VALGRIND)/$(D_TESTS) \
          NOGUI=1 DEBUG=1

.DEFAULT_GOAL = compile-all
DEFAULT_INPUT_FILE = $(D_DATA)/newton-simple.txt
.PHONY: clean mrproper nothing compile-all doc $(D_BIN)/ $(D_TESTS)/
.PHONY: $(TARGETS:%=compile-%) $(TARGETS:%=run-%) $(TARGETS:%=valgrind-%)
.PHONY: $(TEST-TARGETS:%=compile-test-%) $(TEST-TARGETS:%=test-%) $(TEST-TARGETS:%=valgrind-test-%)

.SECONDARY .PHONY: $(D_DATA)/complexity_heap.csv

compile-all: $(D_BIN)/ $(D_TESTS)/

$(D_BIN)/: $(EXECUTABLES)
	@mkdir -p $@

$(D_TESTS)/: $(TEST-EXECUTABLES)
	@mkdir -p $@

# executables compilation
$(patsubst %,compile-%,$(TARGETS)): compile-%: $(D_BIN)/%

# run executables
$(patsubst %,run-%,$(filter-out clash-of-particles snow read-file,$(TARGETS))): run-%: $(D_BIN)/%
	$(PRE_)./$<
$(patsubst %,run-%,clash-of-particles): run-%: $(D_BIN)/% $(DEFAULT_INPUT_FILE)
	$(PRE_)./$< $(DEFAULT_INPUT_FILE)
$(patsubst %,run-%,snow): run-%: $(D_BIN)/%
	( echo 200 ) | $(PRE_)./$<
$(patsubst %,run-%,read-file): run-%: $(D_BIN)/% $(D_DATA)/toread.txt
	$(PRE_)./$< $(D_DATA)/toread.txt
$(patsubst %,valgrind-%,$(TARGETS)): valgrind-%:
	$(MAKE) $(VALGOPT) $(@:valgrind-%=run-%)

# test executables compilation
$(patsubst %,compile-test-%,$(TEST-TARGETS)): compile-test-%: $(D_TESTS)/%

# run test-executables
$(patsubst %,test-%,$(filter-out loader heap-complexity,$(TEST-TARGETS))): test-%: $(D_TESTS)/%
	$(PRE_)./$<
$(patsubst %,test-%,loader): test-%: $(D_TESTS)/% $(DEFAULT_INPUT_FILE)
	$(PRE_)./$< $(DEFAULT_INPUT_FILE)
$(patsubst %,test-%,heap-complexity): $(D_SCRIPTS)/plot_heap_complexity.py $(D_DATA)/complexity_heap.csv
	./$< $(D_DATA)/complexity_heap.csv
$(patsubst %,valgrind-test-%,$(TEST-TARGETS)): valgrind-test-%:
	$(MAKE) $(VALGOPT) $(@:valgrind-test-%=test-%)

$(D_DATA)/complexity_heap.csv: $(D_TESTS)/heap-complexity
	$(PRE_)./$< $@

doc:
	( cat $(D_CONF)/doxygen.conf ; echo "OUTPUT_DIRECTORY=$(D_DOC)" ) | doxygen -
	@echo DOCUMENTATION GENERATED:
	@echo "    file://$$(realpath $(D_DOC)/html/index.html)"


$(D_BUILD)/%.o: $(D_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

$(D_BUILD)/%.d: $(D_SRC)/%.c
	@mkdir -p $(dir $@)
	@set -e; rm -f $@; \
	 $(CC) -MM $(DFLAGS) $< | \
	 sed 's,\($(notdir $*)\)\.o[ :]*,$(@:%.d=%.o) $@: ,g' > $@

-include $(patsubst $(D_SRC)/%.c,$(D_BUILD)/%.d,$(shell find $(D_SRC)/ -name '*.c'))

check-syntax: $(EXECUTABLES:$(D_BIN)/%=$(D_BUILD)/%.o)


$(EXECUTABLES): $(D_BIN)/%: $(D_BUILD)/%.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST-EXECUTABLES): $(D_TESTS)/%: $(D_BUILD)/tests/%.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS-T)

# link executables / test-executables
$(D_BIN)/clash-of-particles: $(patsubst %,$(D_BUILD)/%.o,simulation event particle physics heap disc)
$(D_BIN)/snow: $(D_BUILD)/disc.o
$(D_TESTS)/heap-correctness: $(D_BUILD)/heap.o
$(D_TESTS)/heap-complexity:  $(D_BUILD)/heap.o
$(D_TESTS)/particle: $(patsubst %,$(D_BUILD)/%.o,particle physics)
$(D_TESTS)/loader:  $(patsubst %,$(D_BUILD)/%.o,simulation particle physics  event heap)


add-files-svn:
	svn add --force $(D_SRC)/*.c $(D_INCLUDE)/*.h $(D_DATA)/*.txt --auto-props --parents --depth infinity -q


clean:
	- rm -rf $(D_BUILD)/ *.csv fact.txt $(D_DATA)/complexity_heap.csv $(D_VALGRIND)/$(D_BUILD)/

mrproper: clean
	- rm -rf $(D_BIN)/ $(D_TESTS)/ $(D_DOC)/
