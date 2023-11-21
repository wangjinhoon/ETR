# Top-Level CmakeLists.txt
cmake_minimum_required(VERSION 3.10)
PROJECT( RTABMap )
SET(PROJECT_PREFIX rtabmap)

#################################################################
list(APPEND CMAKE_PREFIX_PATH "C:/Workspace/vcpkg/installed/x64-windows/share/Qt6")
set(Qt6_DIR "C:/Workspace/vcpkg/installed/x64-windows/share/Qt6")
#################################################################



# Catkin doesn't support multiarch library path, 
# fix to "lib" if not set by user.
OPTION(MULTI_ARCH "Activate multi-arch lib directory (debian)" OFF)
IF(NOT MULTI_ARCH AND NOT DEFINED CMAKE_INSTALL_LIBDIR)
   set(CMAKE_INSTALL_LIBDIR "lib")
ENDIF(NOT MULTI_ARCH AND NOT DEFINED CMAKE_INSTALL_LIBDIR)

INCLUDE(GNUInstallDirs)

####### local cmake modules #######
SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules")

#######################
# VERSION
#######################
SET(RTABMAP_MAJOR_VERSION 0)
SET(RTABMAP_MINOR_VERSION 21)
SET(RTABMAP_PATCH_VERSION 2)
SET(RTABMAP_VERSION
  ${RTABMAP_MAJOR_VERSION}.${RTABMAP_MINOR_VERSION}.${RTABMAP_PATCH_VERSION})
  
SET(PROJECT_VERSION "${RTABMAP_VERSION}")

SET(PROJECT_VERSION_MAJOR ${RTABMAP_MAJOR_VERSION})
SET(PROJECT_VERSION_MINOR ${RTABMAP_MINOR_VERSION})
SET(PROJECT_VERSION_PATCH ${RTABMAP_PATCH_VERSION})

SET(PROJECT_SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

####### COMPILATION PARAMS #######
# In case of Makefiles if the user does not setup CMAKE_BUILD_TYPE, assume it's Release:
IF(${CMAKE_GENERATOR} MATCHES ".*Makefiles")
    IF("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(CMAKE_BUILD_TYPE Release)
    ENDIF("${CMAKE_BUILD_TYPE}" STREQUAL "")
ENDIF(${CMAKE_GENERATOR} MATCHES ".*Makefiles")

option(FLANN_KDTREE_MEM_OPT  "Disable multi-threaded FLANN kd-tree to minimize memory allocations"   ON)

IF(FLANN_KDTREE_MEM_OPT)
    ADD_DEFINITIONS("-DFLANN_KDTREE_MEM_OPT")
ENDIF(FLANN_KDTREE_MEM_OPT)

IF(WIN32 AND NOT MINGW)
    ADD_DEFINITIONS("-DNOMINMAX")
    ADD_DEFINITIONS("-wd4100 -wd4127 -wd4150 -wd4191 -wd4242 -wd4244 -wd4251 -wd4305 -wd4365 -wd4512 -wd4514 -wd4548 -wd4571 -wd4619 -wd4625 -wd4626 -wd4628 -wd4668 -wd4710 -wd4711 -wd4738 -wd4820 -wd4946 -wd4986")
ELSE ()
    ADD_DEFINITIONS( "-Wall" )
    ADD_DEFINITIONS("-Wno-unknown-pragmas")
ENDIF()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif()
# To suppress g2o related opengl warning
if(POLICY CMP0072)
    cmake_policy(SET CMP0072 NEW)
endif()

IF(MINGW)
    # Hide the --enable-auto-import warning
    SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-auto-import")
    SET(CMAKE_MODULE_LINKER_FLAGS "-Wl,--enable-auto-import")
    SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--enable-auto-import")
ENDIF(MINGW)

# GCC 4 required
IF(UNIX OR MINGW)
    EXEC_PROGRAM( gcc ARGS "-dumpversion" OUTPUT_VARIABLE GCC_VERSION )
    IF(GCC_VERSION VERSION_LESS "4.0.0")
        MESSAGE(FATAL_ERROR "GCC ${GCC_VERSION} found, but version 4.x.x minimum is required")
    ENDIF(GCC_VERSION VERSION_LESS "4.0.0")
ENDIF(UNIX OR MINGW)

#The CDT Error Parser cannot handle error messages that span 
#more than one line, which is the default gcc behavior. 
#In order to force gcc to generate single line error messages with no line wrapping
IF(CMAKE_COMPILER_IS_GNUCC)
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
  if(MSVC_VERSION GREATER 1500 AND ${CMAKE_VERSION} VERSION_GREATER "2.8.6")
    include(ProcessorCount)
    ProcessorCount(N)
    if(NOT N EQUAL 0)
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP${N}")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${N}")
    endif()
  endif()
  add_compile_options("/bigobj")
endif()

# [Eclipse] Automatic Discovery of Include directories (Optional, but handy) 
#SET(CMAKE_VERBOSE_MAKEFILE ON)

#Other paths...
IF(APPLE)
   # For Mac ports
   SET(CMAKE_INCLUDE_PATH "/opt/local/include;${CMAKE_INCLUDE_PATH}")
   SET(CMAKE_LIBRARY_PATH "/opt/local/lib;${CMAKE_LIBRARY_PATH}")
ENDIF()

####### Build libraries as shared or static #######
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )


####### OUTPUT DIR #######
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# Avoid Visual Studio bin/Release and bin/Debug sub directories
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
SET( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

####### INSTALL DIR #######
set(INSTALL_INCLUDE_DIR include/${PROJECT_PREFIX}-${RTABMAP_MAJOR_VERSION}.${RTABMAP_MINOR_VERSION})
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/${PROJECT_PREFIX}-${RTABMAP_MAJOR_VERSION}.${RTABMAP_MINOR_VERSION})
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR})
  
####### BUILD OPTIONS #######
include (GenerateExportHeader)
# ANDROID_PREBUILD (early exit if true)
OPTION( ANDROID_PREBUILD "Set to ON to build rtabmap resource build tool (required for android build)" OFF )
IF(ANDROID_PREBUILD)
   MESSAGE("Option ANDROID_PREBUILD is set, only rtabmap resource tool will be built. You can use android toolchain after that.")
   ADD_SUBDIRECTORY( utilite )
   return()
ENDIF(ANDROID_PREBUILD)

IF(APPLE OR WIN32)
    OPTION(BUILD_AS_BUNDLE "Set to ON to build as bundle with all embedded dependencies (DragNDrop for Mac, installer for Windows)" OFF)
ENDIF(APPLE OR WIN32)
OPTION(BUILD_APP "Build main application" ON)

