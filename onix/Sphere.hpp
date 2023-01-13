#pragma once

#include <Eigen/Dense>

#include "Hittable.hpp"
#include "Ray.hpp"

class Sphere : public Hittable
{
  public:
    Sphere() = default;
    explicit Sphere(const Eigen::Vector3d &c, double r);

    virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &hRecord) const override;

  private:
    Eigen::Vector3d center;
    double radius;
};

Sphere::Sphere(const Eigen::Vector3d &c, double r) : center{c}, radius{r}
{
}

inline bool Sphere::hit(const Ray &r, double tMin, double tMax, HitRecord &hRecord) const
{
    Eigen::Vector3d oc{r.getOrigin() - center};
    double a{r.getDirection().dot(r.getDirection())};
    double halfB{oc.dot(r.getDirection())};
    double c{oc.dot(oc) - radius * radius};

    double discriminant{halfB * halfB - a * c};

    if (discriminant < 0.0)
    {
        return false;
    }

    double sqrtd{sqrt(discriminant)};

    // find the nearest root that lies in the acceptable range.
    double root{(-halfB - sqrtd) / a};
    if (root < tMin || tMax < root)
    {
        root = (-halfB + sqrtd) / a;
        if (root < tMin || tMax < root)
        {
            return false;
        }
    }

    hRecord.t = root;
    hRecord.p = r.at(hRecord.t);
    Eigen::Vector3d outwardNormal{(hRecord.p - center) / radius};
    hRecord.setFaceNormal(r, outwardNormal);

    return true;
}
