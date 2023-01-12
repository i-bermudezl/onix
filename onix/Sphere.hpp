#pragma once

#include <Eigen/Dense>

#include "Ray.hpp"

class Sphere
{
  public:
    explicit Sphere(Eigen::Vector3d c, double r);

    bool hit(const Ray &r, double tMin, double tMax);

  private:
    Eigen::Vector3d center;
    double radius;
};

Sphere::Sphere(Eigen::Vector3d c, double r) : center{c}, radius{r}
{
}

inline bool Sphere::hit(const Ray &r, double tMin, double tMax)
{
    auto oc = r.getOrigin() - center;
    auto a = r.getDirection().dot(r.getDirection());
    auto halfB = oc.dot(r.getDirection());
    auto c = oc.dot(oc) - radius * radius;

    auto discriminant = halfB * halfB - a * c;
    if (discriminant < 0)
    {

        return false;
    }

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-halfB - sqrtd) / a;
    if (root < tMin || tMax < root)
    {
        root = (-halfB + sqrtd) / a;
        if (root < tMin || tMax < root)
        {
            return false;
        }
    }

    return true;
}
