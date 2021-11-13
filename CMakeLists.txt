cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0.17763.0 CACHE STRING "" FORCE)

project(SimFramework CXX CUDA)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake/cmake_modules)

# find_package(MKL REQUIRED)
set(BLA_VENDOR Intel10_64lp)
find_package(BLAS REQUIRED)
find_path(BLAS_INCLUDE_DIRS mkl.h
    /usr/include/mkl
    /usr/local/include
    $ENV{BLAS_HOME}/include)

# need uiplugin and designer
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui OpenGL OpenGLExtensions Xml Designer UiPlugin)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/Ui/Form ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(QGLViewer REQUIRED)
find_package(CUDA REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenMP REQUIRED)

# set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++14 -l/usr/include/mkl" )
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -l/usr/include/mkl")
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -std=c++14 ${BLAS_LINKER_FLAGS}")

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY AUTOMOC ON)

################################################################################
# Sub-projects
################################################################################
set(PROJECT_NAME SimFramework)

################################################################################
# Source groups
################################################################################
set(Source_Files
    "main.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(Source_Files__Common
    "Commom/DataCore.h"
    "Commom/MatrixCore.h"
    "Commom/AutoFlipSVD.h"
    "Commom/BufferSerialization.h"
    "Commom/ConvertSparseMatrixToCSR.h"
    "Commom/EigenMatrixIO.h"
    "Commom/FileIO.h"
    "Commom/GeometryComputation.cpp"
    "Commom/GeometryComputation.h"
    "Commom/PolynomialSolver.cpp"
    "Commom/PolynomialSolver.h"
    "Commom/SparseMatrixRemoveRows.cpp"
    "Commom/SparseMatrixRemoveRows.h"
    "Commom/SparseMatrixTopology.cpp"
    "Commom/SparseMatrixTopology.h"
    "Commom/SPDProjectFunction.cpp"
    "Commom/SPDProjectFunction.h"
)
source_group("Source Files\\Common" FILES ${Source_Files__Common})

set(Source_Files__Common__DataStructure
    "Commom/DataStructure/DataSet.h"
)
source_group("Source Files\\Common\\DataStructure" FILES ${Source_Files__Common__DataStructure})

set(Source_Files__Common__SVD
    "Commom/SVD/ImplicitQRSVD.h"
    "Commom/SVD/SvdTools.h"
)
source_group("Source Files\\Common\\SVD" FILES ${Source_Files__Common__SVD})

set(Source_Files__Common__tinyxml
    "Commom/tinyxml/tinyxml.cpp"
    "Commom/tinyxml/tinyxml.h"
    "Commom/tinyxml/tinystr.h"
    "Commom/tinyxml/tinystr.cpp"
    "Commom/tinyxml/tinyxmlerror.cpp"
    "Commom/tinyxml/tinyxmlparser.cpp"
)
source_group("Source Files\\Common\\tinyxml" FILES ${Source_Files__Common__tinyxml})

set(Source_Files__Model
    "Model/BaseModel.cpp"
    "Model/BaseModel.h"
    "Model/GeometryElement.cpp"
    "Model/GeometryElement.h"
    "Model/tiny_obj_loader.cc"
    "Model/tiny_obj_loader.h"
)
source_group("Source Files\\Model" FILES ${Source_Files__Model})

set(Source_Files__Model__MedialMesh
    "Model/MedialMesh/BaseMedialMedialHandle.cpp"
    "Model/MedialMesh/BaseMedialMedialHandle.h"
    "Model/MedialMesh/BaseMedialMesh.cpp"
    "Model/MedialMesh/BaseMedialMesh.h"
    "Model/MedialMesh/BaseMedialMeshBufferPool.cpp"
    "Model/MedialMesh/BaseMedialMeshBufferPool.h"
)
source_group("Source Files\\Model\\MedialMesh" FILES ${Source_Files__Model__MedialMesh})

