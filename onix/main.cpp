#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

#include <random>

#include <Eigen/Dense>
#include <OpenEXR/ImfRgbaFile.h>

#include "Buffer.hpp"
#include "Color.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"

std::uniform_real_distribution<double> distribution(-1.0, 1.0);
std::mt19937 generator;

Eigen::Vector3d VectorInUnitSphere()
{
    while (true)
    {
        auto v = Eigen::Vector3d{distribution(generator), distribution(generator), distribution(generator)};
        if (v.dot(v) >= 1.0)
        {
            continue;
        }
        return v;
    }
}

Eigen::Vector3d rayColor(const Ray &r, const Hittable &world, int depth)
{
    HitRecord record;

    // if we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return Eigen::Vector3d(0.0, 0.0, 0.0);
    }

    if (world.hit(r, 0.001, std::numeric_limits<double>::infinity(), record))
    {
        Eigen::Vector3d v{VectorInUnitSphere()};
        v.normalize();
        Eigen::Vector3d target = record.p + record.normal + v;
        return 0.5 * rayColor(Ray{record.p, target - record.p}, world, depth - 1);
    }

    Eigen::Vector3d unitDirection{r.getDirection()};
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * Eigen::Vector3d{1.0, 1.0, 1.0} + t * Eigen::Vector3d{0.5, 0.7, 1.0};
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

    constexpr int samplesPerPixel = 100;

    constexpr int maxDepth = 100;

    auto frameBuffer = Buffer<Color>{width, height};

    HittableList world{};
    world.objects.emplace_back(std::make_shared<Sphere>(Eigen::Vector3d{0.0, 0.0, -2.0}, 0.5));
    world.objects.emplace_back(std::make_shared<Sphere>(Eigen::Vector3d{0.0, -100.5, -2.0}, 100.0));

    auto sr{std::chrono::steady_clock::now()};
    std::cout << "Begining rendering...\n";
    for (int i{0}; i < height; i++)
    {
        for (int j{0}; j < width; j++)
        {
            Eigen::Vector3d color{0.0, 0.0, 0.0};
            for (int k{0}; k < samplesPerPixel; k++)
            {
                // pixel space
                auto pX{static_cast<double>(j) + 0.5};
                auto pY{static_cast<double>(i) + 0.5};

                auto u{pX + distribution(generator)};
                auto v{pY + distribution(generator)};

                // ndc
                auto dX{u / width};
                auto dY{v / height};

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

                color += rayColor(r, world, maxDepth);
            }
            color /= samplesPerPixel;
            frameBuffer.write(Color{color.x(), color.y(), color.z(), 1.0}, j, i);
        }
    }
    auto er{std::chrono::steady_clock::now()};
    auto dr{std::chrono::duration_cast<std::chrono::milliseconds>(er - sr)};
    std::cout << "Rendering finished. Took: " << dr << '\n';

    auto sf{std::chrono::steady_clock::now()};
    std::cout << "Saving to file...\n";
    saveToFile("render.exr", frameBuffer);
    auto ef{std::chrono::steady_clock::now()};
    auto df{std::chrono::duration_cast<std::chrono::milliseconds>(ef - sf)};
    std::cout << "Saving image finished. Took: " << df << '\n';

    std::cout << "Press any key to finish...\n";
    std::cin.get();

    return 0;
}