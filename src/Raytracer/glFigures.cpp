#include "../../include/Raytracer/glFigures.hpp"
#include <cmath>
#include <vector>
#include <algorithm>

Sphere::Sphere (std::vector<float> center, float radius, Material material)
{
    this->center = center;
    this->radius = radius;
    this->material = material;
}

Intersect Sphere::ray_intersect (std::vector<float> orig, std::vector<float> dir)
{
    std::vector<float> L = substract(center, orig);
    float tca = dot(L, dir);
    float d = std::sqrt((std::pow(magnitude(L), 2) - std::pow(tca,2)));

    if (d > radius){
        return Intersect {};
    }

    float thc = std::sqrt((std::pow(radius, 2) - std::pow(d, 2)));

    float t0 = tca - thc;
    float t1 = tca + thc;

    if (t0 < 0) {
        t0 = t1;
    }
    if (t0 < 0) {
        return Intersect {};
    }

    std::vector<float> P = add(orig, mult(t0, dir));
    std::vector<float> normal = substract(P, center);
    normal = normalize(normal);

    return Intersect {false, t0, P, normal, this};

}

Material Sphere::getMaterial()
{
    return this->material;
}

Plane::Plane(std::vector<float> position, std::vector<float> normal, Material material)
{
    this->normal = normal;
    this->position = position;
    this->material = material;
}

Intersect Plane::ray_intersect(std::vector<float> orig, std::vector<float> dir)
{
    float denom = dot(dir, normal);
    if (std::abs(denom) < 0.0001) {
        return {};
    }

    float num = dot(substract(this->position, orig), normal);
    float t = num/denom;

    if (t < 0) {
        return {};
    }

    std::vector<float> P = add(orig, mult(t, dir));

    return Intersect {false, t, P, normal, this};
}

Material Plane::getMaterial()
{
    return this->material;
}


MineCube::MineCube (std::vector<float> size, std::vector<float> position, Material material)
{
    this->position = position;
    this->size = size;
    this->material = material;

    std::vector<float> halfSizes {
        size.at(0) / 2,
        size.at(1) / 2,
        size.at(2) / 2,
    };

    this->planes.push_back(Plane (add(position, {halfSizes.at(0), 0, 0}), {1, 0, 0}, material));
    this->planes.push_back(Plane (add(position, {-halfSizes.at(0), 0, 0}), {-1, 0, 0}, material));

    this->planes.push_back(Plane (add(position, {0, halfSizes.at(1), 0}), {0, 1, 0}, material));
    this->planes.push_back(Plane (add(position, {0, -halfSizes.at(1), 0}), {0, -1, 0}, material));

    this->planes.push_back(Plane (add(position, {0, 0, halfSizes.at(2)}), {0, 0, 1}, material));
    this->planes.push_back(Plane (add(position, {0, 0, -halfSizes.at(2)}), {0, 0, -1}, material));

    float epsilon = 0.0001;

    for (size_t i = 0; i < 3; i++) {
        this->boundMin[i] = position[i] - (epsilon + halfSizes.at(i));
        this->boundMax[i] = position[i] + (epsilon + halfSizes.at(i));
    }
}

Intersect MineCube::ray_intersect(std::vector<float> orig, std::vector<float> dir)
{
    Intersect intersect {};
    float t = INFINITY;
    for (auto& plane : this->planes) {
        Intersect planeInter = plane.ray_intersect(orig, dir);
        if (!planeInter.null) {
            std::vector<float> planePoint = planeInter.point;
            if (boundMin.at(0) <= planePoint.at(0) && boundMax.at(0) >= planePoint.at(0))
                if (boundMin.at(1) <= planePoint.at(1) && boundMax.at(1) >= planePoint.at(1))
                    if (boundMin.at(2) <= planePoint.at(2) && boundMax.at(2) >= planePoint.at(2))
                    {
                        if (planeInter.distance < t) {
                            t = planeInter.distance;
                            intersect = planeInter;
                        }
                    }

        }
    }

    if (intersect.null) {
        return intersect;
    }

    return {false, t, intersect.point, intersect.normal, this};
}

Material MineCube::getMaterial()
{
    return this->material;
}

Triangle::Triangle(std::vector<float> A, std::vector<float> B, std::vector<float> C, Material material)
{
    this->A = A;
    this->B = B;
    this->C = C;

    std::vector<float> edge1 = substract(B, A);
    std::vector<float> edge2 = substract(C, A);

    std::vector<float> triangleNormal = cross(edge1, edge2);
    triangleNormal = normalize(triangleNormal);

    this->plane = {{},triangleNormal, material};
}

Intersect Triangle::ray_intersect(std::vector<float> orig, std::vector<float> dir)
{
    float t = INFINITY;
    Intersect planeInter = plane.ray_intersect(orig, dir);
    if (!planeInter.null) {
            std::vector<float> planePoint = planeInter.point;

            std::array<float, 3> Xvals = {A.at(0), B.at(0), C.at(0)};
            std::array<float, 3> Yvals = {A.at(1), B.at(1), C.at(1)};

            long minX = (long) std::round(*std::min_element(Xvals.begin(), Xvals.end()));
            long minY = (long) std::round(*std::min_element(Yvals.begin(), Yvals.end()));
            long maxX = (long) std::round(*std::max_element(Xvals.begin(), Xvals.end()));
            long maxY = (long) std::round(*std::max_element(Yvals.begin(), Yvals.end()));

            for (long x = minX; x <= maxX; x++) {
                for (long y = minY; y <= maxY; y++) {
                    std::array<float, 3> bCoords = baryCoords(A, B, C, {(float) x, (float) y});

                    if (bCoords.at(0) >= 0 && bCoords.at(1) >= 0 && bCoords.at(2) >= 0) {

                        float z = A.at(2) * bCoords.at(0) + B.at(2) * bCoords.at(1) + C.at(2) * bCoords.at(2);


}