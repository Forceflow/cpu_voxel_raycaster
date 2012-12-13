#ifndef LIGHT_H_
#define LIGHT_H_

#include "util.h"

class Light {
public:

	bool active;
	vec3 position;
	vec3 diffuse;
	vec3 specular;
	
	// light constants
	float CONSTANT_ATTENUATION;
	float LINEAR_ATTENUATION;
	float QUADRATIC_ATTENUATION;

	// other light constants
	float SHININESS;

	Light(vec3 position,vec3 diffuse, vec3 specular);
	virtual ~Light();

private:
	void initLight();
};

inline Light::Light(vec3 position = vec3(0,0,0),vec3 diffuse =vec3(1.0f,1.0f,1.0f), vec3 specular = vec3 (1.0f, 1.0f, 1.0f))
	: position(position), diffuse(diffuse), specular(specular) {
		initLight();
}

inline Light::~Light() {
	// TODO Auto-generated destructor stub
}

inline void Light::initLight(){
	active = true;
	this->CONSTANT_ATTENUATION = 1.0f;
	this->LINEAR_ATTENUATION = 0.0f;
	this->QUADRATIC_ATTENUATION = 0.0f;
	this->SHININESS = 50.0f;
}


#endif /* LIGHTOCTREERENDERER_H_ */