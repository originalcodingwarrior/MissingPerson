#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
using namespace std;

class Rectangle {
protected:
    Uint32 color;
    SDL_Rect rect;
    string text;
    SDL_Surface* image;
public:
    bool isVisible;

    //생성자
    Rectangle(Uint32 color, string text, int x, int y, int w, int h);
    //Rectangle(SDL_Surface* image, string text, int x, int y, int w, int h);
    Rectangle();

    //사각형 그리기
    void render(SDL_Surface* surface, TTF_Font* font);

    void setVisible(bool visible);
    void setText(string text);
    void setRect(SDL_Rect rect);

    SDL_Rect getRect();

};