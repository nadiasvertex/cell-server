# Builds the toolchain code and installs it in the toolchain environment.
import os
import sys
sys.path.append(os.path.join(os.getcwd(), "toolchain"))
from builder.manage import Manager

mgr = Manager()
mgr.build()

print("Finished")
