#include <SDL.h>
#include <SDL_ttf.h>
#include "button.h"
using namespace std;



Button::Button(int x, int y, int w, int h, Uint32 color, string text) {
    rect = { x, y, w, h };
    this->color = color;
    this->text = text;
    isVisible = true;   //처음에 보임
}

Button::Button() {
    isVisible = true;
}

ImageButton::ImageButton(int x, int y, int w, int h, SDL_Surface* image, string text) {
    rect = { x, y, w, h };
    this->image = image;
    this->text = text;
    isVisible = true;   //처음에 보임
}

void Button::render(SDL_Surface* surface, TTF_Font* font) {
    if (!isVisible) return; // isVisible이 false이면 렌더링하지 않음
    //색
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

void ImageButton::render(SDL_Surface* surface, TTF_Font* font) {
    if (!isVisible) return; // isVisible이 false이면 렌더링하지 않음
    //색
    SDL_BlitSurface(image, NULL, surface, &rect);

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

void Button::handleEvent(SDL_Event& e, int num) {
    if (!isVisible) return; // isVisible이 false이면 이벤트 처리하지 않음
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            onClick(num);
        }
    }
}

bool listButton::handleEvent(SDL_Event& e, int num) {
    if (!isVisible) return false; // isVisible이 false이면 이벤트 처리하지 않음.
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            onClick(num);
            return true; //눌렸다고 알려줌
        }
    }
    return false;
}

void Button::setVisible(bool visible) {
    isVisible = visible;
}

void Button::setText(string text) {
    this->text = text;
}

void Button::setColor(Uint32 color) {
    this->color = color;
}

SDL_Rect Button::getRect() {
    return rect;
}

void listButton::setInfo(PERSON_INFO* info) {
    this->info = info;
    this->text = info->name + ", " + info->gender + ", " + info->c1 + ", " + info->c2;
}

PERSON_INFO* listButton::getInfo() {
    return this->info; //리스트의 미아 정보 넘겨주기 (미아 일치 확인용도)
}

//버튼별 클릭했을때
void screenButton::onClick(int num) {
    //cout << "스크린버튼 " << num << endl;
}

void listButton::onClick(int num) {
    //cout << "리스트버튼 " << num << endl;
    //cout << "어디 있나요?" << endl; //플레이어는 일치하는 미아를 클릭하면 됨.
}

void choiceButton::onClick(int num) {
    //cout << "선택지버튼 " << num << endl;
}

void callButton::onClick(int num) {
    //cout << "전화버튼 " << num << endl;
}

void exitButton::onClick(int num) {
    //cout << "x버튼"<< endl;
}

void leftButton::onClick(int num) {
    //cout << "<버튼" << endl;
}

void rightButton::onClick(int num) {
    //cout << ">버튼" << endl;
}

void startButton::onClick(int num) {
    //cout << "시작버튼" << endl;
}

void titleButton::onClick(int num) {
    //cout << "title" << endl;
}
