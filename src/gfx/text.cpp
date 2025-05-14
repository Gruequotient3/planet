#ifndef TEXT_CPP
#define TEXT_CPP

#include "text.hpp"
#include "../world/world.hpp"

Mesh TextMesh::mesh{Mesh::GetQuadMesh()};

TextMesh::TextMesh() : textureId{0}, position{glm::vec3(0.0f)}{ }

void TextMesh::Destroy(){
    if (textureId) { glDeleteTextures(1, &textureId); textureId = 0; }
}

void TextMesh::Init(std::string text) {
    static int firstTime = 1;
    SDL_Color c = {255, 255, 0, 255};
    SDL_Surface * d, * s;
    TTF_Font * font = NULL;
    if(firstTime) {
      /* initialisation de la bibliothèque SDL2 ttf */
      if(TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        exit(2);
      }
      firstTime = 0;
    }
    if(textureId == 0) {
      /* initialisation de la texture côté OpenGL */
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    /* chargement de la font */
    if( !(font = TTF_OpenFont("res/DejaVuSans-Bold.ttf", 64)) ) {
      fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
      return;
    }
    /* création d'une surface SDL avec le texte */
    d = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), c, 2048);
    if(d == NULL) {
      TTF_CloseFont(font);
      fprintf(stderr, "Erreur lors du TTF_RenderText\n");
      return;
    }
    /* copie de la surface SDL vers une seconde aux spécifications qui correspondent au format OpenGL */
    s = SDL_CreateRGBSurface(0, d->w, d->h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    assert(s);
    SDL_BlitSurface(d, NULL, s, NULL);
    SDL_FreeSurface(d);
    /* transfert vers la texture OpenGL */
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, s->pixels);
    SDL_FreeSurface(s);
    TTF_CloseFont(font);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextMesh::Render(Shader &shader){
    shader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    shader.SetMat4("view", GL_FALSE, World::camera.GetViewMatrice());
    shader.SetMat4("projection", GL_FALSE, World::camera.GetPerspectiveMatrice());
    mesh.Draw(shader);
}

#endif