from glob import glob
import os

import settings

toolchain_path = settings.toolchain_dir
compiler = settings.clang

# libs = ['log4cxx', 'aprutil-1', 'expat', 'apr-1', 'zmq',
#        'protobuf', 'jitplus', 'jit', 'pthread', 'rt']
libs = ["python3.3m", "pthread"]

env = Environment(
    CPPPATH=[settings.toolchain_include_dir,
             os.path.join(settings.toolchain_include_dir, "mono-2.0"),
             'src/lib', 'build', 'tests', '.'],
    tools=['default', 'protoc', 'mcs']
)

mono_path = os.path.join(settings.toolchain_platform_dir, "bin", "cell-mono")

env.Append(PATH=":" + (":".join(settings.toolchain_bin_dirs)))
env.Append(CXXFLAGS='-std=c++11 -g ' + compiler["CXXFLAGS"])
env.Append(CPPPATH='/usr/include/python3.3m')
env.Append(LIBPATH=['.', settings.toolchain_lib_dir,
                    '/usr/include/python3.3/config-3.3m'])
env.Replace(CXX=compiler["CXX"])

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
env.Library("sql", glob("src/lib/query/cpp/*.cpp") + \
                   glob("src/lib/query/coco/*.cpp"))

# Data engine library
env.Library("data_engine", glob("src/lib/data_engine/cpp/*.cpp"))
# env.CLILibrary("data_engine_framework", glob("src/lib/data_engine/cs/*.cs"),
# 			   CILLIBS=["Mono.CSharp"])

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
	    LIBS=['sql', 'data_engine'] + libs
)
