#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <random>


using namespace std;

// 세팅 https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php
//sdl, sdl_image(이미지), sdl_ttf(글씨) 다운받았습니다

#include "image.h"  // 이미지들
#include "button.h" // 버튼
#include "rectangle.h" // 사각형
#include "Person.h" //사람
#include "Screen.h" //스크린
////혹시 파일 소스를 열 수 없다고 뜨면 프로젝트->속성->C/C++->일반->추가 포함 디렉터리에 이 프로젝트 경로를 추가해보세여

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

class SDLApp {  //세팅
public:
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    void init() {
        window = SDL_CreateWindow("missingPerson", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        screenSurface = SDL_GetWindowSurface(window);
        SDL_BlitSurface(backgroundImage, NULL, screenSurface, NULL); // 배경화면 그리기

        TTF_Init(); //폰트
    }

    void close() {
        SDL_DestroyWindow(window);
        window = NULL;
        SDL_Quit();
    }
};

class Game {
public:
    SDLApp sdlApp;
    SDL_Event e;
    bool quit = false;
    int currentScreen = -1;
    SDL_Rect btnRect;
    Uint32 redColor, greenColor, blueColor, grayColor, lightGrayColor, blackColor, whiteColor;
    TTF_Font* font;
    TTF_Font* sfont;

    bool isSelecting = false; //플레이어가 I found missing person 버튼을 누른 상태인지(true) 아닌지(false)
    int SelectedList; //플레이어가 선택한 리스트 인덱스
    int SelectedPerson; //플레이어가 선택한 미아의 인덱스

    int correct = 0; //플레이어가 찾은 미아 수

    screenButton* screenBtn[4];
    listButton* personListBtn[3];
    choiceButton* choiceBtn[5];
    callButton* callBtn;
    exitButton* exitBtn;
    leftButton* leftBtn;
    rightButton* rightBtn;

    startButton* startBtn;

    Screen* bigScreen[4];

    Rectangle* callRect;
    Rectangle* date;
    Rectangle* time;
    Rectangle* reception;
    Rectangle* number;

    titleButton* title;


    Uint32 startTime = SDL_GetTicks();  // 게임 시작 시간 기록
    Uint32 lastUpdateTime = 0; // 마지막으로 time 업데이트한 시간

