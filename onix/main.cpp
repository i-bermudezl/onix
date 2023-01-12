#include <chrono>
#include <cmath>
#include <iostream>
#include <numbers>

#include <Eigen/Dense>
#include <OpenEXR/ImfRgbaFile.h>

#include "Buffer.hpp"
#include "Color.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"

Sphere sphere{Eigen::Vector3d{0.0, 0.0, -2.0}, 0.5};

Color trace(const Ray &ray, int depth)
{
    if (sphere.hit(ray, 0.0, std::numeric_limits<double>::infinity()))
    {
        return Color{1.0, 0.0, 0.0, 1.0};
    }

    return Color{1.0, 1.0, 1.0, 1.0};
}

void saveToFile(std::string name, const Buffer<Color> &buffer)
{
    auto width{buffer.getWidth()};
    auto height{buffer.getHeight()};

    Imf::RgbaOutputFile file{name.c_str(), width, height};
    for (int i = 0; i < height; i++)
    {
        std::vector<Imf::Rgba> colorRow(width);
        for (int j = 0; j < width; j++)
        {
            auto c{buffer.read(j, i)};
            colorRow[j] = Imf::Rgba(c.r, c.g, c.b, c.a);
        }
        file.setFrameBuffer(colorRow.data(), 1, 0);
        file.writePixels();
    }
}

int main(int argc, char **argv)
{
    int width{1920};
    int height{1080};

    double aspectRatio{static_cast<double>(width) / height};
    double fov{std::numbers::pi / 4};

    auto frameBuffer = Buffer<Color>{width, height};

    auto start{std::chrono::steady_clock::now()};
    std::cout << "Begining rendering...\n";
    for (int i{0}; i < height; i++)
    {
        for (int j{0}; j < width; j++)
        {
            // pixel space
            auto pX{static_cast<double>(j)};
            auto pY{static_cast<double>(i)};

            // ndc
            auto dX{(pX + 0.5) / width};
            auto dY{(pY + 0.5) / height};

            // screen space
            auto sX{2 * dX - 1};
            auto sY{1 - 2 * dY};

            // camera space
            auto cX{sX * aspectRatio * tan(fov / 2)};
            auto cY{sY * tan(fov / 2)};

            // generate ray
            auto origin{Eigen::Vector3d{0.0, 0.0, 0.0}};
            auto direction{Eigen::Vector3d{cX, cY, -1}};
            direction.normalize();
            Ray r{origin, direction};

            auto c{trace(r, 0)};

            frameBuffer.write(c, j, i);
        }
    }
    auto end{std::chrono::steady_clock::now()};
    auto delta{std::chrono::duration_cast<std::chrono::milliseconds>(end - start)};
    std::cout << "Rendering finished. Took: " << delta << '\n';

    saveToFile("render.exr", frameBuffer);

    return 0;
}