# Introduction #

Cell is a complex system. Building is designed to be very straightforward and automated. The initial setup can take quite a bit of computer time, but should be very straightforward.

The page walks through getting the source and building the system.

## Rationale ##

You might wonder why we don't just use whatever stock environment is on your local build system. The reason is that complex systems require many different pieces. The smaller the tolerances between the pieces, the easier it is for something to break.

This can especially be a problem while building the software because your native compiler may not support certain features we require, or you may not have the correct version of some library. Getting the right version onto your system may be hard, or may break other software you need.

Providing a complete toolchain, from compiler to libraries, allows the system to avoid making assumptions and act on specific knowledge. This also reduces frustrations when building and testing, because we don't have to wonder exactly what is supported. Finally, it improves the stability of the output because you know exactly what went into the end product.



# Getting the Source #

The repository uses Mercurial. Currently there is no set of stable tags.

```
hg clone https://code.google.com/p/cell-server/
```

# Building the Source #

There are steps which need to be run once, and steps which you will run over and over.

To build, you will need to make sure that you have a recent version of scons and python installed on your system. Other than that, the first time build will continue to prompt you to install packages which might be missing on your system.

## First Time ##

If you have an Ubuntu system, you can reduce some irritation by running:

```
sudo apt-get install texinfo gawk bison flex ncurses-dev automake libtool gperf
```

On Debian Wheezy:

```
sudo apt-get install texinfo gawk bison flex ncurses-dev automake libtool gperf gettext expat
```

On openSuSE:

```
sudo zypper install texinfo bison flex automake gperf patch
```

On Arch Linux:

```
sudo pacman -S bison flex automake autoconf libtool gperf expat gettext wget patch make
```


Those dependencies are needed by the toolchain builder, and it will continue to prompt you to install them until you do.

To start the toolchain build:

```
python build-toolchain.py
```

This tool will download sources from various places and build the toolchain for you automatically. This takes quite a bit of time.

Note that the toolchain builder will continue to abort if it finds missing dependencies, so you may need to run the builder several times in order to get it to complete successfully. The good news is that the builder keeps track of its progress, so it only needs to redo failed steps.

If for some reason you want to rebuild a particular dependency you can run:

```
python build-toolchain.py --rebuild=<pkgname>
```

You can see the list of available packages by running:

```
python build-toolchain.py --list
```

The output of "--list" might look like this:
```
cnelson@tdvm:/store/workspace/cell-server$ python build-toolchain.py --list
=============================================================================
        Name             Version                   Dependencies              
==================== =============== ========================================
                coco 1.0                                     core-toolchain-*
                ctng 1.17.0                                                  
                core 1.0                                       crosstool-ng-*
```

An example of expected output of a full build:

