import os
import sys

# Where the toolchain is
toolchain_dir = os.path.join(os.getcwd(), "toolchain")

# Where the system-specific toolchain is
toolchain_platform_dir = os.path.join(toolchain_dir, sys.platform)

# Where toolchain source resides in the repository
toolchain_src_dir = os.path.join(toolchain_dir, "src")

# The linker to use.
# toolchain_linker = os.path.join(toolchain_platform_dir, "bin",
#                                 "x86_64-cell-linux-gnu-ld")

# Where to look for libraries
toolchain_lib_dir = os.path.join(toolchain_platform_dir, "lib")

# Where to look for libraries
toolchain_include_dir = os.path.join(toolchain_platform_dir, "include")

# The database for package building.
package_db = os.path.join(toolchain_dir, "packages.db")

# Update the path to provide access to the bindir
os.environ["PATH"] += ":" + (":".join([os.path.join(toolchain_dir, "bin"),
                                      os.path.join(toolchain_platform_dir, "bin")]))

def get_compiler_options(which="clang"):
   d = {}
   if which == "clang":
      d["CXX"] = os.path.join(toolchain_platform_dir, "bin",
                              "cell-clang++")
      d["CC"] = os.path.join(toolchain_platform_dir, "bin",
                              "cell-clang")
      d["CFLAGS"] = "-I%s -target x86_64-cell-linux" % toolchain_include_dir
      d["CXXFLAGS"] = d["CFLAGS"]
   else:
      d["CXX"] = "g++"
      d["CC"] = "gcc"
      d["CFLAGS"] = "-I%s" % toolchain_include_dir
      d["CXXFLAGS"] = d["CFLAGS"]

   d["CXXCPP"] = "/usr/bin/cpp"
   d["CPP"] = "/usr/bin/cpp"
   d["LD"] = "ld"
   d["LDFLAGS"] = "-L{lib_path} -Wl,-rpath,{lib_path} ".format(
       lib_path=toolchain_lib_dir
   )
   return d

def set_build_paths():
   d = get_compiler_options("gcc")
   for k, v in d.iteritems():
      os.environ[k] = v

def get_std_configure(env_vars=True, compiler="clang"):
   config = ""
   if env_vars:
      d = get_compiler_options(compiler)
      for k, v in d.iteritems():
         config += ("%s='%s' " % (k, v))

   return  config + \
           ("--prefix=%s --disable-dependency-tracking " \
            "--disable-shared --enable-static " % \
                   toolchain_platform_dir)
