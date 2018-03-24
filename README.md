
# Main program
#### SYNOPSIS

<code>bin/clash-of-particles [_SOURCE_] [_DURATION_]</code>

#### OPTIONS
_`SOURCE`_: _`source-file`_ | `-` | _`number-of-generated-particles`_ (default `-`: read from stdin)  
_`DURATION`_: _`n`_ | `inf` | `-inf` (default `inf`: no limit)  

# Informations

I do not call pointer p_dummy because I find this practice stupid.

Vertical & horizontal processed as same (see `event` documentation). It was necessary to extend the calculation to more than 2 dimensions. Actually, it is totally possible to extends the number of dimension, just redefine `NB_DIM` in `physics.h`. The only limit will be the display.

No need to update every particle at each event, thus big sums of floating point numbers and precision loss are avoided. This performance is achieved by memorizing a timestamp for every particle.

For additional informations, see the doxygen documentation (`make doc`).


# File structure
```
    project/                    
     ├─Makefile                 
     ├─README.md                
     ├─src/                     source files
     │  ├─*.c                   
     │  └─tests/                source files for tests
     │     └─*.c                
     ├─include/                 headers - same file structure as src/
     │  ├─*.h                   
     │  └─tests/                
     │     └─*.h                
     ├─data/                    
     ├─scripts/                 
     ├─conf/                    
     │  └─<config files>        
     ├─doc/                     (generated)
     │  ├─html/index.html       
     │  └─...       
     ├─build/                   (generated) intermediate files - same file structure as src/
     │  ├─*.o                   object files
     │  ├─*.d                   dependency files
     │  └─tests/                
     │     ├─*.o                
     │     └─*.d                
     ├─bin/                     (generated)
     │  └─<executables>         
     ├─tests/                   (generated)
     │  └─<test-executables>
     └─valgrind/                (generated) independent directory with compiled files ready for debug
        ├─build/
        ├─bin/
        └─tests/
```
# Makefile targets
Here are some useful target (do not hesitate to use command line `make` auto-completion):
### compilation
- `compile-all`: compile everything (bin+tests)
- `bin/`: compile every executables
- `tests/`: compile every test executables
- `compile-%`: compile an executable (generated in bin/)
- `compile-test-%`: compile a test executable (generated in tests/)

### execution
- `run-%`: run correctly an executable. For example:
  - `run-clash-of-particles` (default file: `data/newton-simple.txt`)
  - `run-clash-of-particles-random` (default particle quantity: `1000`)
  - `run-particles-break-dance` (demo for back in time calculation)
  - `run-snow`
- `valgrind-%`: run correctly an executable using `valgrind`.

### tests
- `test-%`: run correctly a test. For example:
  - `test-heap-correctness`
  - `test-heap-complexity`
  - `test-particle`
  - `test-loader`
- `valgrind-test-%`: run correctly a test using `valgrind`.

### other
- `doc`: gerenate the doxygen documentation
- `clean`: remove intermediate files
- `mrproper`: make repertory proper - delete every generated file
