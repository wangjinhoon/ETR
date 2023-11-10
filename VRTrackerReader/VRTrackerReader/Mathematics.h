#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include "openvr.h"
#include "Structs.h"
#include <Eigen>

const int METERTOUNREALUNITS = 100;

struct Plane {
	double x, y, z, w;
};

class Matrix4X4 {
public:
	float M[4][4];
	Matrix4X4(const vr::HmdMatrix34_t& tm) {
		//swap rows and columns
		M[0][0] = tm.m[0][0];
		M[0][1] = tm.m[1][0];
		M[0][2] = tm.m[2][0];
		M[0][3] = 0.f;
		M[1][0] = tm.m[0][1];
		M[1][1] = tm.m[1][1];
		M[1][2] = tm.m[2][1];
		M[1][3] = 0.f;
		M[2][0] = tm.m[0][2];
		M[2][1] = tm.m[1][2];
		M[2][2] = tm.m[2][2];
		M[2][3] = 0.f;
		M[3][0] = tm.m[0][3];
		M[3][1] = tm.m[1][3];
		M[3][2] = tm.m[2][3];
		M[3][3] = 1.f;
	};
};

Quaternion toQuaternion(const Eigen::Matrix4f& M);
Quaternion toQuaternion(const Matrix4X4& M);
RPY calculateYPR(const Quaternion& orientation);
std::vector<float> getPosition(vr::HmdMatrix34_t& matrix);
