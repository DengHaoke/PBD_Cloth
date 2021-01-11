//#include<GL/glew.h>
#include"Simulator.h"
#include<GL/glut.h>
#include<cmath>

#define PI 3.1415927
using namespace std;

#define DAMPING 0.96 // how much to damp the cloth simulation each frame
#define TIME_STEP 0.03 // Time step each particle takes each frame
float Delta = 0;
//Delta = Delta + 0.1;
Vec3 ball_pos(0, 0, 0); // 球的中心
double ball_radius = 10.0; // 球的半径
Vec3 wind(-80, 0.0, 50);//风力
Vec3 gravity(0, -150, 0);//重力
double groundHeight = -15.0;
//mass_cloth cloth(10, 10, 20, 20);

Simulator::Simulator()
{

}
Simulator::~Simulator()
{

}


void Simulator::Initialize()
{
	timsStep = 0.01;
	ground = Vec3(0.0, -15.0, 0.0);
	sphere = Vec3(0.0, 0.0, 0.0);
	//mass_cloth cloth(10, 10, 20, 20);
	cloth = new mass_cloth(50, 50, 50, 50);


	cloth->drawMode = 0;

	//cloth->init();
}

void Simulator::Update()
{
	cloth->addForce(gravity * TIME_STEP);
	cloth->addForce(wind * TIME_STEP);
	//Vec3 ball_pos(6 * sin(0.1 * PI * Delta), 0, 4 * sin(0.1 * PI * Delta));
	cloth->timeStep(ball_pos, ball_radius, DAMPING, TIME_STEP, groundHeight);
}


void Simulator::Render()
{
	Lighting();
	DrawGround();
	cloth->draw();
	//DrawSphere();
}

void Simulator::Lighting()
{
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float light_pos[] = { 150.0, 150.0, 0.0, 1.0 };
	float light_dir[] = { -1, -1, 0 , 0.0 };
	float light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float frontColor[] = { 0.8, 0.8, 0.8, 1 };

	float matShininess = 20;
	float noMat[] = { 0, 0, 0, 1 };

	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);					// Set ambient property frontColor.
	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);					// Set diffuse property frontColor.
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 80.0f);                  // 80도 원뿔
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 80.0f);                 // 초점 설정
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}
void Simulator::DrawGround(void) {

	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);

	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-250.0f + 250.0f / 64 * x, ground.y, -250.0f + 250.0f / 64 * y);
			glVertex3f(-250.0f + 250.0f / 64 * (x + 1), ground.y, -250.0f + 250.0f / 64 * y);
			glVertex3f(-250.0f + 250.0f / 64 * (x + 1), ground.y, -250.0f + 250.0f / 64 * (y + 1));
			glVertex3f(-250.0f + 250.0f / 64 * x, ground.y, -250.0f + 250.0f / 64 * (y + 1));
		}
	}
	glEnd();
}

void Simulator::DrawSphere(void)
{
	glColor3f(1.0, 1.0, 1.0);
	//glTranslated(6 * sin(0.1 * PI * Delta), 0, 0.4 * sin(0.1 * PI * Delta));
	glutSolidSphere(9.8, 60, 60);
	//Delta = Delta + 0.03;
}

