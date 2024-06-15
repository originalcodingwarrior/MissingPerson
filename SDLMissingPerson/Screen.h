#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Person.h"
#include "rectangle.h"

#define COMMON_PERSON 30

class Screen : public Rectangle {
    MissingPerson* missing[3]; //미아
    int missingNum; //스크린에 있는 미아 수
    Person* common[COMMON_PERSON]; //그냥 사람
    SDL_Surface* image; //배경 이미지
public:
    Screen(SDL_Surface* image, string text, int x = 170, int y = 42, int w = 600, int h = 360);

    void render(SDL_Surface* surface, TTF_Font* font); //화면에 출력

    int handleEvent(SDL_Event& e); // 마우스 클릭 이벤트 처리

    void setVisible(bool visible);

    void setCommonPerson(int number, SDL_Surface* image); //스크린에 일반사람 넣기
    void setMissingPerson(SDL_Surface* image, PERSON_INFO* info); //스크린에 미아 넣기

    MissingPerson* getMissingPerson(int selectedPerson); //선택한 미아 정보 얻어오기 위해서

    void deleteMissingPerson(int selectedPerson); //선택한 미아 삭제
};