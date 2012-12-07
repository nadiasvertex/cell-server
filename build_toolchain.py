# Builds the toolchain code and installs it in the toolchain environment.

import os
import pickle
import shutil
import sys
import urllib2

from glob import glob

# Various important paths
toolchain_dir = os.path.join(os.getcwd(), "toolchain")
toolchain_src_dir = os.path.join(toolchain_dir, "src")
package_db = os.path.join(toolchain_dir, "packages.db")
tool_status_d = {} if not os.path.exists(package_db) else pickle.load(open(package_db, "rb"))

# Add the toolchain path to our PATH.
os.environ["PATH"] += ":" + os.path.join(toolchain_dir, "bin")

def set_status(toolname, status):
   tool_status_d[toolname] = status
   with open(package_db, "wb") as out:
      pickle.dump(tool_status_d, out)

def get_status(toolname):
   return tool_status_d.get(toolname, "MISSING")

def run(cmd):
   print cmd
   return os.system(cmd)

def copy_tree(src, dst):
   for root, dirs, files in os.walk(src):
      for f in files:
         src_file = os.path.join(root, f)
         dst_file = src_file.replace(src, dst)
         dst_dir = os.path.dirname(dst_file)
         if not os.path.exists(dst_dir):
            os.makedirs(dst_dir)
         shutil.copyfile(src_file, dst_file)

def crosstool_ng():
   version = "1.17.0"
   package_name = "crosstool-ng-%s" % version
   status = get_status(package_name)

   # Package is present and compiled.
   if status == "BINARY":
      return True  

   # Package is missing entirely.
   if status == "MISSING":   
      url = 'http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-%s.tar.bz2' % version
      f = urllib2.urlopen(url)
      print ("Downloading %s" % url)
      with open("crosstool-ng.tar.bz2", "wb") as out:
         out.write(f.read())
      f.close()

      extract_cmd = "tar -xjf crosstool-ng.tar.bz2 --directory %s" % toolchain_src_dir
      if run(extract_cmd)!=0:
         return False

      os.unlink("crosstool-ng.tar.bz2")
      set_status(package_name, "SOURCE")  

   # Package is present in source form, and needs compiling
   build_dir = os.path.join(toolchain_src_dir, package_name)
   cur_dir = os.getcwd()
   os.chdir(build_dir)

   try:
      configure_cmd = "./configure --prefix=%s" % toolchain_dir
      if run(configure_cmd)!=0:
         return False

      build_cmd = "make && make install"
      if run(build_cmd)!=0:
         return False
   finally:
      os.chdir(cur_dir)

   set_status(package_name, "BINARY")
   return True
   
def core_toolchain():
   version = "1.0"
   package_name = "core-%s" % version
   status = get_status(package_name)

   # Package is present and compiled.
   if status == "BINARY":
      return True  

   # Package is missing entirely.
   if status == "MISSING":
      config_path = os.path.join(toolchain_dir, "cell.config")
      with open(config_path) as inp:
         data=inp.read()
      with open(config_path) as out:
         data=data.replace("{{toolchain_path}}", toolchain_dir)
         out.write(data)

      prep_cmd = "ct-ng oldconfig %s" % config_path
      if run(prep_cmd)!=0:
         return False

      set_status(package_name, "SOURCE")

   # Package is ready to build.
   build_cmd = "ct-ng build"
   if run(build_cmd)!=0:
      return False

   set_status(package_name, "BINARY")
				
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
   coco_binary = os.path.join(coco_dir, exec_name)
   shutil.copyfile(coc_binary, os.path.join(install_dir, exec_name))
   for filename in glob(os.path.join(coco_dir, "*.frame")):
      dst_file = filename.replace(coco_dir, install_dir)
      shutil.copyfile(filename, dst_file)   

   os.unlink(coco_binary)
   
# Perform installation
crosstool_ng()
core_toolchain()
#install_pegtl()
#install_coco()

print("Finished")