```
cnelson@tdvm:/store/workspace/cell-server$ python build-toolchain.py 
Checking coco-1.0
Need core-toolchain-*
Deferring coco-1.0
Checking crosstool-ng-1.17.0
Building crosstool-ng-1.17.0
Downloading http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-1.17.0.tar.bz2
tar -xjf crosstool-ng.tar.bz2 --directory /store/workspace/cell-server/toolchain/src
./configure --prefix=/store/workspace/cell-server/toolchain
checking build system type... x86_64-unknown-linux-gnu
checking host system type... x86_64-unknown-linux-gnu
checking for a BSD-compatible install... /usr/bin/install -c
checking for grep that handles long lines and -e... /bin/grep
checking for egrep... /bin/grep -E
checking for a sed that does not truncate output... /bin/sed
checking whether sed understands -r -i -e... yes
checking whether ln -s works... yes
checking for gcc... gcc
checking whether the C compiler works... yes
checking for C compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether we are using the GNU C compiler... yes
checking whether gcc accepts -g... yes
checking for gcc option to accept ISO C89... none needed
checking how to run the C preprocessor... gcc -E
checking for ranlib... ranlib
checking for objcopy... objcopy
checking for absolute path to objcopy... /usr/bin/objcopy
checking for objdump... objdump
checking for absolute path to objdump... /usr/bin/objdump
checking for readelf... readelf
checking for absolute path to readelf... /usr/bin/readelf
checking for bison... bison
checking for flex... flex
checking for gperf... gperf
checking for makeinfo... makeinfo
checking for cut... cut
checking for stat... stat
checking for readlink... readlink
checking for wget... wget
checking for tar... tar
checking for gzip... gzip
checking for bzip2... bzip2
checking for patch... /usr/bin/patch
checking for bash >= 3.1... /bin/bash
checking for GNU awk... /usr/bin/awk
checking for GNU make >= 3.80... /usr/bin/make
checking whether /usr/bin/make sets $(MAKE)... yes
checking for GNU libtool >= 1.5.26... /usr/bin/libtool
checking for GNU libtoolize >= 1.5.26... /usr/bin/libtoolize
checking for GNU automake >= 1.10... /usr/bin/automake
checking for xz... xz
checking for cvs... no
checking for svn... svn
checking for inline... inline
checking for ANSI C header files... yes
checking for sys/types.h... yes
checking for sys/stat.h... yes
checking for stdlib.h... yes
checking for string.h... yes
checking for memory.h... yes
checking for strings.h... yes
checking for inttypes.h... yes
checking for stdint.h... yes
checking for unistd.h... yes
checking for stdlib.h... (cached) yes
checking for GNU libc compatible malloc... yes
checking for stdlib.h... (cached) yes
checking for GNU libc compatible realloc... yes
checking for working alloca.h... yes
checking for alloca... yes
checking libintl.h usability... yes
checking libintl.h presence... yes
checking for libintl.h... yes
checking whether gettext is declared... yes
checking ncurses/ncurses.h usability... no
checking ncurses/ncurses.h presence... no
checking for ncurses/ncurses.h... no
checking ncurses/curses.h usability... no
checking ncurses/curses.h presence... no
checking for ncurses/curses.h... no
checking ncursesw/curses.h usability... no
checking ncursesw/curses.h presence... no
checking for ncursesw/curses.h... no
checking ncurses.h usability... yes
checking ncurses.h presence... yes
checking for ncurses.h... yes
checking for library containing initscr... -lncurses
configure: creating ./config.status
config.status: creating Makefile
make && make install
  SED    'ct-ng'
  SED    'scripts/crosstool-NG.sh'
  SED    'scripts/saveSample.sh'
  SED    'scripts/showTuple.sh'
  GEN    'config/configure.in'
  GEN    'paths.mk'
  GEN    'paths.sh'
  SED    'docs/ct-ng.1'
  GZIP   'docs/ct-ng.1.gz'
  GEN    'config/configure.in'
  GEN    'paths.mk'
  GEN    'paths.sh'
  INST    'ct-ng'
  RMDIR   '/store/workspace/cell-server/toolchain/lib/ct-ng.1.17.0/'
  MKDIR   '/store/workspace/cell-server/toolchain/lib/ct-ng.1.17.0/'
  INSTDIR 'config/'
  INSTDIR 'contrib/'
  INSTDIR 'patches/'
  INSTDIR 'scripts/'
  INST    'steps.mk'
  INST    'paths'
  INSTDIR 'samples/'
  INST    'kconfig/'
  INST    'docs/*.txt'
  INST    'ct-ng.1.gz'

For auto-completion, do not forget to install 'ct-ng.comp' into
your bash completion directory (usually /etc/bash_completion.d)
Checking core-toolchain-1.0
Building core-toolchain-1.0
ct-ng build
[INFO ]  Performing some trivial sanity checks
[INFO ]  Build started 20121207.203832
[INFO ]  Building environment variables
[WARN ]  Directory '/home/cnelson/src' does not exist.
[WARN ]  Will not save downloaded tarballs to local storage.
[EXTRA]  Preparing working directories
[EXTRA]  Installing user-supplied crosstool-NG configuration
[EXTRA]  =================================================================
[EXTRA]  Dumping internal crosstool-NG configuration
[EXTRA]    Building a toolchain for:
[EXTRA]      build  = x86_64-unknown-linux-gnu
[EXTRA]      host   = x86_64-unknown-linux-gnu
[EXTRA]      target = x86_64-cell-linux-gnu
[EXTRA]  Dumping internal crosstool-NG configuration: done in 0.06s (at 00:02)
[INFO ]  =================================================================
[INFO ]  Retrieving needed toolchain components' tarballs
[INFO ]  Retrieving needed toolchain components' tarballs: done in 0.06s (at 00:02)
[INFO ]  =================================================================
[INFO ]  Extracting and patching toolchain components
[INFO ]  Extracting and patching toolchain components: done in 2.21s (at 00:05)
[INFO ]  =================================================================
[INFO ]  Installing GMP for host
[EXTRA]    Configuring GMP
[EXTRA]    Building GMP
[EXTRA]    Installing GMP
[INFO ]  Installing GMP for host: done in 28.02s (at 00:33)
[INFO ]  =================================================================
[INFO ]  Installing MPFR for host
[EXTRA]    Configuring MPFR
[EXTRA]    Building MPFR
[EXTRA]    Installing MPFR
[INFO ]  Installing MPFR for host: done in 10.70s (at 00:43)
[INFO ]  =================================================================
[INFO ]  Installing PPL for host
[EXTRA]    Configuring PPL
[EXTRA]    Building PPL
[EXTRA]    Installing PPL
[INFO ]  Installing PPL for host: done in 106.00s (at 02:29)
[INFO ]  =================================================================
[INFO ]  Installing CLooG/PPL for host
[EXTRA]    Configuring CLooG/ppl
[EXTRA]    Building CLooG/ppl
[EXTRA]    Installing CLooG/ppl
[INFO ]  Installing CLooG/PPL for host: done in 5.77s (at 02:35)
[INFO ]  =================================================================
[INFO ]  Installing MPC for host
[EXTRA]    Configuring MPC
[EXTRA]    Building MPC
[EXTRA]    Installing MPC
[INFO ]  Installing MPC for host: done in 6.36s (at 02:42)
[INFO ]  =================================================================
[INFO ]  Installing binutils for host
[EXTRA]    Configuring binutils
[EXTRA]    Prepare binutils for static build
[EXTRA]    Building binutils
[EXTRA]    Installing binutils
[INFO ]  Installing binutils for host: done in 30.36s (at 03:12)
[INFO ]  =================================================================
[INFO ]  Installing pass-1 core C compiler
[EXTRA]    Configuring core C compiler
[EXTRA]    Building core C compiler
[EXTRA]    Installing core C compiler
[INFO ]  Installing pass-1 core C compiler: done in 74.74s (at 04:27)
[INFO ]  =================================================================
[INFO ]  Installing kernel headers
[EXTRA]    Installing kernel headers
[EXTRA]    Checking installed headers
[INFO ]  Installing kernel headers: done in 7.42s (at 04:34)
[INFO ]  =================================================================
[INFO ]  Installing C library headers & start files
[EXTRA]    Configuring C library
[EXTRA]    Installing C library headers
[EXTRA]    Installing C library start files
[INFO ]  Installing C library headers & start files: done in 33.19s (at 05:07)
[INFO ]  =================================================================
[INFO ]  Installing pass-2 core C compiler
[EXTRA]    Configuring core C compiler
[EXTRA]    Building core C compiler
[EXTRA]    Installing core C compiler
[INFO ]  Installing pass-2 core C compiler: done in 105.64s (at 06:53)
[INFO ]  =================================================================
[INFO ]  Installing C library
[EXTRA]    Configuring C library
[EXTRA]    Building C library
[EXTRA]    Installing C library
[INFO ]  Installing C library: done in 299.56s (at 11:53)
[INFO ]  =================================================================
[INFO ]  Installing final compiler
[EXTRA]    Configuring final compiler
[EXTRA]    Building final compiler
[EXTRA]    Installing final compiler
[INFO ]  Installing final compiler: done in 182.55s (at 14:55)
[INFO ]  =================================================================
[INFO ]  Installing binutils for target
[EXTRA]    Configuring binutils for target
[EXTRA]    Building binutils' libraries (libiberty bfd) for target
[EXTRA]    Installing binutils' libraries (libiberty bfd) for target
[INFO ]  Installing binutils for target: done in 53.94s (at 15:49)
[INFO ]  =================================================================
[INFO ]  Installing dmalloc
[EXTRA]    Configuring dmalloc
[EXTRA]    Building dmalloc
[EXTRA]    Installing dmalloc
[INFO ]  Installing dmalloc: done in 12.14s (at 16:01)
[INFO ]  =================================================================
[INFO ]  Installing D.U.M.A.
[EXTRA]    Copying sources
[EXTRA]    Building libraries 'libduma.a libduma.so.0.0.0'
[EXTRA]    Installing libraries 'libduma.a libduma.so.0.0.0'
[EXTRA]    Installing shared library link
[EXTRA]    Installing wrapper script
[INFO ]  Installing D.U.M.A.: done in 1.37s (at 16:03)
[INFO ]  =================================================================
[INFO ]  Installing cross-gdb
[EXTRA]    Configuring cross-gdb
[EXTRA]    Building cross-gdb
[EXTRA]    Installing cross-gdb
[INFO ]  Installing cross-gdb: done in 83.47s (at 17:26)
[INFO ]  =================================================================
[INFO ]  Installing native gdb
[EXTRA]    Building static target ncurses
[EXTRA]    Building static target expat
[EXTRA]    Configuring native gdb
[EXTRA]    Building native gdb
[EXTRA]    Installing native gdb
[INFO ]  Installing native gdb: done in 151.71s (at 19:58)
[INFO ]  =================================================================
[INFO ]  Installing gdbserver
[EXTRA]    Configuring gdbserver
[EXTRA]    Building gdbserver
[EXTRA]    Installing gdbserver
[INFO ]  Installing gdbserver: done in 6.83s (at 20:05)
[INFO ]  =================================================================
[INFO ]  Cleaning-up the toolchain's directory
[INFO ]    Stripping all toolchain executables
[EXTRA]    Installing the populate helper
[EXTRA]    Installing a cross-ldd helper
[EXTRA]    Creating toolchain aliases
[EXTRA]    Removing access to the build system tools
[EXTRA]    Removing installed documentation
[INFO ]  Cleaning-up the toolchain's directory: done in 1.33s (at 20:06)
[INFO ]  Build completed at 20121207.205838
[INFO ]  (elapsed: 20:06.21)
[INFO ]  Finishing installation (may take a few seconds)...
[20:06] / Checking coco-1.0
Building coco-1.0
Installing coco
Finished
```