    Game() {
        sdlApp.init();  //시작

        redColor = SDL_MapRGB(sdlApp.screenSurface->format, 255, 0, 0);     //색(임시)
        greenColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 255, 0);
        blueColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 0, 255);
        grayColor = SDL_MapRGB(sdlApp.screenSurface->format, 128, 128, 128);
        lightGrayColor = SDL_MapRGB(sdlApp.screenSurface->format, 200, 200, 200);
        blackColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 0, 0);
        whiteColor = SDL_MapRGB(sdlApp.screenSurface->format, 255, 255, 255);

        font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);   //폰트, 크기
        sfont = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 12);

        createButtons();    //버튼생성
        createRectangles(); //사각형생성
        createPersons(); //사람생성
    }

    ~Game() {
        sdlApp.close(); //종료
    }

    void createButtons() {

        screenBtn[0] = new screenButton(170, 42, 300, 180, screen_Image[0], "screen 1");   //화면 버튼 4개
        screenBtn[1] = new screenButton(470, 42, 300, 180, screen_Image[1], "screen 2");
        screenBtn[2] = new screenButton(170, 222, 300, 180, screen_Image[2], "screen 3");
        screenBtn[3] = new screenButton(470, 222, 300, 180, screen_Image[3], "screen 4");

        personListBtn[0] = new listButton(776, 47, 254, 65, lightGrayColor, "person 1");  //미아 리스트 버튼 3개
        personListBtn[1] = new listButton(776, 118, 254, 65, lightGrayColor, "person 2");
        personListBtn[2] = new listButton(776, 189, 254, 65, lightGrayColor, "person 3");

        choiceBtn[0] = new choiceButton(785, 322, 230, 33, grayColor, "How can I help?");   //전화 선택지 버튼 4개(하나는 비활성화)
        choiceBtn[1] = new choiceButton(785, 361, 230, 23, lightGrayColor, "I found missing person");
        choiceBtn[2] = new choiceButton(785, 390, 230, 23, lightGrayColor, "What should I do?");
        choiceBtn[3] = new choiceButton(785, 419, 230, 23, lightGrayColor, "I can't find anyone(hint)");
        choiceBtn[4] = new choiceButton(785, 448, 230, 23, lightGrayColor, "end call");
        for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);

        callBtn = new callButton(730, 408, 40, 40, call_Image);  //전화버튼
        exitBtn = new exitButton(725, 47, 30, 30, exit_Image);  //종료버튼
        exitBtn->setVisible(false);
        leftBtn = new leftButton(178, 209, 30, 30, left_Image); //왼쪽버튼
        leftBtn->setVisible(false);
        rightBtn = new rightButton(732, 209, 30, 30, right_Image);   //오른쪽버튼
        rightBtn->setVisible(false);

        startBtn = new startButton(550, 350, 100, 40, grayColor, "press to start");

        title = new titleButton(350, 120, 500, 200, titleImage, ""); //제목
    }

    void createRectangles() {  
        bigScreen[0] = new Screen(bigscreen_Image[0], " ", 170, 42, 600, 360);   //화면
        bigScreen[1] = new Screen(bigscreen_Image[1], " ", 170, 42, 600, 360);
        bigScreen[2] = new Screen(bigscreen_Image[2], " ", 170, 42, 600, 360);
        bigScreen[3] = new Screen(bigscreen_Image[3], " ", 170, 42, 600, 360);


        callRect = new Rectangle(grayColor, " ", 780, 271, 240, 210);  //전화 배경
        callRect->setVisible(false);

        date = new Rectangle(grayColor, "2xxx.xx.xx", 172, 408, 88, 15);
        time = new Rectangle(grayColor, "00:00", 172, 423, 88, 27);
        reception = new Rectangle(grayColor, "reception", 642, 408, 82, 15);
        number = new Rectangle(grayColor, "xxx-xxxx", 642, 422, 82, 15);


    }

    void createPersons() {
        int p = 0;
        int s = 0;
        
        for (int i = 0; i < 3; i++) {
             do{
                random_device rd; //시드값 얻기 위한 random_device 생성
                mt19937 gen(rd()); //난수 생성 엔진 초기화

                uniform_int_distribution<int> whatScreen(0, 3); // 0, 1, 2, 3 중 하나. 어떤 스크린에 넣을지
                uniform_int_distribution<int> whatPerson(0, 16); // 0 ~ 16 중 하나의 정수. 어떤 미아 넣을지
                p = whatPerson(rd);
                s = whatScreen(rd);
             } while (missingPersonInfo[p].isinList);
            
            bigScreen[s]->setMissingPerson(missingPersonImage[p], &missingPersonInfo[p]);
            personListBtn[i]->setInfo(&missingPersonInfo[p]);
            missingPersonInfo[p].isinList = true;
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < COMMON_PERSON; j++) {
                bigScreen[i]->setCommonPerson(j, commonPersonImage[(j + 6) % 6]);
            }
        }
        
    }

    void handleEvents() {   //이벤트
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {   //종료
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) { //마우스 클릭
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                for (int i = 0; i < 4; i++) { // screenButton
                    screenBtn[i]->handleEvent(e, i + 1); // 이벤트핸들러, 버튼 번호 넘겨주기
                }
                for (int i = 0; i < 3; i++) { // listButton
                    if (isSelecting && personListBtn[i]->handleEvent(e, i + 1)) { 
                        //미아 고르는 중이고, 리스트버튼 눌렸으면

                        SelectedList = i; //어떤 리스트 눌렀는지 저장.
                        for (int i = 0; i < 3; i++) personListBtn[i]->setColor(lightGrayColor); //색 초기화
                        personListBtn[SelectedList]->setColor(greenColor); //눌린 리스트 색 바꾸기
                        choiceBtn[0]->setText("Where is " + personListBtn[SelectedList]->getInfo()->name + "?");
                    }
                }
                for (int i = 1; i < 5; i++) { // choiceButton (0 제외 1~4)
                    choiceBtn[i]->handleEvent(e, i);
                }
                callBtn->handleEvent(e, NULL);
                exitBtn->handleEvent(e, NULL);
                leftBtn->handleEvent(e, NULL);
                rightBtn->handleEvent(e, NULL);		//원래 이걸로 이벤트 처리하려고 했는데...

                handleMouseClick(mouseX, mouseY);
            }
        }
    }

    void handleMouseClick(int mouseX, int mouseY) {
        if (bigScreen[0]->isVisible == false && bigScreen[1]->isVisible == false 
            && bigScreen[2]->isVisible == false && bigScreen[3]->isVisible == false) {    //작은 화면일때
            for (int i = 0; i < 4; i++) {       //화면 클릭하면 전체화면
                btnRect = screenBtn[i]->getRect();
                if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                    bigScreen[i]->setVisible(true);
                    exitBtn->setVisible(true);
                    leftBtn->setVisible(true);
                    rightBtn->setVisible(true);
                    
                    for (int j = 0; j < 4; j++) screenBtn[j]->setVisible(false);
                    currentScreen = i;
                }
            }
        }
        else {  //전체화면일때
            handleBigScreenClick(mouseX, mouseY);
        }

        handleCallButtons(mouseX, mouseY);  //전화버튼들 클릭
    }

    void handleBigScreenClick(int mouseX, int mouseY) {
        btnRect = leftBtn->getRect();   //왼쪽버튼
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            bigScreen[currentScreen]->setVisible(false); //보여주던 스크린 치우고
            currentScreen = (currentScreen + 3) % 4;
            bigScreen[currentScreen]->setVisible(true); //다음 스크린 보여주기
        }

        btnRect = rightBtn->getRect();  //오른쪽버튼
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            bigScreen[currentScreen]->setVisible(false); //보여주던 스크린 치우고
            currentScreen = (currentScreen + 1) % 4;
            bigScreen[currentScreen]->setVisible(true); //다음 스크린 보여주기
        }

        btnRect = exitBtn->getRect();   //종료버튼
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            bigScreen[currentScreen]->setVisible(false);
            exitBtn->setVisible(false);
            leftBtn->setVisible(false);
            rightBtn->setVisible(false);
            for (int i = 0; i < 4; i++) screenBtn[i]->setVisible(true);
            currentScreen = -1;
        }
        if (currentScreen != -1 && isSelecting == true) { //큰 스크린 상태이고 선택중이면

            SelectedPerson = bigScreen[currentScreen]->handleEvent(e); //뭐 클릭했는지 저장 (-2:배경, -1:일반사람, 0~16:미아)

            if (SelectedPerson == -2) return; //배경 클릭한 경우엔 그냥 리턴

            if (SelectedPerson == -1) { //미아가 아닌 일반 사람 클릭했을 경우
                
                choiceBtn[0]->setText("You got the wrong person.");

                isSelecting = false; //선택 중 해제
                personListBtn[SelectedList]->setColor(lightGrayColor);
                choiceBtn[1]->setColor(lightGrayColor);
                return;
            }

            if(personListBtn[SelectedList]->getInfo() 
                == bigScreen[currentScreen]->getMissingPerson(SelectedPerson)->getInfo()) { //정보가 일치한 경우

                choiceBtn[0]->setText("Good job!");

                correct++; //정답 수 ++
                cout << "찾은 미아의 수: " << correct << endl;

                //미아 지우기
                bigScreen[currentScreen]->deleteMissingPerson(SelectedPerson);

                //미아 새로 채우기
                int p = 0;
                int s = 0;

                do {
                    random_device rd; //시드값 얻기 위한 random_device 생성
                    mt19937 gen(rd()); //난수 생성 엔진 초기화

                    uniform_int_distribution<int> whatScreen(0, 3); // 0, 1, 2, 3 중 하나. 어떤 스크린에 넣을지
                    uniform_int_distribution<int> whatPerson(0, 16); // 0 ~ 16 중 하나의 정수. 어떤 미아 넣을지
                    p = whatPerson(rd);
                    s = whatScreen(rd);
                } while (missingPersonInfo[p].isinList || s == currentScreen);

                bigScreen[s]->setMissingPerson(missingPersonImage[p], &missingPersonInfo[p]);
                personListBtn[SelectedList]->setInfo(&missingPersonInfo[p]);
                missingPersonInfo[p].isinList = true;

                //선택 중 해제
                isSelecting = false;
                personListBtn[SelectedList]->setColor(lightGrayColor);
                choiceBtn[1]->setColor(lightGrayColor);
            }
            else { //일치하지 않은 경우
                
                choiceBtn[0]->setText("You got the wrong person.");

                //선택 중 해제
                isSelecting = false;
                personListBtn[SelectedList]->setColor(lightGrayColor);
                choiceBtn[1]->setColor(lightGrayColor);
            }
            
        }
    }

    void handleCallButtons(int mouseX, int mouseY) {
        btnRect = callBtn->getRect();   //전화버튼
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            callRect->setVisible(true);
            for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(true);
        }

        btnRect = choiceBtn[1]->getRect();  //미아 찾았습니다 버튼
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            
            isSelecting = true; //미아 고르는 상태가 됨
            choiceBtn[1]->setColor(greenColor);

            choiceBtn[0]->setText("Who did you find?");
            choiceBtn[3]->setText("never mind");
            //플레이어는 어떤 미아인지 리스트 버튼 클릭하면 됨
        }

        btnRect = choiceBtn[2]->getRect();  //무엇을 해야 하나요? 버튼
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            choiceBtn[0]->setText("Look for people on the missing children list.");
        }

        btnRect = choiceBtn[3]->getRect();  //힌트 버튼
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            choiceBtn[0]->setText("Look for people with special characteristics");
            if (isSelecting == true) {   //선택중에는 취소 버튼 - 리셋하기
                choiceBtn[0]->setText("How can I help?");
                choiceBtn[1]->setColor(lightGrayColor);
                choiceBtn[3]->setText("I can't find anyone(hint)");
                isSelecting = false;
            }
        }

        btnRect = choiceBtn[4]->getRect();  //전화 종료 버튼
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            callRect->setVisible(false);
            for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);
            choiceBtn[0]->setText("How can I help?");
            choiceBtn[1]->setColor(lightGrayColor);
            choiceBtn[3]->setText("I can't find anyone(hint)");
            for(int i = 0; i < 3; i++) personListBtn[i]->setColor(lightGrayColor);
            isSelecting = false;    //리셋
        }
    }

    void render() { //그리기
        SDL_BlitSurface(backgroundImage, NULL, sdlApp.screenSurface, NULL); //배경화면

        for (int i = 0; i < 4; ++i) {   //화면 4개
            screenBtn[i]->render(sdlApp.screenSurface, font);
        }
        for (int i = 0; i < 3; ++i) {   //미아 리스트 3개
            personListBtn[i]->render(sdlApp.screenSurface, sfont);
        }
        callRect->render(sdlApp.screenSurface, sfont);  //전화 배경
        for (int i = 0; i < 5; ++i) {   //전화 선택지버튼
            choiceBtn[i]->render(sdlApp.screenSurface, sfont);
        }
        
        if (currentScreen != -1) {
            bigScreen[currentScreen]->render(sdlApp.screenSurface, font);  //전체화면
        }

        callBtn->render(sdlApp.screenSurface, sfont);   //전화버튼
        exitBtn->render(sdlApp.screenSurface, sfont);   //종료버튼
        leftBtn->render(sdlApp.screenSurface, sfont);   //왼쪽버튼
        rightBtn->render(sdlApp.screenSurface, sfont);  //오른쪽버튼

        date->render(sdlApp.screenSurface, sfont);
        time->render(sdlApp.screenSurface, font);
        reception->render(sdlApp.screenSurface, sfont);
        number->render(sdlApp.screenSurface, sfont);
        

        SDL_UpdateWindowSurface(sdlApp.window);     //화면 업데이트
    }

    void mainMenu() {   //메인 메뉴
        SDL_BlitSurface(backgroundImage, NULL, sdlApp.screenSurface, NULL); //배경화면 그리기
        title->render(sdlApp.screenSurface, font);  //제목 그리기
        startBtn->render(sdlApp.screenSurface, sfont);  //시작버튼 그리기

        SDL_UpdateWindowSurface(sdlApp.window);     //화면 업데이트

        while (!quit) { //반복
            while (SDL_PollEvent(&e) != 0) { //메인 메뉴 이벤트 관리
                if (e.type == SDL_QUIT) {   //종료
                    quit = true;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {  //마우스 클릭 - 시작버튼
                    int mouseX = e.button.x, mouseY = e.button.y;
                    btnRect = startBtn->getRect();  //시작버튼 클릭시
                    //return -> run으로 감, 튜토리얼 진행 후 게임 시작
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) return;
                }
            }
        }
    }

    void tutorial() {   //전화가 오고, 받으면(전화버튼 클릭하면) 엄마의 설명이 나온다
        string tutorialText[11] = {  //튜토리얼 내용
            "Thank you for taking on the job!",
            "We need you to find the missing children.",
            "There have been so many of them lately.",
            "We might have to close down...",
            "The way to find a missing child is...",
            "I will tell you the characteristics of them.",
            "If you find the child, call reception,",
            "and send in the correct child.",
            "But don't forget, ",
            "please avoid calling the wrong person!",
            "I'm counting on you!",
        };
        int tutorialProgress = -1;   //하나씩 ++하며 tutorial text 보여줌
        int inCall = 0; //0이면 아님, 1이면 통화중, -1이면 튜토리얼 종료


        while (!quit) { //반복


            if (inCall == 0) {     // 0.5초마다 전화버튼깜빡이게(임시)
                SDL_Delay(500); // 0.5초 기다리기
                callBtn->setVisible(!callBtn->isVisible);
            }

            while (SDL_PollEvent(&e) != 0) { //튜토리얼 이벤트 관리
                if (e.type == SDL_QUIT) {   //종료
                    quit = true;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {  //마우스 클릭
                    int mouseX = e.button.x, mouseY = e.button.y;

                    btnRect = callBtn->getRect();   //전화버튼
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        inCall = 1;
                        callBtn->setVisible(true);
                        callRect->setVisible(true);
                        choiceBtn[0]->setText("~Call from mom~");
                        choiceBtn[0]->setVisible(true);
                        choiceBtn[1]->setText("ok");
                        choiceBtn[1]->setVisible(true);
                    }

                    btnRect = choiceBtn[1]->getRect();  //선택지 1: ok 버튼
                    if (choiceBtn[1]->isVisible && mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        tutorialProgress++;
                        choiceBtn[0]->setText(tutorialText[tutorialProgress]);  //텍스트 배열에서 순서대로 뽑아옴
                        if (tutorialProgress == 10) {
                            choiceBtn[1]->setVisible(false);    //ok 선택지 없애기
                            choiceBtn[4]->setVisible(true);     //전화 종료 선택지 보이기
                        }
                    }

                    btnRect = choiceBtn[4]->getRect();  //선택지 4 - 전화(튜토리얼) 종료
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        callRect->setVisible(false);
                        for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);
                        choiceBtn[0]->setText("How can I help?");
                        choiceBtn[1]->setText("I found missing person");
                        inCall = -1;
                    }

                }
            }
            render();   //화면에 그리기
            if (inCall == -1) return;   //전화 종료 후 게임으로 들어가기
        }
    }

    void run() {
        startTime = SDL_GetTicks(); //게임 시작 시간 기록
        while (!quit) { //게임 루프
            handleEvents(); //이벤트 처리

            Uint32 currentTime = SDL_GetTicks(); // 현재 시간 가져오기
            if (currentTime - lastUpdateTime >= 1000) {     //1초 지났는지 확인하고
                updateTime(); // 1초마다 시간 업데이트
                lastUpdateTime = currentTime; //업데이트한 시간 갱신
            }

            render();   //화면에 그리기

            if ((currentTime - startTime) / 1000 > 96) return;  //5시에 게임 종료(60분(12초) x 8시간)
        }
    }

    void ending() {   //전화가 오고, 받으면(전화버튼 클릭하면) 엄마의 설명이 나온다
        string goodEndingText[10] = {  //해피엔딩 내용
            "Mom: Thank you!",
            "The rumors seem to be disappearing.",
            "The number of customers is increasing,",
            "We won't have to worry anymore!",
            "You did a great job.",
            "I'd like you to keep taking care of this job.",
            "Haha.",
            "Of course, I'll have to pay you well!",
            "You've worked really hard,",
            "Let's keep up the good work together!"
        };
        string badEndingText[9] = {  //배드엔딩 내용
            "Mom: I guess the rumors were true...",
            "Could there really be a ghost here?",
            "Things are getting worse and worse...",
            "The property value is dropping...",
            "It's okay... we'll pay off the debt eventually...",
            "But can we really pay it all off...?",
            "You should start looking for another job.",
            "You've worked hard, son. Thank you.",
            "We'll need to work even harder from now on..."
        };
        int endingProgress = -1;   //하나씩 ++하며 ending text 보여줌
        int inCall = 0; //0이면 아님, 1이면 통화중, -1이면 튜토리얼 종료

        for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);
        callRect->setVisible(false);
        
        isSelecting = false;


        while (!quit) { //반복


            if (inCall == 0) {     // 0.5초마다 전화버튼깜빡이게(임시)
                SDL_Delay(500); // 0.5초 기다리기
                callBtn->setVisible(!callBtn->isVisible);
            }

            while (SDL_PollEvent(&e) != 0) { //엔딩 이벤트 관리
                if (e.type == SDL_QUIT) {   //종료
                    quit = true;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {  //마우스 클릭
                    int mouseX = e.button.x, mouseY = e.button.y;

                    btnRect = callBtn->getRect();   //전화버튼
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        inCall = 1;
                        callBtn->setVisible(true);
                        callRect->setVisible(true);
                        choiceBtn[0]->setText("~Call from mom~");
                        choiceBtn[0]->setVisible(true);
                        choiceBtn[1]->setText("ok");
                        choiceBtn[1]->setVisible(true);
                    }

                    btnRect = choiceBtn[1]->getRect();  //선택지 1: ok 버튼
                    if (choiceBtn[1]->isVisible && mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        endingProgress++;
                        if(correct >= 5) choiceBtn[0]->setText(goodEndingText[endingProgress]);  //해피엔딩
                        else choiceBtn[0]->setText(badEndingText[endingProgress]);  //배드엔딩
                        if ((endingProgress == 10 && correct >= 5) || endingProgress == 9) {  //해피엔딩 = 10문장, 배드엔딩 = 9문장 종료
                            choiceBtn[1]->setVisible(false);    //ok 선택지 없애기
                            choiceBtn[4]->setVisible(true);     //전화 종료 선택지 보이기
                        }
                    }

                    btnRect = choiceBtn[4]->getRect();  //선택지 4 - 전화(엔딩) 종료
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        callRect->setVisible(false);
                        for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);
                        inCall = -1;
                    }

                }
            }
            render();   //화면에 그리기
            if (inCall == -1) return;   //전화 종료 후 게임으로 들어가기
        }
    }

    void updateTime() {     //시간 업데이트
        Uint32 elapsedTime = (SDL_GetTicks() - startTime) / 2000; // 경과 시간(2초 1틱)

        int hour = (elapsedTime / 6 + 9) % 24;  //9시 시작, 24시간 사이클
        int minute = elapsedTime * 10 % 60;     //1초에 10분 증가, 60분 사이클

        char timeString[6];     //시계 형식 맞추기
        snprintf(timeString, sizeof(timeString), "%02d:%02d", hour, minute);

        //printf("hour: %d, minute: %d, 시계형식: %s\n", hour, minute, timeString); //디버깅용

        time->setText(timeString);
    }
};

int main(int argc, char* args[]) {
    Game game;
    game.mainMenu();    //메인 메뉴
    game.tutorial();    //튜토리얼
    game.run();     //게임 실행
    game.ending();  //엔딩
    return 0;
}