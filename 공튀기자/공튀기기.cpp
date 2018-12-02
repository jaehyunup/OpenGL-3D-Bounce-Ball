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


#define GL_BGRA 0x80E1 // 32��Ʈ ����� �ϱ�����.
#define GL_BGR_EXT 0x80E0
#define MAX_NO_TEXTURES 1 // �ؽ��� �������ظ��� ��ó��
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

clock_t go,finish; //��������ð��� �����ϱ����� ���� ����


GLuint texture[MAX_NO_TEXTURES];
GLint clearcount = 0;
GLfloat randomr; // 0.0~1.0���� �������� �����Ǵ� r��
GLfloat randomg; // 0.0~1.0���� �������� �����Ǵ� g��
GLfloat randomb; // 0.0~1.0���� �������� �����Ǵ� b��
GLdouble fradius = 0.1; // �ǴϽ� ���� ��ü�� ������
GLfloat bx, by, bz; // ���� x,y,z �̵��� �� �����ϱ����� ���� = ���� ���� 0,0���� �����Ȱ����� translatef �� ���� �̵���Ų ����.
GLfloat f1x, f1y, f1z; // finish �Ҽ��ִ� ��ü�� �׷��� ���� ����� ����
GLdouble radius = 0.05; // ������ ���� �Լ� 
int start; // �ʱ���� ��ġ ��� ���� �߰��� ����
GLfloat sky; // ���� �ְ�� �ۼ��ִ� ��ġ�� �����ϴ� ����
int upordown = 1; // ���� ���� �ϴ���, ���������ִ��� ���⼺�� �����ϱ����� ���� ����. 1�϶��� up , 0�϶��� down
GLint shot = 0;
GLfloat g = 0.003; // ���� �ڿ������� ƨ��� ���� �߷��� �߰�.
GLfloat rxg = 0.04; // ���� �ڿ������� ������ �����̱����� �߰��� ���� (������)
GLfloat lxg = 0.04; // ���� �ڿ������� �ڷ� �����̱����� �߰��� ���� (����)
GLfloat sxg = 0.04; // ���� �ڿ������� ������ �����̱����� �߰��� ����(�翷)
GLfloat jump = 0; // �����̽��ٸ� ������ ���� ���� �����ϴ� ����
GLfloat randomlightrgb[] = { 0.0,0.0,0.0,0.0 };
GLdouble stage2radius = 0.1;
GLint obstacle = 0; // ���� ��ֹ��� �ε������� �Ⱥε������� �˼��ֵ��� ������ ����
GLfloat stage2otc1zstep=0.06; // 2�������� ù��° ��ֹ��� z ���� �ڵ��̵��� ���ؼ� 0.01�̶�� ������ �ο� (���� �ӵ�)
GLfloat stage2otc2ystep = 0.06;// 2�������� �ι�° ��ֹ���y ���� �ڵ��̵��� ���ؼ� 0.02�̶�� ������ �ο�
GLfloat stage2otc3zstep = 0.08;// 2�������� ����° ��ֹ���z ���� �ڵ��̵��� ���ؼ� 0.03�̶�� ������ �ο�
GLfloat stage2otc4ystep = 0.1;// 2�������� �׹�° ��ֹ��� y ���� �ڵ��̵��� ���ؼ� 0.01�̶�� ������ �ο�

GLfloat stage2otc1x=-4.44;
GLfloat stage2otc1y=-2.5;    // 2�������� �翷���ο����̴� ��ֹ� 1�� x,y,z �� 
GLfloat stage2otc2y = -2.5;    // 2�������� �翷���ο����̴� ��ֹ� 1�� x,y,z �� 
GLfloat stage2otc1z = 0;
GLfloat stage2otc3z = 0;
GLfloat stage2otc4y = -2.5;    // 2�������� �翷���ο����̴� ��ֹ� 1�� x,y,z �� 
GLfloat stage2otc2x = -3.44; // 2�������� �翷���ο����̴� ��ֹ� 2�� x��
GLfloat stage2otc3x = -2.44; // 2�������� �翷���ο����̴� ��ֹ� 3�� x��
GLfloat stage2otc4x = -1.44; // 2�������� �翷���ο����̴� ��ֹ� 4�� x��
GLfloat bossx=3.0;
GLfloat bossy = -2.45;
GLfloat bossz=0.0;
int jumonoff = 0; // �������� �ƴ��� üũ�ϴ� ����
					// ��ü�� ���� �����ϱ����Ͽ� ���� 4���� ������ ����.
GLfloat qaRed[] = { 1.0, 0.0, 0.0, 0.0 }; // ������
GLfloat qaBlack[] = { 0.0, 0.0, 0.0, 0.0 }; // ������ 
GLfloat qaGreen[] = { 0.0, 1.0, 0.0, 0.0 }; // �ʷϻ� 
GLfloat qaWhite[] = { 1.0, 1.0, 1.0, 0.0 }; // �Ͼ��
int timerstart = 0; // ���� ����ð� Ÿ�̸Ӱ� �������� Ȯ���ϴº���

GLfloat red[] = { 1.0, 0.0 , 0.0 , 0.0 };
GLfloat green[] = { 0.0, 1.0, 0.0, 0.0 };
GLfloat blue[] = { 0.0, 0.0, 1.0, 0.0 };

float spotlightDirection[] = { -1.0f, 0.0f, -1.0f };

GLint ballmove = 1; // display ���� ���������� ���� ���� �з��ϱ�����  �������ǹ��� �� ����

