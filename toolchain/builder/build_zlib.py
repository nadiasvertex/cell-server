from builder.paths import toolchain_platform_dir, toolchain_src_dir

import os

__version__ = "1.2.7"
__pakname__ = "zlib-%s"
__depends__ = ["core-toolchain-*"]

def build(mgr, package_name, version, status):
   # Package is missing entirely.
   if status == "MISSING":
      url = 'http://zlib.net/zlib-%s.tar.bz2' % version
      archive = "zlib.tar.bz2"
      mgr.fetch(url, archive)

      extract_cmd = "tar -xjf %s --directory %s" % (archive, toolchain_src_dir)
      if mgr.run(extract_cmd) != 0:
         return False

      os.unlink(archive)
      mgr.set_status(package_name, "SOURCE")

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, package_name)
   cur_dir = os.getcwd()
   os.chdir(build_dir)

   try:
      configure_cmd = "./configure --prefix=%s --static --64" % \
          toolchain_platform_dir
      if mgr.run(configure_cmd) != 0:
         return False

      build_cmd = "make clean && make && make install"
      if mgr.run(build_cmd) != 0:
         return False
   finally:
      os.chdir(cur_dir)

   mgr.set_status(package_name, "BINARY")
   return True

