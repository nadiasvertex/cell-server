from copy import copy
from glob import glob
from fnmatch import fnmatch
from paths import package_db

import imp
import os
import pickle
import shutil

class Manager(object):
   def __init__(self):
      self.packages = self.load_packages(os.path.dirname(__file__))
      self.db = {} if not os.path.exists(package_db) \
                  else pickle.load(open(package_db, "rb"))
       
   def set_status(self, pkg_name, status):
      self.db[pkg_name] = status
      with open(package_db, "wb") as out:
         pickle.dump(self.db, out)

   def get_status(self, pkg_name):
      return self.db.get(pkg_name, "MISSING")
   
   def run(self, cmd):
      print cmd
      return os.system(cmd)
   
   def copy_tree(self, src, dst):
      for root, _, files in os.walk(src):
         for f in files:
            src_file = os.path.join(root, f)
            dst_file = src_file.replace(src, dst)
            dst_dir = os.path.dirname(dst_file)
            if not os.path.exists(dst_dir):
               os.makedirs(dst_dir)
            shutil.copyfile(src_file, dst_file)

       
   def build(self):      
      to_build = copy(self.packages)
      built = []
      drop = []
      
      def has_unmet_dependencies(deps):         
         for dep in deps:
            for pkg in built:               
               if fnmatch(pkg, dep):
                  break
            else:
               print "Need %s" % dep
               return True
         
         return False
         
      while to_build:         
         for k,v in to_build.iteritems():         
            name, version, depends, builder = v
            
            pkg_name = name % version
            print "Checking %s" % pkg_name
            
            if has_unmet_dependencies(depends):      
               print "Deferring %s" % pkg_name
               continue
            
            status = self.get_status(pkg_name)
            if status == "BINARY":
               built.append(pkg_name)
               continue
   
            print "Building %s" % pkg_name
            cur_dir = os.getcwd()
            try:         
               if builder(self, pkg_name, version, status)==False:
                  return False
            finally:
               os.chdir(cur_dir)
               
            built.append(pkg_name)
            drop.append(k)
            
         for k in drop:
            del to_build[k]
            
         if len(drop)==0:
            print "Unmet dependencies in toolchain, aborting."
            return False
         
         
   def load_packages(self, where):
      builders = glob(os.path.join(where, "build_*.py"))
      packages = {}
      for builder in builders:
         _, filename = os.path.split(builder)
         module_name, _ = os.path.splitext(filename)
         m_info = imp.find_module(module_name, [where])
         m = imp.load_module(module_name, *m_info)
         key = module_name.replace("build_", "")
               
         packages[key] = (m.__pakname__, m.__version__,
                          m.__depends__, m.build)
         
      return packages
               
         
         
            
    
    
