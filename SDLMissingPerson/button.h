#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Person.h"
using namespace std;

class Button {
protected:
    Uint32 color;     // 버튼의 색상
    SDL_Rect rect;    // 버튼의 위치 및 크기
    string text; // 버튼 텍스트
public:
    bool isVisible;

    // 생성자
    Button(int x, int y, int w, int h, Uint32 color, string text = "");
    Button();

    // 버튼 그리기
    void render(SDL_Surface* surface, TTF_Font* font);

    // 마우스 클릭 이벤트 처리
    void handleEvent(SDL_Event& e, int num);
    
    void setVisible(bool visible);
    void setText(string text);
    void setColor(Uint32 color);

    SDL_Rect getRect();

    //클릭했을 때 행동
    virtual void onClick(int num) = 0;
};

class ImageButton : public Button {
public:
    SDL_Surface* image;

    ImageButton(int x, int y, int w, int h, SDL_Surface* image, string text);
    
    void render(SDL_Surface* surface, TTF_Font* font);
};

class screenButton : public ImageButton {
public:
    screenButton(int x, int y, int w, int h, SDL_Surface* image, string text) : ImageButton(x, y, w, h, image, text) {}

    void onClick(int num);
};

class listButton : public Button {
    PERSON_INFO* info;
public:
    listButton(int x, int y, int w, int h, Uint32 color, string text) : Button(x, y, w, h, color, text) {}

    bool handleEvent(SDL_Event& e, int num); //눌렸는지 아닌지 리턴

    void setInfo(PERSON_INFO* info);
    PERSON_INFO* getInfo();

    void onClick(int num);
};

class choiceButton : public Button {
public:
    choiceButton(int x, int y, int w, int h, Uint32 color, string text) : Button(x, y, w, h, color, text) {}

    void onClick(int num);
};

class callButton : public ImageButton {
public:
    callButton(int x, int y, int w, int h, SDL_Surface* image, string text = "") : ImageButton(x, y, w, h, image, text) {}

    void onClick(int num);
};

class exitButton : public ImageButton {
public:
    exitButton(int x, int y, int w, int h, SDL_Surface* image, string text = "") : ImageButton(x, y, w, h, image, text) {}

    void onClick(int num);
};

class leftButton : public ImageButton {
public:
    leftButton(int x, int y, int w, int h, SDL_Surface* image, string text = "") : ImageButton(x, y, w, h, image, text) {}

    void onClick(int num);
};

class rightButton : public ImageButton {
public:
    rightButton(int x, int y, int w, int h, SDL_Surface* image, string text = "") : ImageButton(x, y, w, h, image, text) {}

    void onClick(int num);
};

class startButton : public Button {
public:
    startButton(int x, int y, int w, int h, Uint32 color, string text = "") : Button(x, y, w, h, color, text) {}

    void onClick(int num);
};

class titleButton : public ImageButton {
public:
    titleButton(int x, int y, int w, int h, SDL_Surface* image, string text = "") : ImageButton(x, y, w, h, image, text) {}

    void onClick(int num);
};