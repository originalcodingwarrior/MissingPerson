#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

#define MISSING_PERSON_WIDTH 40 //�̾� ���� �ȼ� ũ��
#define MISSING_PERSON_HEIGHT 40 //�̾� ���� �ȼ� ũ��

typedef struct PERSON_INFORMATION { //�÷��̾�� ������ �̾ƿ� ���� ����
    string name; //�̸�
    string gender; //����
    string c1; //Ư¡ 1
    string c2; //Ư¡ 2
    bool isinList; //����Ʈ �ȿ� �ִ��� ������ (�ߺ� ����)
}PERSON_INFO;

extern PERSON_INFO missingPersonInfo[17];

class Person {
protected:
    SDL_Rect rect; // {x��special feature ��ġ, y�� ��ġ, ������, ������}
    SDL_Surface* image; //�̹���

    int direction[2]; //������ ����. [0]�� x�� ����, [1]�� y�� ����
    int step; //�� �������� ������ Ƚ��
    
public:
    bool isVisible;

    Person(SDL_Surface* image, int x, int y, int w, int h);

    void render(SDL_Surface* surface);

    void setImage(SDL_Surface* image); //�̹��� ����


    void set_direction(); //������ ���� ����
    void move(); //������

    // ���콺 Ŭ�� �̺�Ʈ ó��
    bool handleEvent(SDL_Event& e, int num); //Ŭ���Ǿ����� true, �ƴϸ� false ��ȯ

    void setVisible(bool visible);

    bool isEmpty(); //�̹����� ����ִ���

    SDL_Rect getRect();

    //Ŭ������ �� �ൿ
    void onClick(int num);

};

class MissingPerson : public Person {
    PERSON_INFO* info;
public:
    MissingPerson(SDL_Surface* image, int x, int y, int w = MISSING_PERSON_WIDTH, int h = MISSING_PERSON_HEIGHT) 
        : Person(image, x, y, w, h) {};

    bool handleEvent(SDL_Event& e, int num);
    void setInfo(PERSON_INFO* info); //���� ����
    PERSON_INFO* getInfo();
};