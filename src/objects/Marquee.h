#pragma once

#include "core/Object.h"

#include "core/Texture.h"

class Marquee : public Object {
public:
	Marquee(std::string instance_name = "");

	void update();
	void render();

	void updateImage(std::string filename);
private:
	GLuint model_buffer;

	//Uniform location
	GLuint uniform_model_matrix;
	GLuint uniform_world_matrix;
	GLuint uniform_current_aspect;
	GLuint uniform_next_aspect;
	GLuint uniform_progress;
	GLuint uniform_current_image;
	GLuint uniform_next_image;
	GLuint uniform_mask_image;

	//Textures
	Texture transition_mask;
	Texture current_image;
	Texture next_image;

	//Aspect ratio of the marquee
	float dimension_aspect;
	float current_aspect;
	float next_aspect;

	bool queued_image;
	std::string queued_filename;

	float progress;
	float transition_speed;

	void generateTransitionMask(int width, int height);
};