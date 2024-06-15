#pragma once
#include <SDL.h>
#include <SDL_image.h>

SDL_Surface* backgroundImage = IMG_Load("background.png");	//배경화면 이미지
SDL_Surface* missingPersonImage[17] = { IMG_Load("lost1.png"),  IMG_Load("lost2.png"),
									IMG_Load("lost3.png"), IMG_Load("lost4.png"),
									IMG_Load("lost5.png"),  IMG_Load("lost6.png"),
									IMG_Load("lost7.png"), IMG_Load("lost8.png"),
									IMG_Load("lost9.png"),  IMG_Load("lost10.png"),
									IMG_Load("lost11.png"), IMG_Load("lost12.png"),
									IMG_Load("lost13.png"),  IMG_Load("lost14.png"), 
									IMG_Load("lost_special1.png"),  IMG_Load("lost_special2.png"),
									IMG_Load("lost_special3.png") }; //미아 이미지
SDL_Surface* commonPersonImage[6] = { IMG_Load("boy.png"), IMG_Load("girl.png"),
										IMG_Load("female.png"), IMG_Load("female_senior.png"),
										IMG_Load("male.png"), IMG_Load("male_senior.png") }; //사람 이미지
SDL_Surface* screen_Image[4] = { IMG_Load("screen1.png"),  IMG_Load("screen2.png"), 
									IMG_Load("screen3.png"), IMG_Load("screen4.png")}; //스크린 이미지
SDL_Surface* bigscreen_Image[4] = { IMG_Load("bigscreen1.png"),  IMG_Load("bigscreen2.png"),
									IMG_Load("bigscreen3.png"), IMG_Load("bigscreen4.png") }; //큰 스크린 이미지
SDL_Surface* call_Image = IMG_Load("call.png");
SDL_Surface* exit_Image = IMG_Load("exit.png");
SDL_Surface* left_Image = IMG_Load("left.png");
SDL_Surface* right_Image = IMG_Load("right.png");

SDL_Surface* titleImage = IMG_Load("title.png");
