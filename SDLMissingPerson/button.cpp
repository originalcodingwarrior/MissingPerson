#include <SDL.h>
#include <SDL_ttf.h>
#include "button.h"
using namespace std;



Button::Button(int x, int y, int w, int h, Uint32 color, string text) {
    rect = { x, y, w, h };
    this->color = color;
    this->text = text;
    isVisible = true;   //ó���� ����
}

Button::Button() {
    isVisible = true;
}

ImageButton::ImageButton(int x, int y, int w, int h, SDL_Surface* image, string text) {
    rect = { x, y, w, h };
    this->image = image;
    this->text = text;
    isVisible = true;   //ó���� ����
}

void Button::render(SDL_Surface* surface, TTF_Font* font) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����
    //��
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

void ImageButton::render(SDL_Surface* surface, TTF_Font* font) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����
    //��
    SDL_BlitSurface(image, NULL, surface, &rect);

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

void Button::handleEvent(SDL_Event& e, int num) {
    if (!isVisible) return; // isVisible�� false�̸� �̺�Ʈ ó������ ����
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            onClick(num);
        }
    }
}

bool listButton::handleEvent(SDL_Event& e, int num) {
    if (!isVisible) return false; // isVisible�� false�̸� �̺�Ʈ ó������ ����.
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            onClick(num);
            return true; //���ȴٰ� �˷���
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
    return this->info; //����Ʈ�� �̾� ���� �Ѱ��ֱ� (�̾� ��ġ Ȯ�ο뵵)
}

//��ư�� Ŭ��������
void screenButton::onClick(int num) {
    //cout << "��ũ����ư " << num << endl;
}

void listButton::onClick(int num) {
    //cout << "����Ʈ��ư " << num << endl;
    //cout << "��� �ֳ���?" << endl; //�÷��̾�� ��ġ�ϴ� �̾Ƹ� Ŭ���ϸ� ��.
}

void choiceButton::onClick(int num) {
    //cout << "��������ư " << num << endl;
}

void callButton::onClick(int num) {
    //cout << "��ȭ��ư " << num << endl;
}

void exitButton::onClick(int num) {
    //cout << "x��ư"<< endl;
}

void leftButton::onClick(int num) {
    //cout << "<��ư" << endl;
}

void rightButton::onClick(int num) {
    //cout << ">��ư" << endl;
}

void startButton::onClick(int num) {
    //cout << "���۹�ư" << endl;
}

void titleButton::onClick(int num) {
    //cout << "title" << endl;
}
