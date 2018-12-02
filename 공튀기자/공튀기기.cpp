#include <iostream>
#include <GL\glut.h>
#pragma comment(lib,"winmm") 
#include <mmsystem.h>
#include <stdlib.h>

#include <gl\glu.h>
#include <time.h>
#include <random>
#include <gl\glaux.h>
#pragma warning(disable : 4996)  


#define GL_BGRA 0x80E1 // 32비트 사용을 하기위해.
#define GL_BGR_EXT 0x80E0
#define MAX_NO_TEXTURES 1 // 텍스쳐 생성위해만든 전처리
#define SOUND_FILE_NAME_BGM "../sound/bgm.wav"
#define SOUND_FILE_NAME_JUMP "../sound/jumpsound.wav"
#define SOUND_FILE_NAME_BOSSSTART "../sound/bossstart.wav"
#define SOUND_FILE_NAME_BOSSKILL "../sound/bosskill.wav"
#define SOUND_FILE_NAME_BOSSKILLED "../sound/bosskilled.wav"
#define SOUND_FILE_NAME_DEAD "../sound/dead.wav"
#define SOUND_FILE_NAME_ENDING "../sound/ending.wav"
int gamestart = 0;
using namespace std;
int view = 1;
int death = 0;

clock_t go,finish; //게임진행시간을 저장하기위해 만든 변수


GLuint texture[MAX_NO_TEXTURES];
GLint clearcount = 0;
GLfloat randomr; // 0.0~1.0까지 랜덤으로 생성되는 r값
GLfloat randomg; // 0.0~1.0까지 랜덤으로 생성되는 g값
GLfloat randomb; // 0.0~1.0까지 랜덤으로 생성되는 b값
GLdouble fradius = 0.1; // 피니쉬 라인 구체의 반지름
GLfloat bx, by, bz; // 공의 x,y,z 이동을 을 저장하기위한 변수 = 원래 공은 0,0에서 생성된거지만 translatef 로 공을 이동시킨 개념.
GLfloat f1x, f1y, f1z; // finish 할수있는 구체가 그려질 값이 저장된 변수
GLdouble radius = 0.05; // 반지름 저장 함수 
int start; // 초기공의 위치 제어를 위해 추가한 변수
GLfloat sky; // 공이 최고로 뛸수있는 위치를 제어하는 변수
int upordown = 1; // 공이 점프 하는지, 떨어지고있는지 방향성을 제어하기위해 만든 변수. 1일때가 up , 0일때가 down
GLint shot = 0;
GLfloat g = 0.003; // 공이 자연스럽게 튕기기 위해 중력을 추가.
GLfloat rxg = 0.04; // 공이 자연스럽게 앞으로 움직이기위해 추가한 변수 (오른쪽)
GLfloat lxg = 0.04; // 공이 자연스럽게 뒤로 움직이기위해 추가한 변수 (왼쪽)
GLfloat sxg = 0.04; // 공이 자연스럽게 옆으로 움직이기위해 추가한 변수(양옆)
GLfloat jump = 0; // 스페이스바를 누를때 점프 값을 저장하는 변수
GLfloat randomlightrgb[] = { 0.0,0.0,0.0,0.0 };
GLdouble stage2radius = 0.1;
GLint obstacle = 0; // 공이 장애물에 부딪혔는지 안부딪혔는지 알수있도록 생성한 변수
GLfloat stage2otc1zstep=0.06; // 2스테이지 첫번째 장애물의 z 값의 자동이동을 위해서 0.01이라는 일정값 부여 (공의 속도)
GLfloat stage2otc2ystep = 0.06;// 2스테이지 두번째 장애물의y 값의 자동이동을 위해서 0.02이라는 일정값 부여
GLfloat stage2otc3zstep = 0.08;// 2스테이지 세번째 장애물의z 값의 자동이동을 위해서 0.03이라는 일정값 부여
GLfloat stage2otc4ystep = 0.1;// 2스테이지 네번째 장애물의 y 값의 자동이동을 위해서 0.01이라는 일정값 부여

GLfloat stage2otc1x=-4.44;
GLfloat stage2otc1y=-2.5;    // 2스테이지 양옆으로움직이는 장애물 1의 x,y,z 값 
GLfloat stage2otc2y = -2.5;    // 2스테이지 양옆으로움직이는 장애물 1의 x,y,z 값 
GLfloat stage2otc1z = 0;
GLfloat stage2otc3z = 0;
GLfloat stage2otc4y = -2.5;    // 2스테이지 양옆으로움직이는 장애물 1의 x,y,z 값 
GLfloat stage2otc2x = -3.44; // 2스테이지 양옆으로움직이는 장애물 2의 x값
GLfloat stage2otc3x = -2.44; // 2스테이지 양옆으로움직이는 장애물 3의 x값
GLfloat stage2otc4x = -1.44; // 2스테이지 양옆으로움직이는 장애물 4의 x값
GLfloat bossx=3.0;
GLfloat bossy = -2.45;
GLfloat bossz=0.0;
int jumonoff = 0; // 점프인지 아닌지 체크하는 변수
					// 물체의 색을 제어하기위하여 따로 4개의 변수로 만듬.
GLfloat qaRed[] = { 1.0, 0.0, 0.0, 0.0 }; // 빨간색
GLfloat qaBlack[] = { 0.0, 0.0, 0.0, 0.0 }; // 검은색 
GLfloat qaGreen[] = { 0.0, 1.0, 0.0, 0.0 }; // 초록색 
GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 0.0 }; // 하얀색
int timerstart = 0; // 게임 실행시간 타이머가 켜졌는지 확인하는변수

GLfloat red[] = { 1.0, 0.0 , 0.0 , 0.0 };
GLfloat green[] = { 0.0, 1.0, 0.0, 0.0 };
GLfloat blue[] = { 0.0, 0.0, 1.0, 0.0 };

float spotlightDirection[] = { -1.0f, 0.0f, -1.0f };

GLint ballmove = 1; // display 에서 스테이지와 공을 따로 분류하기위해  만든조건문에 들어갈 변수

GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 }; // 조명의 특성 설정 .
GLfloat emitLight[] = { 0.9, 0.9, 0.9, 0.01 };

GLfloat Noemit[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat qaLightPosition[] = { -50.0, 10.0 ,4.0, 0 }; // 조명의 위치.

int bosscount = 0;//보스 등장여부 .
int bossblue = 1; //보스가 파란색일때 1
int bossred = 0; //보스가 빨간색일때
int bossgreen = 0; // 보스가 초록색일떄 
int bluecount = 3; // 파란보스 체력바 량
int greencount = 2; // 초록보스 체력바량
int redcount = 2; // 빨간보스 체력바  량
int bossno = 0;
int totalhelth = bluecount + greencount + redcount;





	







void myidle() {
	glShadeModel(GL_SMOOTH); // 부드러운 색상으로 모델을 그린다 .
	glEnable(GL_DEPTH_TEST); // 객체 여러개를 그릴때 , 깊이마다 색깔이 다를때 명확하게 만들기위해 선언
	glDisable(GL_CULL_FACE); // 뒷면을 지운다 
	glFrontFace(GL_CCW); //반시계방향으로 앞면을 뒤바꾼다
	


}

void initLighting()
{

	// 조명 활성화 
	

	// 조명 특성을 저장한 변수를 함수를 이용하여 조명에 적용시킨다.
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// 조명의 위치 지정하는 함수에 조명의 위치를 저장한 변수를 매개변수로 하여 조명 위치 지정.
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

}





void display(void)
{
	f1x = 5.6399; // 스테이지 1피니쉬라인 초기함수 
	f1y = -2.41;
	if (gamestart == 1) {


		

		sky = -1.8000;  // sky = 공이 뛰는 높이를 제어하기위해 만든 변수 
		
		glMatrixMode(GL_MODELVIEW);


		glLoadIdentity();
		if (view == 1) { // 공을 중앙으로 이동하는 cctv 시점
			gluLookAt(0.0, 0.0, 10.0, // 카메라의 위치
				bx, 0.0, bz, // 카메라가 보는 초점
				0.0, 1.0, 0.0); // 카메라의 각도

		}
		if (view == 3) { // 평면시점
			gluLookAt(bx, by, 10.0, // 카메라 위치
				bx, 0.0, bz, // 카메라가 보는 초점
				0.0, 1.0, 0.0); // 카메라의 각도
		}
		if (view == 2) { // 공의 시점 
			gluLookAt(bx - 2, by, bz, // 카메라의 위치
				f1x, f1y, f1z, // 카메라가 보는 초점
				0.0, 1.0, 0.0); // 카메라의 각도

		}
		if (view == 5) { // 보스모드시점
			gluLookAt(bx, by, 0.0, // 카메라의 위치
				bossx,bossy,bossz, // 카메라가 보는 초점
				0.0, 1.0, 0.0); // 카메라의 각도
			if (bosscount == 0) {
				view = 1;
			}
		}






		//물체의 특성 지정



		if (start == 0) { // 1스테이지 시작할때 공의 위치 초기값

			bx = -5.0;
			by = -2.5;
			bz = 0.0;
			start = 100; // 초기값 설정 이후 쓰레기값을 주어서 공의 움직임을 자유롭게 만든다. 

		}




		// clearcount 변수의 값에 따라서 스테이지가 달라질수있도록할수있게하는 조건문. = finish 라인에 들어갔을때
		if (f1x - 0.11 <= bx && f1y >= by &&f1x + 0.11 >= bx&&clearcount == 0 )  { // 피니쉬라인에 공이 있고 스테이지 1일때
			clearcount = 1; //스테이지 2로 설정
			if (clearcount == 1) { // 스테이지 2일때 
				
				start = 0; // 공의 위치 초기화
				
				
				glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW 의 버퍼를 지운다 (지금까지 그린것 지움)
				glutSwapBuffers(); // 버퍼를 번경 
				glutPostRedisplay(); // 다시그리기
				

			}
		}

		else if (f1x - 0.11 <= bx && f1y >= by &&f1x + 0.11 >= bx&&clearcount == 1) { // 피니쉬라인에 도착하고 스테이지 2일때
			clearcount =2; //스테이지 3으로 설정
			if (clearcount == 2) {
				
				start = 0; // 공의 위치 초기화
				
				glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW 의 버퍼를 지운다 (지금까지 그린것 지움)
				glutSwapBuffers(); // 버퍼를 번경 
				glutPostRedisplay(); // 다시그리기

			}
		}

		else if (f1x - 0.11 <= bx && f1y >= by &&f1x + 0.11 >= bx&&clearcount == 2) { // 스테이지 2일때 3으로 넘어가도록 확인하는 조건문
			
				start = 0; // 공의 위치 초기화
				clearcount = 1; // 스테이지 변수가 1로 바뀐다
				PlaySound(TEXT(SOUND_FILE_NAME_JUMP), 0, SND_PURGE | SND_LOOP);
				glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW 의 버퍼를 지운다 (지금까지 그린것 지움)
				glutSwapBuffers(); // 버퍼를 번경 
				glutPostRedisplay(); // 다시그리기

			
		}
	

		if (clearcount==1 && -4.24>=bx && -4.44<=bx && stage2otc1z-0.1<=bz && stage2otc1z+0.1>=bz){
			
			start = 0; 
			PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
			}
	// 스테이지 2일때 첫번째 구체 맞았는지 안맞았는지 확인


		// 1스테이지 구현부 시작 
		if (clearcount == 0) { // 1스테이지 일 경우에  맵이 이렇게 그려진다.
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW 의 버퍼를 지운다
			

			glPushMatrix(); // 1맵 넓은 바닥 구현부
			

			glColor3f(0.3, 0.4, 0.3);
			glBegin(GL_POLYGON); //1맵 바닥 면 
			glVertex3f(-8.9, -2.5, -2);
			glVertex3f(-8.9, -2.5, 2);
			glVertex3f(8.9, -2.5, 2);
			glVertex3f(8.9, -2.5, -2);
			
			glEnd();
			glFlush();
			glPopMatrix();

			glPushMatrix(); // 1맵 중앙 바닥줄구현부
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON);// 1맵 중앙 바작줄 구현
			glVertex3f(-8.9, -2.5, 0.05);// 
			glVertex3f(-8.9, -2.5, -0.05);
			glVertex3f(8.9, -2.5, -0.05);
			glVertex3f(8.9, -2.5, 0.05);
			glEnd();
			glFlush();
			glPopMatrix();

			glPushMatrix(); // 1맵 오른쪽 바닥줄
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON);// 1맵 오른쪽 바닥줄 구현부
			glVertex3f(-8.9, -2.5, 0.7);// 
			glVertex3f(-8.9, -2.5, 0.6);
			glVertex3f(8.9, -2.5, 0.6);
			glVertex3f(8.9, -2.5, 0.7);
			glEnd();
			glFlush();
			glPopMatrix();

			glPushMatrix(); // 1맵 왼쪽 바닥줄
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON);// 1맵 왼쪽 바닥줄 구현부
			glVertex3f(-8.9, -2.5, -0.7);// 
			glVertex3f(-8.9, -2.5, -0.6);
			glVertex3f(8.9, -2.5, -0.6);
			glVertex3f(8.9, -2.5, -0.7);
			glEnd();
			glFlush();
			glPopMatrix();





			glPushMatrix();// 1번째맵 첫번째 장애물 구현부시작 
			
			glPushMatrix(); //1맵 피니쉬 포탈 구현부 매트릭스 시작
			randomlightrgb[0] = { randomr };
			randomlightrgb[1] = { randomg };
			randomlightrgb[2] = { randomb };
			randomlightrgb[3] = { 0 };
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);

			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
			
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80);

			glTranslatef(f1x, f1y, f1z); // 피니쉬 포탈 구체 구현하는 부분
			glutSolidSphere(fradius, 100, 100);

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glPopMatrix();
			glPushMatrix(); // 1번째맵 첫번째 장애물 삼각형 왼쪽옆면,앞면 구현(공의 위치를 명확하게 구분짓기 위해)



			


			glPushMatrix();
			glPushMatrix(); // 1 번째맵  첫번째 장애물 삼각형 왼쪽옆면구현 
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON); // 1번째맵 첫번째 장애물 삼각형 왼쪽 옆면
			glVertex3f(-3.35, -2.3, -1);
			glVertex3f(-3.35, -2.3, 1);
			glVertex3f(-3.4, -2.6, 1);
			glVertex3f(-3.4, -2.6, -1);
			glEnd();
			glFlush();
			glPopMatrix();
						 

			glPushMatrix(); // 1 번째맵  첫번째 장애물 삼각형 오른쪽옆면구현 시작
			glColor3f(1.1, 0.0, 0.0);
			glBegin(GL_POLYGON); // 1번째맵 첫번째 장애물 삼각형 오른쪽 옆면
			glVertex3f(-3.35, -2.3, -1);
			glVertex3f(-3.35, -2.3, 1);
			glVertex3f(-3.3, -2.6, 1);
			glVertex3f(-3.3, -2.6, -1);
			glEnd();
			glFlush();
			glPopMatrix();  //  첫번째맵 첫번째 삼각형 오른쪽 옆면 구현부 끝
			glPopMatrix();

			glPushMatrix();
			glColor3f(randomr, randomg, randomb);
			glBegin(GL_TRIANGLES); // 1번째맵 첫번째 장애물 삼각형 앞면
			glVertex3f(-3.4, -2.6, 1);
			glVertex3f(-3.35, -2.3, 1);
			glVertex3f(-3.3, -2.6, 1);
			glEnd();
			glFlush();
			glPopMatrix();

			glPopMatrix(); // 1번째맵 첫번째 장애물 구현부 끝

			glPushMatrix(); // 1번째맵 두번째 장애물 구현 시작
			

			glPushMatrix(); // 1 번째맵  두번째 장애물 삼각형 왼쪽옆면구현 
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON); // 1번째맵 두번째 장애물 삼각형 왼쪽 옆면
			glVertex3f(-2.07, -2.2, -1);
			glVertex3f(-2.07, -2.2, 1);
			glVertex3f(-2.12, -1.7, 1);
			glVertex3f(-2.12, -1.7, -1);
			glEnd();
			glFlush();
			glPopMatrix();
			
			if (ballmove == 1) { // 공이 움직일수있는 상태와 못움직이는 상태를 구분짓기위해if 문으로 따로 나눔 
				glPushMatrix(); // 공구현부 매트릭스 1맵
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);

				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
				glTranslatef(bx, by, bz); // 공을 구현하는 부분
				glutSolidSphere(radius, 100, 100);
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				glPopMatrix();
			}

			glPushMatrix(); // 1번째맵 두번째 장애물삼각형 오른쪽옆면 구현 ( 공의 위치를 명확하게 구분짓기 위해)
			glColor3f(1.0, 1.0, 0.3);
			glBegin(GL_POLYGON); // 1번째맵 두번째 장애물 첫번째 장애물 삼각형 오른쪽 옆면
			glVertex3f(-2.07, -2.2, -1);
			glVertex3f(-2.07, -2.2, 1);
			glVertex3f(-2.02, -1.7, 1);
			glVertex3f(-2.02, -1.7, -1);
			glEnd();
			glFlush();
			glPopMatrix();  //  1번째맵 두번째 장애물 삼각형 오른쪽 옆면 구현부 끝

			glPushMatrix(); // 1번째맵 두번째 장애물 삼각형 천장쪽 면 구현 
			glColor3f(0.0, 0.2, 0.3);
			glBegin(GL_POLYGON); // 1번째맵 두번째 장애물 삼각형 천장 면 
			glVertex3f(-2.02, -1.7, -1);
			glVertex3f(-2.02, -1.7, 1);
			glVertex3f(-2.12, -1.7, 1);
			glVertex3f(-2.12, -1.7, -1);
			glEnd();
			glFlush();
			glPopMatrix();  //  1번째맵 두번째 장애물 삼각형천장 면 구현부 끝

			glPushMatrix();
			glBegin(GL_TRIANGLES); // 1 번째맵 두번째 장애물 앞면
			glColor3f(randomb, randomr, randomg);
			glVertex3f(-2.12, -1.7, 1);
			glVertex3f(-2.07, -2.2, 1);
			glVertex3f(-2.02, -1.7, 1);
			glEnd();
			glFlush();
			glPopMatrix();
			/*
			glPushMatrix(); // 1맵 천장 구현부
			glEnable(GL_LIGHTING);
			glColor3f(0.0, 0.3, 0.1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
			*/
			glBegin(GL_POLYGON); //1맵 천장 사각형
			glVertex3f(-8.9, -1.6, 0);
			glVertex3f(-8.9, -1.7, 0);
			glVertex3f(8.9, -1.7, 0);
			glVertex3f(8.9, -1.6, 0);
			glEnd();
			glFlush();
			glDisable(GL_LIGHTING);
			glPopMatrix();
			
			
			glPushMatrix(); // 1 번째맵 세번째 장애물 구현부시작 

			

			glPushMatrix(); // 1 번째맵  첫번째 장애물 삼각형 왼쪽옆면구현 
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_POLYGON); // 1번째맵 첫번째 장애물 삼각형 왼쪽 옆면
			glVertex3f(-1.0, -2.6, -1);
			glVertex3f(-1.0, -2.6, 1);
			glVertex3f(-0.94, -2.15, 1);
			glVertex3f(-0.94, -2.15, -1);
			glEnd();
			glFlush();
			glPopMatrix();
			

			glPushMatrix(); // 1 번째맵  세번째 장애물 삼각형 오른쪽옆면구현 시작
			glColor3f(0.0, 0.2, 0.3);
			glBegin(GL_POLYGON); // 1번째맵 세번째 장애물 삼각형 오른쪽 옆면
			glVertex3f(-0.94, -2.15, -1);
			glVertex3f(-0.94, -2.15, 1);
			glVertex3f(-0.88, -2.6, 1);
			glVertex3f(-0.88, -2.6, -1);
			glEnd();
			glFlush();
			glPopMatrix();  //  1번째맵 세번째 장애물 삼각형 오른쪽 옆면 구현부 끝
			glPopMatrix();

			glPushMatrix(); // 1 번째맵 세번째 장애물 앞면구현부
			glBegin(GL_TRIANGLES);
			glColor3f(randomr, randomg, randomb);
			glVertex3f(-1.0, -2.6, 1);
			glVertex3f(-0.94, -2.15, 1);
			glVertex3f(-0.88, -2.6, 1);
			glEnd();
			glFlush();
			glPopMatrix();
			
			glPopMatrix(); // 1맵 세번째 장애물 구현부 완전 끝 
			

		glPopMatrix(); //1 맵 장애물 구현부  전부 끝 
			


		}
		// 1스테이지 구현부 끝 
		




		//2스테이지 구현부 시작

		else if (clearcount == 1) { // 2스테이지 맵구현부
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW 의 버퍼를 지운다
			
			if (start == 0) { // 1스테이지 시작할때 공의 위치 초기값
				bx = -5.0;
				by = -2.5;
				bz = 0.0;
				start = 100; // 초기값 설정 이후 쓰레기값을 주어서 공의 움직임을 자유롭게 만든다. 

			}

			glPushMatrix();// 2스테이지 맵구현부 시작
				glPushMatrix(); // 2맵 넓은 바닥 구현부
					glColor3f(0.3, 0.4, 0.3);
					glBegin(GL_POLYGON); //2맵 바닥 면 
					glVertex3f(-8.9, -2.5, -1);
					glVertex3f(-8.9, -2.5, 1);
					glVertex3f(8.9, -2.5, 1);
					glVertex3f(8.9, -2.5, -1);
					glEnd();
					glFlush();
				glPopMatrix();
			
				glPushMatrix(); // 2맵 첫째 z축 기준선
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON); 
				glVertex3f(-4.44, -2.5, -1);
				glVertex3f(-4.44, -2.5, 1);
				glVertex3f(-4.24, -2.5, 1);
				glVertex3f(-4.24, -2.5, -1);
				glEnd();
				glFlush();
				glPopMatrix();
				
				glPushMatrix(); // 2맵 둘째 z축 기준선
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON);
				glVertex3f(-3.44, -2.5, -1);
				glVertex3f(-3.44, -2.5, 1);
				glVertex3f(-3.24, -2.5, 1);
				glVertex3f(-3.24, -2.5, -1);
				glEnd();
				glFlush();
				glPopMatrix();


				glPushMatrix(); // 2맵 첫째 z축 기준선
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON);
				glVertex3f(-2.44, -2.5, -1);
				glVertex3f(-2.44, -2.5, 1);
				glVertex3f(-2.24, -2.5, 1);
				glVertex3f(-2.24, -2.5, -1);
				glEnd();
				glFlush();
				glPopMatrix();

				glPushMatrix(); // 2맵 첫째 z축 기준선
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON);
				glVertex3f(-1.44, -2.5, -1);
				glVertex3f(-1.44, -2.5, 1);
				glVertex3f(-1.24, -2.5, 1);
				glVertex3f(-1.24, -2.5, -1);
				glEnd();
				glFlush();
				glPopMatrix();

				glPushMatrix(); // 2맵 중앙 바닥줄구현부
					glColor3f(1.0, 1.0, 0.0);
					glBegin(GL_POLYGON);// 2맵 중앙 바닥줄 구현
					glVertex3f(-8.9, -2.5, 0.05);// 
					glVertex3f(-8.9, -2.5, -0.05);
					glVertex3f(8.9, -2.5, -0.05);
					glVertex3f(8.9, -2.5, 0.05);
					glEnd();
					glFlush();
				glPopMatrix();


				glPushMatrix(); // 2맵 중앙 바닥줄구현부
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON);// 2맵 중앙 바닥줄 구현
				glVertex3f(1.12, -2.5, 0.05);// 
				glVertex3f(1.12, -2.5, -0.05);
				glVertex3f(1.08, -2.5, -0.05);
				glVertex3f(1.08, -2.5, 0.05);
				glEnd();
				glFlush();
				glPopMatrix();

				glPushMatrix(); // 2맵 거미줄 보스 출현구역표시
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_POLYGON);// 2맵 거미줄 보스 출현구역
				glVertex3f(0.1, -2.5, 1);// 
				glVertex3f(0.1, -2.5, -1);
				glVertex3f(0.00, -2.5, -1);
				glVertex3f(0.00, -2.5, 1);
				glEnd();
				glFlush();
				glPopMatrix();
				
			glPopMatrix(); // 2스테이지 맵 구현부 끝




				
			glPushMatrix(); // 2스테이지 장애물 구현부 시작

					

					glPushMatrix();// 2스테이지 첫번째 양옆으로 움직이는 장애물
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
						glTranslatef(stage2otc1x, -2.5, stage2otc1z); // 2스테이지 첫번째 날아다니는 장애물 좌표값 
						glutSolidSphere(stage2radius, 100, 100);
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
					glPopMatrix();
					glPushMatrix();// 2스테이지 두번째 양옆으로 움직이는 장애물
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
						glTranslatef(stage2otc2x, stage2otc2y, 0); // 2스테이지 두번째 날아다니는 장애물 좌표값 
						glutSolidSphere(stage2radius, 100, 100);
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
					glPopMatrix();
					glPushMatrix();// 2스테이지 세번째 양옆으로 움직이는 장애물
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
						glTranslatef(stage2otc3x, -2.5, stage2otc3z); // 2스테이지 세번째 날아다니는 장애물 좌표값 
						glutSolidSphere(stage2radius, 100, 100);
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
					glPopMatrix();
					glPushMatrix();// 2스테이지 네번째 양옆으로 움직이는 장애물
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
						glTranslatef(stage2otc4x, stage2otc4y, 0); // 2스테이지 네번째 날아다니는 장애물 좌표값 
						glutSolidSphere(stage2radius, 100, 100);
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
					glPopMatrix(); // 2스테이지 네번째 양옆으로 움직이는 장애물 끝 


					glPushMatrix(); //2스테이지 피니쉬 포탈 구현부 매트릭스 시작
					randomlightrgb[0] = { randomr };
					randomlightrgb[1] = { randomg };
					randomlightrgb[2] = { randomb };
					randomlightrgb[3] = { 0 };
					glEnable(GL_LIGHTING);
					glEnable(GL_LIGHT0);
					glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);

					glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);

					glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
					glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 80);

					glTranslatef(f1x, f1y, f1z); // 피니쉬 포탈 구체 구현하는 부분
					glutSolidSphere(fradius, 100, 100);

					glDisable(GL_LIGHTING);
					glDisable(GL_LIGHT0);
					glPopMatrix(); // 2스테이지 피니쉬 포탈 구현부 끝
				
				glPopMatrix();  //2스테이지 장애물구현부 끝


					glPushMatrix(); // 2스테이지 보스 구현부분시작 
					
					
					if (bosscount == 1) { // 보스
						if (bossblue == 1) {
							glPushMatrix(); // 파란보스 구현부 시작
							glEnable(GL_LIGHTING);
							glEnable(GL_LIGHT0);
							glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
							glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
							glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
							glTranslatef(bossx, bossy, 0); // 보스좌표.
							glRotated(90, 0, 1, 0); // 보스 회전시켜서 내 시점에서 볼수있도록 한다 .
							glutWireSphere(0.5, 10, 4);
							glDisable(GL_LIGHTING);
							glDisable(GL_LIGHT0);
							if (shot == 1) {
								bluecount = 2;
							}
							else if (shot == 2) {
								bluecount = 1;
							}
							else if (shot == 3) {
								bluecount = 0; // 블루 체력바 다 달기.
								bossblue = 0; // 파란 보스를 내리고
								bossgreen = 1;  // 초록 보스를 등장시킨다
								
							}
							glPopMatrix(); // 파란보스 구현부끝 
						}
						
						else if (bossgreen == 1 && bluecount ==0) { // 보스가 초록색일때
							glPushMatrix(); // 초록보스 구현부 시작
							glEnable(GL_LIGHTING);
							glEnable(GL_LIGHT0);
							glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
							glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
							glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
							glTranslatef(bossx, bossy, 0); // 보스좌표.
							glRotated(90, 0, 1, 0); // 보스 회전시켜서 내 시점에서 볼수있도록 한다 .
							glutWireSphere(0.5, 10, 4);
							glDisable(GL_LIGHTING);
							glDisable(GL_LIGHT0); 
							if (shot == 4) {
								greencount=greencount-1;
							}
							else if (shot == 5) {
								greencount = 0; // 블루 체력바 다 달기.
								bossgreen = 0; // 초록보스를 내리고
								bossred = 1;  // 빨간 보스를 등장시킨다

							}
							glPopMatrix(); // 초록보스 구현부끝 
							
						}

						else if (bossred == 1 && greencount == 0) { // 보스가 빨간색일때
							glPushMatrix(); // 빨간보스 구현부 시작
							glEnable(GL_LIGHTING);
							glEnable(GL_LIGHT0);
							glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
							glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
							glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
							glTranslatef(bossx, bossy, 0); // 보스좌표.
							glRotated(90, 0, 1, 0); // 보스 회전시켜서 내 시점에서 볼수있도록 한다 .
							glutWireSphere(0.5, 10, 4);
							glDisable(GL_LIGHTING);
							glDisable(GL_LIGHT0);
							if (shot == 6) {
								redcount = 1;
							}
							else if (shot == 7) {
								redcount = 0; // 빨간보스 체력바 다 달기.
								bossred = 0; // 빨간보스를 내리고
								bosscount = 0; // 보스 구현부끝내기
								bossno = 1; //보스 한번 나왔다고 저장 

								}
							glPopMatrix(); // 빨간보스 구현부끝 
							}
						
						glPopMatrix(); // 보스 구현부끝 
						
						
						
						glPushMatrix(); // 보스 체력바 구현부
						if (shot <= 0) {
							glPushMatrix(); // 왼쪽 첫번째
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, 0.2); // 오른쪽값 
							glVertex3f(3.0, -1.8, 0.15);//왼쪽값 = 오른쪽값 - 0.05
							glVertex3f(3.0, -1.9, 0.15);
							glVertex3f(3.0, -1.9, 0.2);
							glRotated(90, 0, 1, 0); // 시점에서 보일수 있게회전
							glEnd();
							glFlush();
							glPopMatrix(); // 왼쪽첫번째 체력바
						}
						if (shot<= 1) {
							glPushMatrix(); //왼쪽 두번째 체력바 시작
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, 0.14); // 오른쪽값 
							glVertex3f(3.0, -1.8, 0.09);//왼쪽값 = 오른쪽값 - 0.05
							glVertex3f(3.0, -1.9, 0.09);
							glVertex3f(3.0, -1.9, 0.14);
							glRotated(90, 0, 1, 0); // 시점에서 보일수 있게회전
							glEnd();
							glFlush();
							glPopMatrix(); // 왼쪽두번째 체력바 끝
						}
						if (shot <= 2) {
							glPushMatrix(); // 왼쪽 세번째 체력바 시작
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, 0.08); // 오른쪽값 
							glVertex3f(3.0, -1.8, 0.03);//왼쪽값 = 오른쪽값 - 0.05
							glVertex3f(3.0, -1.9, 0.03);
							glVertex3f(3.0, -1.9, 0.08);
							glRotated(90, 0, 1, 0); // 시점에서 보일수 있게회전
							glEnd();
							glFlush();
							glPopMatrix(); // 왼쪽 세번째 체력바 끝
						}
						if (shot <= 3) {
							glPushMatrix(); // 왼쪽 네번째 체력바 시작
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, 0.02); // 오른쪽값 
							glVertex3f(3.0, -1.8, -0.03);//왼쪽값 = 오른쪽값 - 0.05
							glVertex3f(3.0, -1.9, -0.03);
							glVertex3f(3.0, -1.9, 0.02);
							glRotated(90, 0, 1, 0); // 시점에서 보일수 있게회전
							glEnd();
							glFlush();
							glPopMatrix(); // 왼쪽 네번째 체력바 끝
						}
						if (shot <= 4) {
							glPushMatrix(); // 왼쪽 다섯번째 체력바 시작
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, -0.04); // 오른쪽값 
							glVertex3f(3.0, -1.8, -0.09);//왼쪽값 = 오른쪽값 - 0.05
							glVertex3f(3.0, -1.9, -0.09);
							glVertex3f(3.0, -1.9, -0.04);
							glRotated(90, 0, 1, 0); // 시점에서 보일수 있게회전
							glEnd();
							glFlush();
							glPopMatrix(); // 왼쪽 다섯번째 체력바 끝
						}
						if (shot<= 5) {
							glPushMatrix(); // 왼쪽 여섯번째 체력바 시작
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, -0.1); // 오른쪽값 
							glVertex3f(3.0, -1.8, -0.15);//왼쪽값 = 오른쪽값 - 0.05
							glVertex3f(3.0, -1.9, -0.15);
							glVertex3f(3.0, -1.9, -0.1);
							glRotated(90, 0, 1, 0); // 시점에서 보일수 있게회전
							glEnd();
							glFlush();
							glPopMatrix(); // 왼쪽 여섯번째 체력바 끝
							
						}
						if (shot <=6) {

							glPushMatrix(); // 왼쪽 일곱번째 체력바 시작
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, -0.16); // 오른쪽값 
							glVertex3f(3.0, -1.8, -0.21);//왼쪽값 = 오른쪽값 - 0.05
							glVertex3f(3.0, -1.9, -0.21);
							glVertex3f(3.0, -1.9, -0.16);
							glRotated(90, 0, 1, 0); // 시점에서 보일수 있게회전
							glEnd();
							glFlush();
							glPopMatrix(); // 왼쪽 일곱번째 체력바 끝
							totalhelth = 0;
						}
						if (totalhelth == 0 && shot == 7) {
							PlaySound(TEXT(SOUND_FILE_NAME_BOSSKILLED), 0, SND_ASYNC);

							bosscount = 0;
							bossno = 1;
							if (bx >= 0.0 && bx <= 0.3) {
								bx = 0.4;
							}

						}
						
						glPopMatrix(); // 보스 체력바 구현부 끝 


						
					}
			


			else if (clearcount ==2) {
				glClearColor(0.0, 0.0, 0.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW 의 버퍼를 지운다

				if (start == 0) { // 1스테이지 시작할때 공의 위치 초기값
					bx = -5.0;
					by = -2.5;
					bz = 0.0;
					start = 100;
				}// 초기값 설정 이후 쓰레기값을 주어서 공의 움직임을 자유롭게 만든다. 

			}



				
			


			if (ballmove == 1) { // 공이 움직일수있는 상태와 못움직이는 상태를 구분짓기위해if 문으로 따로 나눔 
				glPushMatrix(); // 공구현부 매트릭스 1맵
				
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);

				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
				glTranslatef(bx, by, bz); // 공을 구현하는 부분
				glutSolidSphere(radius, 100, 100);
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				
				glPopMatrix();
			}
			glPopMatrix();

		}

		//2t스테이지 구현부 끝

		//3스테이지 구현부 시작

		else if (clearcount == 2&&timerstart==1) {
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW 의 버퍼를 지운다
			char str[100];



			cout << "게임이 끝나 랭킹을 기록합니다." << endl;


			printf("이름을 입력하세요 :  \n");
			gets_s(str, 100);

			FILE *fp;

			fp = fopen("../save.txt", "wt");
			if (fp == NULL)
			{
				printf("\n파일 불러오기실패\n");
			}

			fprintf(fp, "%s", str);
			fprintf(fp, " 죽은횟수: %d", death);
			fprintf(fp, " 진행시간 : %3.d \n", (finish - go) / CLK_TCK);
			




			fclose(fp);

			printf("\n완료\n");
			clearcount = 4;
			
		}



		if (obstacle == 1) {// 공이 장애물에 걸린 상황일때 
			death = death + 1; //죽은횟수 1 증가
			cout << "죽은횟수 : " << death << endl;
			start = 100;
			obstacle = 0;

		}



		//cout << bx << endl;
		//cout << by << endl;	




		glutSwapBuffers(); // DOUBLE 을 사용하였으니 SWAPBUFFERS 사용.
		glutPostRedisplay();

	}
	}

	
	



