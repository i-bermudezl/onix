#pragma once

#include <Eigen/Dense>

struct HitRecord;

struct Material
{
    virtual bool scatter(const Ray &in, const HitRecord &record, Eigen::Vector3d &attenuation,
                         Ray &scattered) const = 0;
};