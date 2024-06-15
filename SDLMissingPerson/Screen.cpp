#include "Screen.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <windows.h>
#include <random>

using namespace std;

Screen::Screen(SDL_Surface* image, string text, int x, int y, int w, int h) {
	rect = { x, y, w, h };

    int px, py = 0; //사람 x축 위치, y축 위치

    random_device rd; //시드값 얻기 위한 random_device 생성
    mt19937 gen(rd()); //난수 생성 엔진 초기화

	for (int i = 0; i < 3; i++) {
        uniform_int_distribution<int> PX(170, 730); // 170 ~ 730 사이 정수
        px = PX(rd);
        uniform_int_distribution<int> PY(42, 360); //42 ~ 360 사이 정수
        py = PY(rd);

        missing[i] = new MissingPerson(NULL, px, py); //미아
	}

    

	for (int i = 0; i < COMMON_PERSON; i++) {
        
        uniform_int_distribution<int> PX(170, 730); // 170 ~ 730 사이 정수
        px = PX(rd);
        uniform_int_distribution<int> PY(42, 360); //42 ~ 360 사이 정수
        py = PY(rd);

        common[i] = new Person(NULL, px, py, 40, 50); //그냥 사람
	}

	this->image = image; //배경 이미지 넣기
	isVisible = false; //처음엔 안 보임
    missingNum = 0;
}

void Screen::setCommonPerson(int number, SDL_Surface* image) {
    common[number]->setImage(image);
}

void Screen::setMissingPerson(SDL_Surface* image, PERSON_INFO* info) {

    missing[missingNum]->setImage(image); //이미지 넣기
    missing[missingNum]->setInfo(info); //정보 넣기
    missing[missingNum]->setVisible(true);
    missingNum++;
    
}

MissingPerson* Screen::getMissingPerson(int selectedPerson) {
    return missing[selectedPerson];
}

void Screen::deleteMissingPerson(int selectedPerson) {

    //printf("미아 %d번 삭제\n", selectedPerson); 
    missingNum--;
    
    for (int i = selectedPerson; i < missingNum; i++)
        missing[i] = missing[i + 1];

    //printf("현재 스크린의 미아 수 : %d\n", missingNum);
}

void Screen::render(SDL_Surface* surface, TTF_Font* font) {
    if (!isVisible) return; // isVisible이 false이면 렌더링하지 않음
    
    SDL_BlitSurface(image, NULL, surface, &rect);

    for (int i = 0; i < COMMON_PERSON; i++) common[i]->move(); //사람 이동
    for (int i = 0; i < missingNum; i++) missing[i]->move();
    Sleep(300);
    for (int i = 0; i < COMMON_PERSON; i++) common[i]->render(surface); //출력
    for (int i = 0; i < missingNum; i++) missing[i]->render(surface);

}

int Screen::handleEvent(SDL_Event& e) {
    for (int i = 0; i < missingNum; i++) {
        if (missing[i]->handleEvent(e, NULL))
            return i; //클릭한 미아 인덱스 리턴
    }
    for (int i = 0; i < COMMON_PERSON; i++) {
        if (common[i]->handleEvent(e, NULL))
            return -1; //미아가 아닌 일반사람을 선택했을 경우
    }
    
    return -2; //배경을 클릭했을 경우
}

void Screen::setVisible(bool visible) {
    isVisible = visible;
    for (int i = 0; i < COMMON_PERSON; i++) common[i]->setVisible(visible);
    for (int i = 0; i < missingNum; i++) missing[i]->setVisible(visible);
}