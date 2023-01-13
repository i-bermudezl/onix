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
#include "Ray.hpp"
#include "Sphere.hpp"

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
    int width{9};
    int height{9};

    double aspectRatio{static_cast<double>(width) / height};
    double fov{std::numbers::pi / 4};

    constexpr int samplesPerPixel = 100;

    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::mt19937 generator;

    auto frameBuffer = Buffer<Color>{width, height};

    std::vector<std::unique_ptr<Hittable>> world;
    world.emplace_back(std::make_unique<Sphere>(Eigen::Vector3d{0.0, 0.0, -2.0}, 0.5));

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

                // ray trace
                HitRecord record{};
                auto closestSoFar{std::numeric_limits<double>::infinity()};
                bool hitAnything{false};

                Color c{};

                for (const auto &object : world)
                {
                    if (object->hit(r, 0.0, closestSoFar, record))
                    {
                        hitAnything = true;
                        closestSoFar = record.t;
                    }
                }

                // coloring
                if (hitAnything)
                {
                    Eigen::Vector3d vk = record.normal;
                    c = Color{0.5 * (vk.x() + 1.0), 0.5 * (vk.y() + 1.0), 0.5 * (vk.z() + 1.0), 1.0};
                }
                else
                {
                    c = Color{1.0, 1.0, 1.0, 1.0};
                }

                frameBuffer.write(c, j, i);
            }
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