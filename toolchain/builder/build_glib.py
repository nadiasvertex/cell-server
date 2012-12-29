from builder.paths import toolchain_src_dir
from builder.paths import toolchain_include_dir, toolchain_lib_dir
from builder.paths import get_std_configure

import os

__version__ = "2.34.3"
__pakname__ = "glib-%s"
__depends__ = ["core-toolchain-*", "libffi-*", "zlib-*"]

def build(mgr, package_name, version, status):
   # Package is missing entirely.
   if status == "MISSING":
      url = 'http://ftp.gnome.org/pub/gnome/sources/glib/2.34/glib-%s.tar.xz' % version
      archive = "glib.tar.xz"
      mgr.fetch(url, archive)

      extract_cmd = "tar -xJf %s --directory %s" % (archive, toolchain_src_dir)
      if mgr.run(extract_cmd) != 0:
         return False

      os.unlink(archive)
      mgr.set_status(package_name, "SOURCE")

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, package_name)
   cur_dir = os.getcwd()
   os.chdir(build_dir)

   ffi = "'LIBFFI_CFLAGS=-I%s' 'LIBFFI_LIBS=-L%s -lffi' " % \
         (toolchain_include_dir, toolchain_lib_dir)
   zlib = "'ZLIB_CFLAGS=-I%s' 'ZLIB_LIBS=-L%s -lz' " % \
         (toolchain_include_dir, toolchain_lib_dir)

   try:
      configure_cmd = "./configure " + \
                      ffi + zlib + \
                      get_std_configure(compiler="gcc")
      if mgr.run(configure_cmd) != 0:
         return False

      build_cmd = "make clean && make && make install"
      if mgr.run(build_cmd) != 0:
         return False
   finally:
      os.chdir(cur_dir)

   mgr.set_status(package_name, "BINARY")
   return True

