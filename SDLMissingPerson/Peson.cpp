#include "Person.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <random>
using namespace std;

PERSON_INFO missingPersonInfo[17] = {
    {"Alex", "boy", "crying", "hair scratch", false},
    {"James", "boy", "glasses", "crying", false},
    {"Sam", "boy", "snapback", "eating candy", false},
    {"Callie", "girl", "ponytail", "ribbon", false},
    {"Marie", "girl", "twintail", "ribbon", false},
    {"David", "boy", "crying", "rotten smile", false},
    {"Tom", "boy", "can't see ahead", "long sleeves", false},
    {"Dominic", "boy", "can't see ahead", "black clothes", false},
    {"Nagi", "boy", "big mouth", "bagaji hair", false},
    {"Pearl", "girl", "bear headband", "cotton candy", false},
    {"Marina", "girl", "cat headband", "lollipop", false},
    {"Tama", "boy", "propeller hat", "eating chocolate", false},
    {"Sou", "boy", "big eyes", "short sleeves", false},
    {"Jones", "boy", "glasses", "balloon", false},
    {"Naruto", "boy", "cat whiskers", "narrow eyes", false},
    {"Kiyoung", "boy", "dangerous head", "freckles", false},
    {"Simpson", "boy", "dangerous head", "skateboard", false}
};

Person::Person(SDL_Surface* image, int x, int y, int w, int h) {

    this->image = image; //�̹��� �ֱ�
    isVisible = false;   //ó���� �� ����. screen Ŀ���� ����.

    rect = { x, y, w, h };
    step = 5;
    direction[0] = 1;
    direction[1] = -1;
}

void Person::render(SDL_Surface* surface) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����

    SDL_BlitSurface(image, NULL, surface, &rect);
}

void Person::setImage(SDL_Surface* image) {
    this->image = image;
}

void Person::set_direction() {

    if (step >= 5) {

        step = 0;

        random_device rd; //�õ尪 ��� ���� random_device ����
        mt19937 gen(rd()); //���� ���� ���� �ʱ�ȭ
        uniform_int_distribution<int> X_direction(-1, 1); //-1, 0, 1 �� �ϳ��� ����
        uniform_int_distribution<int> Y_direction(-1, 1); //-1, 0, 1 �� �ϳ��� ����

        direction[0] = X_direction(rd);
        direction[1] = Y_direction(rd); //�ٸ� �������� �ٲٱ�
    }

    //ȭ�� �� �� �Ѿ��
    if (rect.x <= 180) direction[0] = 1;
    else if (rect.x + rect.w >= 760) direction[0] = -1;
    if (rect.y <= 52) direction[1] = 1;
    else if (rect.y + rect.h >= 392) direction[1] = -1; 

    step++;
}

void Person::move() {
    
    this->set_direction();

    rect.x += direction[0] * 3;
    rect.y += direction[1] * 3;

}

bool Person::handleEvent(SDL_Event& e, int num) {
    if (!isVisible) return false; // isVisible�� false�̸� �̺�Ʈ ó������ ����
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            onClick(num);
            return true;
        }
    }
    return false;
}

bool MissingPerson::handleEvent(SDL_Event& e, int num) {
    if (!isVisible) return false; // isVisible�� false�̸� �̺�Ʈ ó������ ����
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;
        if (mouseX >= rect.x && mouseX <= rect.x + rect.w && mouseY >= rect.y && mouseY <= rect.y + rect.h) {
            onClick(num);
            return true;
        }
    }
    return false;
}

void MissingPerson::setInfo(PERSON_INFO* info) {
    this->info = info;
}

PERSON_INFO* MissingPerson::getInfo() {
    return this->info;
}

void Person::setVisible(bool visible) {
    isVisible = visible;
}

bool Person::isEmpty() {
    if (this->image == NULL) {
        return true;
    }
    return false;
}

SDL_Rect Person::getRect() {
    return rect;
}

void Person::onClick(int num) {
    //cout << "�̾� " << num << endl;
}
