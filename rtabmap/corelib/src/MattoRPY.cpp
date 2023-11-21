
#include "rtabmap/core/MattoRPY.h"

#define M_PI       3.14159265358979323846   // pi

MattoRPY::MattoRPY(const Eigen::Matrix4f& transformation)
{
    Eigen::Vector3f translation = transformation.block<3, 1>(0, 3);
    Eigen::Matrix3f rotationMatrix = transformation.block<3, 3>(0, 0);

    Eigen::Vector3f euler = rotationMatrix.eulerAngles(0, 1, 2); // 0 : Roll, 1 : Pitch, 2 : Yaw

    //std::cout << "Translation: " << translation.transpose() << std::endl;
    //std::cout << "x : " << translation(0) << "y : " << translation(1) << "z : " << translation(2) << std::endl;
    //std::cout << "Roll: " << euler(0) << " Pitch: " << euler(1) << " Yaw: " << euler(2) << std::endl;

    //x_ = translation(0);
    //y_ = translation(1);
    //z_ = translation(2);
    //roll_ = euler(0);
    //pitch_ = euler(1);
    //yaw_ = euler(2);

    x_ = translation(1);
    y_ = translation(2);
    z_ = translation(0);
    roll_ = euler(0) * (180 / M_PI);
    pitch_ = euler(1) * (180 / M_PI);
    yaw_ = euler(2) * (180 / M_PI);

}

std::vector<float> MattoRPY::toVector() const
{
    return { x_, y_, z_, roll_, pitch_, yaw_ };
}