void reshape(int x, int y) // 윈도우 창의 크기가 변경될때 모든 물체들의 종횡비를 그대로 유지시키려고 지정.
{
	
		if (y == 0 || x == 0) return;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(39.0, (GLdouble)x / (GLdouble)y, 1, 21.0); // 시야각 39,종횡비=윈도우 사이즈가로/세로, 앞부분절단면,뒷부분절단면
		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, x, y);  //Use the whole window for rendering
	

}

void myspkeyboard(int pressedkey, int x, int y) {
	if (pressedkey == GLUT_KEY_LEFT) {
		if (lxg != 0) { //lxg 값이 0이 아닐때 = 왼쪽으로이동하는 가속도가 존재할때 , lxg의 초기값은 0.04,
			if (bosscount == 0) {
				bx = bx - 0.04;
			}
			bx = bx;
		}
		else if (lxg <0.0002) { // lxg가 0.002보다 커졌을때 = 가속도가 점점 사라져야한다
			if (bosscount == 0 ) {
				bx = bx - 0.04;
			}
		}



	}

	if (pressedkey == GLUT_KEY_RIGHT) {
		if (rxg != 0) { //lxg 값이 0이 아닐때 = 왼쪽으로이동하는 가속도가 존재할때 , lxg의 초기값은 0.04,
			if (bosscount == 0) {
				bx = bx + 0.04;
			}
			bx = bx;
		}

	}
	
	

	if (pressedkey == GLUT_KEY_HOME) {
		if (view == 1) {
			view = 2;
		}
		else {
			view = 1;
		}
	}
	if (pressedkey == GLUT_KEY_F1) {
		cout << "x : " << bx << endl;
		cout << "y : " << by << endl;
		cout << "z : " << bz << endl;
	}

	if (pressedkey == GLUT_KEY_UP) {
		if (sxg != 0) { //sxg 값이 0이 아닐때 = 왼쪽으로이동하는 가속도가 존재할때 , lxg의 초기값은 0.04,

			bz = bz - 0.04;
		}
	}

	if (pressedkey == GLUT_KEY_DOWN) {
		if (sxg != 0) { //sxg 값이 0이 아닐때 = 왼쪽으로이동하는 가속도가 존재할때 , lxg의 초기값은 0.04,
			bz = bz + 0.04;
		}

	}

	if (pressedkey == GLUT_KEY_PAGE_UP) {
		if (view == 1&&2) {
			view = 3;
		}
		else {
			view = 1;
		}
	}


	glutPostRedisplay();
}

