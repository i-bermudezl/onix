find_package(OpenEXR CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

add_executable (onix "main.cpp" "Buffer.hpp" "Color.hpp" "Ray.hpp" "Sphere.hpp" "Hittable.hpp" "HittableList.hpp" "Material.hpp")

target_link_libraries(onix PRIVATE OpenEXR::OpenEXR)
target_link_libraries(onix PRIVATE Eigen3::Eigen)