####### DEPENDENCIES #######
option(WITH_QT            "Include Qt support"                   ON)

option(WITH_PDAL          "Include PDAL support"                 ON)
option(WITH_OPENNI2       "Include OpenNI2 support"              ON)
option(WITH_G2O           "Include g2o support"                  ON)
option(WITH_GTSAM         "Include GTSAM support"                ON)
option(WITH_TORO          "Include TORO support"                 ON)
option(WITH_CERES         "Include Ceres support"                OFF)
option(WITH_MRPT          "Include MRPT support"                 ON)
option(WITH_POINTMATCHER  "Include libpointmatcher support"      ON)
option(WITH_OPEN3D        "Include Open3D support"               OFF)

option(WITH_REALSENSE2    "Include RealSense support"            ON)
option(WITH_OCTOMAP       "Include Octomap support"              ON)
option(WITH_CPUTSDF       "Include CPUTSDF support"              OFF)
option(WITH_MADGWICK      "Include Madgwick IMU filtering support" ON)
option(WITH_FASTCV        "Include FastCV support"               ON)
option(WITH_OPENMP        "Include OpenMP support"               ON)
option(WITH_OPENGV        "Include OpenGV support"               ON)
option(PCL_OMP            "With PCL OMP implementations"         ON)

set(RTABMAP_QT_VERSION AUTO CACHE STRING "Force a specific Qt version.")
set_property(CACHE RTABMAP_QT_VERSION PROPERTY STRINGS AUTO 4 5 6)

FIND_PACKAGE(OpenCV REQUIRED QUIET COMPONENTS core calib3d imgproc highgui stitching photo video videoio OPTIONAL_COMPONENTS aruco xfeatures2d nonfree gpu cudafeatures2d)

IF(WITH_QT)
FIND_PACKAGE(PCL 1.7 REQUIRED QUIET COMPONENTS common io kdtree search surface filters registration sample_consensus segmentation visualization)
ELSE()
FIND_PACKAGE(PCL 1.7 REQUIRED QUIET COMPONENTS common io kdtree search surface filters registration sample_consensus segmentation )
ENDIF()
if(PCL_COMPILE_OPTIONS)
  if("${PCL_COMPILE_OPTIONS}" MATCHES "-march=native")
    MESSAGE(WARNING "PCL compile options contain \"-march=native\", make sure all libraries using Eigen are also compiled with that flag to avoid some segmentation faults (with gdb referring to some Eigen functions).")
  else()
    MESSAGE(STATUS "PCL compile options don't contain \"-march=native\", make sure all libraries using Eigen are also compiled without that flag to avoid some segmentation faults (with gdb referring to some Eigen functions).")
  endif()
else()
  if("${PCL_DEFINITIONS}" MATCHES "-march=native")
    MESSAGE(WARNING "PCL definitions contain \"-march=native\", make sure all libraries using Eigen are also compiled with that flag to avoid some segmentation faults (with gdb referring to some Eigen functions).")
  else()
    MESSAGE(STATUS "PCL definitions don't contain \"-march=native\", make sure all libraries using Eigen are also compiled without that flag to avoid some segmentation faults (with gdb referring to some Eigen functions).")
  endif()
endif()

FIND_PACKAGE(ZLIB REQUIRED QUIET)

FIND_PACKAGE(Sqlite3 QUIET)
IF(Sqlite3_FOUND)
   MESSAGE(STATUS "Found Sqlite3: ${Sqlite3_INCLUDE_DIRS} ${Sqlite3_LIBRARIES}")
ENDIF(Sqlite3_FOUND)

if(NOT "${PCL_LIBRARIES}" STREQUAL "")
   # fix libproj.so not found on Xenial
   list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")
endif()

