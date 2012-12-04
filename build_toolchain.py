# Builds the toolchain code and installs it in the toolchain environment.

import os
import shutil

toolchain_dir = os.path.join(os.path.get_cwd(), "toolchain")

def copy_tree(src, dst):
	for root, dirs, files in os.walk(src):
			for f in files:
				src_file = os.path.join(root, f)
				dst_file = src_file.replace(src, dst)
				shutil.copyfile(src_file, dst_file)
				
def install_pegtl():
   print("Installing pegtl")
	install_dir = os.path.join(toolchain_dir, "include")
	src_dir = os.path.join(toolchain_dir, "src", "pegtl-0.31", "include")
	copy_tree(src_dir, install_dir)
   
   
# Perform installation
install_pegtl()
	