# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("HttpConnectionThroughProxy")

# Include sub-projects.
add_subdirectory ("HttpProxyConnection")

set (selected_boost_components) # Specify the required Boost components here

find_package (Boost REQUIRED COMPONENTS ${selected_boost_components})
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	message ("Boost found")
    message ("  include dirs: ${Boost_INCLUDE_DIRS}")
    message ("  library dirs: ${Boost_LIBRARY_DIRS}")
    message ("  boost components: '${selected_boost_components}'")
    message ("  boost libraries: '${Boost_LIBRARIES}'")
endif()