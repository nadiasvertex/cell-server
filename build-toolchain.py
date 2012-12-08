# Builds the toolchain code and installs it in the toolchain environment.
import argparse
import os
import sys
sys.path.append(os.path.join(os.getcwd(), "toolchain"))
from builder.manage import Manager

import argparse

parser = argparse.ArgumentParser(description='Build the cell server toolchain')
parser.add_argument('--list', dest='list_pkgs', action='store_true',
                   default=False,
                   help='List the available packages')
parser.add_argument('--rebuild', dest='rebuild_pkgs', action='store',
                   default="",
                   help='Forces a rebuild of the specified packages.')
parser.add_argument('--build', dest='build_pkgs', action='store',
                   default="",
                   help='Build all packages (the default), or specify a '\
                        'comma separated list of packages to build. If a '\
                        'package depends on another package, it will be '\
                        'selected automatically.')


args = parser.parse_args()

mgr = Manager()

if args.list_pkgs:
   print "="*77
   print "Name".center(20), "Version".center(15), "Dependencies".center(40)
   print "=" * 20, "="*15, "="*40
   for k, v in mgr.packages.iteritems():
      print k.rjust(20), v[1].ljust(15),
      print ",".join(v[2]).rjust(40)
   sys.exit(0)
   
if args.rebuild_pkgs:
   pkg_names = [x.strip() for x in args.rebuild_pkgs.split(",")]
   for pkg_name in pkg_names:
      mgr.set_status(pkg_name, "MISSING")
   
if mgr.build(args.build_pkgs)==False:
   sys.exit(1)

print("Finished")
