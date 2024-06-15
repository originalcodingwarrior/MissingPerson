#include <SDL.h>
#include <SDL_ttf.h>
#include "rectangle.h"


Rectangle::Rectangle(Uint32 color, string text, int x, int y, int w, int h) {
    rect = { x, y, w, h };
    this->color = color;
    this->text = text;
    isVisible = true;   //ó���� ����
}

//Rectangle::Rectangle(SDL_Surface* image, string text, int x, int y, int w, int h) {
//    rect = { x, y, w, h };
//    this->image = image;
//    this->text = text;
//    isVisible = true;   //ó���� ����
//}

Rectangle::Rectangle() {
    isVisible = false;
}

void Rectangle::render(SDL_Surface* surface, TTF_Font* font) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����

    SDL_FillRect(surface, &rect, color);

    // �ؽ�Ʈ ������
    // �ؽ�Ʈ ũ�� ���
    int textWidth, textHeight;
    TTF_SizeText(font, text.c_str(), &textWidth, &textHeight);

    // �ؽ�Ʈ ��ġ ���
    int textX = rect.x + (rect.w - textWidth) / 2; // ���� ��� ����
    int textY = rect.y + (rect.h - textHeight) / 2; // ���� ��� ����

    // �ؽ�Ʈ ���
    SDL_Color textColor = { 0, 0, 0 }; // ������
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