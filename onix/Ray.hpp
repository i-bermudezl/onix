#pragma once

#include <Eigen/Dense>

class Ray
{
  public:
    explicit Ray(const Eigen::Vector3d &o, const Eigen::Vector3d &d);

    Eigen::Vector3d getOrigin() const;
    Eigen::Vector3d getDirection() const;

    Eigen::Vector3d at(double t) const;

  private:
    Eigen::Vector3d origin;
    Eigen::Vector3d direction;
};

Ray::Ray(const Eigen::Vector3d &o, const Eigen::Vector3d &d) : origin{o}, direction{d}
{
}

inline Eigen::Vector3d Ray::getOrigin() const
{
    return origin;
}

inline Eigen::Vector3d Ray::getDirection() const
{
    return direction;
}

inline Eigen::Vector3d Ray::at(double t) const
{
    return origin + t * direction;
}