set(Source_Files__Model__MedialMesh__Cuda
    "Cuda/CudaHeader.cuh"
    "Cuda/CudaMatrixOperator.cuh"
    "Cuda/CudaSVD.cuh"
)
source_group("Source Files\\Model\\MedialMesh\\Cuda" FILES ${Source_Files__Model__MedialMesh__Cuda})

set(Source_Files__Model__TriangleMesh
    "Model/TriangleMesh/BaseSurfaceHandle.cpp"
    "Model/TriangleMesh/BaseSurfaceHandle.h"
    "Model/TriangleMesh/BaseSurfaceMesh.cpp"
    "Model/TriangleMesh/BaseSurfaceMesh.h"
    "Model/TriangleMesh/BaseSurfaceMeshBufferPool.cpp"
    "Model/TriangleMesh/BaseSurfaceMeshBufferPool.h"
)
source_group("Source Files\\Model\\TriangleMesh" FILES ${Source_Files__Model__TriangleMesh})

set(Source_Files__Model__VolumetricMesh
    "Model/VolumetricMesh/BaseTetMesh.cpp"
    "Model/VolumetricMesh/BaseTetMesh.h"
    "Model/VolumetricMesh/BaseTetMeshBufferPool.cpp"
    "Model/VolumetricMesh/BaseTetMeshBufferPool.h"
    "Model/VolumetricMesh/BaseTetMeshHandle.cpp"
    "Model/VolumetricMesh/BaseTetMeshHandle.h"
    "Model/VolumetricMesh/BaseTetMeshMaterial.h"
)
source_group("Source Files\\Model\\VolumetricMesh" FILES ${Source_Files__Model__VolumetricMesh})

set(Source_Files__Model__VolumetricMesh__IsotropicMaterial
    "Model/VolumetricMesh/IsotropicMaterial/IsotropicMaterial.cpp"
    "Model/VolumetricMesh/IsotropicMaterial/IsotropicMaterial.h"
    "Model/VolumetricMesh/IsotropicMaterial/IsotropicMaterialWithCompressionResistance.cpp"
    "Model/VolumetricMesh/IsotropicMaterial/IsotropicMaterialWithCompressionResistance.h"
    "Model/VolumetricMesh/IsotropicMaterial/NeoHookeanIsotropicMaterial.cpp"
    "Model/VolumetricMesh/IsotropicMaterial/NeoHookeanIsotropicMaterial.h"
    "Model/VolumetricMesh/IsotropicMaterial/StvkIsotropicMaterial.cpp"
    "Model/VolumetricMesh/IsotropicMaterial/StvkIsotropicMaterial.h"
    "Model/VolumetricMesh/IsotropicMaterial/VolumetricMeshENuMaterial.cpp"
    "Model/VolumetricMesh/IsotropicMaterial/VolumetricMeshENuMaterial.h"
)
source_group("Source Files\\Model\\VolumetricMesh\\IsotropicMaterial" FILES ${Source_Files__Model__VolumetricMesh__IsotropicMaterial})

set(Source_Files__Simulator
    "Simulator/BaseSimulator.cpp"
    "Simulator/BaseSimulator.h"
    "Simulator/SimulatorFactor.h"
)
source_group("Source Files\\Simulator" FILES ${Source_Files__Simulator})

set(Source_Files__Simulator__CUDA
    # "Simulator/Cuda/CudaHandle.cpp"
    # "Simulator/Cuda/CudaHandle.h"
    # "Simulator/Cuda/CudaHeader.cuh"
    # "Simulator/Cuda/CudaMatrixOperator.cuh"
    # "Simulator/Cuda/CudaSVD.cuh"
    "Simulator/Cuda/CudaHandle.h"
    "Simulator/Cuda/CudaHandle.cpp"
    "Simulator/Cuda/CudaHeader.cuh"
    "Simulator/Cuda/CudaMatrixOperator.cu"
    "Simulator/Cuda/CudaMatrixOperator.cuh"
    "Simulator/Cuda/CudaSVD.cu"
    "Simulator/Cuda/CudaSVD.cuh"
)
source_group("Source Files\\Simulator\\CUDA" FILES ${Source_Files__Simulator__CUDA})

