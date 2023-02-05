#pragma once

#include <memory>
#include <vector>

#include "Hittable.hpp"
#include "Ray.hpp"

struct HittableList : public Hittable
{
    virtual bool hit(const Ray &r, double tMin, double tMax, HitRecord &record) const override;

    std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::hit(const Ray &r, double tMin, double tMax, HitRecord &record) const
{
    HitRecord tmp;
    bool hittedAnything = false;
    auto closestSoFar = tMax;

    for (const auto &object : objects)
    {
        if (object->hit(r, tMin, closestSoFar, tmp))
        {
            hittedAnything = true;
            closestSoFar = tmp.t;
            record = tmp;
        }
    }

    return hittedAnything;
}