GLfloat qaAmbientLight[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat qaDiffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat qaSpecularLight[] = { 1.0, 1.0, 1.0, 1.0 }; // ������ Ư�� ���� .
GLfloat emitLight[] = { 0.9, 0.9, 0.9, 0.01 };

GLfloat Noemit[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat qaLightPosition[] = { -50.0, 10.0 ,4.0, 0 }; // ������ ��ġ.

int bosscount = 0;//���� ���忩�� .
int bossblue = 1; //������ �Ķ����϶� 1
int bossred = 0; //������ �������϶�
int bossgreen = 0; // ������ �ʷϻ��ϋ� 
int bluecount = 3; // �Ķ����� ü�¹� ��
int greencount = 2; // �ʷϺ��� ü�¹ٷ�
int redcount = 2; // �������� ü�¹�  ��
int bossno = 0;
int totalhelth = bluecount + greencount + redcount;





	







void myidle() {
	glShadeModel(GL_SMOOTH); // �ε巯�� �������� ���� �׸��� .
	glEnable(GL_DEPTH_TEST); // ��ü �������� �׸��� , ���̸��� ������ �ٸ��� ��Ȯ�ϰ� ��������� ����
	glDisable(GL_CULL_FACE); // �޸��� ����� 
	glFrontFace(GL_CCW); //�ݽð�������� �ո��� �ڹٲ۴�
	


}

void initLighting()
{

	// ���� Ȱ��ȭ 
	

	// ���� Ư���� ������ ������ �Լ��� �̿��Ͽ� ���� �����Ų��.
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);

	// ������ ��ġ �����ϴ� �Լ��� ������ ��ġ�� ������ ������ �Ű������� �Ͽ� ���� ��ġ ����.
	glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);

}





