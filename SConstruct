from glob import glob
import os
import sys

toolchain_path = os.path.join(os.getcwd(), "toolchain")

#libs = ['log4cxx', 'aprutil-1', 'expat', 'apr-1', 'zmq', 
#        'protobuf', 'jitplus', 'jit', 'pthread', 'rt']
libs = []

env = Environment(
    CPPPATH=['toolchain/include', 'src/lib', 'build', 'tests', '.'],
    tools=['default', 'protoc', 'mcs']
)

if sys.platform.startswith("win"):
	Tool("mingw")(env)	
	mono_path = "c:\\program files (x86)\\mono-3.0.1"
	mono_flags = "-mno-cygwin -mms-bitfields -mwindows"
	mono_include = os.path.join(mono_path, "include", "mono-2.0")
	mono_libs = ["mono-2.0", "ws2_32", "psapi", "ole32", "winmm", "oleaut32",
	             "advapi32", "version"]
				 
	env.Replace(CCFLAGS = []) # Fixes dumb mingw build bug
	env.Append(PATH=";" + os.path.join(toolchain_path, "bin"))
	env.Append(PATH=";" + os.path.join(mono_path, "bin"))
	env.Replace(CSC='call "%s"' % os.path.join(mono_path, "bin", "mcs.bat"))
else:
	mono_path = "/usr/local/mono"
	env.Append(PATH=":" + os.path.join(toolchain_path, "bin"))

env.Append(CXXFLAGS='-std=gnu++11 -g -I"' + mono_include + '"')
env.Append(LIBPATH=['.', os.path.join(toolchain_path, "lib"), 
                    os.path.join(mono_path, "lib")])

if os.environ.has_key('TERM'):
	env['ENV']['TERM'] = os.environ['TERM']

# proto = env.Protoc(
	   # [],
	   # glob("src/lib/edge/proto/*.proto") +\
       # glob("src/lib/processor/proto/*.proto")+\
       # glob("src/lib/cell/proto/*.proto"),
	   # PROTOCPROTOPATH=["src/lib"],
	   # PROTOCOUTDIR="build"
# )

# Query parser library
env.Library("sql", glob("src/lib/query/cpp/*.cpp") +\
                   glob("src/lib/query/coco/*.cpp"))

# Data engine library
env.Library("data_engine", glob("src/lib/data_engine/cpp/*.cpp"))
env.CLILibrary("data_engine_framework", glob("src/lib/data_engine/cs/*.cs"),
			   CILLIBS=["Mono.CSharp"])

# Plane library
# env.Library("plane", glob("src/lib/plane/cpp/*.cpp"))
# Cell library
# env.Library("cell", glob("build/cell/proto/*.cc")+ \
                    # glob("src/lib/cell/cpp/*.cpp"))
# Processor library
# env.Library("processor", glob("build/processor/proto/*.cc")+\
                         # glob("src/lib/processor/cpp/*.cpp"))

# Group server
# env.Program("group",
            # glob("src/group/*.cpp"),
       # LIBS=['plane', 'edge', 'cell', 'processor'] + libs
# )

# Unit tests
env.Program("cell_test",
            ["tests/gtest/gtest-all.cc",
            "tests/gtest/gtest_main.cc"] + glob("tests/*.cpp"),
	    LIBS=['data_engine'] + libs
)
