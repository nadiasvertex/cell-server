from builder.paths import toolchain_platform_dir, toolchain_src_dir
from builder.paths import get_std_configure

import os

__version__ = "3.0.9"
__pakname__ = "libffi-%s"
__depends__ = ["core-toolchain-*"]

def build(mgr, package_name, version, status):
   # Package is missing entirely.
   if status == "MISSING":
      url = 'ftp://sourceware.org/pub/libffi/libffi-%s.tar.gz' % version
      archive = "libffi.tar.gz"
      mgr.fetch(url, archive)

      extract_cmd = "tar -xzf %s --directory %s" % (archive, toolchain_src_dir)
      if mgr.run(extract_cmd) != 0:
         return False

      os.unlink(archive)
      mgr.set_status(package_name, "SOURCE")

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, package_name)
   cur_dir = os.getcwd()
   os.chdir(build_dir)

   try:
      configure_cmd = "./configure " + get_std_configure(compiler="gcc")
      if mgr.run(configure_cmd) != 0:
         return False

      build_cmd = "make clean && make && make install"
      if mgr.run(build_cmd) != 0:
         return False

      include_src_dir = os.path.join(toolchain_platform_dir, "lib",
                                     package_name, "include")
      include_dst_dir = os.path.join(toolchain_platform_dir, "include")
      mgr.copy_tree(include_src_dir, include_dst_dir)

   finally:
      os.chdir(cur_dir)

   mgr.set_status(package_name, "BINARY")
   return True

