# Builds the toolchain code and installs it in the toolchain environment.

import os
import shutil

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
   coco_dir = os.path.join(toolchain_dir, "src", "coco_cpp")
   install_dir = os.path.join(toolchain_dir, "bin")
   cur_dir = os.getcwd()
   os.chdir(coco_dir)
   exec_name = "coco" if not sys.platform.startswith("win") else "coco.exe"
   
   if os.system("g++ *.cpp -o %s" % exec_name)!=0:
      sys.exit(1)
      
   shutil.copyfile(os.path.join(coco_dir, exec_name), install_dir)
   for filename in glob(os.path.join(coco_dir, "*.frame")):
      shutil.copyfile(filename, install_dir)   
   
# Perform installation
install_pegtl()
install_coco()

print("Finished")