set(Source_Files__Simulator__CollisionDetection
    "Simulator/CollisionDetection/CollisionDetectionMedialMesh.cpp"
    "Simulator/CollisionDetection/CollisionDetectionMedialMesh.h"
)
source_group("Source Files\\Simulator\\CollisionDetection" FILES ${Source_Files__Simulator__CollisionDetection})

set(Source_Files__Simulator__FiniteElementMethod
    "Simulator/FiniteElementMethod/FemModel.cpp"
    "Simulator/FiniteElementMethod/FemModel.h"
    "Simulator/FiniteElementMethod/FemSimulator.cpp"
    "Simulator/FiniteElementMethod/FemSimulator.h"
    "Simulator/FiniteElementMethod/ImplicitNewMarkSolverConfig.cpp"
    "Simulator/FiniteElementMethod/ImplicitNewMarkSolverConfig.h"
)
source_group("Source Files\\Simulator\\FiniteElementMethod" FILES ${Source_Files__Simulator__FiniteElementMethod})

set(Source_Files__Simulator__FiniteElementMethod__Reduced
    "Simulator/FiniteElementMethod/Reduced/ReducedFrame.cpp"
    "Simulator/FiniteElementMethod/Reduced/ReducedFrame.h"
)
source_group("Source Files\\Simulator\\FiniteElementMethod\\Reduced" FILES ${Source_Files__Simulator__FiniteElementMethod__Reduced})

set(Source_Files__Simulator__mipc
    "Simulator/mipc/MipcConstraint.h"
    "Simulator/mipc/MipcModel.h"
    "Simulator/mipc/MipcSimulator.h"
    "Simulator/mipc/MipcConstraint.cpp"
    "Simulator/mipc/MipcModel.cpp"
    "Simulator/mipc/MipcSimulator.cpp"
    "Simulator/mipc/MPsCCD.cu"
    "Simulator/mipc/MPsCCD.cuh"
    "Simulator/mipc/gpuFunc.cu"
    "Simulator/mipc/gpuFunc.cuh"
)
source_group("Source Files\\Simulator\\mipc" FILES ${Source_Files__Simulator__mipc})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Source_Files}
    ${Source_Files__Common}
    ${Source_Files__Common__DataStructure}
    ${Source_Files__Common__SVD}
    ${Source_Files__Common__tinyxml}
    ${Source_Files__Model}
    ${Source_Files__Model__MedialMesh}
    ${Source_Files__Model__TriangleMesh}
    ${Source_Files__Model__VolumetricMesh}
    ${Source_Files__Model__VolumetricMesh__IsotropicMaterial}
    ${Source_Files__Scene}
    ${Source_Files__Shader}
    ${Source_Files__Shader__Program}
    ${Source_Files__Simulator}
    ${Source_Files__Simulator__CollisionDetection}
    ${Source_Files__Simulator__FiniteElementMethod}
    ${Source_Files__Simulator__FiniteElementMethod__Reduced}
    ${Source_Files__Simulator__mipc}
    ${Source_Files__Model__MedialMesh__Cuda}
    ${Source_Files__Simulator__CUDA}
    # ${Source_Files__Ui}
    ${UI_SRCS}
    ${UI_MOC_HDRS}
    ${Source_Files__Ui__Form}
    ${Source_Files__Resources}
)


################################################################################
# Target
################################################################################
# qt5_wrap_cpp(cbtn ${CMAKE_SOURCE_DIR}/Ui/colorbutton.h TARGET ${PROJECT_NAME})
# add_executable(${PROJECT_NAME} ${ALL_FILES} "SimFramework.qrc" ${cbtn})
add_executable(${PROJECT_NAME} ${ALL_FILES})

# use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
# ################################################################################
# # Includes for CMake from *.props
# ################################################################################
# use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "$ENV{VCTargetsPath}/BuildCustomizations/CUDA 10.0.cmake")
# use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "$ENV{QtMsBuild}/qt_defaults.cmake")
# use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "$ENV{QtMsBuild}/qt.cmake")

