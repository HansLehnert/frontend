#include <iostream>

#include <dirent.h>
#include <unistd.h>

#include "Context.h"
#include "AppConfig.hpp"
#include "ui/Marquee.hpp"
#include "ui/Background.hpp"
#include "ui/Label.hpp"
#include "core/Text.hpp"

#include "util/string.hpp"


//Creates directory
//(Currently only works on UNIX system)
bool createDir(std::string dir_name) {
    DIR* dir = opendir(dir_name.c_str());

    if (dir == NULL) {
        closedir(dir);

        std::string command = "mkdir " + dir_name;

        if (system(command.c_str()) == 0)
            return true;
        else
            return false;
    }
    else {
        closedir(dir);
        return false;
    }
}


int main() {
    Context context;
    if (!context.init()) {
        return 0;
    }


    AppConfig app_config("settings.yaml");

    // Initialize freetype
    initFreetype();
    loadTypeface(app_config.font());
    std::cout << app_config.font() << std::endl;

    // Adjust world matrix to correct aspect ratio and rotate screen
    GraphicObject::world_matrix = glm::transpose(glm::mat4({
        2, 0,                             0, -1,
        0, -2 * context.getAspectRatio(), 0, 1,
        0, 0,                             1, 0,
        0, 0,                             0, 1,
    }));

    Object base("base");
    Background background;
    background.fit(1, 1 / context.getAspectRatio(), 0.3);
    base.addChild(background);

    Label label;
    label.setText("Hola");
    label.setLineHeight(0.1);
    label.position = glm::vec3(0.5, 0.5, 0);
    base.addChild(label);

    Marquee marquee(Texture::fromFile("screenshot/outrun.png"));
    marquee.setSize(1, 0.4);
    base.addChild(marquee);

    Image image(Texture::fromFile("logo/outrun.png"));
    image.origin = Plane::Origin::CENTER;
    image.fitBounds(1, 0.2);
    image.x = 0.5;
    image.y = 0.2;
    base.addChild(image);

    int counter = 0;

    while (context.poll()) {
        context.swapBuffers();

        base.update();

        counter++;
    }


    //Create folders
    // if (createDir("game"))
    // 	std::cout << "[System]\tCreated 'game' directory" << std::endl;


    // GameManager game_manager(&context, &main_config);

    /*Image title_image;
    Marquee marquee;
    Background background(6);*/

    //Game selection (temporal)
    /*int selection = 0;

    if (game_manager.getGameList()->size() > 0) {
        title_image.setContent((game_manager.getGameList()->begin()->second)["game_logo"]);
        marquee.updateImage((game_manager.getGameList()->begin()->second)["screenshot"]);
    }

    marquee.addListener(EVENT_INPUT_KEYDOWN, [&selection, &game_manager, &marquee, &title_image] (Event event) {
        if (event.input.key == KEY_DOWN) {
            selection++;
        }
        else if (event.input.key == KEY_UP) {
            selection--;
        }

        if (selection < 0) {
            selection = 0;
        }
        if (selection >= game_manager.getGameList()->size()) {
            selection = game_manager.getGameList()->size() - 1;
        }

        auto game = game_manager.getGameList()->begin();
        for (int i = 0; i < selection; i++)
            game++;

        marquee.updateImage((game->second)["screenshot"]);
        if (title_image.setContent((game->second)["game_logo"])) {
            title_image.visible = true;
        }
        else {
            title_image.visible = false;
        }

        if (event.input.key == KEY_SELECT) {
            game_manager.launchGame(game->first);
        }
    });




    float marquee_position = context.getAspectRatio() - 0.5;

    marquee.setPosition(glm::vec3(0, marquee_position, 0));
    title_image.setPosition(glm::vec3(0, marquee_position, 0));


    //Main loop
    while (context.poll()) {
        context.swapBuffers();
        Object::updateAll();
        GraphicObject::renderAll();
    }

    return 1;*/
}
