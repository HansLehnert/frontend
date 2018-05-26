#pragma once

#include <string>
#include <memory>

#include "../gl_inc.h"
#include "glm/glm.hpp"

#include "core/GraphicObject.h"
#include "core/Texture.h"
#include "core/Program.h"

class Image: public GraphicObject {
public:
	/**
	 * @brief Construct a new Image object
	 */
	Image(std::string image_file = "", std::string instance_name = "");

	virtual void render();

	/**
	 * @brief Loads an image file to load as content.
	 */
	int setContent(std::string image_file);


	/**
	 * @brief Sets the bounding box size used to fit the image
	 */
	void setBounds(glm::vec2 bounds);

protected:
	Texture texture;

private:
	std::string content;
	glm::vec2 bounds;

	static GLuint model_buffer;

	void updateSize();
};