# OpenMP ("-fopenmp" should be added for flann included in PCL)
# the gcc-4.2.1 coming with MacOS X is not compatible with the OpenMP pragmas we use, so disabling OpenMP for it
if(((NOT APPLE) OR (NOT CMAKE_COMPILER_IS_GNUCXX) OR (GCC_VERSION VERSION_GREATER 4.2.1) OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")) AND WITH_OPENMP)
  find_package(OpenMP COMPONENTS C CXX)
endif()
if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_INSTALL_OPENMP_LIBRARIES TRUE)
  message (STATUS "Found OpenMP: ${OpenMP_CXX_LIBRARIES}")
  if(PCL_OMP)
    message (STATUS "Add PCL_OMP to definitions")
    add_definitions(-DPCL_OMP)
  endif(PCL_OMP)
elseif(WITH_OPENMP)
  message (STATUS "Not found OpenMP")
endif()

IF(OpenCV_FOUND)
   MESSAGE(STATUS "Found OpenCV: ${OpenCV_INCLUDE_DIRS}")
ENDIF(OpenCV_FOUND)
IF(PCL_FOUND)
   MESSAGE(STATUS "Found PCL: ${PCL_INCLUDE_DIRS}")
ENDIF(PCL_FOUND)
IF(ZLIB_FOUND)
   MESSAGE(STATUS "Found ZLIB: ${ZLIB_INCLUDE_DIRS}")
ENDIF(ZLIB_FOUND)

SET(ADD_VTK_GUI_SUPPORT_QT_TO_CONF FALSE)
IF(WITH_QT)
    FIND_PACKAGE(VTK)
    IF(NOT VTK_FOUND)
        MESSAGE(FATAL_ERROR "VTK is required when using Qt. Set -DWITH_QT=OFF if you don't want gui tools.")
    ENDIF(NOT VTK_FOUND)

    # If Qt is here, the GUI will be built
    IF(NOT(${VTK_MAJOR_VERSION} LESS 9))
	  IF(NOT VTK_QT_VERSION)
	    MESSAGE(FATAL_ERROR "WITH_QT option is ON, but VTK ${VTK_MAJOR_VERSION} has not been built with Qt support, disabling Qt.")
	  ENDIF()
      MESSAGE(STATUS "VTK>=9 detected, will use VTK_QT_VERSION=${VTK_QT_VERSION} for Qt version.")
      IF(${VTK_QT_VERSION} EQUAL 6)
        FIND_PACKAGE(Qt6 COMPONENTS Widgets Core Gui OpenGL PrintSupport QUIET OPTIONAL_COMPONENTS Svg)
        MESSAGE(STATUS "Found Qt_include dir: ${Qt6_INCLUDE_DIR}")
        MESSAGE(STATUS "Found Qt_dir: ${Qt6_DIR}")
      ELSEIF(${VTK_QT_VERSION} EQUAL 5)
        FIND_PACKAGE(Qt5 COMPONENTS Widgets Core Gui OpenGL PrintSupport QUIET OPTIONAL_COMPONENTS Svg)
      ELSE()
        FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui OPTIONAL_COMPONENTS QtSvg)
      ENDIF()
    ELSE()
      # look for Qt5 (if vtk>5 is installed) before Qt4
      IF("${VTK_MAJOR_VERSION}" GREATER 5)
        IF(RTABMAP_QT_VERSION STREQUAL "AUTO" OR RTABMAP_QT_VERSION STREQUAL "5")
          FIND_PACKAGE(Qt5 COMPONENTS Widgets Core Gui OpenGL PrintSupport QUIET OPTIONAL_COMPONENTS Svg)
        ENDIF(RTABMAP_QT_VERSION STREQUAL "AUTO" OR RTABMAP_QT_VERSION STREQUAL "5")
      ENDIF("${VTK_MAJOR_VERSION}" GREATER 5)

      IF(NOT Qt5_FOUND)
        IF(RTABMAP_QT_VERSION STREQUAL "AUTO" OR RTABMAP_QT_VERSION STREQUAL "4")
          FIND_PACKAGE(Qt4 COMPONENTS QtCore QtGui OPTIONAL_COMPONENTS QtSvg)
        ENDIF(RTABMAP_QT_VERSION STREQUAL "AUTO" OR RTABMAP_QT_VERSION STREQUAL "4")
      ENDIF(NOT Qt5_FOUND)
    ENDIF()
        
    IF(QT4_FOUND OR Qt5_FOUND OR Qt6_FOUND)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTORCC ON)
        
        MESSAGE(STATUS "Found Qt: ${Qt6_INCLUDE_DIRS}")
        IF("${VTK_MAJOR_VERSION}" EQUAL 5)
            FIND_PACKAGE(QVTK REQUIRED) # only for VTK 5
        ELSE()
            list(FIND PCL_LIBRARIES VTK::GUISupportQt value)
            IF(value EQUAL -1)
                list(FIND PCL_LIBRARIES vtkGUISupportQt value)
                IF(value EQUAL -1)
                    IF(NOT(${VTK_MAJOR_VERSION} LESS 9))
                        SET(PCL_LIBRARIES "${PCL_LIBRARIES};VTK::GUISupportQt")
                    ELSE()
                        SET(PCL_LIBRARIES "${PCL_LIBRARIES};vtkGUISupportQt")
                    ENDIF()
                    SET(ADD_VTK_GUI_SUPPORT_QT_TO_CONF TRUE)
                ENDIF(value EQUAL -1)
            ENDIF(value EQUAL -1)

            MESSAGE(STATUS "VTK_RENDERING_BACKEND=${VTK_RENDERING_BACKEND}")

            IF(VTK_RENDERING_BACKEND STREQUAL "OpenGL2")
                ADD_DEFINITIONS("-DVTK_OPENGL2")
                # see issue #525
                list(FIND PCL_LIBRARIES vtkRenderingVolumeOpenGL2 value)
                IF(value EQUAL -1)
                    SET(PCL_LIBRARIES "${PCL_LIBRARIES};vtkRenderingVolumeOpenGL2")
                ENDIF(value EQUAL -1)
            ELSEIF(VTK_RENDERING_BACKEND STREQUAL "OpenGL")
                # PCL 1.9 not adding vtkRenderingVolumeOpenGL as dependency anymore, so add it for us
                list(FIND PCL_LIBRARIES vtkRenderingVolumeOpenGL value)
                IF(value EQUAL -1)
                    SET(PCL_LIBRARIES "${PCL_LIBRARIES};vtkRenderingVolumeOpenGL")
                ENDIF(value EQUAL -1)
            ELSEIF("${VTK_MAJOR_VERSION}" EQUAL 9)
                list(FIND PCL_LIBRARIES VTK::RenderingOpenGL2 value)
                IF(NOT value EQUAL -1)
                    list(FIND PCL_LIBRARIES VTK::RenderingVolumeOpenGL2 value)
                    IF(value EQUAL -1)
                        SET(PCL_LIBRARIES "${PCL_LIBRARIES};VTK::RenderingVolumeOpenGL2")
                    ENDIF(value EQUAL -1)
                ENDIF(NOT value EQUAL -1)
            ENDIF()

        ENDIF()
        ADD_DEFINITIONS(-DQT_NO_KEYWORDS) # To avoid conflicts with boost signals/foreach and Qt macros
    ENDIF(QT4_FOUND OR Qt5_FOUND OR Qt6_FOUND)
ENDIF(WITH_QT)

IF(NOT VTK_FOUND)
    # Newest PCL versions won't set -DDISABLE_VTK
    IF(NOT "${PCL_DEFINITIONS}" MATCHES "-DDISABLE_VTK")
        SET(PCL_DEFINITIONS "${PCL_DEFINITIONS};-DDISABLE_VTK")
    ENDIF()
ENDIF(NOT VTK_FOUND)

IF(WITH_TORCH)
    FIND_PACKAGE(Torch QUIET)
    IF(TORCH_FOUND)
        MESSAGE(STATUS "Found Torch: ${TORCH_INCLUDE_DIRS}")
    ENDIF(TORCH_FOUND)
ENDIF(WITH_TORCH)

IF(WITH_PDAL)
    FIND_PACKAGE(PDAL QUIET)
    IF(PDAL_FOUND)
       MESSAGE(STATUS "Found PDAL ${PDAL_VERSION}: ${PDAL_INCLUDE_DIRS}")
    ENDIF(PDAL_FOUND)
ENDIF(WITH_PDAL)

# IF PCL depends on OpenNI2 (already found), ignore WITH_OPENNI2
IF(WITH_OPENNI2 OR OpenNI2_FOUND)
    FIND_PACKAGE(OpenNI2 QUIET)
    IF(OpenNI2_FOUND)
       MESSAGE(STATUS "Found OpenNI2: ${OpenNI2_INCLUDE_DIRS}")
    ENDIF(OpenNI2_FOUND)
ENDIF(WITH_OPENNI2 OR OpenNI2_FOUND)

