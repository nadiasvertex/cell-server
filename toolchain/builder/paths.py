import os
import sys

# Where the toolchain is
toolchain_dir = os.path.join(os.getcwd(), "toolchain")

# Where the system-specific toolchain is
toolchain_platform_dir = os.path.join(toolchain_dir, sys.platform)

# Where toolchain source resides in the repository
toolchain_src_dir = os.path.join(toolchain_dir, "src")

# The c++ compiler for the toolchain
toolchain_compiler = os.path.join(toolchain_platform_dir, "bin",
                                  "x86_64-cell-linux-gnu-g++")

# The c compiler for the toolchain
toolchain_compiler_c = os.path.join(toolchain_platform_dir, "bin",
                                    "x86_64-cell-linux-gnu-gcc")

# The preprocessor for the toolchain
toolchain_preprocessor = os.path.join(toolchain_platform_dir, "bin",
                                    "x86_64-cell-linux-gnu-cpp")

# The linker to use.
toolchain_linker = os.path.join(toolchain_platform_dir, "bin",
                                    "x86_64-cell-linux-gnu-ld")
# Where to look for libraries
toolchain_lib_dir = os.path.join(toolchain_platform_dir, "lib")

# Where to look for libraries
toolchain_include_dir = os.path.join(toolchain_platform_dir, "include")

# The database for package building.
package_db = os.path.join(toolchain_dir, "packages.db")

# Update the path to provide access to the bindir
os.environ["PATH"] += ":" + (":".join([os.path.join(toolchain_dir, "bin"),
                                      os.path.join(toolchain_platform_dir, "bin")]))

def set_build_paths():
   os.environ["CC"] = toolchain_compiler_c
   os.environ["CXX"] = toolchain_compiler
   os.environ["CPP"] = toolchain_preprocessor
   os.environ["LD"] = toolchain_linker
   os.environ["CFLAGS"] = "-I%s " % toolchain_include_dir
   os.environ["CXXFLAGS"] = "-I%s " % toolchain_include_dir
   os.environ["LDFLAGS"] = "-L{lib_path} -Wl,-rpath,{lib_path} ".format(
       lib_path=toolchain_lib_dir
   )

def get_std_configure(vars=True):
   config = " CC=%s CXX=%s CPP=%s LD=%s CFLAGS=%s CXXFLAGS=%s LDFLAGS=%s " % \
               (toolchain_compiler_c,
                toolchain_compiler,
                toolchain_preprocessor,
                toolchain_linker,
                "'-I%s'" % toolchain_include_dir,
                "'-I%s'" % toolchain_include_dir,
                "'-L{lib_path} -Wl,-rpath,{lib_path}'".format(lib_path=toolchain_lib_dir)) \
   if vars else ""

   return  config + \
           ("--prefix=%s --disable-dependency-tracking " \
            "--disable-shared --enable-static " % \
                   toolchain_platform_dir)
