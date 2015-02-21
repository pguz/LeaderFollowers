import platform

env = Environment()  # Initialize the environment

# change compiler
# env = Environment(tools = ['default','mingw'])
# print compiler info
# print "CC is:", env['CC']

# lib names
BOOST_THREAD_LINUX = 'boost_thread'
BOOST_SYSTEM_LINUX = 'boost_system'
BOOST_UNIT_TEST_LINUX = 'boost_unit_test_framework'
ZMQ_LINUX = 'zmq'
BOOST_THREAD_WINDOWS = 'libboost_thread-vc120-mt-1_55'
BOOST_UNIT_TEST_WINDOWS = 'libboost_unit_test_framework-vc120-mt-1_55'
ZMQ_WINDOWS = 'libzmq-v120-mt-4_0_4'

# include paths
BOOST_INCLUDE_WINDOWS = 'c:/local/boost_1_55_0'		 		
BOOST_INCLUDE_LINUX = '/usr/include/boost'              
ZMQ_INCLUDE_WINDOWS = 'c:/Program Files/ZeroMQ 4.0.4/include'
# ZMQ_INCLUDE_LINUX = ...

# lib paths
BOOST_LIB_WINDOWS = 'c:/local/boost_1_55_0/lib64-msvc-12.0'		
BOOST_LIB_LINUX = '/usr/lib'    
ZMQ_LIB_WINDOWS = 'c:/Program Files/ZeroMQ 4.0.4/lib'
ZMQ_LIB_LINUX = '/usr/local/lib'

# settings for debug and release
if(platform.system() == "Linux"):
   env.Append( CPPPATH = BOOST_INCLUDE_LINUX )
   env.Append( LIBPATH = [ BOOST_LIB_LINUX, ZMQ_LIB_LINUX ] )
   env.Append( CPPFLAGS = '-Wall -pedantic -pthread' )  # Use -pthread for most portability
   env.Append( LINKFLAGS = '-Wall -pthread' )
elif(platform.system() == "Windows"):
   env.Append( CPPPATH = [ BOOST_INCLUDE_WINDOWS, ZMQ_INCLUDE_WINDOWS ] )
   env.Append( LIBPATH = [ BOOST_LIB_WINDOWS, ZMQ_LIB_WINDOWS ] )
   #env.Append( WINDOWS_INSERT_MANIFEST = True )
else:
   print "System " + platform.system() + " not supported "

# libs
if(platform.system() == "Linux"):
   env.Append( CPPFLAGS = ' -std=c++11' )      # optimize for code size and execution time
   env.Append( LINKFLAGS = ' -std=c++11' )
   env.Append( LIBS = [ BOOST_THREAD_LINUX, BOOST_SYSTEM_LINUX, ZMQ_LINUX ] )
elif(platform.system() == "Windows"):
   env.Append( CPPFLAGS = ' /EHsc /MD /DWIN32 /D_WIN32_WINNT=0x0501 /D_CONSOLE  /W4 /Ox ' )
   env.Append( LINKFLAGS = ' /SUBSYSTEM:CONSOLE ' )
   env.Append( LIBS = [ BOOST_THREAD_WINDOWS, ZMQ_WINDOWS ] )
else:
   print "System " + platform.system() + " not supported "
   
library_files = [ 'EventHandler.hpp', 'Handle.hpp', 'Reactor.hpp', 'ThreadManager.hpp', 'ThreadSet.hpp' ]

def build_library(env):
    install_dir = './mt4cpp/'
    env.Install(install_dir)
    for file in library_files:
       installed = env.Install(install_dir, 'src/' + file)
    return
    
#example_files = [ 'example/EventHandlerZmq.cpp', 'example/EventHandlerBoost.cpp', 'example/HandleZmq.cpp', 'example/HandleBoost.cpp', 'example/Server.cpp' ]
example_files_linux = [ 'example/Server/EventHandlers/EventHandlerZmq.cpp', 'example/Server/Handles/HandleZmq.cpp', 'example/Server/Server.cpp', 'example/Server/Handles/HandleBoost.cpp', 'example/Server/EventHandlers/EventHandlerBoost.cpp' ]
example_files_windows = [ 'example/Server/EventHandlers/EventHandlerZmq.cpp', 'example/Server/Handles/HandleZmq.cpp', 'example/Server/Server.cpp' ]

def build_example(env):
   e = env.Clone()
   e.Append( CPPPATH = [ Dir('.') ] )
   examples = []
   if(platform.system() == "Linux"): 
		examples = example_files_linux;
   elif(platform.system() == "Windows"): 
		examples = example_files_windows;
   t = e.Program( target = 'server', source = examples )
   return t

boostclient_files_linux = [ 'example/BoostClient/BoostClient.cpp', 'example/BoostClient/main.cpp' ]

def build_boostclient(env):
   e = env.Clone()
   e.Append( CPPPATH = [ Dir('.') ] )
   files = []
   if(platform.system() == "Linux"): 
		files = boostclient_files_linux;
   t = e.Program( target = 'boostClient', source = files )
   return t

test_files = [ 'tests/ServerTest.cpp', 'example/BoostClient/BoostClient.cpp', 'tests/TestAll.cpp' ]

def build_tests(env):
	e = env.Clone()
	e.Append( CPPPATH = [ Dir('.') ] )
	if(platform.system() == "Linux"): 
		e.Append( LIBS = BOOST_UNIT_TEST_LINUX ) 
 	elif(platform.system() == "Windows"): 
		e.Append( LIBS = BOOST_UNIT_TEST_WINDOWS ) 
	t = e.Program(target = 'testAll', source = test_files)
	return

build_tests(env)
build_library(env)
build_example(env)
if(platform.system() == "Linux"): 
	build_boostclient(env)
	
# files = [ 'example/EventHandlerZmq.cpp', 'example/HandleZmq.cpp', 'example/Server.cpp', 'ThreadManager.cpp', 'ThreadSet.cpp' ]
# env.Program(target = 'mt4cpp', source = files)
