find_package(OpenEXR CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

add_executable (onix "main.cpp" "Buffer.hpp" "Color.hpp" "Ray.hpp" "Sphere.hpp" "Hittable.hpp")

target_link_libraries(onix PRIVATE OpenEXR::OpenEXR)
target_link_libraries(onix PRIVATE Eigen3::Eigen)

if (CMAKE_VERSION VERSION_GREATER 3.24)
  set_property(TARGET onix PROPERTY CXX_STANDARD 20)
endif()
