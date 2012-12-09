from builder.paths import toolchain_platform_dir, toolchain_src_dir
from builder.paths import toolchain_compiler,toolchain_compiler_c

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
      if mgr.run(extract_cmd)!=0:
         return False

      os.unlink(archive)
      mgr.set_status(package_name, "SOURCE")  

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, package_name)
   cur_dir = os.getcwd()
   os.chdir(build_dir)

   try:
      os.environ["CC"] = toolchain_compiler_c
      os.environ["CFLAGS"] = "-I%s" % (os.path.join(toolchain_platform_dir, "include"))
      os.environ["LDFLAGS"] = "-L%s" % (os.path.join(toolchain_platform_dir, "lib"))
      configure_cmd = "./configure --prefix=%s" %\
          toolchain_platform_dir
      if mgr.run(configure_cmd)!=0:
         return False

      build_cmd = "make && make install"
      if mgr.run(build_cmd)!=0:
         return False
   finally:
      os.chdir(cur_dir)

   mgr.set_status(package_name, "BINARY")
   return True

