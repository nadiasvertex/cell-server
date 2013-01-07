from glob import glob
from builder.paths import toolchain_dir, get_compiler_options

import os
import shutil
import sys

__version__ = "1.0"
__pakname__ = "coco-%s"
__depends__ = ["core-toolchain-*"]

def build(mgr, package_name, version, status):
   coco_dir = os.path.join(toolchain_dir, "src", "coco_cpp")
   install_dir = os.path.join(toolchain_dir, "bin")
   if not os.path.exists(install_dir):
      os.makedirs(install_dir)

   os.chdir(coco_dir)
   exec_name = "coco" if not sys.platform.startswith("win") else "coco.exe"

   d = get_compiler_options()
   if os.system("%s %s *.cpp -o %s" % (d["CXX"], d["CXXFLAGS"], exec_name)) != 0:
      sys.exit(1)

   print "Installing coco"
   coco_binary = os.path.join(coco_dir, exec_name)
   shutil.copyfile(coco_binary, os.path.join(install_dir, exec_name))
   for filename in glob(os.path.join(coco_dir, "*.frame")):
      dst_file = filename.replace(coco_dir, install_dir)
      shutil.copyfile(filename, dst_file)

   os.unlink(coco_binary)
   mgr.set_status(package_name, "BINARY")
   return True
