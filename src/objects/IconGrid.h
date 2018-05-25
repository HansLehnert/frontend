#pragma once

#include "gl_inc.h"

#include "core/GraphicObject.h"
#include "core/Program.h"
#include "core/Texture.h"
#include "core/Text.h"


class IconGrid : public GraphicObject {
public:
	IconGrid(std::string instance_name = "");

	virtual void update();
	virtual void render();

	//Object properties
	//void setGridColumns();
	//void setGridBounds();
	void setNumIcons(int n);

	//Element manipulation
	bool setIconId(unsigned int index, std::string id);
	bool setIconName(unsigned int index, std::string name);
	bool setIconGraphic(unsigned int index, std::string filename);

	//Input callback
	void handleInput(Event event);

private:
	GLuint model_buffer;

	//GL shaders
	Program* image_program;
	GLuint image_uniform_world_matrix;
	GLuint image_uniform_model_matrix;
	GLuint image_uniform_texture;

	//Icon properties
	struct Icon {
		std::string id;
		std::string name;
		Text name_texture;
		Texture icon_texture;
	};
	std::vector<Icon> icons;

	//Grid properties
	int columns;
	float icon_ratio;
	float icon_margin;
	float icon_width;
	float icon_height;

	//Control variables
	int cursor_pos;
};