void display(void)
{
	f1x = 5.6399; // �������� 1�ǴϽ����� �ʱ��Լ� 
	f1y = -2.41;
	if (gamestart == 1) {


		

		sky = -1.8000;  // sky = ���� �ٴ� ���̸� �����ϱ����� ���� ���� 
		
		glMatrixMode(GL_MODELVIEW);


		glLoadIdentity();
		if (view == 1) { // ���� �߾����� �̵��ϴ� cctv ����
			gluLookAt(0.0, 0.0, 10.0, // ī�޶��� ��ġ
				bx, 0.0, bz, // ī�޶� ���� ����
				0.0, 1.0, 0.0); // ī�޶��� ����

		}
		if (view == 3) { // ������
			gluLookAt(bx, by, 10.0, // ī�޶� ��ġ
				bx, 0.0, bz, // ī�޶� ���� ����
				0.0, 1.0, 0.0); // ī�޶��� ����
		}
		if (view == 2) { // ���� ���� 
			gluLookAt(bx - 2, by, bz, // ī�޶��� ��ġ
				f1x, f1y, f1z, // ī�޶� ���� ����
				0.0, 1.0, 0.0); // ī�޶��� ����

		}
		if (view == 5) { // ����������
			gluLookAt(bx, by, 0.0, // ī�޶��� ��ġ
				bossx,bossy,bossz, // ī�޶� ���� ����
				0.0, 1.0, 0.0); // ī�޶��� ����
			if (bosscount == 0) {
				view = 1;
			}
		}






		//��ü�� Ư�� ����



		if (start == 0) { // 1�������� �����Ҷ� ���� ��ġ �ʱⰪ

			bx = -5.0;
			by = -2.5;
			bz = 0.0;
			start = 100; // �ʱⰪ ���� ���� �����Ⱚ�� �־ ���� �������� �����Ӱ� �����. 

		}




		// clearcount ������ ���� ���� ���������� �޶������ֵ����Ҽ��ְ��ϴ� ���ǹ�. = finish ���ο� ������
		if (f1x - 0.11 <= bx && f1y >= by &&f1x + 0.11 >= bx&&clearcount == 0 )  { // �ǴϽ����ο� ���� �ְ� �������� 1�϶�
			clearcount = 1; //�������� 2�� ����
			if (clearcount == 1) { // �������� 2�϶� 
				
				start = 0; // ���� ��ġ �ʱ�ȭ
				
				
				glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW �� ���۸� ����� (���ݱ��� �׸��� ����)
				glutSwapBuffers(); // ���۸� ���� 
				glutPostRedisplay(); // �ٽñ׸���
				

			}
		}

		else if (f1x - 0.11 <= bx && f1y >= by &&f1x + 0.11 >= bx&&clearcount == 1) { // �ǴϽ����ο� �����ϰ� �������� 2�϶�
			clearcount =2; //�������� 3���� ����
			if (clearcount == 2) {
				
				start = 0; // ���� ��ġ �ʱ�ȭ
				
				glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW �� ���۸� ����� (���ݱ��� �׸��� ����)
				glutSwapBuffers(); // ���۸� ���� 
				glutPostRedisplay(); // �ٽñ׸���

			}
		}

		else if (f1x - 0.11 <= bx && f1y >= by &&f1x + 0.11 >= bx&&clearcount == 2) { // �������� 2�϶� 3���� �Ѿ���� Ȯ���ϴ� ���ǹ�
			
				start = 0; // ���� ��ġ �ʱ�ȭ
				clearcount = 1; // �������� ������ 1�� �ٲ��
				PlaySound(TEXT(SOUND_FILE_NAME_JUMP), 0, SND_PURGE | SND_LOOP);
				glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW �� ���۸� ����� (���ݱ��� �׸��� ����)
				glutSwapBuffers(); // ���۸� ���� 
				glutPostRedisplay(); // �ٽñ׸���

			
		}
	

		if (clearcount==1 && -4.24>=bx && -4.44<=bx && stage2otc1z-0.1<=bz && stage2otc1z+0.1>=bz){
			
			start = 0; 
			PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
			}
	// �������� 2�϶� ù��° ��ü �¾Ҵ��� �ȸ¾Ҵ��� Ȯ��


		// 1�������� ������ ���� 
		if (clearcount == 0) { // 1�������� �� ��쿡  ���� �̷��� �׷�����.
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW �� ���۸� �����
			

			glPushMatrix(); // 1�� ���� �ٴ� ������
			

			glColor3f(0.3, 0.4, 0.3);
			glBegin(GL_POLYGON); //1�� �ٴ� �� 
			glVertex3f(-8.9, -2.5, -2);
			glVertex3f(-8.9, -2.5, 2);
			glVertex3f(8.9, -2.5, 2);
			glVertex3f(8.9, -2.5, -2);
			
			glEnd();
			glFlush();
			glPopMatrix();

			glPushMatrix(); // 1�� �߾� �ٴ��ٱ�����
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON);// 1�� �߾� ������ ����
			glVertex3f(-8.9, -2.5, 0.05);// 
			glVertex3f(-8.9, -2.5, -0.05);
			glVertex3f(8.9, -2.5, -0.05);
			glVertex3f(8.9, -2.5, 0.05);
			glEnd();
			glFlush();
			glPopMatrix();

			glPushMatrix(); // 1�� ������ �ٴ���
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON);// 1�� ������ �ٴ��� ������
			glVertex3f(-8.9, -2.5, 0.7);// 
			glVertex3f(-8.9, -2.5, 0.6);
			glVertex3f(8.9, -2.5, 0.6);
			glVertex3f(8.9, -2.5, 0.7);
			glEnd();
			glFlush();
			glPopMatrix();

			glPushMatrix(); // 1�� ���� �ٴ���
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON);// 1�� ���� �ٴ��� ������
			glVertex3f(-8.9, -2.5, -0.7);// 
			glVertex3f(-8.9, -2.5, -0.6);
			glVertex3f(8.9, -2.5, -0.6);
			glVertex3f(8.9, -2.5, -0.7);
			glEnd();
			glFlush();
			glPopMatrix();





			glPushMatrix();// 1��°�� ù��° ��ֹ� �����ν��� 
			
			glPushMatrix(); //1�� �ǴϽ� ��Ż ������ ��Ʈ���� ����
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

			glTranslatef(f1x, f1y, f1z); // �ǴϽ� ��Ż ��ü �����ϴ� �κ�
			glutSolidSphere(fradius, 100, 100);

			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			glPopMatrix();
			glPushMatrix(); // 1��°�� ù��° ��ֹ� �ﰢ�� ���ʿ���,�ո� ����(���� ��ġ�� ��Ȯ�ϰ� �������� ����)



			


			glPushMatrix();
			glPushMatrix(); // 1 ��°��  ù��° ��ֹ� �ﰢ�� ���ʿ��鱸�� 
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON); // 1��°�� ù��° ��ֹ� �ﰢ�� ���� ����
			glVertex3f(-3.35, -2.3, -1);
			glVertex3f(-3.35, -2.3, 1);
			glVertex3f(-3.4, -2.6, 1);
			glVertex3f(-3.4, -2.6, -1);
			glEnd();
			glFlush();
			glPopMatrix();
						 

			glPushMatrix(); // 1 ��°��  ù��° ��ֹ� �ﰢ�� �����ʿ��鱸�� ����
			glColor3f(1.1, 0.0, 0.0);
			glBegin(GL_POLYGON); // 1��°�� ù��° ��ֹ� �ﰢ�� ������ ����
			glVertex3f(-3.35, -2.3, -1);
			glVertex3f(-3.35, -2.3, 1);
			glVertex3f(-3.3, -2.6, 1);
			glVertex3f(-3.3, -2.6, -1);
			glEnd();
			glFlush();
			glPopMatrix();  //  ù��°�� ù��° �ﰢ�� ������ ���� ������ ��
			glPopMatrix();

			glPushMatrix();
			glColor3f(randomr, randomg, randomb);
			glBegin(GL_TRIANGLES); // 1��°�� ù��° ��ֹ� �ﰢ�� �ո�
			glVertex3f(-3.4, -2.6, 1);
			glVertex3f(-3.35, -2.3, 1);
			glVertex3f(-3.3, -2.6, 1);
			glEnd();
			glFlush();
			glPopMatrix();

			glPopMatrix(); // 1��°�� ù��° ��ֹ� ������ ��

			glPushMatrix(); // 1��°�� �ι�° ��ֹ� ���� ����
			

			glPushMatrix(); // 1 ��°��  �ι�° ��ֹ� �ﰢ�� ���ʿ��鱸�� 
			glColor3f(1.0, 1.0, 0.0);
			glBegin(GL_POLYGON); // 1��°�� �ι�° ��ֹ� �ﰢ�� ���� ����
			glVertex3f(-2.07, -2.2, -1);
			glVertex3f(-2.07, -2.2, 1);
			glVertex3f(-2.12, -1.7, 1);
			glVertex3f(-2.12, -1.7, -1);
			glEnd();
			glFlush();
			glPopMatrix();
			
			if (ballmove == 1) { // ���� �����ϼ��ִ� ���¿� �������̴� ���¸� ������������if ������ ���� ���� 
				glPushMatrix(); // �������� ��Ʈ���� 1��
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);

				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
				glTranslatef(bx, by, bz); // ���� �����ϴ� �κ�
				glutSolidSphere(radius, 100, 100);
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				glPopMatrix();
			}

			glPushMatrix(); // 1��°�� �ι�° ��ֹ��ﰢ�� �����ʿ��� ���� ( ���� ��ġ�� ��Ȯ�ϰ� �������� ����)
			glColor3f(1.0, 1.0, 0.3);
			glBegin(GL_POLYGON); // 1��°�� �ι�° ��ֹ� ù��° ��ֹ� �ﰢ�� ������ ����
			glVertex3f(-2.07, -2.2, -1);
			glVertex3f(-2.07, -2.2, 1);
			glVertex3f(-2.02, -1.7, 1);
			glVertex3f(-2.02, -1.7, -1);
			glEnd();
			glFlush();
			glPopMatrix();  //  1��°�� �ι�° ��ֹ� �ﰢ�� ������ ���� ������ ��

			glPushMatrix(); // 1��°�� �ι�° ��ֹ� �ﰢ�� õ���� �� ���� 
			glColor3f(0.0, 0.2, 0.3);
			glBegin(GL_POLYGON); // 1��°�� �ι�° ��ֹ� �ﰢ�� õ�� �� 
			glVertex3f(-2.02, -1.7, -1);
			glVertex3f(-2.02, -1.7, 1);
			glVertex3f(-2.12, -1.7, 1);
			glVertex3f(-2.12, -1.7, -1);
			glEnd();
			glFlush();
			glPopMatrix();  //  1��°�� �ι�° ��ֹ� �ﰢ��õ�� �� ������ ��

			glPushMatrix();
			glBegin(GL_TRIANGLES); // 1 ��°�� �ι�° ��ֹ� �ո�
			glColor3f(randomb, randomr, randomg);
			glVertex3f(-2.12, -1.7, 1);
			glVertex3f(-2.07, -2.2, 1);
			glVertex3f(-2.02, -1.7, 1);
			glEnd();
			glFlush();
			glPopMatrix();
			/*
			glPushMatrix(); // 1�� õ�� ������
			glEnable(GL_LIGHTING);
			glColor3f(0.0, 0.3, 0.1);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
			*/
			glBegin(GL_POLYGON); //1�� õ�� �簢��
			glVertex3f(-8.9, -1.6, 0);
			glVertex3f(-8.9, -1.7, 0);
			glVertex3f(8.9, -1.7, 0);
			glVertex3f(8.9, -1.6, 0);
			glEnd();
			glFlush();
			glDisable(GL_LIGHTING);
			glPopMatrix();
			
			
			glPushMatrix(); // 1 ��°�� ����° ��ֹ� �����ν��� 

			

			glPushMatrix(); // 1 ��°��  ù��° ��ֹ� �ﰢ�� ���ʿ��鱸�� 
			glColor3f(0.0, 0.0, 0.0);
			glBegin(GL_POLYGON); // 1��°�� ù��° ��ֹ� �ﰢ�� ���� ����
			glVertex3f(-1.0, -2.6, -1);
			glVertex3f(-1.0, -2.6, 1);
			glVertex3f(-0.94, -2.15, 1);
			glVertex3f(-0.94, -2.15, -1);
			glEnd();
			glFlush();
			glPopMatrix();
			

			glPushMatrix(); // 1 ��°��  ����° ��ֹ� �ﰢ�� �����ʿ��鱸�� ����
			glColor3f(0.0, 0.2, 0.3);
			glBegin(GL_POLYGON); // 1��°�� ����° ��ֹ� �ﰢ�� ������ ����
			glVertex3f(-0.94, -2.15, -1);
			glVertex3f(-0.94, -2.15, 1);
			glVertex3f(-0.88, -2.6, 1);
			glVertex3f(-0.88, -2.6, -1);
			glEnd();
			glFlush();
			glPopMatrix();  //  1��°�� ����° ��ֹ� �ﰢ�� ������ ���� ������ ��
			glPopMatrix();

			glPushMatrix(); // 1 ��°�� ����° ��ֹ� �ո鱸����
			glBegin(GL_TRIANGLES);
			glColor3f(randomr, randomg, randomb);
			glVertex3f(-1.0, -2.6, 1);
			glVertex3f(-0.94, -2.15, 1);
			glVertex3f(-0.88, -2.6, 1);
			glEnd();
			glFlush();
			glPopMatrix();
			
			glPopMatrix(); // 1�� ����° ��ֹ� ������ ���� �� 
			

		glPopMatrix(); //1 �� ��ֹ� ������  ���� �� 
			


		}
		// 1�������� ������ �� 
		




		//2�������� ������ ����

		else if (clearcount == 1) { // 2�������� �ʱ�����
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW �� ���۸� �����
			
			if (start == 0) { // 1�������� �����Ҷ� ���� ��ġ �ʱⰪ
				bx = -5.0;
				by = -2.5;
				bz = 0.0;
				start = 100; // �ʱⰪ ���� ���� �����Ⱚ�� �־ ���� �������� �����Ӱ� �����. 

			}

			glPushMatrix();// 2�������� �ʱ����� ����
				glPushMatrix(); // 2�� ���� �ٴ� ������
					glColor3f(0.3, 0.4, 0.3);
					glBegin(GL_POLYGON); //2�� �ٴ� �� 
					glVertex3f(-8.9, -2.5, -1);
					glVertex3f(-8.9, -2.5, 1);
					glVertex3f(8.9, -2.5, 1);
					glVertex3f(8.9, -2.5, -1);
					glEnd();
					glFlush();
				glPopMatrix();
			
				glPushMatrix(); // 2�� ù° z�� ���ؼ�
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON); 
				glVertex3f(-4.44, -2.5, -1);
				glVertex3f(-4.44, -2.5, 1);
				glVertex3f(-4.24, -2.5, 1);
				glVertex3f(-4.24, -2.5, -1);
				glEnd();
				glFlush();
				glPopMatrix();
				
				glPushMatrix(); // 2�� ��° z�� ���ؼ�
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON);
				glVertex3f(-3.44, -2.5, -1);
				glVertex3f(-3.44, -2.5, 1);
				glVertex3f(-3.24, -2.5, 1);
				glVertex3f(-3.24, -2.5, -1);
				glEnd();
				glFlush();
				glPopMatrix();


				glPushMatrix(); // 2�� ù° z�� ���ؼ�
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON);
				glVertex3f(-2.44, -2.5, -1);
				glVertex3f(-2.44, -2.5, 1);
				glVertex3f(-2.24, -2.5, 1);
				glVertex3f(-2.24, -2.5, -1);
				glEnd();
				glFlush();
				glPopMatrix();

				glPushMatrix(); // 2�� ù° z�� ���ؼ�
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON);
				glVertex3f(-1.44, -2.5, -1);
				glVertex3f(-1.44, -2.5, 1);
				glVertex3f(-1.24, -2.5, 1);
				glVertex3f(-1.24, -2.5, -1);
				glEnd();
				glFlush();
				glPopMatrix();

				glPushMatrix(); // 2�� �߾� �ٴ��ٱ�����
					glColor3f(1.0, 1.0, 0.0);
					glBegin(GL_POLYGON);// 2�� �߾� �ٴ��� ����
					glVertex3f(-8.9, -2.5, 0.05);// 
					glVertex3f(-8.9, -2.5, -0.05);
					glVertex3f(8.9, -2.5, -0.05);
					glVertex3f(8.9, -2.5, 0.05);
					glEnd();
					glFlush();
				glPopMatrix();


				glPushMatrix(); // 2�� �߾� �ٴ��ٱ�����
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_POLYGON);// 2�� �߾� �ٴ��� ����
				glVertex3f(1.12, -2.5, 0.05);// 
				glVertex3f(1.12, -2.5, -0.05);
				glVertex3f(1.08, -2.5, -0.05);
				glVertex3f(1.08, -2.5, 0.05);
				glEnd();
				glFlush();
				glPopMatrix();

				glPushMatrix(); // 2�� �Ź��� ���� ��������ǥ��
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_POLYGON);// 2�� �Ź��� ���� ��������
				glVertex3f(0.1, -2.5, 1);// 
				glVertex3f(0.1, -2.5, -1);
				glVertex3f(0.00, -2.5, -1);
				glVertex3f(0.00, -2.5, 1);
				glEnd();
				glFlush();
				glPopMatrix();
				
			glPopMatrix(); // 2�������� �� ������ ��




				
			glPushMatrix(); // 2�������� ��ֹ� ������ ����

					

					glPushMatrix();// 2�������� ù��° �翷���� �����̴� ��ֹ�
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
						glTranslatef(stage2otc1x, -2.5, stage2otc1z); // 2�������� ù��° ���ƴٴϴ� ��ֹ� ��ǥ�� 
						glutSolidSphere(stage2radius, 100, 100);
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
					glPopMatrix();
					glPushMatrix();// 2�������� �ι�° �翷���� �����̴� ��ֹ�
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
						glTranslatef(stage2otc2x, stage2otc2y, 0); // 2�������� �ι�° ���ƴٴϴ� ��ֹ� ��ǥ�� 
						glutSolidSphere(stage2radius, 100, 100);
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
					glPopMatrix();
					glPushMatrix();// 2�������� ����° �翷���� �����̴� ��ֹ�
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
						glTranslatef(stage2otc3x, -2.5, stage2otc3z); // 2�������� ����° ���ƴٴϴ� ��ֹ� ��ǥ�� 
						glutSolidSphere(stage2radius, 100, 100);
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
					glPopMatrix();
					glPushMatrix();// 2�������� �׹�° �翷���� �����̴� ��ֹ�
						glEnable(GL_LIGHTING);
						glEnable(GL_LIGHT0);
						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, randomlightrgb);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, randomlightrgb);
						glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
						glTranslatef(stage2otc4x, stage2otc4y, 0); // 2�������� �׹�° ���ƴٴϴ� ��ֹ� ��ǥ�� 
						glutSolidSphere(stage2radius, 100, 100);
						glDisable(GL_LIGHTING);
						glDisable(GL_LIGHT0);
					glPopMatrix(); // 2�������� �׹�° �翷���� �����̴� ��ֹ� �� 


					glPushMatrix(); //2�������� �ǴϽ� ��Ż ������ ��Ʈ���� ����
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

					glTranslatef(f1x, f1y, f1z); // �ǴϽ� ��Ż ��ü �����ϴ� �κ�
					glutSolidSphere(fradius, 100, 100);

					glDisable(GL_LIGHTING);
					glDisable(GL_LIGHT0);
					glPopMatrix(); // 2�������� �ǴϽ� ��Ż ������ ��
				
				glPopMatrix();  //2�������� ��ֹ������� ��


					glPushMatrix(); // 2�������� ���� �����κн��� 
					
					
					if (bosscount == 1) { // ����
						if (bossblue == 1) {
							glPushMatrix(); // �Ķ����� ������ ����
							glEnable(GL_LIGHTING);
							glEnable(GL_LIGHT0);
							glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
							glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blue);
							glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
							glTranslatef(bossx, bossy, 0); // ������ǥ.
							glRotated(90, 0, 1, 0); // ���� ȸ�����Ѽ� �� �������� �����ֵ��� �Ѵ� .
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
								bluecount = 0; // ��� ü�¹� �� �ޱ�.
								bossblue = 0; // �Ķ� ������ ������
								bossgreen = 1;  // �ʷ� ������ �����Ų��
								
							}
							glPopMatrix(); // �Ķ����� �����γ� 
						}
						
						else if (bossgreen == 1 && bluecount ==0) { // ������ �ʷϻ��϶�
							glPushMatrix(); // �ʷϺ��� ������ ����
							glEnable(GL_LIGHTING);
							glEnable(GL_LIGHT0);
							glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
							glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, green);
							glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
							glTranslatef(bossx, bossy, 0); // ������ǥ.
							glRotated(90, 0, 1, 0); // ���� ȸ�����Ѽ� �� �������� �����ֵ��� �Ѵ� .
							glutWireSphere(0.5, 10, 4);
							glDisable(GL_LIGHTING);
							glDisable(GL_LIGHT0); 
							if (shot == 4) {
								greencount=greencount-1;
							}
							else if (shot == 5) {
								greencount = 0; // ��� ü�¹� �� �ޱ�.
								bossgreen = 0; // �ʷϺ����� ������
								bossred = 1;  // ���� ������ �����Ų��

							}
							glPopMatrix(); // �ʷϺ��� �����γ� 
							
						}

						else if (bossred == 1 && greencount == 0) { // ������ �������϶�
							glPushMatrix(); // �������� ������ ����
							glEnable(GL_LIGHTING);
							glEnable(GL_LIGHT0);
							glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
							glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
							glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
							glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
							glTranslatef(bossx, bossy, 0); // ������ǥ.
							glRotated(90, 0, 1, 0); // ���� ȸ�����Ѽ� �� �������� �����ֵ��� �Ѵ� .
							glutWireSphere(0.5, 10, 4);
							glDisable(GL_LIGHTING);
							glDisable(GL_LIGHT0);
							if (shot == 6) {
								redcount = 1;
							}
							else if (shot == 7) {
								redcount = 0; // �������� ü�¹� �� �ޱ�.
								bossred = 0; // ���������� ������
								bosscount = 0; // ���� �����γ�����
								bossno = 1; //���� �ѹ� ���Դٰ� ���� 

								}
							glPopMatrix(); // �������� �����γ� 
							}
						
						glPopMatrix(); // ���� �����γ� 
						
						
						
						glPushMatrix(); // ���� ü�¹� ������
						if (shot <= 0) {
							glPushMatrix(); // ���� ù��°
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, 0.2); // �����ʰ� 
							glVertex3f(3.0, -1.8, 0.15);//���ʰ� = �����ʰ� - 0.05
							glVertex3f(3.0, -1.9, 0.15);
							glVertex3f(3.0, -1.9, 0.2);
							glRotated(90, 0, 1, 0); // �������� ���ϼ� �ְ�ȸ��
							glEnd();
							glFlush();
							glPopMatrix(); // ����ù��° ü�¹�
						}
						if (shot<= 1) {
							glPushMatrix(); //���� �ι�° ü�¹� ����
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, 0.14); // �����ʰ� 
							glVertex3f(3.0, -1.8, 0.09);//���ʰ� = �����ʰ� - 0.05
							glVertex3f(3.0, -1.9, 0.09);
							glVertex3f(3.0, -1.9, 0.14);
							glRotated(90, 0, 1, 0); // �������� ���ϼ� �ְ�ȸ��
							glEnd();
							glFlush();
							glPopMatrix(); // ���ʵι�° ü�¹� ��
						}
						if (shot <= 2) {
							glPushMatrix(); // ���� ����° ü�¹� ����
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, 0.08); // �����ʰ� 
							glVertex3f(3.0, -1.8, 0.03);//���ʰ� = �����ʰ� - 0.05
							glVertex3f(3.0, -1.9, 0.03);
							glVertex3f(3.0, -1.9, 0.08);
							glRotated(90, 0, 1, 0); // �������� ���ϼ� �ְ�ȸ��
							glEnd();
							glFlush();
							glPopMatrix(); // ���� ����° ü�¹� ��
						}
						if (shot <= 3) {
							glPushMatrix(); // ���� �׹�° ü�¹� ����
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, 0.02); // �����ʰ� 
							glVertex3f(3.0, -1.8, -0.03);//���ʰ� = �����ʰ� - 0.05
							glVertex3f(3.0, -1.9, -0.03);
							glVertex3f(3.0, -1.9, 0.02);
							glRotated(90, 0, 1, 0); // �������� ���ϼ� �ְ�ȸ��
							glEnd();
							glFlush();
							glPopMatrix(); // ���� �׹�° ü�¹� ��
						}
						if (shot <= 4) {
							glPushMatrix(); // ���� �ټ���° ü�¹� ����
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, -0.04); // �����ʰ� 
							glVertex3f(3.0, -1.8, -0.09);//���ʰ� = �����ʰ� - 0.05
							glVertex3f(3.0, -1.9, -0.09);
							glVertex3f(3.0, -1.9, -0.04);
							glRotated(90, 0, 1, 0); // �������� ���ϼ� �ְ�ȸ��
							glEnd();
							glFlush();
							glPopMatrix(); // ���� �ټ���° ü�¹� ��
						}
						if (shot<= 5) {
							glPushMatrix(); // ���� ������° ü�¹� ����
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, -0.1); // �����ʰ� 
							glVertex3f(3.0, -1.8, -0.15);//���ʰ� = �����ʰ� - 0.05
							glVertex3f(3.0, -1.9, -0.15);
							glVertex3f(3.0, -1.9, -0.1);
							glRotated(90, 0, 1, 0); // �������� ���ϼ� �ְ�ȸ��
							glEnd();
							glFlush();
							glPopMatrix(); // ���� ������° ü�¹� ��
							
						}
						if (shot <=6) {

							glPushMatrix(); // ���� �ϰ���° ü�¹� ����
							glColor3f(1.0, 1.0, 0.0);
							glBegin(GL_POLYGON);
							glVertex3f(3.0, -1.8, -0.16); // �����ʰ� 
							glVertex3f(3.0, -1.8, -0.21);//���ʰ� = �����ʰ� - 0.05
							glVertex3f(3.0, -1.9, -0.21);
							glVertex3f(3.0, -1.9, -0.16);
							glRotated(90, 0, 1, 0); // �������� ���ϼ� �ְ�ȸ��
							glEnd();
							glFlush();
							glPopMatrix(); // ���� �ϰ���° ü�¹� ��
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
						
						glPopMatrix(); // ���� ü�¹� ������ �� 


						
					}
			


			else if (clearcount ==2) {
				glClearColor(0.0, 0.0, 0.0, 0.0);
				glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW �� ���۸� �����

				if (start == 0) { // 1�������� �����Ҷ� ���� ��ġ �ʱⰪ
					bx = -5.0;
					by = -2.5;
					bz = 0.0;
					start = 100;
				}// �ʱⰪ ���� ���� �����Ⱚ�� �־ ���� �������� �����Ӱ� �����. 

			}



				
			


			if (ballmove == 1) { // ���� �����ϼ��ִ� ���¿� �������̴� ���¸� ������������if ������ ���� ���� 
				glPushMatrix(); // �������� ��Ʈ���� 1��
				
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);

				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);

				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, red);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20);
				glTranslatef(bx, by, bz); // ���� �����ϴ� �κ�
				glutSolidSphere(radius, 100, 100);
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				
				glPopMatrix();
			}
			glPopMatrix();

		}

		//2t�������� ������ ��

		//3�������� ������ ����

		else if (clearcount == 2&&timerstart==1) {
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT); //GL_MODELVIEW �� ���۸� �����
			char str[100];



			cout << "������ ���� ��ŷ�� ����մϴ�." << endl;


			printf("�̸��� �Է��ϼ��� :  \n");
			gets_s(str, 100);

			FILE *fp;

			fp = fopen("../save.txt", "wt");
			if (fp == NULL)
			{
				printf("\n���� �ҷ��������\n");
			}

			fprintf(fp, "%s", str);
			fprintf(fp, " ����Ƚ��: %d", death);
			fprintf(fp, " ����ð� : %3.d \n", (finish - go) / CLK_TCK);
			




			fclose(fp);

			printf("\n�Ϸ�\n");
			clearcount = 4;
			
		}



		if (obstacle == 1) {// ���� ��ֹ��� �ɸ� ��Ȳ�϶� 
			death = death + 1; //����Ƚ�� 1 ����
			cout << "����Ƚ�� : " << death << endl;
			start = 100;
			obstacle = 0;

		}



		//cout << bx << endl;
		//cout << by << endl;	




		glutSwapBuffers(); // DOUBLE �� ����Ͽ����� SWAPBUFFERS ���.
		glutPostRedisplay();

	}
	}

	
	



