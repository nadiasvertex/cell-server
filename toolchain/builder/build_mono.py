from builder.paths import toolchain_platform_dir, toolchain_src_dir
from builder.paths import toolchain_compiler,toolchain_compiler_c

import os 
import urllib2

__version__ = "3.0.2"
__pakname__ = "mono-%s"
__depends__ = ["core-toolchain-*"]

def build(mgr, package_name, version, status):   
   # Package is missing entirely.
   if status == "MISSING":   
      url = 'http://download.mono-project.com/sources/mono/mono-%s.tar.bz2' % version
      f = urllib2.urlopen(url)
      print ("Downloading %s" % url)
      with open("mono.tar.bz2", "wb") as out:
         out.write(f.read())
      f.close()

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
      os.environ["CC"] = toolchain_compiler_c
      os.environ["CXX"] = toolchain_compiler      
      configure_cmd = "./configure --prefix=%s --program-prefix=cell- " \
                      "--enable-silent-rules --disable-dependency-tracking " \
                      "--disable-shared --with-mcs-docs=no" %\
          toolchain_platform_dir
      if mgr.run(configure_cmd)!=0:
         return False

      build_cmd = "make && make install"
      if mgr.run(build_cmd)!=0:
         return False
   finally:
      os.chdir(cur_dir)

   #mgr.set_status(package_name, "BINARY")
   return True

