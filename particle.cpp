#include <iostream>
#include <time.h>
#include <unistd.h>
#include "particle.h"

using cimg_library::CImg;
using cimg_library::CImgDisplay;
using std::vector;


static const double pi = 3.14159265359;
static const double pi18 = (pi/18);
static const double pi9 = (pi/9);
static const double pi_ = pi + pi;
static const double cos_pi = std::cos(pi9);
static const double sin_pi = std::sin(pi9);
static const unsigned char col[3] = {1,2,2};
double Particle::time = 0;


double GetMonotonicTime() {
  timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  const double time =
  static_cast<double>(ts.tv_sec) + static_cast<double>(ts.tv_nsec)*(1.0E-9);
  return time;
}

double Rand() {
  return rand() / static_cast<double>(RAND_MAX);
}

double Rand2(double randi){
  return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX/randi));
}


Particle::Particle() {
    Set();
    const double alpha = 9.0 + (Rand() * (2.0));
    const double phi = (Rand() * (pi18));
    const double theta = (Rand() * (pi_));
    const double s_phi = std::sin(phi);
    const double s_alpha = alpha * s_phi;
    v0.SetX(s_alpha * std::cos(theta));
    v0.SetY(s_alpha * std::sin(theta));
    v0.SetZ(alpha * std::cos(phi));
    //v0 = Vector3(s_alpha * std::cos(theta) , s_alpha * std::sin(theta) , alpha * std::cos(phi));
    t0 = GetMonotonicTime();
    time = t0;
}


void Particle::Update() {
  static Vector3 g2(0,0,-4.9);
  double delta_t = time - t0;
  if(delta_t == 0){
    delta_t = GetMonotonicTime() - t0;
  }
  p = p0 + (v0 * delta_t) + (g2 * (delta_t * delta_t));
}

bool Particle::Reflect() {
  if(p.GetZ() <= 0){
    t0 = time;
    p.SetZ(p.GetZ() * -1);
    p0 = p;
    v0.SetZ(v0.GetZ() * 0.25);
    return true;
  }
  return false;
}

void Particle::Set(){
    p0.SetX(0);
    p0.SetY(0);
    p0.SetZ(0);
}

bool Particle::CheckAndReset() {
  if(v0.GetZ() < 0.05){ 
    Set();
    const double alpha = 9.0 + (Rand() * (2.0));
    const double phi = (Rand() * (pi18));
    const double theta = (Rand() * (pi_));
    const double s_phi = std::sin(phi);
    const double s_alpha = alpha * s_phi;
    v0.SetX(s_alpha * std::cos(theta));
    v0.SetY(s_alpha * std::sin(theta));
    v0.SetZ(alpha * std::cos(phi));

    //v0 = Vector3(s_alpha * std::cos(theta) , s_alpha * std::sin(theta) , alpha * std::cos(phi));
    t0 = time;
    return true;
  }
  return false;
}

void TransformPoint(const Vector3& p, Vector3* p_img) {
  const double x = (60 * p.GetX()) + 300;
  const double y = 400 - 60 *((cos_pi * p.GetZ()) - (sin_pi * p.GetY()));
  Vector3 v;
  v.SetX(x);
  v.SetY(y);
  *p_img = v;
}

void PointShadow(const Vector3& point, Vector3* p_shadow) {
  const double x = (60 * point.GetX()) + 300;
  const double y = 400 - 60 *(sin_pi * point.GetY());
  Vector3 v;
  v.SetX(x);
  v.SetY(y);
  *p_shadow = v;
  
}


void DrawParticles(const std::vector<Particle>& particles,
                   CImg<unsigned char>* img_ptr) {
  Vector3* v = new Vector3();
  for(Particle p : particles){
    TransformPoint(p.p , v);
    img_ptr->draw_circle(v->GetX() , v->GetY() , 1, col , 1);
  }
  Particle::time = GetMonotonicTime();
  delete v;
  //vector<Particle>::iterator iter = particles.begin();
  //while(iter!=particles.end())

}


void DrawShadows(const std::vector<Particle>& particles,
                     CImg<unsigned char>* img_ptr) {
  
  Vector3* v = new Vector3();
  for(Particle p : particles){
    PointShadow(p.p , v);
    img_ptr->draw_circle(v->GetX() , v->GetY() , 1, col , 0.3);
  }
  delete v;

}  


