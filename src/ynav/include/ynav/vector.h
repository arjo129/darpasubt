#ifndef YNAV_VECTOR_H
#define YNAV_VECTOR_H

#include <cmath>

namespace YNav
{

class Vector
{
public:
  double x;
  double y;

  Vector(double x, double y) : x(x), y(y) { }
  Vector() : Vector(0, 0) { }
  static Vector fromAngle(double angle) { return Vector(std::cos(angle), std::sin(angle)); }

  friend Vector operator-(const Vector& v) { return Vector(-v.x, -v.y); }
  friend Vector operator+(const Vector& v, const Vector& w) { return Vector(v.x + w.x, v.y + w.y); }
  friend Vector& operator+=(Vector& v, const Vector& w) { v.x += w.x; v.y += w.y; return v; }
  friend Vector operator-(const Vector& v, const Vector& w) { return Vector(v.x - w.x, v.y - w.y); }
  friend Vector& operator-=(Vector& v, const Vector& w) { v.x -= w.x; v.y -= w.y; return v; }
  friend Vector operator*(double a, const Vector& v) { return Vector(a * v.x, a * v.y); }
  friend Vector operator*(const Vector& v, double a) { return Vector(a * v.x, a * v.y); }
  friend Vector& operator*=(Vector& v, double a) { v.x *= a; v.y *= a; return v; }
  friend Vector operator/(const Vector& v, double a) { return Vector(v.x / a, v.y / a); }
  friend Vector& operator/=(Vector& v, double a) { v.x /= a; v.y /= a; return v; }

  double norm2() const { return this->x * this->x + this->y * this->y; }
  double norm() const { return std::sqrt(this->norm2()); }
  Vector unitVector() const { return *this / this->norm(); }
  double dotProduct(const Vector& w) const { return this->x * w.x + this->y * w.y; }
  double angle() const
  {
    double a = std::atan2(this->y, this->x); 
    if (a == M_PI) a = -M_PI;
    return a;
  }
  double angleTo(const Vector& w) const
  {
    double angle = w.angle() - this->angle();
    angle = fmod(angle + M_PI, 2 * M_PI);
    if (angle < 0) 
      angle += 2 * M_PI;
    return angle - M_PI;
  }
  Vector rotate(double angle) const { 
    return Vector(
        std::cos(angle) * this->x - std::sin(angle) * this->y,
        std::sin(angle) * this->x + std::cos(angle) * this->y); 
  }
};

}

#endif
