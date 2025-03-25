#ifndef BOOST_H_INCLUDED
#define BOOST_H_INCLUDED

#include "Const.h"
#include "Entity.h"

struct Boost : public Entity {
private:
    SDL_Texture* tex;
public:
    Uint32 existTime;
    Uint32 lastingTime;
    void destroy() {if (tex) { SDL_DestroyTexture(tex); isDestroyed = true;} }
};

#endif // BOOST_H_INCLUDED