IF(WITH_G2O)
    FIND_PACKAGE(g2o NO_MODULE)
    IF(g2o_FOUND)
       MESSAGE(STATUS "Found g2o (targets)")
         SET(G2O_FOUND ${g2o_FOUND})
         get_target_property(G2O_INCLUDES g2o::core INTERFACE_INCLUDE_DIRECTORIES)
         MESSAGE(STATUS "g2o include dir: ${G2O_INCLUDES}")
         FIND_FILE(G2O_FACTORY_FILE g2o/core/factory.h 
         PATHS ${G2O_INCLUDES}
         NO_DEFAULT_PATH)
       FILE(READ ${G2O_FACTORY_FILE} TMPTXT)
       STRING(FIND "${TMPTXT}" "shared_ptr" matchres)
       IF(${matchres} EQUAL -1)
           MESSAGE(STATUS "Old g2o factory version detected without shared ptr (factory file: ${G2O_FACTORY_FILE}).")
           SET(G2O_CPP11 2)
       ELSE()
           MESSAGE(STATUS "Latest g2o factory version detected with shared ptr (factory file: ${G2O_FACTORY_FILE}).")
           SET(G2O_CPP11 1)
       ENDIF()
    ELSE()
        FIND_PACKAGE(G2O QUIET)
        IF(G2O_FOUND)
            MESSAGE(STATUS "Found g2o: ${G2O_INCLUDE_DIRS}")
        ENDIF(G2O_FOUND)
    ENDIF()
ENDIF(WITH_G2O)

IF(WITH_GTSAM)
    # Force config mode to ignore PCL's findGTSAM.cmake file
    FIND_PACKAGE(GTSAM CONFIG QUIET)
ENDIF(WITH_GTSAM)

IF(WITH_MRPT)
    FIND_PACKAGE(MRPT COMPONENTS poses QUIET)
    IF(MRPT_FOUND)
        message(STATUS "MRPT_VERSION: ${MRPT_VERSION}")
        message(STATUS "MRPT_LIBRARIES: ${MRPT_LIBRARIES}")
    ENDIF(MRPT_FOUND)
ENDIF(WITH_MRPT)

IF(WITH_POINTMATCHER)
    find_package(libpointmatcher QUIET)
    IF(libpointmatcher_FOUND)
        MESSAGE(STATUS "Found libpointmatcher: ${libpointmatcher_INCLUDE_DIRS}")
        string(FIND "${libpointmatcher_LIBRARIES}" "libnabo" value)
        IF(value EQUAL -1)
            # Find libnabo (Issue #1117):
            find_package(libnabo REQUIRED PATHS ${LIBNABO_INSTALL_DIR})
            message(STATUS "libnabo found, version ${libnabo_VERSION} (Config mode)")
            SET(libpointmatcher_LIBRARIES "${libpointmatcher_LIBRARIES};libnabo::nabo")
        ENDIF(value EQUAL -1)
    ENDIF(libpointmatcher_FOUND)
ENDIF(WITH_POINTMATCHER)

IF(libpointmatcher_FOUND OR GTSAM_FOUND)
    find_package(Boost COMPONENTS thread filesystem system program_options date_time REQUIRED)
    IF(Boost_MINOR_VERSION GREATER 47)
        find_package(Boost COMPONENTS thread filesystem system program_options date_time chrono timer serialization REQUIRED)
    ENDIF(Boost_MINOR_VERSION GREATER 47)
    IF(WIN32)
        MESSAGE(STATUS "Boost_LIBRARY_DIRS=${Boost_LIBRARY_DIRS}")
        link_directories(${Boost_LIBRARY_DIRS})
    ENDIF(WIN32)
ENDIF(libpointmatcher_FOUND OR GTSAM_FOUND)

IF(WITH_OPEN3D)
    IF(${CMAKE_VERSION} VERSION_LESS "3.19.0") 
       MESSAGE(WARNING "Open3D requires CMake version >=3.19 (current is ${CMAKE_VERSION})")
    ELSE()
       # Build Open3D like this to avoid linker errors in rtabmap:
       # cmake -DBUILD_SHARED_LIBS=ON -DGLIBCXX_USE_CXX11_ABI=ON -DCMAKE_BUILD_TYPE=Release ..
       find_package(Open3D QUIET)
       IF(Open3D_FOUND)
          MESSAGE(STATUS "Found Open3D: ${Open3DINCLUDE_DIRS}")
       ENDIF(Open3D_FOUND)
    ENDIF()
ENDIF(WITH_OPEN3D)

IF(WITH_REALSENSE2)
    IF(WIN32)
        FIND_PACKAGE(RealSense2 QUIET)
    ELSE()
        FIND_PACKAGE(realsense2 QUIET)
    ENDIF()
    IF(realsense2_FOUND)
       MESSAGE(STATUS "Found RealSense2: ${realsense2_INCLUDE_DIRS}")
    ENDIF(realsense2_FOUND)
ENDIF(WITH_REALSENSE2)

IF(WITH_OCTOMAP)
    FIND_PACKAGE(octomap QUIET)
    IF(octomap_FOUND)
       MESSAGE(STATUS "Found octomap ${octomap_VERSION}: ${OCTOMAP_INCLUDE_DIRS}")
       IF(octomap_VERSION VERSION_LESS 1.8)
          ADD_DEFINITIONS("-DOCTOMAP_PRE_18")
       ENDIF(octomap_VERSION VERSION_LESS 1.8)
    ENDIF(octomap_FOUND)
ENDIF(WITH_OCTOMAP)

IF(WITH_CPUTSDF)
    FIND_PACKAGE(CPUTSDF QUIET)
    IF(CPUTSDF_FOUND)
       MESSAGE(STATUS "Found CPUTSDF: ${CPUTSDF_INCLUDE_DIRS}")
    ENDIF(CPUTSDF_FOUND)
ENDIF(WITH_CPUTSDF)

# If built with okvis, we found already ceres above
IF(WITH_CERES)
   FIND_PACKAGE(Ceres QUIET)
   MESSAGE(STATUS "Found ceres ${Ceres_VERSION}: ${CERES_INCLUDE_DIRS}")
ELSEIF(Ceres_FOUND)
  MESSAGE(WARNING "WITH_CERES is OFF, but it still included by dependencies Okvis or FLOAM")
ENDIF()

IF(WITH_FASTCV)
    FIND_PACKAGE(FastCV QUIET)
    IF(FastCV_FOUND)
       MESSAGE(STATUS "Found FastCV: ${FastCV_INCLUDE_DIRS}")
    ENDIF(FastCV_FOUND)
ENDIF(WITH_FASTCV)

IF(WITH_OPENGV)
    FIND_PACKAGE(opengv QUIET)
    IF(opengv_FOUND)
       MESSAGE(STATUS "Found OpenGV: ${opengv_INCLUDE_DIRS}")
    ENDIF(opengv_FOUND)
ENDIF(WITH_OPENGV)