void MyKeyboard(unsigned char KeyPressed, int x, int y)
{
	
		switch (KeyPressed) {
		
		case 32: //스페이스바
			gamestart = 1;
			
			jump = 0.1;
			if (jump > 0.1) {
				jump = 0;
			}
			break;
		case 'a': //a 눌렀을때
			if (shot <= 2) {
			shot= shot + 1;
			PlaySound(TEXT(SOUND_FILE_NAME_BOSSKILL), 0, SND_ASYNC);
			}
			
			
			break;
		case 's': //s 눌렀을때
			if (shot <=4 && shot>=3) {
				shot = shot + 1;
				PlaySound(TEXT(SOUND_FILE_NAME_BOSSKILL), 0, SND_ASYNC);
			}
			break;
		case 'd': //d 눌렀을때
			if (shot<=6 && shot>=5) { // 빨간
				shot = shot+1;
				PlaySound(TEXT(SOUND_FILE_NAME_BOSSKILL), 0, SND_ASYNC);
			}
			
			break;

		case 'f': //관리자 보스구현 하기위해 만든메뉴
			
			bosscount = 1;
			view = 5;
			break;


		case 'q': //q 눌렀을때
			clearcount = 1;
			break;
		default:
			break;
		}
	glutPostRedisplay(); //화면다시그리기
}


