from builder.paths import toolchain_platform_dir, toolchain_src_dir
from builder.paths import toolchain_compiler,toolchain_compiler_c
from builder.paths import get_std_configure

import os 

__version__ = "3.0.2"
__pakname__ = "mono-%s"
__depends__ = ["core-toolchain-*", "glib-*"]

def build(mgr, package_name, version, status):
   # Package is missing entirely.
   if status == "MISSING":   
      url = 'http://download.mono-project.com/sources/mono/mono-%s.tar.bz2' % version
      mgr.fetch(url, "mono.tar.bz2")

      extract_cmd = "tar -xjf mono.tar.bz2 --directory %s" % toolchain_src_dir
      if mgr.run(extract_cmd)!=0:
         return False

      os.unlink("mono.tar.bz2")
      mgr.set_status(package_name, "SOURCE")  

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, package_name)
   cur_dir = os.getcwd()
   os.chdir(build_dir)

   try:
      configure_cmd = "./configure " + get_std_configure() +\
                      "--program-prefix=cell- " \
                      "--enable-silent-rules " \
                      "--without-gtk --without-gtk+ " \
                      "--without-gdiplus --with-mcs-docs=no"
      if mgr.run(configure_cmd)!=0:
         return False

      build_cmd = "make clean && make && make install"
      if mgr.run(build_cmd)!=0:
         return False
      
      ln_cmd = "ln %s/bin/cell-mono %s/bin/mono" %\
       (toolchain_platform_dir, toolchain_platform_dir)
      if mgr.run(ln_cmd)!=0:
         return False
   finally:
      os.chdir(cur_dir)

   mgr.set_status(package_name, "BINARY")
   return True

