import os
import sys

# Where the toolchain is
toolchain_dir = os.path.join(os.getcwd(), "toolchain")

# Where the system-specific toolchain is
toolchain_platform_dir = os.path.join(toolchain_dir, sys.platform)

# Where toolchain source resides in the repository
toolchain_src_dir = os.path.join(toolchain_dir, "src")

# The c++ compiler for the toolchain
toolchain_compiler = os.path.join(toolchain_platform_dir, 
                                  "x86_64-cell-linux-gnu-g++")

# The database for package building.
package_db = os.path.join(toolchain_dir, "packages.db")

# Update the path to provide access to the bindir
os.environ["PATH"] +=":" + (":".join([os.path.join(toolchain_dir, "bin"),
                                      os.path.join(toolchain_platform_dir, "bin")]))