# # list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules) 
# # # find_package(QGLViewer)

# set_target_properties(${PROJECT_NAME} PROPERTIES
#     VS_GLOBAL_KEYWORD "QtVS_v301"
# )
# ################################################################################
# # Target name
# ################################################################################
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     TARGET_NAME_DEBUG   "${PROJECT_NAME}"
#     TARGET_NAME_RELEASE "${PROJECT_NAME}"
# )
# ################################################################################
# # Output directory
# ################################################################################
# set_target_properties(${PROJECT_NAME} PROPERTIES
#     OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>"
#     OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>"
# )

# ################################################################################
# # Include directories
# ################################################################################
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${FREETYPE_INCLUDE_DIRS};"
    "${MKL_INCLUDE_DIRS};"
    "${QGLViewer_INCLUDE_DIRS};"
    "${CUDA_INCLUDE_DIRS};"
    "${OPENGL_INCLUDE_DIRS};"
    "${GLUT_INCLUDE_DIRS};"
    "${OpenMP_CXX_INCLUDE_DIRS};"
    "$<$<CONFIG:Debug>:"
        "$ENV{QT_INCLUDE}"
    ">"
    "${CMAKE_CURRENT_SOURCE_DIR}/GeneratedFiles/$<CONFIG>;"
    "${CMAKE_CURRENT_SOURCE_DIR}/GeneratedFiles;"
    "${CMAKE_CURRENT_SOURCE_DIR}/."
    "${Qt_INCLUDE_DIRS}"
    "${BLAS_INCLUDE_DIRS}"
)
message("${BLAS_INCLUDE_DIRS}")

# ################################################################################
# # Dependencies
# ################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    # "$<$<CONFIG:Debug>:"
    #     "QGLViewerd;"
    #     # "qucd"
    # ">"
    # "$<$<CONFIG:Release>:"
    #     "QGLViewer;"
    #     "OpenCL;"
    #     # "quc"
    # ">"
    "${MKL_INCLUDE_DIR};"
    "${BLAS_LIBRARIES};"
    "${OPENGL_LIBRARIES};"
    "${GLUT_LIBRARY};"
    "${CUDA_LIBRARIES};"
    "${CUDA_CUBLAS_LIBRARIES};"
    "${CUDA_cudadevrt_LIBRARY};"
    "${CUDA_cudart_static_LIBRARY};"
    "${CUDA_cusolver_LIBRARY};"
    "${CUDA_cusparse_LIBRARY};"
    "${OpenMP_CXX_LIBRARIES};"
    # "glu32;"
    "freetype;"
    "cuda;"
    "cudadevrt;"
    "cudart;"
    "cudart_static;"
    "cusolver;"
    "cusparse;"
    "cublas;"
    "tinyxml;"
    "mkl_intel_ilp64;"
    "mkl_sequential;"
    "mkl_core;"
    "mkl_intel_lp64;"
    "mkl_intel_thread;"
    # "libiomp5md;"
    "Qt5::Widgets;"
    "Qt5::Xml;"
    "Qt5::Core;"
    "Qt5::OpenGL;"
    "Qt5::OpenGLExtensions;"
    "Qt5::Gui;"
    "Qt5::Designer;"
    "Qt5::UiPlugin;"
    "Eigen3::Eigen;"
    "${FREETYPE_LIBRARIES};"
    "${QGLViewer_LIBRARY};"
    "${CUDA_LIBRARIES};"
    "${MKL_LIBRARIES};"
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE 
    "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if ( TARGET Qt5::Core )
    get_property( core_options TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_OPTIONS )
    string( REPLACE "-fPIC" "" new_core_options "${core_options}" )
    set_property( TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_OPTIONS ${new_core_options} )
    set_property( TARGET Qt5::Core PROPERTY INTERFACE_POSITION_INDEPENDENT_CODE "ON" )
    set( CMAKE_CXX_COMPILE_OPTIONS_PIE "-fPIC" )
endif()