void mytimer(int value) {
	randomr = (float(rand()%10)/10); // 0.0~1.0까지 랜덤으로 생성되는 rgb값 timer 이용해 계속 값을 바꾼다 .
	randomg = (float(rand() % 10) / 10);
	randomb = (float(rand() % 10) / 10);

	

	if (jump > 0) {
		jump = jump - 0.01;
	}
	
	

	switch (upordown) {
	case 0: // 공이 떨어질때 (down 일때)
		if (bosscount == 0&&clearcount!=2) {
			if (by > sky) { // 최고점프 높이보다 높을때 공을 최고높이 값으로 계속 고정
				by = sky;
			}
			if (by > -2.5000) { // 최저 높이보다 공의 위치가 높을때 내려간다
				by = by - 0.03 - g;
				g = g + 0.0001;
			}
			if (by <= -2.5000 && clearcount!=2) { //최저높이보다 공의위치가 낮거나 같아졌을때 공의 운동성을 점프하도록 바꾼다 
				by = by + 0.03;
				g = 0.0001;
				upordown = 1;
				PlaySound(TEXT(SOUND_FILE_NAME_JUMP), 0, SND_ASYNC);

			}
		}
		break;

	case 1:// 공이 점프할때 ( up일때)
		
		
		if (bosscount == 0&&clearcount!=2) {
			if (by < sky) { // 최고점프 높이 보다 낮을때 올라가도록 한다.
				by = by + 0.03 - g;
				g = g - 0.0001;
			}
			if (by == sky) { // 최고점프 높이와 같아졌을때 공의 운동방향을 떨어지도록바꾼다

				by = by - 0.03;
				upordown = 0;
				g = 0.0001;

			}
			if (by > sky) { // 최고 점프보다 크게 뛰었을대 더 높이 올라가지 못하도록 한다.
				by = sky;
			}
		}
		break;

		}
	
	if (bx < -5.84) { // 공이 화면밖으로 나가지않게 막아주는 함수
		bx =-5.84;
	}
	if (bx > 5.84) { // 공이 화면밖으로 나가지않게 막아주는함수 
		bx =5.84;
	}

	
	stage2otc1z = stage2otc1z +stage2otc1zstep;
	stage2otc2y = stage2otc2y + stage2otc2ystep;
	stage2otc3z = stage2otc3z + stage2otc3zstep;
	stage2otc4y = stage2otc4y + stage2otc4ystep;

	if (stage2otc1z<-1.0 || stage2otc1z>1) {
		stage2otc1zstep = -stage2otc1zstep;
	}
	if (stage2otc2y<-2.5 || stage2otc2y>1.0) {
		stage2otc2ystep = -stage2otc2ystep;
	}
	if (stage2otc3z<-1.0 || stage2otc3z>1) {
		stage2otc3zstep = -stage2otc3zstep;
	}
	if (stage2otc4y<-2.5 || stage2otc4y>0.6) {
		stage2otc4ystep = -stage2otc4ystep;
	}






	//장애물,피니쉬라인 인식 구현부 

	if(clearcount == 0 && bx<=-3.3 && bx >=-3.4 && by<=-2.3 && by>=-2.4) //  첫번째 스테이지공이 첫번째 장애물의 좌표내에 위치할때 
	{
		obstacle = 1; // 장애물에 부딪혔다고 인식할수있게 변수값에 1을 대입 (장애물에 부딪혀야 공의 위치가 초기화됨)
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
	}

	if (clearcount== 0 && bx >= -2.12 && bx <= -2.02 && by >= -2.2 && by <= -1.7) { //  첫번째 스테이지 에서 공이 두번째 장애물의 좌표내에 위치할때
		obstacle = 1; // 장애물에 부딪혔다고 인식할수있게 변수값에 1을 대입 
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
	}

	if (clearcount == 0 && bx >= -1.0 && bx <= -0.88 && by <= -2.15 && by >= -2.6) { // 첫번째 스테이지 공이 세번째 장애물의 좌표내에 위치할때
		obstacle = 1; // 장애물에 부딪혔다고 인식할수있게 변수값에 1을 대입 
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
	}

	if (clearcount == 1 && bz >= stage2otc1z - 0.1 && bz <= stage2otc1z + 0.1 && bx+0.05>=-4.44 && bx-0.05<=-4.24 && by>=-2.5-0.1 && by<=2.5+0.1) { // stage2otc1 에 걸렸을때 
		obstacle = 1;
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
		
	}

	if (clearcount == 1 && by >= stage2otc2y - 0.1 && by <= stage2otc2y + 0.1 && bx >= stage2otc2x-0.1 && bx <= stage2otc2x+0.1 && bz>=-0.1 && bz<=0.1) { // stage2otc2 에 걸렸을때 
		obstacle = 1;
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
		
	}
	
	if (clearcount == 1 && bz >= stage2otc3z - 0.1 && bz <= stage2otc3z + 0.1 && bx+0.1 >= stage2otc3x-0.1 && bx-0.1 <= stage2otc3x+0.1&& by <= -2.4 && by >=-2.6) { // stage2otc1 에 걸렸을때 
		obstacle = 1;
		start = 0; PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
		
	}


	if (clearcount == 1 && by >= stage2otc4y - 0.1 && by <= stage2otc4y + 0.1 && bx+0.1 >= stage2otc4x - 0.1 && bx-0.1 <= stage2otc4x + 0.1 && bz >= -0.1 && bz <= 0.1) { // stage2otc2 에 걸렸을때 
		obstacle = 1;
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
		
	}
	if (clearcount == 1 && bosscount == 0 && bx >= 0.0 && bx <= 0.3 && bossno==0) { //보스모드 발동부 (0.0 ~0.3 사이에 공이 들어왔을때.)
		bosscount = 1;
		by = -2.5;
		PlaySound(TEXT(SOUND_FILE_NAME_BOSSSTART), 0, SND_ASYNC);
		
		view = 5;
		
	}
	//glTranslatef(stage2otc1x, -2.5, stage2otc1z); // 2스테이지 첫번째 날아다니는 장애물 좌표값 
	glutTimerFunc(10, mytimer, 3000);

}

