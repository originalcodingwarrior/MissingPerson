#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include "Person.h"
using namespace std;

class Button {
protected:
    Uint32 color;     // ��ư�� ����
    SDL_Rect rect;    // ��ư�� ��ġ �� ũ��
    string text; // ��ư �ؽ�Ʈ
public:
    bool isVisible;

    // ������
    Button(int x, int y, int w, int h, Uint32 color, string text = "");
    Button();

    // ��ư �׸���
    void render(SDL_Surface* surface, TTF_Font* font);

    // ���콺 Ŭ�� �̺�Ʈ ó��
    void handleEvent(SDL_Event& e, int num);
    
    void setVisible(bool visible);
    void setText(string text);
    void setColor(Uint32 color);

    SDL_Rect getRect();

    //Ŭ������ �� �ൿ
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

    bool handleEvent(SDL_Event& e, int num); //���ȴ��� �ƴ��� ����

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