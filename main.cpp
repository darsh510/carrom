#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <sys/time.h>
#include <stdlib.h>
#endif
#define PI 3.1415926
#include <cstdio>
void move(int);
//flags
int vf;
int rf;
int uf;
int mob=0;
bool mouseleftdown=false;
bool lb,rb;
int Mx,My;
int vis=0;
using namespace std;
int hit=0;
int flag=1;
int score=30;
float speed;
void reset(void);
void drawPower();
void Collection();
void pocket();
void checkCollision();
void initRendering()
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glEnable(GL_DEPTH_TEST);
}
void RenderString(float x, float y, void *font, const char* string,float r,float g,float b)
{  
  //char *c;
  glColor3f(r, g, b); 
  glRasterPos3f(x, y,0);
  int xx = strlen(string);
  for (int i=0;i<xx;i++)
  glutBitmapCharacter(font, string[i]);
}
void handleResize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity(); // reset camera
	gluPerspective( 45.0,(double)w/(double)h,1.0, 400.0);
}
void solidcircle(float x,float y,float z,float r,int loop)
{
	glBegin(GL_POLYGON);
	for(int i=0;i<loop;i++)
	{
		float theta= ((2*PI)/loop)*i;
		float xc=x+r*cos(theta);
		float yc=y+r*sin(theta);
		glVertex3f(xc,yc,z);
	}	
	glEnd();
}
void hollowcircle(float x,float y,float z,float r,int loop)
{
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<loop;i++)
	{
		float theta= ((2*PI)/loop)*i;
		float xc=x+r*cos(theta);
		float yc=y+r*sin(theta);
		glVertex3f(xc,yc,z);
	}	
	glEnd();
}
class coin
{
	private:

