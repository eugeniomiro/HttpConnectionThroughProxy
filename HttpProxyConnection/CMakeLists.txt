# CMakeList.txt : CMake project for HttpProxyConnection, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (
	HttpProxyConnection

	"HttpProxyConnection.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET HttpProxyConnection PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(
	HttpProxyConnection 
	PRIVATE 
		${Boost_INCLUDE_DIRS}
)