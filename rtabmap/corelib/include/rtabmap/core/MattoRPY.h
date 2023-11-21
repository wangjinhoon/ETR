#pragma once

#include <iostream>
#include <Eigen/Dense>

class MattoRPY {
public:
    MattoRPY(const Eigen::Matrix4f& transformationMatrix);
    std::vector<float> toVector() const;
    float x_;
    float y_;
    float z_;
    float roll_;
    float pitch_;
    float yaw_;

private:

};