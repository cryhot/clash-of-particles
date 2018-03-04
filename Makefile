_GUI = $(if $(NOGUI),,-D GUI)
CC = gcc

TARGETS = read-file write-fact snow
EXECUTABLES = $(TARGETS:%=bin/%)

DFLAGS = -I include/
CFLAGS = -g -std=c99 -Wall -Werror $(DFLAGS) $(_GUI)
LDFLAGS = -lm -lSDL

.DEFAULT_GOAL = bin/
.PHONY: clean mrproper doc bin/ $(TARGETS)


bin/: $(EXECUTABLES)
	@mkdir -p $@

$(TARGETS): %: bin/%

doc:
	doxygen conf/doxygen.conf
	@echo DOCUMENTATION GENERATED:
	@echo "    file://$(realpath doc/html/index.html)"


build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

build/%.d: src/%.c
	@mkdir -p $(dir $@)
	@set -e; rm -f $@; \
	$(CC) -MM $(DFLAGS) $< | \
	sed 's,\($(notdir $*)\)\.o[ :]*,$(@:%.d=%.o) $@: ,g' > $@

-include $(patsubst src/%.c,build/%.d,$(shell find src/ -name '*.c'))

check-syntax: $(EXECUTABLES:bin/%=build/%.o)


$(EXECUTABLES): bin/%: build/%.o
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bin/snow: build/disc.o


add-files-svn:
	svn add --force src/*.c include/*.h data/*.txt --auto-props --parents --depth infinity -q


clean:
	- rm -rf build/ *.csv fact.txt

mrproper: clean
	- rm -rf bin/ doc/
