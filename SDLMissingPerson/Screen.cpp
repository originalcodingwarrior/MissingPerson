#include "Screen.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <random>

using namespace std;

Screen::Screen(SDL_Surface* image, string text, int x, int y, int w, int h) {
	rect = { x, y, w, h };

    int px, py = 0; //��� x�� ��ġ, y�� ��ġ

    random_device rd; //�õ尪 ��� ���� random_device ����
    mt19937 gen(rd()); //���� ���� ���� �ʱ�ȭ

	for (int i = 0; i < 3; i++) {
        uniform_int_distribution<int> PX(170, 730); // 170 ~ 730 ���� ����
        px = PX(rd);
        uniform_int_distribution<int> PY(42, 360); //42 ~ 360 ���� ����
        py = PY(rd);

        missing[i] = new MissingPerson(NULL, px, py); //�̾�
	}

    

	for (int i = 0; i < COMMON_PERSON; i++) {
        
        uniform_int_distribution<int> PX(170, 730); // 170 ~ 730 ���� ����
        px = PX(rd);
        uniform_int_distribution<int> PY(42, 360); //42 ~ 360 ���� ����
        py = PY(rd);

        common[i] = new Person(NULL, px, py, 40, 50); //�׳� ���
	}

	this->image = image; //��� �̹��� �ֱ�
	isVisible = false; //ó���� �� ����
    missingNum = 0;
}

void Screen::setCommonPerson(int number, SDL_Surface* image) {
    common[number]->setImage(image);
}

void Screen::setMissingPerson(SDL_Surface* image, PERSON_INFO* info) {

    missing[missingNum]->setImage(image); //�̹��� �ֱ�
    missing[missingNum]->setInfo(info); //���� �ֱ�
    missing[missingNum]->setVisible(true);
    missingNum++;
    
}

MissingPerson* Screen::getMissingPerson(int selectedPerson) {
    return missing[selectedPerson];
}

void Screen::deleteMissingPerson(int selectedPerson) {

    //printf("�̾� %d�� ����\n", selectedPerson); 
    missingNum--;
    
    for (int i = selectedPerson; i < missingNum; i++)
        missing[i] = missing[i + 1];

    //printf("���� ��ũ���� �̾� �� : %d\n", missingNum);
}

void Screen::render(SDL_Surface* surface, TTF_Font* font) {
    if (!isVisible) return; // isVisible�� false�̸� ���������� ����
    
    SDL_BlitSurface(image, NULL, surface, &rect);

    for (int i = 0; i < COMMON_PERSON; i++) common[i]->move(); //��� �̵�
    for (int i = 0; i < missingNum; i++) missing[i]->move();
    Sleep(300);
    for (int i = 0; i < COMMON_PERSON; i++) common[i]->render(surface); //���
    for (int i = 0; i < missingNum; i++) missing[i]->render(surface);

}

int Screen::handleEvent(SDL_Event& e) {
    for (int i = 0; i < missingNum; i++) {
        if (missing[i]->handleEvent(e, NULL))
            return i; //Ŭ���� �̾� �ε��� ����
    }
    for (int i = 0; i < COMMON_PERSON; i++) {
        if (common[i]->handleEvent(e, NULL))
            return -1; //�̾ư� �ƴ� �Ϲݻ���� �������� ���
    }
    
    return -2; //����� Ŭ������ ���
}

void Screen::setVisible(bool visible) {
    isVisible = visible;
    for (int i = 0; i < COMMON_PERSON; i++) common[i]->setVisible(visible);
    for (int i = 0; i < missingNum; i++) missing[i]->setVisible(visible);
}