IF(NOT MSVC)
    IF(Qt6_FOUND OR (G2O_FOUND AND G2O_CPP11 EQUAL 1) OR TORCH_FOUND)
      # Qt6 requires c++17
      include(CheckCXXCompilerFlag)
      CHECK_CXX_COMPILER_FLAG("-std=c++17" COMPILER_SUPPORTS_CXX17)
      IF(COMPILER_SUPPORTS_CXX17)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
        set(CMAKE_CXX_STANDARD 17)
      ELSE()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++17 support. Please use a different C++ compiler if you want to use Qt6.")
      ENDIF()
    ENDIF()
    IF((NOT (${CMAKE_CXX_STANDARD} STREQUAL "17")) AND (PCL_VERSION VERSION_GREATER "1.9.1" OR G2O_FOUND OR Open3D_FOUND))
     # PCL>=1.10, latest g2o require c++14
     include(CheckCXXCompilerFlag)
      CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
      IF(COMPILER_SUPPORTS_CXX14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
        set(CMAKE_CXX_STANDARD 14)
      ELSE()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler if you want to use LOAM, latest PCL or g2o.")
      ENDIF()
    ENDIF()

    IF( (NOT (${CMAKE_CXX_STANDARD} STREQUAL "17") AND NOT (${CMAKE_CXX_STANDARD} STREQUAL "14")) AND (
           G2O_FOUND OR
           GTSAM_FOUND OR
           CERES_FOUND OR
           ANDROID OR
           realsense2_FOUND OR
           libpointmatcher_FOUND))
     #Newest versions require std11
     include(CheckCXXCompilerFlag)
      CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
      CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
      IF(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
      ELSEIF(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
      ELSE()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
      ENDIF()
    ENDIF()
ENDIF()

####### OSX BUNDLE CMAKE_INSTALL_PREFIX #######
IF(APPLE AND BUILD_AS_BUNDLE)
  IF(Qt6_FOUND OR Qt5_FOUND OR (QT4_FOUND AND QT_QTCORE_FOUND AND QT_QTGUI_FOUND))
       
    # Required when packaging, and set CMAKE_INSTALL_PREFIX to "/".
    SET(CMAKE_INSTALL_PREFIX "/")
    SET(CPACK_SET_DESTDIR TRUE) 
    
    SET(CMAKE_BUNDLE_NAME "${PROJECT_NAME}")
    SET(CMAKE_BUNDLE_LOCATION "${CMAKE_INSTALL_PREFIX}")
    
    # make sure CMAKE_INSTALL_PREFIX ends in /
    STRING(LENGTH "${CMAKE_INSTALL_PREFIX}" LEN)
    MATH(EXPR LEN "${LEN} -1" )
    STRING(SUBSTRING "${CMAKE_INSTALL_PREFIX}" ${LEN} 1 ENDCH)
    IF(NOT "${ENDCH}" STREQUAL "/")
      SET(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/")
    ENDIF(NOT "${ENDCH}" STREQUAL "/")
    SET(CMAKE_INSTALL_PREFIX 
      "${CMAKE_INSTALL_PREFIX}${CMAKE_BUNDLE_NAME}.app/Contents")
  ELSE()
    
    #If Qt is not here, no need to build a bundle
    SET(BUILD_AS_BUNDLE OFF)
  ENDIF()
ENDIF(APPLE AND BUILD_AS_BUNDLE)


####### SOURCES (Projects) #######

# OpenCV2 has nonfree if OPENCV_NONFREE_FOUND
# OpenCV<=3.4.2 has nonfree if OPENCV_XFEATURES2D_FOUND
# OpenCV>3.4.2 has nonfree if OPENCV_XFEATURES2D_FOUND and OPENCV_ENABLE_NONFREE is defined
IF(NOT (OPENCV_NONFREE_FOUND OR OPENCV_XFEATURES2D_FOUND))
   SET(NONFREE "//")
ELSEIF(OpenCV_VERSION VERSION_GREATER "3.4.2")
  FIND_FILE(OpenCV_MODULES_HPP opencv2/opencv_modules.hpp 
    PATHS ${OpenCV_INCLUDE_DIRS}
    NO_DEFAULT_PATH)
  FILE(READ ${OpenCV_MODULES_HPP} TMPTXT)
  STRING(FIND "${TMPTXT}" "#define OPENCV_ENABLE_NONFREE" matchres)
  IF(${matchres} EQUAL -1)
    SET(NONFREE "//")
  ENDIF(${matchres} EQUAL -1)
ENDIF()

IF(NOT G2O_FOUND)
   SET(G2O "//")
   SET(G2O_CPP_CONF "//")
ELSE()
   IF(NOT G2O_CPP11)
     SET(G2O_CPP_CONF "//")
   ENDIF(NOT G2O_CPP11)
ENDIF()
IF(NOT GTSAM_FOUND)
   SET(GTSAM "//")
ENDIF()
IF(NOT MRPT_FOUND)
   SET(MRPT "//")
ENDIF(NOT MRPT_FOUND)
IF(NOT CERES_FOUND)
   SET(CERES "//")
ENDIF(NOT CERES_FOUND)
IF(NOT WITH_TORO)
   SET(TORO "//")
ENDIF(NOT WITH_TORO)
IF(NOT libpointmatcher_FOUND)
   SET(POINTMATCHER "//")
ENDIF(NOT libpointmatcher_FOUND)
IF(NOT Open3D_FOUND)
   SET(OPEN3D "//")
ENDIF(NOT Open3D_FOUND)
IF(NOT FastCV_FOUND)
   SET(FASTCV "//")
ENDIF(NOT FastCV_FOUND)
IF(NOT opengv_FOUND)
   SET(OPENGV "//")
ENDIF(NOT opengv_FOUND)
IF(NOT PDAL_FOUND)
   SET(PDAL "//")
ENDIF(NOT PDAL_FOUND)
IF(NOT OpenNI2_FOUND)
   SET(OPENNI2 "//")
ENDIF(NOT OpenNI2_FOUND)
IF(NOT realsense2_FOUND)
   SET(REALSENSE2 "//")
   SET(CONF_WITH_REALSENSE2 0)
ELSE()
   SET(CONF_WITH_REALSENSE2 1)
   IF(WIN32)
      install(
         FILES
         "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/FindRealSense2.cmake"
         DESTINATION ${INSTALL_CMAKE_DIR}/Modules/.
         COMPONENT devel
      )
   ENDIF(WIN32)
ENDIF()
IF(NOT octomap_FOUND)
   SET(OCTOMAP "//")
   SET(CONF_WITH_OCTOMAP 0)
ELSE()
   SET(CONF_WITH_OCTOMAP 1)
ENDIF()
IF(NOT CPUTSDF_FOUND)
   SET(CPUTSDF "//")
ENDIF()
IF(NOT TORCH_FOUND)
   SET(TORCH "//")
ENDIF()
IF(ADD_VTK_GUI_SUPPORT_QT_TO_CONF)
   SET(CONF_VTK_QT true)
ELSE()
   SET(CONF_VTK_QT false)
ENDIF()
IF(NOT WITH_MADGWICK)
   SET(MADGWICK "//")
ENDIF()

CONFIGURE_FILE(Version.h.in ${CMAKE_CURRENT_BINARY_DIR}/corelib/src/include/${PROJECT_PREFIX}/core/Version.h)

ADD_SUBDIRECTORY( utilite )
ADD_SUBDIRECTORY( corelib )

IF(Qt6_FOUND OR Qt5_FOUND OR (QT4_FOUND AND QT_QTCORE_FOUND AND QT_QTGUI_FOUND))
   ADD_SUBDIRECTORY( guilib )
   IF(BUILD_APP)
      ADD_SUBDIRECTORY( app )
   ENDIF(BUILD_APP)
ELSEIF(WITH_QT)
   MESSAGE(WARNING "Qt not found, the GUI lib and the stand-alone application will not be compiled...")
ENDIF()



#######################
# Uninstall target, for "make uninstall"
#######################
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

####
# Global Export Target
####
add_library(rtabmap INTERFACE)
add_library(rtabmap::rtabmap ALIAS rtabmap)
IF(QT4_FOUND OR Qt5_FOUND OR Qt6_FOUND)
   set(CONF_WITH_GUI ON)
   IF(QT4_FOUND)
      set(CONF_QT_VERSION 4)
   ELSEIF(Qt5_FOUND)
      set(CONF_QT_VERSION 5)
   ELSE()
      set(CONF_QT_VERSION 6)
   ENDIF()
   target_link_libraries(rtabmap INTERFACE rtabmap_utilite rtabmap_core rtabmap_gui)
ELSE()
   set(CONF_WITH_GUI OFF)
   target_link_libraries(rtabmap INTERFACE rtabmap_utilite rtabmap_core)
ENDIF()
install(TARGETS rtabmap EXPORT rtabmapTargets)
export(EXPORT rtabmapTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
  NAMESPACE rtabmap::
)
install(EXPORT rtabmapTargets
  FILE
    ${PROJECT_NAME}Targets.cmake
  DESTINATION
    ${INSTALL_CMAKE_DIR}
  NAMESPACE rtabmap::
  COMPONENT
    devel
)

####
# Setup RTABMapConfig.cmake
####
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# Build tree:
SET(CONF_MODULES_DIR "../cmake_modules")
configure_file(
  ${PROJECT_NAME}Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
)

# Install tree:
SET(CONF_MODULES_DIR "Modules")
configure_file(
  ${PROJECT_NAME}Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
  @ONLY
)
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION
    ${INSTALL_CMAKE_DIR}
  COMPONENT
    devel
)
  
####

### Install package.xml for catkin
install(FILES package.xml DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_PREFIX}" COMPONENT devel)

#######################
# CPACK (Packaging)
#######################
IF(BUILD_AS_BUNDLE)
  SET(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT runtime)
  INCLUDE(InstallRequiredSystemLibraries)
ENDIF(BUILD_AS_BUNDLE)

SET(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
SET(CPACK_PACKAGE_VENDOR "${PROJECT_NAME} project")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "RTAB-MAP is a Real-Time Appearance-Based Mapping approach.")
#SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/Description.txt")
#SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
#SET(CPACK_PACKAGE_EXECUTABLES "")
#SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_PACKAGE_CONTACT "matlabbe@gmail.com")


set(CPACK_SOURCE_IGNORE_FILES 
  "\\\\.svn/" 
  "\\\\.settings/" 
  "${PROJECT_SOURCE_DIR}/build/[a-zA-Z0-9_]+" 
  "~$" 
  "${PROJECT_SOURCE_DIR}/bin/.*${PROJECT_PREFIX}"
  "${PROJECT_SOURCE_DIR}/bin/.*${PROJECT_NAME}"
  "${PROJECT_SOURCE_DIR}/bin/.*[tT]est"
  "${PROJECT_SOURCE_DIR}/bin/.*[eE]xample"
  "${PROJECT_SOURCE_DIR}/bin/.*uresourcegenerator"
  "\\\\.DS_Store"
)

IF(WIN32)
  SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
  IF(CMAKE_CL_64) 
    SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64") 
  ELSE() 
    SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES") 
  ENDIF()
  IF(BUILD_AS_BUNDLE)
    SET(CPACK_GENERATOR "ZIP;NSIS")
  ELSE()
    SET(CPACK_GENERATOR "ZIP")
  ENDIF()
  SET(CPACK_SOURCE_GENERATOR "ZIP")
  SET(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}")
  SET(ICON_PATH "${PROJECT_SOURCE_DIR}/app/src/${PROJECT_NAME}.ico")
  SET(CPACK_NSIS_MUI_ICON ${ICON_PATH})
  SET(CPACK_NSIS_MUI_UNIICON ${ICON_PATH})
  SET(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
  #SET(CPACK_PACKAGE_ICON ${ICON_PATH})
  #SET(CPACK_NSIS_INSTALLED_ICON_NAME ${ICON_PATH})
  #SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.${PROJECT_PREFIX}.googlecode.com")
  #SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.${PROJECT_PREFIX}.googlecode.com")
  SET(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
  SET(CPACK_NSIS_CONTACT ${CPACK_PACKAGE_CONTACT})
  # Set the icon used for the Windows "Add or Remove Programs" tool.
  SET(CPACK_NSIS_INSTALLED_ICON_NAME bin\\\\${PROJECT_NAME}.exe)
  # Desktop link ("executableName" "linkName")
  SET(CPACK_PACKAGE_EXECUTABLES "${PROJECT_NAME}" "${PROJECT_NAME}" ${CPACK_PACKAGE_EXECUTABLES}) 
  SET(CPACK_CREATE_DESKTOP_LINKS "${PROJECT_NAME}" ${CPACK_CREATE_DESKTOP_LINKS}) 
  SET(CPACK_NSIS_MODIFY_PATH ON)
  
ELSEIF(APPLE)
  # The project is created as a bundle over the main app (see ./app/src).
  # Here we package only this bundle. Note that we set 
  # CMAKE_INSTALL_PREFIX to "/" when packaging...
  IF(BUILD_AS_BUNDLE)
    SET(CPACK_GENERATOR "DragNDrop")
  ELSE()
    SET(CPACK_GENERATOR "PackageMaker;TBZ2")
  ENDIF()
  
  SET(CPACK_SOURCE_GENERATOR "ZIP")

  SET(CPACK_PACKAGE_ICON "${PROJECT_SOURCE_DIR}/app/src/${PROJECT_NAME}.icns")

ELSE()
  SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
  SET(CPACK_GENERATOR "TBZ2")
  SET(CPACK_SOURCE_GENERATOR "ZIP")
ENDIF()

INCLUDE(CPack)

MESSAGE(STATUS "--------------------------------------------")
MESSAGE(STATUS "Info :")
MESSAGE(STATUS "  RTAB-Map Version =     ${RTABMAP_VERSION}")
MESSAGE(STATUS "  CMAKE_VERSION =        ${CMAKE_VERSION}")
MESSAGE(STATUS "  CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")
MESSAGE(STATUS "  CMAKE_BUILD_TYPE =     ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "  CMAKE_INSTALL_LIBDIR = ${CMAKE_INSTALL_LIBDIR}")
MESSAGE(STATUS "  BUILD_APP =            ${BUILD_APP}")

IF(NOT WIN32)
    # see comment above for the BUILD_SHARED_LIBS option on Windows
    MESSAGE(STATUS "  BUILD_SHARED_LIBS =    ${BUILD_SHARED_LIBS}")
ENDIF(NOT WIN32)
IF(APPLE OR WIN32)
MESSAGE(STATUS "  BUILD_AS_BUNDLE =      ${BUILD_AS_BUNDLE}")
ENDIF(APPLE OR WIN32)
MESSAGE(STATUS "  CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
MESSAGE(STATUS "  FLANN_KDTREE_MEM_OPT = ${FLANN_KDTREE_MEM_OPT}")
MESSAGE(STATUS "  PCL_DEFINITIONS = ${PCL_DEFINITIONS}")
MESSAGE(STATUS "  PCL_VERSION = ${PCL_VERSION}")
IF(PCL_COMPILE_OPTIONS)
MESSAGE(STATUS "  PCL_COMPILE_OPTIONS = ${PCL_COMPILE_OPTIONS}")
ENDIF(PCL_COMPILE_OPTIONS)

MESSAGE(STATUS "")
MESSAGE(STATUS "Optional dependencies ('*' affects some default parameters) :")
IF(OpenCV_FOUND)
  IF(OpenCV_VERSION_MAJOR EQUAL 2)
    IF(OPENCV_NONFREE_FOUND)
      MESSAGE(STATUS " *With OpenCV 2 nonfree module (SIFT/SURF) = YES (License: Non commercial)")
    ELSE()
      MESSAGE(STATUS " *With OpenCV 2 nonfree module (SIFT/SURF) = NO (not found, License: BSD)")
    ENDIF()
  ELSE()
    IF(OPENCV_XFEATURES2D_FOUND)
      IF(NONFREE STREQUAL "//")
        IF((OpenCV_VERSION_MAJOR LESS 4) OR ((OpenCV_VERSION_MAJOR EQUAL 4) AND (OpenCV_VERSION_MINOR LESS 5)))
          MESSAGE(STATUS " *With OpenCV ${OpenCV_VERSION} xfeatures2d = YES, nonfree = NO (License: BSD)")
        ELSE()
          MESSAGE(STATUS " *With OpenCV ${OpenCV_VERSION} xfeatures2d = YES, nonfree = NO (License: Apache 2)")
        ENDIF()
      ELSE()
        MESSAGE(STATUS " *With OpenCV ${OpenCV_VERSION} xfeatures2d = YES, nonfree = YES (License: Non commercial)")
      ENDIF()
    ELSE()
      IF((OpenCV_VERSION_MAJOR LESS 4) OR ((OpenCV_VERSION_MAJOR EQUAL 4) AND (OpenCV_VERSION_MINOR LESS 5)))
        MESSAGE(STATUS " *With OpenCV ${OpenCV_VERSION} xfeatures2d = NO, nonfree = NO (License: BSD)")
      ELSE()
        MESSAGE(STATUS " *With OpenCV ${OpenCV_VERSION} xfeatures2d = NO, nonfree = NO (License: Apache 2)")
      ENDIF()
    ENDIF()
  ENDIF()
ENDIF(OpenCV_FOUND)

IF(QT4_FOUND)
MESSAGE(STATUS "  With Qt4                  = YES (License: Open Source or Commercial)")
MESSAGE(STATUS "  With VTK ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}              = YES (License: BSD)")
ELSEIF(Qt5_FOUND)
MESSAGE(STATUS "  With Qt ${Qt5_VERSION}            = YES (License: Open Source or Commercial)")
MESSAGE(STATUS "  With VTK ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}              = YES (License: BSD)")
ELSEIF(Qt6_FOUND)
MESSAGE(STATUS "  With Qt ${Qt6_VERSION}            = YES (License: Open Source or Commercial)")
MESSAGE(STATUS "  With VTK ${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}              = YES (License: BSD)")

ELSEIF(NOT WITH_QT)
MESSAGE(STATUS "  With Qt                   = NO (WITH_QT=OFF)")
ELSE()
MESSAGE(STATUS "  With Qt                   = NO (Qt not found)")
ENDIF()

IF(Sqlite3_FOUND)
MESSAGE(STATUS "  With external SQLite3     = YES (License: Public Domain)")
ELSE()
MESSAGE(STATUS "  With external SQLite3     = NO (sqlite3 not found, internal version is used for convenience)")
ENDIF()

IF(WITH_MADGWICK)
MESSAGE(STATUS "  With Madgwick             = YES (License: GPL)")
ELSE()
MESSAGE(STATUS "  With Madgwick             = NO (WITH_MADGWICK=OFF)")
ENDIF()

IF(FastCV_FOUND)
MESSAGE(STATUS "  With FastCV               = YES (License: Apache v2)")
ELSEIF(NOT WITH_FASTCV)
MESSAGE(STATUS "  With FastCV               = NO (WITH_FASTCV=OFF)")
ELSE()
MESSAGE(STATUS "  With FastCV               = NO (FastCV not found)")
ENDIF()

IF(PDAL_FOUND)
MESSAGE(STATUS "  With PDAL ${PDAL_VERSION}           = YES (License: BSD)")
ELSEIF(NOT WITH_PDAL)
MESSAGE(STATUS "  With PDAL                 = NO (WITH_PDAL=OFF)")
ELSE()
MESSAGE(STATUS "  With PDAL                 = NO (PDAL not found)")
ENDIF()

MESSAGE(STATUS "")
MESSAGE(STATUS " Solvers:")
IF(WITH_TORO)
MESSAGE(STATUS "  With TORO                 = YES (License: Creative Commons [Attribution-NonCommercial-ShareAlike])")
ELSE()
MESSAGE(STATUS "  With TORO                 = NO (WITH_TORO=OFF)")
ENDIF()

IF(G2O_FOUND)
MESSAGE(STATUS " *With g2o ${g2o_VERSION}            = YES (License: BSD)")
ELSEIF(NOT WITH_G2O)
MESSAGE(STATUS " *With g2o                  = NO (WITH_G2O=OFF)")
ELSE()
MESSAGE(STATUS " *With g2o                  = NO (g2o not found)")
ENDIF()

IF(GTSAM_FOUND)
MESSAGE(STATUS " *With GTSAM ${GTSAM_VERSION}          = YES (License: BSD)")
ELSEIF(NOT WITH_GTSAM)
MESSAGE(STATUS " *With GTSAM                = NO (WITH_GTSAM=OFF)")
ELSE()
MESSAGE(STATUS " *With GTSAM                = NO (GTSAM not found)")
ENDIF()

IF(CERES_FOUND)
IF(WITH_CERES)
MESSAGE(STATUS " *With Ceres ${Ceres_VERSION}         = YES (License: BSD)")
ELSE()
MESSAGE(STATUS " *With Ceres ${Ceres_VERSION}         = YES (License: BSD, WITH_CERES=OFF but it is enabled by okvis or floam dependencies)")
ENDIF()
ELSEIF(NOT WITH_CERES)
MESSAGE(STATUS " *With Ceres                = NO (WITH_CERES=OFF)")
ELSE()
MESSAGE(STATUS " *With Ceres                = NO (Ceres not found)")
ENDIF()

IF(MRPT_FOUND)
MESSAGE(STATUS "  With MRPT ${MRPT_VERSION}           = YES (License: BSD)")
ELSEIF(NOT WITH_MRPT)
MESSAGE(STATUS "  With MRPT                 = NO (WITH_MRPT=OFF)")
ELSE()
MESSAGE(STATUS "  With MRPT                 = NO (MRPT not found)")
ENDIF()

IF(libpointmatcher_FOUND)
MESSAGE(STATUS " *With libpointmatcher ${libpointmatcher_VERSION} = YES (License: BSD)")
ELSEIF(NOT WITH_POINTMATCHER)
MESSAGE(STATUS " *With libpointmatcher      = NO (WITH_POINTMATCHER=OFF)")
ELSE()
MESSAGE(STATUS " *With libpointmatcher      = NO (libpointmatcher not found)")
ENDIF()

IF(Open3D_FOUND)
MESSAGE(STATUS "  With Open3D               = YES (License: MIT)")
ELSEIF(NOT WITH_OPEN3D)
MESSAGE(STATUS "  With Open3D               = NO (WITH_OPEN3D=OFF)")
ELSEIF(${CMAKE_VERSION} VERSION_LESS "3.19.0")
MESSAGE(STATUS "  With Open3D               = NO (Open3D requires CMake>=3.19)")
ELSE()
MESSAGE(STATUS "  With Open3D               = NO (Open3D not found)")
ENDIF()

IF(opengv_FOUND)
MESSAGE(STATUS "  With OpenGV ${opengv_VERSION}           = YES (License: BSD)")
ELSEIF(NOT WITH_OPENGV)
MESSAGE(STATUS "  With OpenGV               = NO (WITH_OPENGV=OFF)")
ELSE()
MESSAGE(STATUS "  With OpenGV               = NO (OpenGV not found)")
ENDIF()

MESSAGE(STATUS "")
MESSAGE(STATUS " Reconstruction Approaches:")
IF(octomap_FOUND)
MESSAGE(STATUS "  With OCTOMAP ${octomap_VERSION}        = YES (License: BSD)")
ELSEIF(NOT WITH_OCTOMAP)
MESSAGE(STATUS "  With OCTOMAP              = NO (WITH_OCTOMAP=OFF)")
ELSE()
MESSAGE(STATUS "  With OCTOMAP              = NO (octomap not found)")
ENDIF()

IF(CPUTSDF_FOUND)
MESSAGE(STATUS "  With CPUTSDF              = YES (License: BSD)")
ELSEIF(NOT WITH_CPUTSDF)
MESSAGE(STATUS "  With CPUTSDF              = NO (WITH_CPUTSDF=OFF)")
ELSE()
MESSAGE(STATUS "  With CPUTSDF              = NO (CPUTSDF not found)")
ENDIF()

MESSAGE(STATUS "")
MESSAGE(STATUS " Camera Drivers:")

IF(OpenNI2_FOUND)
MESSAGE(STATUS "  With OpenNI2              = YES (License: Apache v2)")
ELSEIF(NOT WITH_OPENNI2)
MESSAGE(STATUS "  With OpenNI2              = NO (WITH_OPENNI2=OFF)")
ELSE()
MESSAGE(STATUS "  With OpenNI2              = NO (OpenNI2 not found)")
ENDIF()

IF(realsense2_FOUND)
IF(WIN32)
MESSAGE(STATUS "  With RealSense2           = YES (License: Apache-2)")
ELSE()
MESSAGE(STATUS "  With RealSense2 ${realsense2_VERSION}    = YES (License: Apache-2)")
ENDIF()
ELSEIF(NOT WITH_REALSENSE2)
MESSAGE(STATUS "  With RealSense2           = NO (WITH_REALSENSE2=OFF)")
ELSE()
MESSAGE(STATUS "  With RealSense2           = NO (librealsense2 not found)")
ENDIF()

MESSAGE(STATUS "Show all options with: cmake -LA | grep WITH_")
MESSAGE(STATUS "--------------------------------------------")

IF(NOT GTSAM_FOUND AND NOT G2O_FOUND AND NOT WITH_TORO AND NOT WITH_CERES AND NOT CERES_FOUND)
MESSAGE(SEND_ERROR "No graph optimizer found! You should have at least one of these options:
       g2o (https://github.com/RainerKuemmerle/g2o)
       GTSAM (https://collab.cc.gatech.edu/borg/gtsam)
       Ceres (http://ceres-solver.org)
       set -DWITH_TORO=ON")
ENDIF(NOT GTSAM_FOUND AND NOT G2O_FOUND AND NOT WITH_TORO AND NOT WITH_CERES AND NOT CERES_FOUND)

# vim: set et ft=cmake fenc=utf-8 ff=unix sts=0 sw=2 ts=2 :
