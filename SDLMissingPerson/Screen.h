#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Person.h"
#include "rectangle.h"

#define COMMON_PERSON 30

class Screen : public Rectangle {
    MissingPerson* missing[3]; //�̾�
    int missingNum; //��ũ���� �ִ� �̾� ��
    Person* common[COMMON_PERSON]; //�׳� ���
    SDL_Surface* image; //��� �̹���
public:
    Screen(SDL_Surface* image, string text, int x = 170, int y = 42, int w = 600, int h = 360);

    void render(SDL_Surface* surface, TTF_Font* font); //ȭ�鿡 ���

    int handleEvent(SDL_Event& e); // ���콺 Ŭ�� �̺�Ʈ ó��

    void setVisible(bool visible);

    void setCommonPerson(int number, SDL_Surface* image); //��ũ���� �Ϲݻ�� �ֱ�
    void setMissingPerson(SDL_Surface* image, PERSON_INFO* info); //��ũ���� �̾� �ֱ�

    MissingPerson* getMissingPerson(int selectedPerson); //������ �̾� ���� ������ ���ؼ�

    void deleteMissingPerson(int selectedPerson); //������ �̾� ����
};