#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <random>


using namespace std;

// ���� https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/msvc2019/index.php
//sdl, sdl_image(�̹���), sdl_ttf(�۾�) �ٿ�޾ҽ��ϴ�

#include "image.h"  // �̹�����
#include "button.h" // ��ư
#include "rectangle.h" // �簢��
#include "Person.h" //���
#include "Screen.h" //��ũ��
////Ȥ�� ���� �ҽ��� �� �� ���ٰ� �߸� ������Ʈ->�Ӽ�->C/C++->�Ϲ�->�߰� ���� ���͸��� �� ������Ʈ ��θ� �߰��غ�����

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 675;

class SDLApp {  //����
public:
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    void init() {
        window = SDL_CreateWindow("missingPerson", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        screenSurface = SDL_GetWindowSurface(window);
        SDL_BlitSurface(backgroundImage, NULL, screenSurface, NULL); // ���ȭ�� �׸���

        TTF_Init(); //��Ʈ
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

    bool isSelecting = false; //�÷��̾ I found missing person ��ư�� ���� ��������(true) �ƴ���(false)
    int SelectedList; //�÷��̾ ������ ����Ʈ �ε���
    int SelectedPerson; //�÷��̾ ������ �̾��� �ε���

    int correct = 0; //�÷��̾ ã�� �̾� ��

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


    Uint32 startTime = SDL_GetTicks();  // ���� ���� �ð� ���
    Uint32 lastUpdateTime = 0; // ���������� time ������Ʈ�� �ð�

    Game() {
        sdlApp.init();  //����

        redColor = SDL_MapRGB(sdlApp.screenSurface->format, 255, 0, 0);     //��(�ӽ�)
        greenColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 255, 0);
        blueColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 0, 255);
        grayColor = SDL_MapRGB(sdlApp.screenSurface->format, 128, 128, 128);
        lightGrayColor = SDL_MapRGB(sdlApp.screenSurface->format, 200, 200, 200);
        blackColor = SDL_MapRGB(sdlApp.screenSurface->format, 0, 0, 0);
        whiteColor = SDL_MapRGB(sdlApp.screenSurface->format, 255, 255, 255);

        font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);   //��Ʈ, ũ��
        sfont = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 12);

        createButtons();    //��ư����
        createRectangles(); //�簢������
        createPersons(); //�������
    }

    ~Game() {
        sdlApp.close(); //����
    }

    void createButtons() {

        screenBtn[0] = new screenButton(170, 42, 300, 180, screen_Image[0], "screen 1");   //ȭ�� ��ư 4��
        screenBtn[1] = new screenButton(470, 42, 300, 180, screen_Image[1], "screen 2");
        screenBtn[2] = new screenButton(170, 222, 300, 180, screen_Image[2], "screen 3");
        screenBtn[3] = new screenButton(470, 222, 300, 180, screen_Image[3], "screen 4");

        personListBtn[0] = new listButton(776, 47, 254, 65, lightGrayColor, "person 1");  //�̾� ����Ʈ ��ư 3��
        personListBtn[1] = new listButton(776, 118, 254, 65, lightGrayColor, "person 2");
        personListBtn[2] = new listButton(776, 189, 254, 65, lightGrayColor, "person 3");

        choiceBtn[0] = new choiceButton(785, 322, 230, 33, grayColor, "How can I help?");   //��ȭ ������ ��ư 4��(�ϳ��� ��Ȱ��ȭ)
        choiceBtn[1] = new choiceButton(785, 361, 230, 23, lightGrayColor, "I found missing person");
        choiceBtn[2] = new choiceButton(785, 390, 230, 23, lightGrayColor, "What should I do?");
        choiceBtn[3] = new choiceButton(785, 419, 230, 23, lightGrayColor, "I can't find anyone(hint)");
        choiceBtn[4] = new choiceButton(785, 448, 230, 23, lightGrayColor, "end call");
        for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);

        callBtn = new callButton(730, 408, 40, 40, call_Image);  //��ȭ��ư
        exitBtn = new exitButton(725, 47, 30, 30, exit_Image);  //�����ư
        exitBtn->setVisible(false);
        leftBtn = new leftButton(178, 209, 30, 30, left_Image); //���ʹ�ư
        leftBtn->setVisible(false);
        rightBtn = new rightButton(732, 209, 30, 30, right_Image);   //�����ʹ�ư
        rightBtn->setVisible(false);

        startBtn = new startButton(550, 350, 100, 40, grayColor, "press to start");

        title = new titleButton(350, 120, 500, 200, titleImage, ""); //����
    }

    void createRectangles() {  
        bigScreen[0] = new Screen(bigscreen_Image[0], " ", 170, 42, 600, 360);   //ȭ��
        bigScreen[1] = new Screen(bigscreen_Image[1], " ", 170, 42, 600, 360);
        bigScreen[2] = new Screen(bigscreen_Image[2], " ", 170, 42, 600, 360);
        bigScreen[3] = new Screen(bigscreen_Image[3], " ", 170, 42, 600, 360);


        callRect = new Rectangle(grayColor, " ", 780, 271, 240, 210);  //��ȭ ���
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
                random_device rd; //�õ尪 ��� ���� random_device ����
                mt19937 gen(rd()); //���� ���� ���� �ʱ�ȭ

                uniform_int_distribution<int> whatScreen(0, 3); // 0, 1, 2, 3 �� �ϳ�. � ��ũ���� ������
                uniform_int_distribution<int> whatPerson(0, 16); // 0 ~ 16 �� �ϳ��� ����. � �̾� ������
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

    void handleEvents() {   //�̺�Ʈ
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {   //����
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) { //���콺 Ŭ��
                int mouseX = e.button.x;
                int mouseY = e.button.y;

                for (int i = 0; i < 4; i++) { // screenButton
                    screenBtn[i]->handleEvent(e, i + 1); // �̺�Ʈ�ڵ鷯, ��ư ��ȣ �Ѱ��ֱ�
                }
                for (int i = 0; i < 3; i++) { // listButton
                    if (isSelecting && personListBtn[i]->handleEvent(e, i + 1)) { 
                        //�̾� ���� ���̰�, ����Ʈ��ư ��������

                        SelectedList = i; //� ����Ʈ �������� ����.
                        for (int i = 0; i < 3; i++) personListBtn[i]->setColor(lightGrayColor); //�� �ʱ�ȭ
                        personListBtn[SelectedList]->setColor(greenColor); //���� ����Ʈ �� �ٲٱ�
                        choiceBtn[0]->setText("Where is " + personListBtn[SelectedList]->getInfo()->name + "?");
                    }
                }
                for (int i = 1; i < 5; i++) { // choiceButton (0 ���� 1~4)
                    choiceBtn[i]->handleEvent(e, i);
                }
                callBtn->handleEvent(e, NULL);
                exitBtn->handleEvent(e, NULL);
                leftBtn->handleEvent(e, NULL);
                rightBtn->handleEvent(e, NULL);		//���� �̰ɷ� �̺�Ʈ ó���Ϸ��� �ߴµ�...

                handleMouseClick(mouseX, mouseY);
            }
        }
    }

    void handleMouseClick(int mouseX, int mouseY) {
        if (bigScreen[0]->isVisible == false && bigScreen[1]->isVisible == false 
            && bigScreen[2]->isVisible == false && bigScreen[3]->isVisible == false) {    //���� ȭ���϶�
            for (int i = 0; i < 4; i++) {       //ȭ�� Ŭ���ϸ� ��üȭ��
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
        else {  //��üȭ���϶�
            handleBigScreenClick(mouseX, mouseY);
        }

        handleCallButtons(mouseX, mouseY);  //��ȭ��ư�� Ŭ��
    }

    void handleBigScreenClick(int mouseX, int mouseY) {
        btnRect = leftBtn->getRect();   //���ʹ�ư
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            bigScreen[currentScreen]->setVisible(false); //�����ִ� ��ũ�� ġ���
            currentScreen = (currentScreen + 3) % 4;
            bigScreen[currentScreen]->setVisible(true); //���� ��ũ�� �����ֱ�
        }

        btnRect = rightBtn->getRect();  //�����ʹ�ư
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            bigScreen[currentScreen]->setVisible(false); //�����ִ� ��ũ�� ġ���
            currentScreen = (currentScreen + 1) % 4;
            bigScreen[currentScreen]->setVisible(true); //���� ��ũ�� �����ֱ�
        }

        btnRect = exitBtn->getRect();   //�����ư
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            bigScreen[currentScreen]->setVisible(false);
            exitBtn->setVisible(false);
            leftBtn->setVisible(false);
            rightBtn->setVisible(false);
            for (int i = 0; i < 4; i++) screenBtn[i]->setVisible(true);
            currentScreen = -1;
        }
        if (currentScreen != -1 && isSelecting == true) { //ū ��ũ�� �����̰� �������̸�

            SelectedPerson = bigScreen[currentScreen]->handleEvent(e); //�� Ŭ���ߴ��� ���� (-2:���, -1:�Ϲݻ��, 0~16:�̾�)

            if (SelectedPerson == -2) return; //��� Ŭ���� ��쿣 �׳� ����

            if (SelectedPerson == -1) { //�̾ư� �ƴ� �Ϲ� ��� Ŭ������ ���
                
                choiceBtn[0]->setText("You got the wrong person.");

                isSelecting = false; //���� �� ����
                personListBtn[SelectedList]->setColor(lightGrayColor);
                choiceBtn[1]->setColor(lightGrayColor);
                return;
            }

            if(personListBtn[SelectedList]->getInfo() 
                == bigScreen[currentScreen]->getMissingPerson(SelectedPerson)->getInfo()) { //������ ��ġ�� ���

                choiceBtn[0]->setText("Good job!");

                correct++; //���� �� ++
                cout << "ã�� �̾��� ��: " << correct << endl;

                //�̾� �����
                bigScreen[currentScreen]->deleteMissingPerson(SelectedPerson);

                //�̾� ���� ä���
                int p = 0;
                int s = 0;

                do {
                    random_device rd; //�õ尪 ��� ���� random_device ����
                    mt19937 gen(rd()); //���� ���� ���� �ʱ�ȭ

                    uniform_int_distribution<int> whatScreen(0, 3); // 0, 1, 2, 3 �� �ϳ�. � ��ũ���� ������
                    uniform_int_distribution<int> whatPerson(0, 16); // 0 ~ 16 �� �ϳ��� ����. � �̾� ������
                    p = whatPerson(rd);
                    s = whatScreen(rd);
                } while (missingPersonInfo[p].isinList || s == currentScreen);

                bigScreen[s]->setMissingPerson(missingPersonImage[p], &missingPersonInfo[p]);
                personListBtn[SelectedList]->setInfo(&missingPersonInfo[p]);
                missingPersonInfo[p].isinList = true;

                //���� �� ����
                isSelecting = false;
                personListBtn[SelectedList]->setColor(lightGrayColor);
                choiceBtn[1]->setColor(lightGrayColor);
            }
            else { //��ġ���� ���� ���
                
                choiceBtn[0]->setText("You got the wrong person.");

                //���� �� ����
                isSelecting = false;
                personListBtn[SelectedList]->setColor(lightGrayColor);
                choiceBtn[1]->setColor(lightGrayColor);
            }
            
        }
    }

    void handleCallButtons(int mouseX, int mouseY) {
        btnRect = callBtn->getRect();   //��ȭ��ư
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            callRect->setVisible(true);
            for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(true);
        }

        btnRect = choiceBtn[1]->getRect();  //�̾� ã�ҽ��ϴ� ��ư
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            
            isSelecting = true; //�̾� ���� ���°� ��
            choiceBtn[1]->setColor(greenColor);

            choiceBtn[0]->setText("Who did you find?");
            choiceBtn[3]->setText("never mind");
            //�÷��̾�� � �̾����� ����Ʈ ��ư Ŭ���ϸ� ��
        }

        btnRect = choiceBtn[2]->getRect();  //������ �ؾ� �ϳ���? ��ư
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            choiceBtn[0]->setText("Look for people on the missing children list.");
        }

        btnRect = choiceBtn[3]->getRect();  //��Ʈ ��ư
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            choiceBtn[0]->setText("Look for people with special characteristics");
            if (isSelecting == true) {   //�����߿��� ��� ��ư - �����ϱ�
                choiceBtn[0]->setText("How can I help?");
                choiceBtn[1]->setColor(lightGrayColor);
                choiceBtn[3]->setText("I can't find anyone(hint)");
                isSelecting = false;
            }
        }

        btnRect = choiceBtn[4]->getRect();  //��ȭ ���� ��ư
        if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
            callRect->setVisible(false);
            for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);
            choiceBtn[0]->setText("How can I help?");
            choiceBtn[1]->setColor(lightGrayColor);
            choiceBtn[3]->setText("I can't find anyone(hint)");
            for(int i = 0; i < 3; i++) personListBtn[i]->setColor(lightGrayColor);
            isSelecting = false;    //����
        }
    }

    void render() { //�׸���
        SDL_BlitSurface(backgroundImage, NULL, sdlApp.screenSurface, NULL); //���ȭ��

        for (int i = 0; i < 4; ++i) {   //ȭ�� 4��
            screenBtn[i]->render(sdlApp.screenSurface, font);
        }
        for (int i = 0; i < 3; ++i) {   //�̾� ����Ʈ 3��
            personListBtn[i]->render(sdlApp.screenSurface, sfont);
        }
        callRect->render(sdlApp.screenSurface, sfont);  //��ȭ ���
        for (int i = 0; i < 5; ++i) {   //��ȭ ��������ư
            choiceBtn[i]->render(sdlApp.screenSurface, sfont);
        }
        
        if (currentScreen != -1) {
            bigScreen[currentScreen]->render(sdlApp.screenSurface, font);  //��üȭ��
        }

        callBtn->render(sdlApp.screenSurface, sfont);   //��ȭ��ư
        exitBtn->render(sdlApp.screenSurface, sfont);   //�����ư
        leftBtn->render(sdlApp.screenSurface, sfont);   //���ʹ�ư
        rightBtn->render(sdlApp.screenSurface, sfont);  //�����ʹ�ư

        date->render(sdlApp.screenSurface, sfont);
        time->render(sdlApp.screenSurface, font);
        reception->render(sdlApp.screenSurface, sfont);
        number->render(sdlApp.screenSurface, sfont);
        

        SDL_UpdateWindowSurface(sdlApp.window);     //ȭ�� ������Ʈ
    }

    void mainMenu() {   //���� �޴�
        SDL_BlitSurface(backgroundImage, NULL, sdlApp.screenSurface, NULL); //���ȭ�� �׸���
        title->render(sdlApp.screenSurface, font);  //���� �׸���
        startBtn->render(sdlApp.screenSurface, sfont);  //���۹�ư �׸���

        SDL_UpdateWindowSurface(sdlApp.window);     //ȭ�� ������Ʈ

        while (!quit) { //�ݺ�
            while (SDL_PollEvent(&e) != 0) { //���� �޴� �̺�Ʈ ����
                if (e.type == SDL_QUIT) {   //����
                    quit = true;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {  //���콺 Ŭ�� - ���۹�ư
                    int mouseX = e.button.x, mouseY = e.button.y;
                    btnRect = startBtn->getRect();  //���۹�ư Ŭ����
                    //return -> run���� ��, Ʃ�丮�� ���� �� ���� ����
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) return;
                }
            }
        }
    }

    void tutorial() {   //��ȭ�� ����, ������(��ȭ��ư Ŭ���ϸ�) ������ ������ ���´�
        string tutorialText[11] = {  //Ʃ�丮�� ����
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
        int tutorialProgress = -1;   //�ϳ��� ++�ϸ� tutorial text ������
        int inCall = 0; //0�̸� �ƴ�, 1�̸� ��ȭ��, -1�̸� Ʃ�丮�� ����


        while (!quit) { //�ݺ�


            if (inCall == 0) {     // 0.5�ʸ��� ��ȭ��ư�����̰�(�ӽ�)
                SDL_Delay(500); // 0.5�� ��ٸ���
                callBtn->setVisible(!callBtn->isVisible);
            }

            while (SDL_PollEvent(&e) != 0) { //Ʃ�丮�� �̺�Ʈ ����
                if (e.type == SDL_QUIT) {   //����
                    quit = true;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {  //���콺 Ŭ��
                    int mouseX = e.button.x, mouseY = e.button.y;

                    btnRect = callBtn->getRect();   //��ȭ��ư
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        inCall = 1;
                        callBtn->setVisible(true);
                        callRect->setVisible(true);
                        choiceBtn[0]->setText("~Call from mom~");
                        choiceBtn[0]->setVisible(true);
                        choiceBtn[1]->setText("ok");
                        choiceBtn[1]->setVisible(true);
                    }

                    btnRect = choiceBtn[1]->getRect();  //������ 1: ok ��ư
                    if (choiceBtn[1]->isVisible && mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        tutorialProgress++;
                        choiceBtn[0]->setText(tutorialText[tutorialProgress]);  //�ؽ�Ʈ �迭���� ������� �̾ƿ�
                        if (tutorialProgress == 10) {
                            choiceBtn[1]->setVisible(false);    //ok ������ ���ֱ�
                            choiceBtn[4]->setVisible(true);     //��ȭ ���� ������ ���̱�
                        }
                    }

                    btnRect = choiceBtn[4]->getRect();  //������ 4 - ��ȭ(Ʃ�丮��) ����
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        callRect->setVisible(false);
                        for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);
                        choiceBtn[0]->setText("How can I help?");
                        choiceBtn[1]->setText("I found missing person");
                        inCall = -1;
                    }

                }
            }
            render();   //ȭ�鿡 �׸���
            if (inCall == -1) return;   //��ȭ ���� �� �������� ����
        }
    }

    void run() {
        startTime = SDL_GetTicks(); //���� ���� �ð� ���
        while (!quit) { //���� ����
            handleEvents(); //�̺�Ʈ ó��

            Uint32 currentTime = SDL_GetTicks(); // ���� �ð� ��������
            if (currentTime - lastUpdateTime >= 1000) {     //1�� �������� Ȯ���ϰ�
                updateTime(); // 1�ʸ��� �ð� ������Ʈ
                lastUpdateTime = currentTime; //������Ʈ�� �ð� ����
            }

            render();   //ȭ�鿡 �׸���

            if ((currentTime - startTime) / 1000 > 96) return;  //5�ÿ� ���� ����(60��(12��) x 8�ð�)
        }
    }

    void ending() {   //��ȭ�� ����, ������(��ȭ��ư Ŭ���ϸ�) ������ ������ ���´�
        string goodEndingText[10] = {  //���ǿ��� ����
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
        string badEndingText[9] = {  //��忣�� ����
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
        int endingProgress = -1;   //�ϳ��� ++�ϸ� ending text ������
        int inCall = 0; //0�̸� �ƴ�, 1�̸� ��ȭ��, -1�̸� Ʃ�丮�� ����

        for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);
        callRect->setVisible(false);
        
        isSelecting = false;


        while (!quit) { //�ݺ�


            if (inCall == 0) {     // 0.5�ʸ��� ��ȭ��ư�����̰�(�ӽ�)
                SDL_Delay(500); // 0.5�� ��ٸ���
                callBtn->setVisible(!callBtn->isVisible);
            }

            while (SDL_PollEvent(&e) != 0) { //���� �̺�Ʈ ����
                if (e.type == SDL_QUIT) {   //����
                    quit = true;
                }
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {  //���콺 Ŭ��
                    int mouseX = e.button.x, mouseY = e.button.y;

                    btnRect = callBtn->getRect();   //��ȭ��ư
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        inCall = 1;
                        callBtn->setVisible(true);
                        callRect->setVisible(true);
                        choiceBtn[0]->setText("~Call from mom~");
                        choiceBtn[0]->setVisible(true);
                        choiceBtn[1]->setText("ok");
                        choiceBtn[1]->setVisible(true);
                    }

                    btnRect = choiceBtn[1]->getRect();  //������ 1: ok ��ư
                    if (choiceBtn[1]->isVisible && mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        endingProgress++;
                        if(correct >= 5) choiceBtn[0]->setText(goodEndingText[endingProgress]);  //���ǿ���
                        else choiceBtn[0]->setText(badEndingText[endingProgress]);  //��忣��
                        if ((endingProgress == 10 && correct >= 5) || endingProgress == 9) {  //���ǿ��� = 10����, ��忣�� = 9���� ����
                            choiceBtn[1]->setVisible(false);    //ok ������ ���ֱ�
                            choiceBtn[4]->setVisible(true);     //��ȭ ���� ������ ���̱�
                        }
                    }

                    btnRect = choiceBtn[4]->getRect();  //������ 4 - ��ȭ(����) ����
                    if (mouseX >= btnRect.x && mouseX <= btnRect.x + btnRect.w && mouseY >= btnRect.y && mouseY <= btnRect.y + btnRect.h) {
                        callRect->setVisible(false);
                        for (int i = 0; i < 5; i++) choiceBtn[i]->setVisible(false);
                        inCall = -1;
                    }

                }
            }
            render();   //ȭ�鿡 �׸���
            if (inCall == -1) return;   //��ȭ ���� �� �������� ����
        }
    }

    void updateTime() {     //�ð� ������Ʈ
        Uint32 elapsedTime = (SDL_GetTicks() - startTime) / 2000; // ��� �ð�(2�� 1ƽ)

        int hour = (elapsedTime / 6 + 9) % 24;  //9�� ����, 24�ð� ����Ŭ
        int minute = elapsedTime * 10 % 60;     //1�ʿ� 10�� ����, 60�� ����Ŭ

        char timeString[6];     //�ð� ���� ���߱�
        snprintf(timeString, sizeof(timeString), "%02d:%02d", hour, minute);

        //printf("hour: %d, minute: %d, �ð�����: %s\n", hour, minute, timeString); //������

        time->setText(timeString);
    }
};

int main(int argc, char* args[]) {
    Game game;
    game.mainMenu();    //���� �޴�
    game.tutorial();    //Ʃ�丮��
    game.run();     //���� ����
    game.ending();  //����
    return 0;
}