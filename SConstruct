env = Environment()  # Initialize the environment

print("Using compiler is:", env['CC'])

# lib names
BOOST_THREAD_LINUX = 'boost_thread'
BOOST_SYSTEM_LINUX = 'boost_system'
BOOST_UNIT_TEST_LINUX = 'boost_unit_test_framework'
ZMQ_LINUX = 'zmq'

# settings for debug and release
env.Append( CPPFLAGS = '-Wall -pedantic -pthread -std=c++17' )  # Use -pthread for most portability
env.Append( LINKFLAGS = '-Wall -pthread -std=c++17' )
env.Append( LIBS = [ BOOST_THREAD_LINUX, BOOST_SYSTEM_LINUX, ZMQ_LINUX ] )
env.Append( CPPPATH = [ Dir('.') ] )

library_files = [ 'EventHandler.hpp', 'Handle.hpp', 'Reactor.hpp', 'ThreadManager.hpp', 'ThreadSet.hpp' ]
implementation_files = [ 'example/Server/EventHandlers/EventHandlerZmq.cpp', 'example/Server/Handles/HandleZmq.cpp', 'example/Server/Server.cpp', 'example/Server/Handles/HandleBoost.cpp', 'example/Server/EventHandlers/EventHandlerBoost.cpp' ]
boost_client_files = [ 'example/BoostClient/BoostClient.cpp', 'example/BoostClient/main.cpp' ]
test_files = [ 'tests/ServerTest.cpp', 'example/BoostClient/BoostClient.cpp', 'tests/TestAll.cpp' ]

def build_library(env):
    install_dir = './mt4cpp/'
    env.Install(install_dir)
    for file in library_files:
       env.Install(install_dir, 'src/' + file)
    return

def build_implementation(env):
   return env.Program(target='server', source=implementation_files )

def build_boostclient(env):
   return env.Program(target='boostClient', source=boost_client_files)

def build_tests(env):
    e = env.Clone()
    e.Append( LIBS = BOOST_UNIT_TEST_LINUX )
    return e.Program(target='testAll', source=test_files)

build_library(env)
build_implementation(env)
build_boostclient(env)
build_tests(env)
