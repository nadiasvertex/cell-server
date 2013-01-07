from builder.paths import toolchain_dir, toolchain_platform_dir

import os

__version__ = "1.0"
__pakname__ = "core-toolchain-%s"
__depends__ = ["crosstool-ng-*", "clang-*"]

def build(mgr, package_name, version, status):
   # Package is missing entirely.
   if status == "MISSING":
      config_path = os.path.join(toolchain_dir, "cell.config")
      with open(config_path) as inp:
         data = inp.read()
      with open(".config", "w") as out:
         data = data.replace("{{toolchain_path}}", toolchain_platform_dir)
         out.write(data)

      mgr.set_status(package_name, "SOURCE")

   # Package is ready to build.
#   build_cmd = "ct-ng build"
#   if mgr.run(build_cmd) != 0:
#      return False
#
#   rw_cmd = "chmod -R +w %s" % toolchain_platform_dir
#   if mgr.run(rw_cmd) != 0:
#      return False

   mgr.set_status(package_name, "BINARY")
   return True
