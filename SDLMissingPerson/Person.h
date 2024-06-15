#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

#define MISSING_PERSON_WIDTH 40 //미아 가로 픽셀 크기
#define MISSING_PERSON_HEIGHT 40 //미아 세로 픽셀 크기

typedef struct PERSON_INFORMATION { //플레이어에게 보여줄 미아에 대한 정보
    string name; //이름
    string gender; //성별
    string c1; //특징 1
    string c2; //특징 2
    bool isinList; //리스트 안에 있는지 없는지 (중복 방지)
}PERSON_INFO;

extern PERSON_INFO missingPersonInfo[17];

class Person {
protected:
    SDL_Rect rect; // {x축special feature 위치, y축 위치, 가로폭, 세로폭}
    SDL_Surface* image; //이미지

    int direction[2]; //움직일 방향. [0]이 x축 방향, [1]이 y축 방향
    int step; //그 방향으로 움직인 횟수
    
public:
    bool isVisible;

    Person(SDL_Surface* image, int x, int y, int w, int h);

    void render(SDL_Surface* surface);

    void setImage(SDL_Surface* image); //이미지 설정


    void set_direction(); //움직일 방향 선택
    void move(); //움직임

    // 마우스 클릭 이벤트 처리
    bool handleEvent(SDL_Event& e, int num); //클릭되었으면 true, 아니면 false 반환

    void setVisible(bool visible);

    bool isEmpty(); //이미지가 비어있는지

    SDL_Rect getRect();

    //클릭했을 때 행동
    void onClick(int num);

};

class MissingPerson : public Person {
    PERSON_INFO* info;
public:
    MissingPerson(SDL_Surface* image, int x, int y, int w = MISSING_PERSON_WIDTH, int h = MISSING_PERSON_HEIGHT) 
        : Person(image, x, y, w, h) {};

    bool handleEvent(SDL_Event& e, int num);
    void setInfo(PERSON_INFO* info); //정보 설정
    PERSON_INFO* getInfo();
};