void reshape(int x, int y) // ������ â�� ũ�Ⱑ ����ɶ� ��� ��ü���� ��Ⱦ�� �״�� ������Ű���� ����.
{
	
		if (y == 0 || x == 0) return;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(39.0, (GLdouble)x / (GLdouble)y, 1, 21.0); // �þ߰� 39,��Ⱦ��=������ �������/����, �պκ����ܸ�,�޺κ����ܸ�
		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, x, y);  //Use the whole window for rendering
	

}

void myspkeyboard(int pressedkey, int x, int y) {
	if (pressedkey == GLUT_KEY_LEFT) {
		if (lxg != 0) { //lxg ���� 0�� �ƴҶ� = ���������̵��ϴ� ���ӵ��� �����Ҷ� , lxg�� �ʱⰪ�� 0.04,
			if (bosscount == 0) {
				bx = bx - 0.04;
			}
			bx = bx;
		}
		else if (lxg <0.0002) { // lxg�� 0.002���� Ŀ������ = ���ӵ��� ���� ��������Ѵ�
			if (bosscount == 0 ) {
				bx = bx - 0.04;
			}
		}



	}

	if (pressedkey == GLUT_KEY_RIGHT) {
		if (rxg != 0) { //lxg ���� 0�� �ƴҶ� = ���������̵��ϴ� ���ӵ��� �����Ҷ� , lxg�� �ʱⰪ�� 0.04,
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
		if (sxg != 0) { //sxg ���� 0�� �ƴҶ� = ���������̵��ϴ� ���ӵ��� �����Ҷ� , lxg�� �ʱⰪ�� 0.04,

			bz = bz - 0.04;
		}
	}

	if (pressedkey == GLUT_KEY_DOWN) {
		if (sxg != 0) { //sxg ���� 0�� �ƴҶ� = ���������̵��ϴ� ���ӵ��� �����Ҷ� , lxg�� �ʱⰪ�� 0.04,
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
		
		case 32: //�����̽���
			gamestart = 1;
			
			jump = 0.1;
			if (jump > 0.1) {
				jump = 0;
			}
			break;
		case 'a': //a ��������
			if (shot <= 2) {
			shot= shot + 1;
			PlaySound(TEXT(SOUND_FILE_NAME_BOSSKILL), 0, SND_ASYNC);
			}
			
			
			break;
		case 's': //s ��������
			if (shot <=4 && shot>=3) {
				shot = shot + 1;
				PlaySound(TEXT(SOUND_FILE_NAME_BOSSKILL), 0, SND_ASYNC);
			}
			break;
		case 'd': //d ��������
			if (shot<=6 && shot>=5) { // ����
				shot = shot+1;
				PlaySound(TEXT(SOUND_FILE_NAME_BOSSKILL), 0, SND_ASYNC);
			}
			
			break;

		case 'f': //������ �������� �ϱ����� ����޴�
			
			bosscount = 1;
			view = 5;
			break;


		case 'q': //q ��������
			clearcount = 1;
			break;
		default:
			break;
		}
	glutPostRedisplay(); //ȭ��ٽñ׸���
}


void mytimer(int value) {
	randomr = (float(rand()%10)/10); // 0.0~1.0���� �������� �����Ǵ� rgb�� timer �̿��� ��� ���� �ٲ۴� .
	randomg = (float(rand() % 10) / 10);
	randomb = (float(rand() % 10) / 10);

	

	if (jump > 0) {
		jump = jump - 0.01;
	}
	
	

	switch (upordown) {
	case 0: // ���� �������� (down �϶�)
		if (bosscount == 0&&clearcount!=2) {
			if (by > sky) { // �ְ����� ���̺��� ������ ���� �ְ���� ������ ��� ����
				by = sky;
			}
			if (by > -2.5000) { // ���� ���̺��� ���� ��ġ�� ������ ��������
				by = by - 0.03 - g;
				g = g + 0.0001;
			}
			if (by <= -2.5000 && clearcount!=2) { //�������̺��� ������ġ�� ���ų� ���������� ���� ����� �����ϵ��� �ٲ۴� 
				by = by + 0.03;
				g = 0.0001;
				upordown = 1;
				PlaySound(TEXT(SOUND_FILE_NAME_JUMP), 0, SND_ASYNC);

			}
		}
		break;

	case 1:// ���� �����Ҷ� ( up�϶�)
		
		
		if (bosscount == 0&&clearcount!=2) {
			if (by < sky) { // �ְ����� ���� ���� ������ �ö󰡵��� �Ѵ�.
				by = by + 0.03 - g;
				g = g - 0.0001;
			}
			if (by == sky) { // �ְ����� ���̿� ���������� ���� ������� ���������Ϲٲ۴�

				by = by - 0.03;
				upordown = 0;
				g = 0.0001;

			}
			if (by > sky) { // �ְ� �������� ũ�� �پ����� �� ���� �ö��� ���ϵ��� �Ѵ�.
				by = sky;
			}
		}
		break;

		}
	
	if (bx < -5.84) { // ���� ȭ������� �������ʰ� �����ִ� �Լ�
		bx =-5.84;
	}
	if (bx > 5.84) { // ���� ȭ������� �������ʰ� �����ִ��Լ� 
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






	//��ֹ�,�ǴϽ����� �ν� ������ 

	if(clearcount == 0 && bx<=-3.3 && bx >=-3.4 && by<=-2.3 && by>=-2.4) //  ù��° ������������ ù��° ��ֹ��� ��ǥ���� ��ġ�Ҷ� 
	{
		obstacle = 1; // ��ֹ��� �ε����ٰ� �ν��Ҽ��ְ� �������� 1�� ���� (��ֹ��� �ε����� ���� ��ġ�� �ʱ�ȭ��)
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
	}

	if (clearcount== 0 && bx >= -2.12 && bx <= -2.02 && by >= -2.2 && by <= -1.7) { //  ù��° �������� ���� ���� �ι�° ��ֹ��� ��ǥ���� ��ġ�Ҷ�
		obstacle = 1; // ��ֹ��� �ε����ٰ� �ν��Ҽ��ְ� �������� 1�� ���� 
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
	}

	if (clearcount == 0 && bx >= -1.0 && bx <= -0.88 && by <= -2.15 && by >= -2.6) { // ù��° �������� ���� ����° ��ֹ��� ��ǥ���� ��ġ�Ҷ�
		obstacle = 1; // ��ֹ��� �ε����ٰ� �ν��Ҽ��ְ� �������� 1�� ���� 
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
	}

	if (clearcount == 1 && bz >= stage2otc1z - 0.1 && bz <= stage2otc1z + 0.1 && bx+0.05>=-4.44 && bx-0.05<=-4.24 && by>=-2.5-0.1 && by<=2.5+0.1) { // stage2otc1 �� �ɷ����� 
		obstacle = 1;
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
		
	}

	if (clearcount == 1 && by >= stage2otc2y - 0.1 && by <= stage2otc2y + 0.1 && bx >= stage2otc2x-0.1 && bx <= stage2otc2x+0.1 && bz>=-0.1 && bz<=0.1) { // stage2otc2 �� �ɷ����� 
		obstacle = 1;
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
		
	}
	
	if (clearcount == 1 && bz >= stage2otc3z - 0.1 && bz <= stage2otc3z + 0.1 && bx+0.1 >= stage2otc3x-0.1 && bx-0.1 <= stage2otc3x+0.1&& by <= -2.4 && by >=-2.6) { // stage2otc1 �� �ɷ����� 
		obstacle = 1;
		start = 0; PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
		
	}


	if (clearcount == 1 && by >= stage2otc4y - 0.1 && by <= stage2otc4y + 0.1 && bx+0.1 >= stage2otc4x - 0.1 && bx-0.1 <= stage2otc4x + 0.1 && bz >= -0.1 && bz <= 0.1) { // stage2otc2 �� �ɷ����� 
		obstacle = 1;
		start = 0;
		PlaySound(TEXT(SOUND_FILE_NAME_DEAD), 0, SND_ASYNC);
		
	}
	if (clearcount == 1 && bosscount == 0 && bx >= 0.0 && bx <= 0.3 && bossno==0) { //������� �ߵ��� (0.0 ~0.3 ���̿� ���� ��������.)
		bosscount = 1;
		by = -2.5;
		PlaySound(TEXT(SOUND_FILE_NAME_BOSSSTART), 0, SND_ASYNC);
		
		view = 5;
		
	}
	//glTranslatef(stage2otc1x, -2.5, stage2otc1z); // 2�������� ù��° ���ƴٴϴ� ��ֹ� ��ǥ�� 
	glutTimerFunc(10, mytimer, 3000);

}

int main(int argc, char **argv)
       {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(600, 350);
	glutInitWindowSize(1000, 600);
	glutCreateWindow("��Ƣ���");
	if (clearcount == 0&&timerstart==0) {
		go = clock();
		timerstart = 1;
	}
	
	initLighting();
	srand((unsigned)time(NULL));

	cout << " >>>>>>>>>>>>>>> ��Ƣ��� ���ӿ� ���Ű��� ȯ���մϴ� <<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>���� �ݴ��� ū �� �ȿ� ���� �������� Ŭ�����Դϴ�<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>>>>>  �̵� : Ű���� ȭ��ǥ Ű  <<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>>>>>    ���� : �����̽� ��     <<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>> page up = ī�޶� ���� ��� ���� ����<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>  home   = ���� �������ιٶ󺸱�  <<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>home Ű�� �ٽô����� �����ǽ������� ����˴ϴ�<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>��ֹ��� ������ ���� �ɸ����� �����غ�����<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>> ��ֹ��� �ɸ��� �������� ��Ÿ���ϴ�<<<<<<<<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�<<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>> �Ź��� �������� ������ �Ź��ٰ����� ��������<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>       ������ �ؾ��մϴ�.                  <<<<<<<<<<<<" << endl;
	cout << " >>>>>>>>>>>>>  a:�Ķ��� s:�ʷϻ� d:������               <<<<<<<<<<<<" << endl;
	cout << " >>>>>>>�ѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤѤ�<<<<<<<<<<<<" << endl;



	

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