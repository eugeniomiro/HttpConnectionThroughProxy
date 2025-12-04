cmake_minimum_required(VERSION 3.21.1)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("HttpConnectionThroughProxy")

# Boost
set (selected_boost_components url) # Specify the required Boost components here

find_package (Boost REQUIRED COMPONENTS ${selected_boost_components})
if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	message ("Boost found")
    message ("  include dirs: ${Boost_INCLUDE_DIRS}")
    message ("  library dirs: ${Boost_LIBRARY_DIRS}")
    message ("  boost components: '${selected_boost_components}'")
    message ("  boost libraries: '${Boost_LIBRARIES}'")
endif()

# OpenSSL
find_package(OpenSSL REQUIRED)
if (OpenSSL_FOUND)
    message("OpenSSL found")
    message("  include dir: ${OPENSSL_INCLUDE_DIR}")
    message("  version:     ${OPENSSL_VERSION}")
    message("  libraries:   ${OPENSSL_LIBRARIES}")
else()
    message("OpenSSL not found")
endif()


# Include sub-projects.
add_subdirectory ("HttpProxyConnection")
