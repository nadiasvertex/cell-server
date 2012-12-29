from builder.paths import toolchain_platform_dir, toolchain_src_dir
from builder.paths import toolchain_compiler, toolchain_compiler_c
from builder.paths import get_std_configure

import os

__version__ = "3.2"
__pakname__ = "clang-%s"
__depends__ = []

def build(mgr, package_name, version, status):
   # Package is missing entirely.
   if status == "MISSING":
      url1 = 'http://llvm.org/releases/{version}/llvm-{version}.src.tar.gz'.format(version=version)
      url2 = 'http://llvm.org/releases/{version}/clang-{version}.src.tar.gz'.format(version=version)

      archive1 = "llvm.tar.gz"
      archive2 = "clang.tar.gz"

      mgr.fetch(url1, archive1)
      mgr.fetch(url2, archive2)

      extract_cmd = "tar -xzf %s --directory %s" % (archive1, toolchain_src_dir)
      if mgr.run(extract_cmd) != 0:
         return False

      extract_cmd = "tar -xzf {archive} --directory {src_dir}/llvm-{version}.src/tools && " \
                    "mv {src_dir}/llvm-{version}.src/tools/clang-{version}.src " \
                    "   {src_dir}/llvm-{version}.src/tools/clang".format(
        archive=archive2, src_dir=toolchain_src_dir, version=version
      )
      if mgr.run(extract_cmd) != 0:
         return False

      os.unlink(archive1)
      os.unlink(archive2)
      mgr.set_status(package_name, "SOURCE")

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, "llvm-%s" % version)
   if os.path.exists("%s.src" % build_dir):
       rn_cmd = "mv %s.src %s" % (build_dir, build_dir)
       if mgr.run(rn_cmd) != 0:
           return False

   build_from_dir = os.path.join(toolchain_src_dir, "llvm-build-%s" % version)
   cur_dir = os.getcwd()

   if not os.path.exists(build_from_dir):
       os.makedirs(build_from_dir)
   os.chdir(build_from_dir)

   try:
      configure_cmd = build_dir + "/configure " + \
                      get_std_configure(vars=False) + \
                      "--program-prefix=cell- --disable-docs "
      if mgr.run(configure_cmd) != 0:
         return False

      build_cmd = "make clean && make && make install"
      if mgr.run(build_cmd) != 0:
         return False
   finally:
      os.chdir(cur_dir)

   mgr.set_status(package_name, "BINARY")
   return True

