from builder.paths import toolchain_platform_dir, toolchain_src_dir
from builder.paths import toolchain_compiler,toolchain_compiler_c

import os 
import urllib2

__version__ = "3.0.9"
__pakname__ = "libffi-%s"
__depends__ = ["core-toolchain-*"]

def build(mgr, package_name, version, status):   
   # Package is missing entirely.
   if status == "MISSING":
      url = 'ftp://sourceware.org/pub/libffi/libffi-%s.tar.gz' % version
      mgr.fetch(url, "libffi.tar.gz")

      extract_cmd = "tar -xzf libffi.tar.gz --directory %s" % toolchain_src_dir
      if mgr.run(extract_cmd)!=0:
         return False

      os.unlink("libffi.tar.gz")
      mgr.set_status(package_name, "SOURCE")  

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, package_name)
   cur_dir = os.getcwd()
   os.chdir(build_dir)

   try:
      os.environ["CC"] = toolchain_compiler_c
      os.environ["CXX"] = toolchain_compiler
      configure_cmd = "./configure --prefix=%s" %\
          toolchain_platform_dir
      if mgr.run(configure_cmd)!=0:
         return False

      build_cmd = "make && make install"
      if mgr.run(build_cmd)!=0:
         return False
     
      include_src_dir = os.path.join(toolchain_platform_dir, "lib",
                                     package_name, "include")
      include_dst_dir = os.path.join(toolchain_platform_dir, "include")      
      mgr.copy_tree(include_src_dir, include_dst_dir)
      
   finally:
      os.chdir(cur_dir)

   mgr.set_status(package_name, "BINARY")
   return True

