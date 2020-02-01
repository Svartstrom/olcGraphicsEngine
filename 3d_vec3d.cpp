
#include "3d_vec3d.hpp"

vec3d vec3d::operator + (const vec3d &rhs) { return vec3d(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z);}
vec3d vec3d::operator - (const vec3d &rhs) { return vec3d(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);}
vec3d vec3d::operator / (const float rhs)  { return vec3d(this->x / rhs,   this->y / rhs,   this->z / rhs  );}
vec3d vec3d::operator * (const int rhs)    { return vec3d(this->x * rhs,   this->y * rhs,   this->z * rhs  );}
vec3d vec3d::operator * (const float rhs)  { return vec3d(this->x * rhs,   this->y * rhs,   this->z * rhs  );}
float vec3d::operator * (const vec3d &rhs) { return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;}
vec3d vec3d::operator * (const mat4x4 &rhs) 
{
    vec3d out;

    out.x = this->x * rhs.m[0][0] + this->y * rhs.m[1][0] + this->z * rhs.m[2][0] + this->w * rhs.m[3][0];
    out.y = this->x * rhs.m[0][1] + this->y * rhs.m[1][1] + this->z * rhs.m[2][1] + this->w * rhs.m[3][1];
    out.z = this->x * rhs.m[0][2] + this->y * rhs.m[1][2] + this->z * rhs.m[2][2] + this->w * rhs.m[3][2];
    out.w = this->x * rhs.m[0][3] + this->y * rhs.m[1][3] + this->z * rhs.m[2][3] + this->w * rhs.m[3][3];

    return out;
}


vec3d vec3d::cross(const vec3d &rhs)
{
    return vec3d(this->y * rhs.z - this->z * rhs.y,
                 this->z * rhs.x - this->x * rhs.z,
                 this->x * rhs.y - this->y * rhs.x);
}

vec3d vec3d::normal(void)
{
    float l = sqrtf(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    return vec3d(this->x / l, this->y / l, this->z / l);
}

float vec3d::dot(const vec3d &rhs)
{
    return this->x * rhs.x + this->y * rhs.y + this->z * rhs.z;
}

std::ostream& operator<<(std::ostream& os, const vec3d& dt)
{
    os << dt.x << " "<< dt.y << " "<< dt.z;
    return os;
}