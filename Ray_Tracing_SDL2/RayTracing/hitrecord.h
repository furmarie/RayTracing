#ifndef HITRECORD_H
#define HITRECORD_H

struct hitRecord {
	vec3 intPoint{ 3 };
	vec3 localNormal{ 3 };
	vec3 localColour{ 3 };
	std::shared_ptr<fRT::objectBase> obj;
};


#endif