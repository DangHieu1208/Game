#include "Button.h"

bool Button::isClicked(int mouseX, int mouseY) {
    return (mouseX >= dst.x && mouseX <= dst.x + dst.w &&
            mouseY >= dst.y && mouseY <= dst.y + dst.h);
}

void Button::renderButton(SDL_Renderer* ren, SDL_Rect camera) {
    render(ren, dst, camera);

}
