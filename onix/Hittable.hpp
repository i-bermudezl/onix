#pragma once

#include <memory>

#include <Eigen/Dense>

#include "Ray.hpp"

struct Material;

struct HitRecord
{
    HitRecord() = default;
    explicit HitRecord(Eigen::Vector3d p, Eigen::Vector3d normal, double t);
    Eigen::Vector3d p;
    Eigen::Vector3d normal;
    std::shared_ptr<Material> material;
    double t;
    bool frontFace;

    inline void setFaceNormal(const Ray &r, const Eigen::Vector3d outwardNormal);
};

HitRecord::HitRecord(Eigen::Vector3d p, Eigen::Vector3d normal, double t) : p{p}, normal{normal}, t{t}
{
}

inline void HitRecord::setFaceNormal(const Ray &r, const Eigen::Vector3d outwardNormal)
{
    frontFace = r.getDirection().dot(outwardNormal) < 0.0;
    normal = frontFace ? outwardNormal : -outwardNormal;
}

struct Hittable
{
    virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &rec) const = 0;
};