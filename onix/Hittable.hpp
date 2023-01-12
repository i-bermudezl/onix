#prama once

#include <Eigen/Dense>

#include "Ray.hpp"

struct HitRecord
{
    Eigen::Vector3d p;
    Eigen::Vector3d normal;
    double t;
};

struct Hittable
{
    virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const = 0;
};