int main(int argc, char **argv)
       {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(600, 350);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("공튀기기");
	if (clearcount == 0&&timerstart==0) {
		go = clock();
		timerstart = 1;
	}
	
	initLighting();
	srand((unsigned)time(NULL));

	cout << " >>>>>>>>>>>>>>> 공튀기기 게임에 오신걸을 환영합니다 <<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>공이 반대쪽 큰 공 안에 들어가면 스테이지 클리어입니다<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>>>>>  이동 : 키보드 화살표 키  <<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>>>>>    시작 : 스페이스 바     <<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>> page up = 카메라가 공을 계속 보는 시점<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>  home   = 공의 시점으로바라보기  <<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>home 키를 다시누르면 원래의시점으로 변경됩니다<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>장애물에 누가더 적게 걸리는지 경쟁해보세요<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>> 장애물에 걸릴시 죽은수가 나타납니다<<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>> 거미줄 보스한테 잡힐시 거미줄과같은 색상으로<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>       공격을 해야합니다.                  <<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>  a:파란색 s:초록색 d:빨간색               <<<<<<<<<<<<" << endl;
	cout << " >>>>>>>ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ<<<<<<<<<<<<" << endl;



	

	glutDisplayFunc(display);
	



	glutReshapeFunc(reshape);
	glutTimerFunc(10, mytimer, 3000);
	glutSpecialFunc(myspkeyboard);
	glutKeyboardFunc(MyKeyboard);
	
	if (clearcount == 0) {
		PlaySound(TEXT(SOUND_FILE_NAME_BGM), 0, SND_ASYNC | SND_LOOP);

	}
	

	glutMainLoop();
	

	return 0;
}