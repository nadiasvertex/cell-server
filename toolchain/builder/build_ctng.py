from builder.paths import toolchain_dir, toolchain_src_dir

import os 
import urllib2

__version__ = "1.17.0"
__pakname__ = "crosstool-ng-%s"
__depends__ = []

def build(mgr, package_name, version, status):   
   # Package is missing entirely.
   if status == "MISSING":   
      url = 'http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-%s.tar.bz2' % version
      f = urllib2.urlopen(url)
      print ("Downloading %s" % url)
      with open("crosstool-ng.tar.bz2", "wb") as out:
         out.write(f.read())
      f.close()

      extract_cmd = "tar -xjf crosstool-ng.tar.bz2 --directory %s" % toolchain_src_dir
      if mgr.run(extract_cmd)!=0:
         return False

      os.unlink("crosstool-ng.tar.bz2")
      mgr.set_status(package_name, "SOURCE")  

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, package_name)
   cur_dir = os.getcwd()
   os.chdir(build_dir)

   try:
      configure_cmd = "./configure --prefix=%s" % toolchain_dir
      if mgr.run(configure_cmd)!=0:
         return False

      build_cmd = "make && make install"
      if mgr.run(build_cmd)!=0:
         return False
   finally:
      os.chdir(cur_dir)

   mgr.set_status(package_name, "BINARY")
   return True