#include <SDL.h>
#include <SDL_ttf.h>
#include "rectangle.h"


Rectangle::Rectangle(Uint32 color, string text, int x, int y, int w, int h) {
    rect = { x, y, w, h };
    this->color = color;
    this->text = text;
    isVisible = true;   //처음에 보임
}

//Rectangle::Rectangle(SDL_Surface* image, string text, int x, int y, int w, int h) {
//    rect = { x, y, w, h };
//    this->image = image;
//    this->text = text;
//    isVisible = true;   //처음에 보임
//}

Rectangle::Rectangle() {
    isVisible = false;
}

void Rectangle::render(SDL_Surface* surface, TTF_Font* font) {
    if (!isVisible) return; // isVisible이 false이면 렌더링하지 않음

    SDL_FillRect(surface, &rect, color);

    // 텍스트 렌더링
    // 텍스트 크기 계산
    int textWidth, textHeight;
    TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);

    // 텍스트 위치 계산
    int textX = rect.x + (rect.w - textWidth) / 2; // 수평 가운데 정렬
    int textY = rect.y + (rect.h - textHeight) / 2; // 수직 가운데 정렬

    // 텍스트 출력
    SDL_Color textColor = { 0, 0, 0 }; // 검정색
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Rect textRect = { textX, textY, 0, 0 };
    SDL_BlitSurface(textSurface, NULL, surface, &textRect);
    SDL_FreeSurface(textSurface);
}

void Rectangle::setVisible(bool visible) {
    isVisible = visible;
}

void Rectangle::setText(string text) {
    this->text = text;
}

void Rectangle::setRect(SDL_Rect rect) {
    this->rect = rect;
}

SDL_Rect Rectangle::getRect() {
    return rect;
}