	public:
		float x,y,r,vel_x,vel_y;
		int value;
		float pocket;
		coin(float,float,float,int );
		void draw();
};
timeval t1;
coin::coin(float a,float b,float c,int u)
{
	x=a;
	y=b;
	r=c;
	value=u;
	vel_x=0;
	vel_y=0;
	pocket=0;
}
void coin::draw()
{
	glLoadIdentity();
	glTranslatef(0,0,-1.07f);
	if(value==10) glColor3f(1.0f,1.0f,1.0f);
	else if(value==-5) glColor3f(0.0f,0.0f,0.0f);
	else glColor3f(1.0f,0,0);
	solidcircle(x,y,0,r,20);
}
coin men[7]={coin(0,0,0,0),coin(0,0,0,0),coin(0,0,0,0),coin(0,0,0,0),coin(0,0,0,0),coin(0,0,0,0),coin(0,0,0,50)};
//coin Qn(0,0,0,50);
class striker
{
	public:
		double x;	//x-cordinate
		double y;	//y-coordinate
		double r;	//radius
		double vx,vy;
		float theta;
		void init(float); //initalising
		void draw();
};
void striker::init(float yc)
{
	x=0.0f;
	y=-yc;
}
void striker::draw()
{
	glColor4f(0,0,1.0f,0.7f);
	solidcircle(x,y,0,r,100);
	glColor3f(1.0f,1.0f,1.0f);
	glTranslatef(0,0,0.001);
	glLineWidth(1.0f);
	hollowcircle(x,y,0,r*0.9,100);
	hollowcircle(x,y,0,r*0.7,100);

}
striker S;
class Board
{
	private:
		float inboard_length;
		float outboard_length;
		float hole_radius;
	public:
		Board(float, float, float);
		void drawboard();
		float getInL()
		{
			return inboard_length;
		}
		float getHR()
		{
			return hole_radius;
		}
};
Board::Board(float in,float out, float r)
{
	inboard_length=in;
	outboard_length=out;
	hole_radius=r;
}
void Board::drawboard()
{
	glPushMatrix();
	glTranslatef(0.0f,0.0f,-1.100f);
	glColor3f(0.45f,0.28f,0.07f);
	glBegin(GL_POLYGON);
	glVertex3f((float)inboard_length/1.414,(float)inboard_length/1.414,0);
	glVertex3f(-(float)inboard_length/1.414,(float)inboard_length/1.414,0);
	glVertex3f(-(float)inboard_length/1.414,-(float)inboard_length/1.414,0);
	glVertex3f((float)inboard_length/1.414,-(float)inboard_length/1.414,0);
	glEnd();
	
	//outer portion
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-1.200f);
	glColor3f(0.41f,0.12f,0.0f);
	glBegin(GL_POLYGON);
	glVertex3f((float)outboard_length/1.414,(float)outboard_length/1.414,0.0);
	glVertex3f(-(float)outboard_length/1.414,(float)outboard_length/1.414,0.0);
	glVertex3f(-(float)outboard_length/1.414,-(float)outboard_length/1.414,0.0);
	glVertex3f((float)outboard_length/1.414,-(float)outboard_length/1.414,0.0);
	glEnd();

	//corner holes
	glLoadIdentity();
	glColor3f(0.0f,0.0f,0.0f);
	solidcircle(((float)inboard_length/1.414)- hole_radius-0.004,((float)inboard_length/1.414)- hole_radius-0.004,-1.090f,hole_radius,100);
	solidcircle(-((float)inboard_length/1.414)+ hole_radius+0.004,((float)inboard_length/1.414)- hole_radius-0.004,-1.090f,hole_radius,100);
	solidcircle(-((float)inboard_length/1.414)+ hole_radius+0.004,-((float)inboard_length/1.414)+ hole_radius+0.004,-1.090f,hole_radius,100);
	solidcircle(((float)inboard_length/1.414)- hole_radius-0.004,-((float)inboard_length/1.414)+ hole_radius+0.004,-1.090f,hole_radius,100);

	//traingles at the corners
	int angle=0;
	glColor3f(0.41f,0.12f,0.0f);
	triangle:
		glLoadIdentity();
		glRotatef((float)angle,0.0f,0.0f,-1.0f); 
		glTranslatef((float)inboard_length/1.414,(float)inboard_length/1.414,-1.090); 
		glBegin(GL_POLYGON);
		glVertex3f(0,0,0);
		glVertex3f(-hole_radius,0,0);
		glVertex3f(0,-hole_radius,0);
		glEnd();
		angle+=90;
	if(angle!=360) goto triangle;


	//play-area
    angle=0;
	ring:
		glLoadIdentity();
		glColor3f(0.0f,0.0f,0.0f);
		glRotatef((float)angle,0.0,0.0,1.0);
		glTranslatef(0.0f,0.0f,-1.088f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(inboard_length*0.45,inboard_length*0.48,0.0f);
		glVertex3f(-inboard_length*0.45,inboard_length*0.48,0.0f);
		glEnd();
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(inboard_length*0.45,inboard_length*0.52,0.0f);
		glVertex3f(-inboard_length*0.45,inboard_length*0.52,0.0f);
		glEnd();
		hollowcircle(-inboard_length*0.45,inboard_length*0.50,0,inboard_length*0.02,10);
		hollowcircle(inboard_length*0.45,inboard_length*0.50,0,inboard_length*0.02,10);
		glLineWidth(1.0f);
		glColor3f(1.0f,0.0f,0.0f);
		solidcircle(inboard_length*0.45,inboard_length*0.50,0,inboard_length*0.02,100);
		solidcircle(-inboard_length*0.45,inboard_length*0.50,0,inboard_length*0.02,100);
	angle+=90;
	if(angle!=360) goto ring;
	angle=0;
	
	//center ring
	spokes:
		glLoadIdentity();
		glRotatef((float)angle,0.0,0.0,1.0);
		glTranslatef(0.0f,0.0f,-1.088f);
		glColor3f(0.0f,0,0);
		glBegin(GL_LINE_LOOP);
		glVertex3f(0,0,0);
		glVertex3f(0,inboard_length*0.18,0);
		glVertex3f(inboard_length*0.02,inboard_length*0.05,0);
		glEnd();		
	//	glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_POLYGON);
		glVertex3f(0,0,0);
		glVertex3f(0,inboard_length*0.18,0);
		glVertex3f(-inboard_length*0.02,inboard_length*0.05,0);
		glEnd();
		angle+=45;
		if(angle!=360) goto spokes;	

	//glColor3f(0,0,0);
	hollowcircle(0,0,0,inboard_length*0.19,100);
	glLineWidth(2.0f);
	hollowcircle(0,0,0,inboard_length*0.20,100);

	//4-circle-arrows
	angle=0;
	arrow:
		glLoadIdentity();
		glTranslatef(0,0,-1.088);
		glColor3f(0,0,0);
		glRotatef(angle+(float)45,0,0,1.0);
			glBegin(GL_LINE_STRIP);
			glLineWidth(5.0f);
		float x=inboard_length*0.45;
		float r=inboard_length*0.06;
		for(int i=0;i<100;i++)
		{
			float theta= PI/8+((7*PI)/400)*i;
			float xc=x+r*cos(theta);
			float yc=r*sin(theta);
			glVertex3f(xc,yc,0);
		}	
		glEnd();
		glBegin(GL_LINE_LOOP);
		glVertex3f(inboard_length*0.45-inboard_length*0.06,0,0);
		glVertex3f(inboard_length*0.780,0,0);
		glEnd();
		glPushMatrix();
		glTranslatef(inboard_length*0.780,0,0);
		glBegin(GL_POLYGON);
		glVertex3f(0.01,0,0);
		glVertex3f(-0.005,0.005,0);
		glVertex3f(0,0,0);
		glVertex3f(-0.005,-0.005,0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(inboard_length*0.45+r*cos((52*PI)/400),r*sin((52*PI)/400),-0);
		glRotatef(-(float)90,0,0,1.0);
		glBegin(GL_POLYGON);
		glVertex3f(0.0,0,0);
		glVertex3f(-0.01,0.01,0);
		glVertex3f(-0.005,0,0);
		glVertex3f(-0.01,-0.01,0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(inboard_length*0.45+r*cos((15*PI)/8),r*sin((15*PI)/8),0);
		glRotatef((float)90,0,0,1.0);
		glBegin(GL_POLYGON);
		glVertex3f(0.0,0,0);
		glVertex3f(-0.01,0.01,0);
		glVertex3f(-0.005,0,0);
		glVertex3f(-0.01,-0.01,0);
		glEnd();
		glPopMatrix();
		angle+=90;
		if(angle!=360) goto arrow;
	if(flag) S.init(inboard_length*0.50);
	flag=0;
	glPopMatrix();
};
float ww,wh;
void drawScene()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glutFullScreen();
	glLoadIdentity();
	Collection();
	ww=glutGet(GLUT_WINDOW_WIDTH);
	wh=glutGet(GLUT_WINDOW_HEIGHT);
	float ox = ((float(Mx)-ww/2)/ww)*1.51;
	float oy = -((float(My)-wh/2)/wh)*0.83;
	if(mob)
	{
		S.theta=atan(oy-S.y/ox-S.x);
		if(oy>S.y&&ox<S.x) S.theta=PI + S.theta;
		else if(oy<S.y&&ox<S.x) S.theta+=PI;
	}
	glLoadIdentity();
	Board carrom((float)0.5656,(float)0.67872,(float)0.040);
	carrom.drawboard();
	glLoadIdentity();
	glTranslatef(0.0,0.0,-1.087f);
	if(!vis)S.draw();
	glLoadIdentity();
	glTranslatef(0.0,0.0,-1.087f);
	if(!hit)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3f(S.x,S.y,0.0);
		glVertex3f(S.x+0.12*cos(S.theta),S.y+0.12*sin(S.theta),0.0);
		glEnd();
	}
	for(int i=0;i<7;i++)
	{
		if(!men[i].pocket)men[i].draw();
	}
	checkCollision();
	pocket();
	char a[10];
	sprintf(a,"%d",score);
	RenderString(0.65f, 0.3f, GLUT_BITMAP_TIMES_ROMAN_24, a, 1.0f, 0.0f, 0.0f);
	RenderString(0.55f, 0.3f, GLUT_BITMAP_TIMES_ROMAN_24, "Score: ", 1.0f, 0.0f, 0.0f);
	
	glColor3f(0,0,0);
	glBegin(GL_POLYGON);
	glVertex3f(0.72f,0.35f,0);
	glVertex3f(0.52,0.35,0);
	glVertex3f(0.52,0.25,0);
	glVertex3f(0.72,0.25,0);
	glEnd();

	drawPower();
//	for(int i=0;i<7;i++)
//		//////<<men[i].value<<' ';
	int ff=0;
	for(int i=0;i<7;i+=2)
		if(men[i].pocket==0) {ff=1;break;}
	if(ff==0)
	{
		glLoadIdentity();
		glTranslatef(0,0,-1.002);
		RenderString(-0.2, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Game Complete!!! Your score:", 1.0f,1.0f, 1.0f);
		RenderString(0.15f, 0, GLUT_BITMAP_TIMES_ROMAN_24, a, 1.0f, 1.0f, 1.0f);
		glTranslatef(0,0,1.002);
		glColor3f(0,0,1.0f);
		glBegin(GL_POLYGON);
		glVertex3f(1.0f,1.0f,-1.003);
		glVertex3f(1.0f,-1.0f,-1.003);
		glVertex3f(-1.0f,-1.0f,-1.003);
		glVertex3f(-1.0f,1.0f,-1.003);
		glEnd();
		glTranslatef(0,0,0.001);
		gettimeofday(&t1,NULL);
		
	}
	glutSwapBuffers();
}
float mew=0.007;
void motion(int x,int y)
{
	glutPostRedisplay();
	Mx=x;
	My=y;
}
void click(int x,int y)
{
	glutPostRedisplay();
	Mx=x;
	My=y;
	if(rf&&(speed < 0.016)) speed+=0.001;
	if(vf==1) move(x);
}
int oa=0;
int glo=0;
void update(int value)
{
	timeval t2;
	gettimeofday(&t2, NULL);	
	score-=t2.tv_sec - t1.tv_sec;
	t1=t2;
	for(int i=0;i<7;i++)
	{
		if(!men[i].pocket)
		{
			float prev_vx= men[i].vel_x;
			float prev_vy= men[i].vel_y;
			float alph=PI/2;
			alph=atan(men[i].vel_y/men[i].vel_x);
			if(alph<0) alph*=-1;
					if((men[i].vel_x<0.00009)&&(men[i].vel_x>-0.00009)) men[i].vel_x=0.0;
					else 
					{	
					if(men[i].vel_x > 0.0000) men[i].vel_x-=mew*0.01*cos(alph);
					else men[i].vel_x+=mew*0.01*cos(alph);
					}
		
			if((men[i].vel_y < 0.00009)&&(men[i].vel_y>-0.00009)) men[i].vel_y=0.0;
			else 
			{
			if(men[i].vel_y > 0.00000) men[i].vel_y-=mew*0.01*sin(alph);
			else men[i].vel_y+=mew*0.01*sin(alph);
			}
			if(prev_vx*men[i].vel_x < 0.00000) men[i].vel_x=0.0;
			if(prev_vy*men[i].vel_y < 0.00000) men[i].vel_y=0.0;	
			men[i].x+=men[i].vel_x;
			men[i].y+=men[i].vel_y;
			if(men[i].x < -0.400+men[i].r) 
			{
				men[i].vel_x*=-1;
					men[i].x+=0.01;
			}
			else if (men[i].x > 0.400-men[i].r) 
				{
					men[i].vel_x*=-1;
					men[i].x-=0.01;
				}
			if(men[i].y < -0.400+men[i].r) 
			{
			men[i].y+=0.01;
				men[i].vel_y*=-1;
			}
			if(men[i].y > 0.400-men[i].r) 
			{
					men[i].y-=0.01;
					men[i].vel_y*=-1;
			}
		}
	}
	if(hit)
	{
		float prev_vx= S.vx;
		float prev_vy= S.vy;
		float alph=PI/2;
		alph=atan(S.vy/S.vx);
		if(alph<0) alph*=-1;
		if((S.vx<0.00009)&&(S.vx>-0.00009)) S.vx=0.0;
		else 
		{	
			if(S.vx > 0.00001) S.vx-=mew*0.01*cos(alph);
			else S.vx+=mew*0.01*cos(alph);
		}
			if((S.vy<0.00009)&&(S.vy>-0.00009))S.vy=0.0;
			else 
			{
				if(S.vy > 0.00001) S.vy-=mew*0.01*sin(alph);
				else S.vy+=mew*0.01*sin(alph);
			}
			if(prev_vx*S.vx < 0.000001) S.vx=0.0;
			if(prev_vy*S.vy < 0.000001) S.vy=0.0;
		if(((S.vx<0.00009)&&(S.vx>-0.00009)) && ((S.vy<0.00009)&&(S.vy>-0.00009)))
		{
			int i;
			for(i=0;i<7;i++)
			{
				if(men[i].pocket) continue;
				if(((men[i].vel_x<0.00009)&&(men[i].vel_x>-0.00009)) && ((men[i].vel_y<0.00009)&&(men[i].vel_y>-0.00009)));
				else break;
			}
			if(i==7) reset();
		}
		S.x+=S.vx;
		S.y+=S.vy;
		if(S.x < -0.400+S.r)
		{
			S.vx*=-1;
			S.x+=0.01;
		}
		else if(S.x > 0.400-S.r)
		{
			S.vx*=-1;
			S.x-=0.01;
		} 
		if(S.y < -0.400+S.r)
		{
			S.vy*=-1;
			S.y+=0.01;
		}
		else if(S.y > 0.400-S.r)
		{
		 S.vy*=-1;
		 S.y-=0.01;
		}
	}
	glutTimerFunc(10,update,0);
}
void handleKeypress ( unsigned char key, int x, int y)
{
	switch(key)
	{
		//cout<<key;
		case 27:
			exit(0);
		case 32:
			S.vx=speed*cos(S.theta);
			S.vy=speed*sin(S.theta);
			speed=0;
			hit=1;
			break;
		case 97:
				S.theta +=(0.04);
				if(S.theta > 2*PI) S.theta=0;
				break;
		case 99:
				S.theta -=(0.04);
				if(S.theta < -2*PI) S.theta=0;
				break;
		case 109:
				mob=1;
				break;
		case 107:
				mob=0;
				break;
	}
	glutPostRedisplay();
}
void splKeys(int key,int x,int y)
{
	if(!hit)
	{
		float p=S.x;
		switch(key)
		{
			case 101:
				if(speed < 0.016) speed+=0.001;
				break;
			case 103:
				if(speed > 0.000) speed-=0.001;
				break;
			case GLUT_KEY_LEFT:
				p-=0.01;
				int i;
				for( i=0;i<7;i++)
				{
					if(p<-0.25) break;
						if((!men[i].pocket) && sqrt(pow(p-men[i].x,2)+pow(S.y-men[i].y,2)) <= S.r+men[i].r)
						{
							p-=0.01;
							i=-1;
						}
				}
				if(i==7) S.x=p;
				break;
			case GLUT_KEY_RIGHT:
				p+=0.01;
				for( i=0;i<7;i++)
				{
					if(p>0.25) break;
					if((!men[i].pocket) && sqrt(pow(p-men[i].x,2)+pow(S.y-men[i].y,2)) <= S.r+men[i].r)
					{
						p+=0.01;
						i=-1;
					}
				}
				if(i==7) S.x=p;
		} 
	}
		drawPower();
	glutPostRedisplay();

}
void reset()
{
	speed=0;
	hit=0;
	flag=1;
	vis=0;
	S.theta=(PI/2);
	S.y=0.001f;
	S.r=0.03;
	S.vx=0.00;
	S.vy=0.00;
	float p=0.00;
	int i;
	for( i=0;i<7;i++)
	{
			if(p<-0.25) break;
			if((!men[i].pocket) && sqrt(pow(p-men[i].x,2)+pow(S.y-men[i].y,2)) <= S.r+men[i].r)
			{
				p-=0.01;
				i=-1;
			}
	}
	if(i!=7)
	{
		p=0.00;
		for( i=0;i<7;i++)
		{
			if(p>0.25) break;	
			if((!men[i].pocket) && sqrt(pow(p-men[i].x,2)+pow(S.y-men[i].y,2)) <= S.r+men[i].r)
			{
				p+=0.01;
				i=-1;
			}
		}
	}
	S.x=p;
}
float Cr=0,Cg=0,Cb=0;
void drawPower()
{
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(-0.6,-0.4,-1.02);
//	cout<<t1.tv_usec<<endl;
	if((t1.tv_sec%5)%2==0)
	{
	Cg=(float)(rand()%100)/100;
	Cb=(float)(rand()%100)/100;
	Cr=(float)(rand()%100)/100;
	}
	glColor3f(Cr,Cg,Cb);
	glBegin(GL_POLYGON);
	glVertex3f(0,0,0);
	glVertex3f(0.05,0,0);
	glVertex3f(0.05,0.8*speed/0.016,0);
	glVertex3f(0,0.8*speed/0.016,0);
	glEnd();
	glPopMatrix();
}
void initalise()
{
	for(int i=0;i<6;i++)
	{
		float a=(PI/3)*i;
		if(i%2) men[i]=coin(0.05*cos(a),0.05*sin(a),0.02,-5);
		else  men[i]=coin(0.05*cos(a),0.05*sin(a),0.02,10);
	}
	men[6]=coin(0,0,0.02,50);
}
void checkCollision()
{	
	for(int j=0;j<7;j++)
	{
		if((!men[j].pocket) && sqrt(pow(S.x-men[j].x,2)+pow(S.y-men[j].y,2)) <= S.r+men[j].r)
		{
			float theata =PI-atan((S.y-men[j].y)/(S.x-men[j].x));
				float 	v1x=S.vx;
				float 	v1y=S.vy;
				float	v2x=men[j].vel_x;
				float	v2y=men[j].vel_y;			
				S.vx=v1x*sin(theata)*sin(theata) + 0.5*v2x*cos(theata)*cos(theata)+cos(theata)*sin(theata)*(v1y-v2y*0.5);
				S.vy=sin(theata)*cos(theata)*(v1x-0.5*v2x)+v1y*cos(theata)*cos(theata)+0.5*v2y*sin(theata)*sin(theata);
				men[j].vel_x=v2x*sin(theata)*sin(theata)+2*v1x*cos(theata)*cos(theata)+sin(theata)*cos(theata)*(v2y-2*v1y);
				men[j].vel_y=v2y*cos(theata)*cos(theata)+2*v1y*sin(theata)*sin(theata)+sin(theata)*cos(theata)*(v2x-2*v1x);
		}
	}
	for(int i=0;i<7;i++)
	{
		if(!men[i].pocket)
		for(int j=i+1;j<7;j++)
		{
			if((!men[j].pocket)&&sqrt(pow(men[i].x-men[j].x,2)+pow(men[i].y-men[j].y,2)) <= men[i].r+men[j].r)
			{
				float theata;
				if((men[i].x-men[j].x < 0.0001) && (men[i].x-men[j].x >  -0.0001))	theata = PI/2;
				else theata =PI-atan((men[i].y-men[j].y)/(men[i].x-men[j].x));
				float v1x=men[i].vel_x;
				float v1y=men[i].vel_y;
				float v2x=men[j].vel_x;
				float v2y=men[j].vel_y;
				men[i].vel_x=v1x*sin(theata)*sin(theata) + v2x*cos(theata)*cos(theata)+ cos(theata)*sin(theata)*(v1y-v2y);
				men[i].vel_y=sin(theata)*cos(theata)*(v1x-v2x)+v1y*cos(theata)*cos(theata)+v2y*sin(theata)*sin(theata);
				men[j].vel_x=v2x*sin(theata)*sin(theata)+v1x*cos(theata)*cos(theata)+sin(theata)*cos(theata)*(v2y-v1y);
				men[j].vel_y=v2y*cos(theata)*cos(theata)+v1y*sin(theata)*sin(theata)+sin(theata)*cos(theata)*(v2x-v1x);
			}
		}
	}
}
void pocket()
{
		for(int i=0;i<7;i++)
		{
			float x=0.360;
			float y=0.360;
			if(!men[i].pocket)
			{
				
				if(sqrt(pow(men[i].x-x,2)+pow(men[i].y-y,2)) <= 0.040)
				{
					men[i].pocket=1;
					score+=men[i].value;
					continue;
				}
				x*=-1;
				if(sqrt(pow(men[i].x-x,2)+pow(men[i].y-y,2)) <= 0.040)
				{
					men[i].pocket=1;
					score+=men[i].value;
					continue;
				}
				y*=-1;
				if(sqrt(pow(men[i].x-x,2)+pow(men[i].y-y,2)) <= 0.040)
				{
					men[i].pocket=1;
					score+=men[i].value;
					continue;
				}
				x*=-1;
				if(sqrt(pow(men[i].x-x,2)+pow(men[i].y-y,2)) <= 0.040)
				{
					men[i].pocket=1;
					score+=men[i].value;
					continue;
				}
			}
			if(sqrt(pow(S.x-x,2)+pow(S.y-y,2)) <= 0.040)
			{
				vis=1;
				score-=5;
				reset();
				return;
			}
			x*=-1;
			if(sqrt(pow(S.x-x,2)+pow(S.y-y,2)) <= 0.040)
			{
				vis=1;
				score-=5;
				reset();
				return;
			}
			y*=-1;
			if(sqrt(pow(S.x-x,2)+pow(S.y-y,2)) <= 0.040)
			{
				vis=1;
				score-=5;
				reset();
				return;
			}
			x*=-1;
			if(sqrt(pow(S.x-x,2)+pow(S.y-y,2)) <= 0.040)
			{
				vis=1;
				score-=5;
				reset();
				return;
			}
		}
}
void move(int x)
{
	if(vf)
	{
		float aa=(x/ww-0.5);
		float p=aa;
	    int i;
	    if(aa<0.25&&aa>-0.25)
	    {
	    	for( i=0;i<7;i++)
	    	{
				if(p<-0.25) break;
				if((!men[i].pocket) && sqrt(pow(p-men[i].x,2)+pow(S.y-men[i].y,2)) <= S.r+men[i].r)
				{
					p-=0.01;
					i=-1;
				}
	    	}
			if(i!=7)
			{
	 			p=aa;
	 			for( i=0;i<7;i++)
				{
					if(p>0.25) break;	
					if((!men[i].pocket) && sqrt(pow(p-men[i].x,2)+pow(S.y-men[i].y,2)) <= S.r+men[i].r)
					{
						p+=0.01;
						i=-1;
					}
				}
	    	}
	    S.x=p;
		}
	}
}
GLdouble ox=0.0,oy=0.0,oz=0.0;
void Mouse(int button,int state,int x,int y) 
{
	if(button == GLUT_LEFT_BUTTON)
	{
	 	if(state==GLUT_DOWN) rf=1;
	 	else if(state==GLUT_UP)
 		{
 			S.vx=speed*cos(S.theta);
			S.vy=speed*sin(S.theta);
			speed=0;
 			hit =1;
 			rf=0;
 		}
  	}
  	if(button == GLUT_RIGHT_BUTTON)
  	{
  		if(state==GLUT_DOWN)
  		{
  	 		vf=1;
  	 	}
  	 	else vf=0;
  	}
}
void Collection()
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.5,-0.3,-1.046);
	RenderString(-0.01, 0.25f, GLUT_BITMAP_TIMES_ROMAN_24, "Collection : ", 1.0f, 1.0f, 1.0f);
	glTranslatef(0,0,-0.01);
	glColor3f(0,1.0f,0);
	glBegin(GL_POLYGON);
	glVertex3f(0,0,0);
	glVertex3f(0,0.2,0);
	glVertex3f(0.1,0.2,0);
	glVertex3f(0.1,0,0);
	glEnd();
	glColor3f(0.2,0.1,0.3);
	glBegin(GL_POLYGON);
	glVertex3f(-0.03,-0.02,-0.01);
	glVertex3f(-0.03,0.34,-0.01);
	glVertex3f(0.25,0.34,-0.01);
	glVertex3f(0.25,-0.02,-0.01);
	glEnd();
	glColor3f(0,1.0f,0);
	glBegin(GL_POLYGON);
	glVertex3f(0.12,0,0);
	glVertex3f(0.12,0.2,0);
	glVertex3f(0.22,0.2,0);
	glVertex3f(0.22,0,0);
	glEnd();
	solidcircle(0.17,0.25,0,0.04,100);
	glTranslatef(0,0,0.001);
	float xc=0.01f;
	float yc1,yc2;
	xc+=men[0].r;
	yc1=men[0].r;yc2=men[0].r;
	for(int i=0;i<7;i++)
	{
		if(men[i].pocket)
		{
			if(men[i].value==-5) {
				glColor3f(0.0f,0.0f,0.0f);	
				xc=0.01+8*men[i].r;
				solidcircle(xc,yc1,0,men[i].r,20);
				yc1+=2.2*men[0].r;
			}
			else if(men[i].value==10){
			 glColor3f(1.0f,1.0f,1.0f);
			 	xc=0.01+2*men[i].r;
			    solidcircle(xc,yc2,0,men[i].r,20);
			    yc2+=2.2*men[0].r;
			}
			else{ glColor3f(1.0f,0,0);solidcircle(0.17,0.25,0,men[i].r,20);}
		}
	}
	glPopMatrix();
}
int main(int argc, char **argv)
{
	gettimeofday(&t1, NULL);	
	reset();
	initalise();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Darshan");
	initRendering();
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(splKeys);
	glutReshapeFunc(handleResize);	
	glutMouseFunc(Mouse);
	glutMotionFunc(click);
	glutPassiveMotionFunc(motion);
	glutTimerFunc(10,update,0);
	glutMainLoop();
	return 0;
}
