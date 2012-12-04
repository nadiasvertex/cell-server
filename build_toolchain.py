# Builds the toolchain code and installs it in the toolchain environment.

import os
import shutil
import sys

from glob import glob

toolchain_dir = os.path.join(os.getcwd(), "toolchain")

def copy_tree(src, dst):
   for root, dirs, files in os.walk(src):
      for f in files:
         src_file = os.path.join(root, f)
         dst_file = src_file.replace(src, dst)
         dst_dir = os.path.dirname(dst_file)
         if not os.path.exists(dst_dir):
            os.makedirs(dst_dir)
         shutil.copyfile(src_file, dst_file)
				
def install_pegtl():
   print("Installing pegtl")
   install_dir = os.path.join(toolchain_dir, "include")
   src_dir = os.path.join(toolchain_dir, "src", "pegtl-0.31", "include")
   copy_tree(src_dir, install_dir)
   
def install_coco():
   print "Building coco"
   coco_dir = os.path.join(toolchain_dir, "src", "coco_cpp")
   install_dir = os.path.join(toolchain_dir, "bin")
   if not os.path.exists(install_dir):
      os.makedirs(install_dir)
   cur_dir = os.getcwd()
   os.chdir(coco_dir)
   exec_name = "coco" if not sys.platform.startswith("win") else "coco.exe"
   
   if os.system("g++ *.cpp -o %s" % exec_name)!=0:
      sys.exit(1)
      
   print "Installing coco"
   shutil.copyfile(os.path.join(coco_dir, exec_name), os.path.join(install_dir, exec_name))
   for filename in glob(os.path.join(coco_dir, "*.frame")):
      dst_file = filename.replace(coco_dir, install_dir)
      shutil.copyfile(filename, dst_file)   
   
# Perform installation
install_pegtl()
install_coco()

print("Finished")