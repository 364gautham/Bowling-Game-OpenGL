/*
 *  Bowling game lane in Nature
 *  Key bindings:
 *  t         Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 *  more key bindings in readme
 */
 #include "CSCIx229.h"

int th=180;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
double zh=0;      //  Rotation
int axes=1;       //  Display axes
//int mode=1;       //  What to display
int move=1;      // move light
int light=1;     // lighting
int fov=55;       //Field of view(for perspective)
double asp=1;     //aspect Ratio
double dim=500;    // Size of world

double eye_x = 1;
double eye_y = 9;
double eye_z = 38;

double Cx = 0;
double Cz = 0;
double Cy =0;

//texture
double rep=1;
int texture_mode=0;

// Light values
int one       =   1;  // Unit value
int distance  =   14;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
float ylight  =   10;  // Elevation of light

#define rgb(r,g,b) glColor3ub(r,g,b)
#define GL_NORMAL(x1,y1,z1,x2,y2,z2,x3,y3,z3) glNormal3f((y2-y1)*(z3-z1)-(y3-y1)*(z2-z1),(z2-z1)*(x3-x1)-(x2-x1)*(z3-z1),(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))
#define GL_NORMAL1(x1,y1,z1,x2,y2,z2,x3,y3,z3) glNormal3f((-(y2-y1)*(z3-z1)-(y3-y1)*(z2-z1)),(-(z2-z1)*(x3-x1)-(x2-x1)*(z3-z1)),(-(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1)))

unsigned int texture[15];

//Bowling
#define BALL_INIT 30
#define BALL_COLLIDE (-15)
double ball_path=BALL_INIT;
double ball_pathx=0;
GLUquadric *quadric = NULL;
int ball_roll=0;
float angle_pin = 0.0f;
int angle_rotate = 0;
float move_pin = 0.0f;
int change=0,rBall=0,lBall=0;

int ballCol=0;

int flag=1,flag1=1;
GLfloat angle,angle2;
int moving,startx,starty;
enum myLists{ tree=1,leaf=2,stem,conetree,riverlist,tree1};
int call_once=0,call_once1=0;

double cy1=1,cy2=1.5,first=1;
#define drand48() (((float) rand())/((float) RAND_MAX))

//draw_river
float c_y=0;

double flagPt[44][44][3];
unsigned int tex_flag = 0;
#define PI 3.1415926


void Project(double fov,double asp,double dim)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();

   //  Perspective transformation
   if (fov)
      gluPerspective(fov,asp,2,3*dim);
   //  Orthogonal transformation
   else
      glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);

   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void draw_cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double r, double g, double b,double phi,double th,double psi,
               unsigned int texture)
{

  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glRotated(phi,1,0,0);
   glRotated(psi,0,0,1);
   glScaled(dx,dy,dz);

   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture);

   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(r,g,b);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0, 0.0);   glVertex3f(-1, -1, +1);
   glTexCoord2f(rep, 0.0);   glVertex3f(+1, -1, +1);
   glTexCoord2f(rep, rep);   glVertex3f(+1, +1, +1);
   glTexCoord2f(0.0, rep);   glVertex3f(-1, +1, +1);
   //  Back
   glColor3f(r,g,b);
   glNormal3f( 0, 0, -1);
   glTexCoord2f(rep, 0.0);    glVertex3f(+1, -1, -1);
   glTexCoord2f(0.0, 0.0);    glVertex3f(-1, -1, -1);
   glTexCoord2f(0.0, rep);    glVertex3f(-1, +1, -1);
   glTexCoord2f(rep, rep);    glVertex3f(+1, +1, -1);
   //  Right
   glColor3f(r,g,b);
   glNormal3f( 1, 0, 0);
   glTexCoord2f(0.0, rep);    glVertex3f(+1, -1, +1);
   glTexCoord2f(0.0, 0.0);    glVertex3f(+1, -1, -1);
   glTexCoord2f(rep, 0.0);    glVertex3f(+1, +1, -1);
   glTexCoord2f(rep, rep);    glVertex3f(+1, +1, +1);
   //  Left
   glColor3f(r,g,b);
   glNormal3f( -1, 0, 0);
   glTexCoord2f(0.0, 0.0);    glVertex3f(-1, -1, -1);
   glTexCoord2f(0.0, rep);    glVertex3f(-1, -1, +1);
   glTexCoord2f(rep, rep);    glVertex3f(-1, +1, +1);
   glTexCoord2f(rep, 0.0);    glVertex3f(-1, +1, -1);
   //  Top
   glColor3f(r,g,b);
   glNormal3f( 0, 1, 0);
   glTexCoord2f(0,0);     glVertex3f(-1,+1,+1);
   glTexCoord2f(rep,0);   glVertex3f(+1,+1,+1);
   glTexCoord2f(rep,rep); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,rep);   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(r,g,b);
   glNormal3f( 0, -one, 0);
   glTexCoord2f(0,0);     glVertex3f(-1,-1,-1);
   glTexCoord2f(rep,0);   glVertex3f(+1,-1,-1);
   glTexCoord2f(rep,rep); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,rep);   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph){
   // glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
   // glNormal3f(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
   // glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));

   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}
/*
 *  Draw sky box
 */
static void Sky(double D)
{
  double hy=100;
  //glDisable(GL_LIGHTING);
   glPushMatrix();
    glColor3f(1,1,1);

    //  Sides
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    glBegin(GL_QUADS);

    glTexCoord2f(0.00,0); glVertex3f(-D,0,-D);
    glTexCoord2f(0.25,0); glVertex3f(+D,0,-D);
    glTexCoord2f(0.25,1); glVertex3f(+D,+hy,-D);
    glTexCoord2f(0.00,1); glVertex3f(-D,+hy,-D);

    glTexCoord2f(0.25,0); glVertex3f(+D,0,-D);
    glTexCoord2f(0.50,0); glVertex3f(+D,0,+D);
    glTexCoord2f(0.50,1); glVertex3f(+D,+hy,+D);
    glTexCoord2f(0.25,1); glVertex3f(+D,+hy,-D);

    glTexCoord2f(0.50,0); glVertex3f(+D,0,+D);
    glTexCoord2f(0.75,0); glVertex3f(-D,0,+D);
    glTexCoord2f(0.75,1); glVertex3f(-D,+hy,+D);
    glTexCoord2f(0.50,1); glVertex3f(+D,+hy,+D);

    glTexCoord2f(0.75,0); glVertex3f(-D,0,+D);
    glTexCoord2f(1.00,0); glVertex3f(-D,0,-D);
    glTexCoord2f(1.00,1); glVertex3f(-D,+hy,-D);
    glTexCoord2f(0.75,1); glVertex3f(-D,+hy,+D);
    glEnd();

    //  Top and bottom
    glBindTexture(GL_TEXTURE_2D,texture[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0,0); glVertex3f(+D,+hy,-D);
    glTexCoord2f(0.5,0); glVertex3f(+D,+hy,+D);
    glTexCoord2f(0.5,1); glVertex3f(-D,+hy,+D);
    glTexCoord2f(0.0,1); glVertex3f(-D,+hy,-D);
    ///glEnd();
    //glBegin(GL_QUADS);
    glTexCoord2f(1.0,1); glVertex3f(-D,0,+D);
    glTexCoord2f(0.5,1); glVertex3f(+D,0,+D);
    glTexCoord2f(0.5,0); glVertex3f(+D,0,-D);
    glTexCoord2f(1.0,0); glVertex3f(-D,0,-D);
    glEnd();

    glPopMatrix();
    //glEnable(GL_LIGHTING);

}
/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   glDisable(GL_TEXTURE_2D);
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialf(GL_FRONT,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}
void draw_pins(){
  quadric = gluNewQuadric();
  // glEnable(GL_TEXTURE_2D);
  // glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  // glColor3f(1,1,1);
  // gluQuadricTexture(quadric,true);
  // glBindTexture(GL_TEXTURE_2D,texture[4]);
  gluQuadricNormals(quadric,GLU_SMOOTH);

	//draw the head of the pin
  glPushMatrix();
	glTranslatef(0.0, 3.0, 0.0);
	glScalef(1.0, 2.0, 1.0);
  glColor3f(1,1,1);
	gluSphere(quadric, 2.0f, 30, 30);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(quadric, 2.0, 1.2, 2.5, 30, 30);
	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glPopMatrix();

  // //draw the  strip and curve
	glPushMatrix();
	glColor3f(1,0,0);
	glTranslatef(0.0, -0.2, 0.0);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	gluCylinder(quadric, 1.2, 1.5, 2.0, 30, 30);
	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glPopMatrix();

	//Draw the body of the pin
	glPushMatrix();
	glColor3f(1,1,1);
	glTranslatef(0.0, -2.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(quadric, 1.2, 2.8, 2.5, 30, 30);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -8.9, 0.0);
	glScalef(1.2, 2.45, 1.2);
	gluSphere(quadric, 3.5, 30, 30);
  //glDisable(GL_TEXTURE_2D);
	glPopMatrix();

  glPushMatrix();
  glColor3f(1,0,0);
  glTranslatef(0.0, -5, 0.0);
  glRotatef(90.0f, 1.0, 0.0, 0.0);
  gluCylinder(quadric, 3.4, 3.9, 2.0, 30, 30);
  //glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
  glPopMatrix();


}
void Bowling(float x, float y,float z,int pin){

  glPushMatrix();
  glScalef(.1,.1,.1);
	glColor3f(1.0f, 1.0f, 1.0f);
  glTranslatef(x,y,z);
  glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
		//glTranslatef(0.0f, move_pin, 0.0f);
		//glRotatef((GLfloat)angle_pin, -1.0f, 0.0f, 0.0f);
  if(ball_path <= -5.0f && lBall){
    if (pin == 2)
    {
      glRotatef(45.0f, 0.0f, 0.0f, 1.0f); glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, -1.0f);
      draw_pins();
    }
    else if (pin == 4)
    {
      glRotatef(45.0f, 1.0f, 0.0f, 1.0f);  glTranslatef(0.0f, move_pin, 0.0f); glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, 1.0f);
      draw_pins();
    }
    else if (pin == 7)
    {
      glRotatef(-30.0f, 0.0f, 1.0f, 1.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, -1.0f);
      draw_pins();
    }
    else if (pin == 8)
    {
      glRotatef(45.0f, 1.0f, 1.0f, 0.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, -1.0f, 0.0f, 0.0f);
      draw_pins();
    }
    else draw_pins();
  }
  if(ball_path <= -5.0f && rBall){
    if (pin == 3)
    {
      glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); glTranslatef(0.0f, move_pin, 0.0f); glRotatef((GLfloat)angle_pin, -1.0f, 0.0f, 0.0f);
      draw_pins();
    }
    else if (pin == 6)
    {
      glRotatef(30.0f, 0.0f, 1.0f, 1.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 1.0f, 0.0f, 0.0f);
      draw_pins();
    }
    else if (pin == 9)
    {
      glRotatef(-45.0f, 0.0f, 1.0f, 1.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 1.0f, 0.0f, 0.0f);
      draw_pins();
    }
    else if (pin == 10)
    {
      glRotatef(-30.0f, 1.0f, 1.0f, 0.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, 1.0f);
      draw_pins();
    }
    else draw_pins();

  }
  else if (ball_path <= BALL_COLLIDE)
   {
     if (pin == 1)
     {
       glRotatef(0.0f, 0.0f, 0.0f, 1.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, -1.0f, 0.0f, 0.0f);
       draw_pins();
     }
     else if (pin == 2)
     {
       glRotatef(45.0f, 0.0f, 0.0f, 1.0f); glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, -1.0f);
       draw_pins();
     }
     else if (pin == 3)
     {
       glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); glTranslatef(0.0f, move_pin, 0.0f); glRotatef((GLfloat)angle_pin, -1.0f, 0.0f, 0.0f);
       draw_pins();
     }
     else if (pin == 4)
     {
       glRotatef(45.0f, 1.0f, 0.0f, 1.0f);  glTranslatef(0.0f, move_pin, 0.0f); glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, 1.0f);
       draw_pins();
     }
     else if (pin == 5)
     {
       glRotatef(-45.0f, 1.0f, 0.0f, 1.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, 1.0f);
       draw_pins();
     }
     else if (pin == 6)
     {
       glRotatef(30.0f, 0.0f, 1.0f, 1.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 1.0f, 0.0f, 0.0f);
       draw_pins();
     }
     else if (pin == 7)
     {
       glRotatef(-30.0f, 0.0f, 1.0f, 1.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, -1.0f);
       draw_pins();
     }
     else if (pin == 8)
     {
       glRotatef(45.0f, 1.0f, 1.0f, 0.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, -1.0f, 0.0f, 0.0f);
       draw_pins();
     }
     else if (pin == 9)
     {
       glRotatef(-45.0f, 0.0f, 1.0f, 1.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 1.0f, 0.0f, 0.0f);
       draw_pins();
     }
     else if (pin == 10)
     {
       glRotatef(-30.0f, 1.0f, 1.0f, 0.0f);glTranslatef(0.0f, move_pin, 0.0f);glRotatef((GLfloat)angle_pin, 0.0f, 0.0f, 1.0f);
       draw_pins();
     }
  }
  else
    draw_pins();

  glPopMatrix();

}


void Bowling_ball()
{
  float ballR=1,ballG=1,ballB=1;
  float bCol[11][3]={{0.196078,0.8,0.6},{0.309804,0.309804,0.184314},{1,1,1},{0.8,0.498039,0.196078},{0,0.498039,1.0},
                      {0.8,0.196078,0.6},{0.89,0.47,0.20},{0.55,0.09,0.09},{0.94,0.81,0.99},{0.137255,0.556863,0.137255}};
  if(ballCol<10){
    ballR=bCol[ballCol][0];ballG=bCol[ballCol][1];ballB=bCol[ballCol][2];
  }
  glPushMatrix();
	quadric = gluNewQuadric();
  gluQuadricNormals(quadric,GLU_SMOOTH);
  glScaled(0.4,0.4,.4);
  glEnable (GL_COLOR_MATERIAL);
  glDisable(GL_TEXTURE_2D);
	//ball(ball_pathx, 14.0f, ball_path,2);
  glTranslatef(ball_pathx, 14.5f, ball_path);
  glColor3f(ballR,ballG,ballB);
	gluSphere(quadric, 2.0f, 30.f, 30.f);
//	ball_details();
  glPopMatrix();
}
void shelter(float x, float y, float z){
  //  Save transformation
  glPushMatrix();
  //  Offset
  glTranslated(x,y,z);
  //glRotated(th, 0, 1, 0);
  glScaled(1.5,1.5,1.5);

  // draw pillars
  draw_cube(4,2,4,0.2,5.8,0.2,1,1,1,0,0,0,texture[4]);
  draw_cube(4,2,-4,0.2,5.8,0.2,1,1,1,0,0,0,texture[4]);
  draw_cube(-4,2,4,0.2,5.8,0.2,1,1,1,0,0,0,texture[4]);
  draw_cube(-4,2,-4,0.2,5.8,0.2,1,1,1,0,0,0,texture[4]);

  draw_cube(0,8,0,4,0.3,7,0.85 , 0.53 , 0.10,0,0,0,texture[6]);

  //draw chair
  draw_cube(0,2.1,3.3,0.2,2,0.2,0.576471 , 0.858824 , 0.439216,60,0,00,texture[4]);
  draw_cube(0.7,2.1,3.3,0.2,2,0.2,1.00,0.43,0.78,60,0,00,texture[4]);
  draw_cube(1.4,2.1,3.3,0.2,2,0.2,0.576471 , 0.858824 , 0.439216,60,0,00,texture[4]);
  draw_cube(2,2.1,3.3,0.2,2,0.2,1.00,0.43,0.78,60,0,00,texture[4]);

  draw_cube(0,1,0,0.2,2,0.2,0.576471 , 0.858824 , 0.439216,90,0,0,texture[4]);
  draw_cube(0.7,1,0,0.2,2,0.2,1.00,0.43,0.78,90,0,0,texture[4]);
  draw_cube(1.4,1,0,0.2,2,0.2,0.576471 , 0.858824 , 0.439216,90,0,0,texture[4]);
  draw_cube(2,1,0,0.2,2,0.2,1.00,0.43,0.78,90,0,0,texture[4]);

  draw_cube(1,0.7,-0.8,0.2,1,0.2,0.35,0.16,0.14,0,0,90,texture[4]);
  draw_cube(1,0.7,1,0.2,1,0.2,0.35,0.16,0.14,0,0,90,texture[4]);

  draw_cube(2,0.4,1.6,0.2,0.6,0.2,0.35,0.16,0.14,0,0,0,texture[4]);
  draw_cube(0,0.4,1.6,0.2,0.6,0.2,0.35,0.16,0.14,0,0,0,texture[4]);
  draw_cube(0,0.4,-1.3,0.2,0.6,0.2,0.35,0.16,0.14,0,0,0,texture[4]);
  draw_cube(2,0.4,-1.3,0.2,0.6,0.2,0.35,0.16,0.14,0,0,0,texture[4]);



  glPopMatrix();

}

 void bridge_lane(float x,float y,float z){
  glPushMatrix();

  glTranslatef(x,y,z);

  draw_cube(0,0,0,3,0.5,15,1,1,1,0,0,0,texture[4]);
  //steps -one
  draw_cube(0,-2,15,3,2.5,1,1,1,1,0,0,0,texture[4]);
  draw_cube(0,-3,15.8,3,1.5,1,1,1,1,0,0,0,texture[4]);
  draw_cube(0,-4,16.4,3,0.5,1,1,1,1,0,0,0,texture[4]);
  // steps -two
  draw_cube(0,-2,-15,3,2.5,1,1,1,1,0,0,0,texture[4]);
  draw_cube(0,-3,-15.8,3,1.5,1,1,1,1,0,0,0,texture[4]);
  draw_cube(0,-4,-16.4,3,0.5,1,1,1,1,0,0,0,texture[4]);

  // strip lines
  draw_cube(2.5,0.5,0,0.2,0.3,15,0.85 , 0.53 , 0.10,0,0,0,texture[4]);
  draw_cube(-2.5,0.5,0,0.2,0.3,15,0.85 , 0.53 , 0.10,0,0,0,texture[4]);

  draw_cube(0,0.5,12,3,0.1,0.3,0.35 , 0.16 , 0.14,0,0,0,texture[6]);

  // back
  draw_cube(0,1,-15.2,3.1,2.5,1,1,1,1,0,0,0,texture[5]);

  //sides
  draw_cube(2.8,1,-12.2,0.3,3,3,1,1,1,0,0,0,texture[5]);
  draw_cube(-2.8,1,-12.2,0.3,3,3,1,1,1,0,0,0,texture[5]);

  glPopMatrix();
}



void draw_tetrahedron1(){
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glPushMatrix();
  //glColor3f(0.13f, 0.41f , 0.55f);
  glBegin(GL_TRIANGLES);
      GL_NORMAL(0,2,0,-2,0,2,2,0,2);
      glTexCoord2f(0.5,1);glVertex3f(0, cy1, 0);
      glTexCoord2f(0,1);glVertex3f(-2, 0, 2);
      glTexCoord2f(0,0);glVertex3f(2, 0, 2);

      GL_NORMAL(0,2,0,2,0,-2,2,0,2);
      glTexCoord2f(0.5,1);glVertex3f(0, cy1, 0);
      glTexCoord2f(0,1);glVertex3f(2, 0, -2);
      glTexCoord2f(0,0);glVertex3f(2, 0, 2);

      GL_NORMAL(0,2,0,-2,0,-2,2,0,-2);
      glTexCoord2f(0.5,1);glVertex3f(0, cy1, 0);
      glTexCoord2f(0,1);glVertex3f(-2, 0, -2);
      glTexCoord2f(0,0);glVertex3f(2, 0, -2);

      GL_NORMAL(0,2,0,-2,0,-2,-2,0,2);
      glTexCoord2f(0.5,1);glVertex3f(0, cy1, 0);
      glTexCoord2f(0,1);glVertex3f(-2, 0, -2);
      glTexCoord2f(0,0);glVertex3f(-2, 0, 2);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);


}
//1-(1.5*Sin(cy1))+Cos(cy1)

void draw_tetrahedron2(){
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glPushMatrix();
  //glColor3f(0.13f, 0.41f , 0.55f);
  glBegin(GL_TRIANGLES);
      GL_NORMAL(0,2,0,-2,0,2,2,0,2);
      glTexCoord2f(0.5,1);glVertex3f(0, (cy2), 0);
      glTexCoord2f(0,1);glVertex3f(-2, 0, 2);
      glTexCoord2f(0,0);glVertex3f(2, 0, 2);

      GL_NORMAL(0,2,0,2,0,-2,2,0,2);
      glTexCoord2f(0.5,1);glVertex3f(0, (cy2), 0);
      glTexCoord2f(0,0);glVertex3f(2, 0, -2);
      glTexCoord2f(0,1);glVertex3f(2, 0, 2);

      GL_NORMAL(0,2,0,-2,0,-2,2,0,-2);
      glTexCoord2f(0.5,1);glVertex3f(0, (cy2), 0);
      glTexCoord2f(0,1);glVertex3f(-2, 0, -2);
      glTexCoord2f(0,0);glVertex3f(2, 0, -2);

      GL_NORMAL(0,2,0,-2,0,-2,-2,0,2);
      glTexCoord2f(0.5,1);glVertex3f(0, (cy2), 0);
      glTexCoord2f(0,1);glVertex3f(-2, 0, -2);
      glTexCoord2f(0,0);glVertex3f(-2, 0, 2);


  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);

}

void draw_leaf()
{
  glPushMatrix();
        glBegin(GL_TRIANGLES);
        glColor3f(0.8 , 0.498039 , 0.196078);
        GL_NORMAL(0, 0, 0,0.25, 0.25, 0.1,0, 0.5, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0.25, 0.25, 0.1);
        glVertex3f(0, 0.5, 0);

        GL_NORMAL(0, 0, 0,0, 0.5, 0,-0.25, 0.25, 0.1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0.5, 0);
        glVertex3f(-0.25, 0.25, 0.1);
        glEnd();
  glPopMatrix();
  //glEndList();
}
void draw_leaves2(){
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glColor3f(0.6 , 0.8 , 0.196078);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
        // glBegin(GL_TRIANGLES);
        // glTexCoord2f(0,0);glVertex3f(0, 0, 0);
        // glTexCoord2f(0.5,1);glVertex3f(2,2, 0);
        // glTexCoord2f(1,0);glVertex3f(3,0,0);
        // glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.5,0.5); glVertex3f(0,0,0);
        for (int k=0;k<=360;k+=10)
        {
           glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
           glVertex2f(Cos(k),Sin(k));
        }
        glEnd();
  glDisable(GL_TEXTURE_2D);
  glPopMatrix();


}
void draw_leaves(){
  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glColor3f(0.6 , 0.8 , 0.196078);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1,0.5,1);
        glTexCoord2f(0.5,0.5);glVertex3f(0,0,0);
        for (int k=0;k<=360;k+=10){
          glTexCoord2f(0.5*Cos(k)+0.5,0.5*Sin(k)+0.5);
          glVertex2f(Cos(k),Sin(k));
        }

        glEnd();
        glDisable(GL_TEXTURE_2D);
  glPopMatrix();

}
void cdraw_cylinder(float flx,float fly,float flz,float height,float base)
{
    GLUquadricObj *qobj;
    qobj = gluNewQuadric();
    gluQuadricNormals(qobj,GLU_SMOOTH);
    glColor3f(0.64f, 0.16f, 0.16f);
    glPushMatrix();
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    glTranslatef(flx,fly,flz);
    gluCylinder(qobj, base, base - (0.22 * base), height, 20, 20);
    glPopMatrix();
}
void makecylinder(float height,float base)
{
  //  Enable textures
  // glEnable(GL_TEXTURE_2D);
  // glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  // glColor3f(1,1,1);
  // glBindTexture(GL_TEXTURE_2D,texture[12]);
    GLUquadricObj *qobj;
    qobj = gluNewQuadric();
    glPushMatrix();
    //glColor3f(0.64f, 0.16f, 0.16f);
    gluQuadricNormals(qobj,GLU_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor3f(1,1,1);
    gluQuadricTexture(qobj,true);
    glBindTexture(GL_TEXTURE_2D,texture[12]);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    gluCylinder(qobj, base, base - (0.22 * base), height, 20, 20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
//for conetrees
void makecylinder1(float height,float base)
{
    GLUquadricObj *qobj;
    qobj = gluNewQuadric();
    //glColor3f(0.64f, 0.16f, 0.16f);
    glPushMatrix();
    gluQuadricNormals(qobj,GLU_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor3f(1,1,1);
    gluQuadricTexture(qobj,true);
    glBindTexture(GL_TEXTURE_2D,texture[12]);
    glRotatef(-90, 1.0f, 0.0f, 0.0f);
    gluCylinder(qobj, base, base - (0.22 * base), height, 20, 20);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
/* Understood and Implemented using help from -  https://www.cs.put.poznan.pl/kbonenberg/portfolio/work5/*/
void maketree(float height,float base)
{
    long savedseed;
    //srand(0);
    glPushMatrix();
    float angle;
    makecylinder(height, base);//draw_leaf();
    glTranslatef(0.0f, height,0.0f);
    height -=height*0.2f;
    base -=base*0.3f;

  //  for(int a=0; a<3; a++)
    //{
       angle = 20+((rand()%50));
       if(angle >48)
            {angle = -20+((rand()%50));}
       if (height > 1)
       {
          //srand48(savedseed);
          draw_leaf();
          savedseed = (long) drand48()*ULONG_MAX;
          glPushMatrix();
          if (flag){
            //glRotatef(angle, 1.0f, 0.0f, 1.0f);
            glRotatef(110 + drand48()*40, 0, 1, 0);
            glRotatef(30 + drand48()*20, 0, 0, 1);
          }
          else
          {
            //glRotatef(angle, 0.0f, 1.0f, 1.0f);
            glRotatef(110 + drand48()*40, 0, 1, 0);
            glRotatef(30 + drand48()*20, 0, 0, 1);
          }

          flag = !flag;
          maketree(height, base); //recursive call
          glPopMatrix();

          srand48(savedseed);
          savedseed = (long) drand48()*ULONG_MAX;
          glPushMatrix();
          if (flag){
            //glRotatef(angle, 1.0f, 0.0f, 1.0f);
            glRotatef(-130 + drand48()*40, 0, 1, 0);
            glRotatef(30 + drand48()*20, 0, 0, 1);
          }
          else
          {
            //glRotatef(angle, 0.0f, 1.0f, 1.0f);
            glRotatef(-130 + drand48()*40, 0, 1, 0);
            glRotatef(30 + drand48()*20, 0, 0, 1);
          }

          flag = !flag;
          maketree(height, base); //recursive call
          glPopMatrix();
      }
      else
      {
          glColor3f(0.0f, 1.0f , 0.0f);
          //draw_leaves2();
          //glutSolidSphere(0.3f, 10, 10);
          draw_leaves2();glTranslatef(0,0.33,0);glRotated(30,0,1,0);
          draw_leaves2();glTranslatef(0,0.33,0);glRotated(40,0,1,0);
          draw_leaves2();glTranslatef(0,0.33,0);glRotated(60,0,1,0);
          draw_leaves2();glTranslatef(0,0.33,0);glRotated(80,0,1,0);
          draw_leaves2();
          //glCallList(leaf);
          //break;
      }
    //}
   glPopMatrix();

 }
 void maketree1(float height,float base)
 {
     long savedseed;
     //srand(0);
     glPushMatrix();
     float angle;
     makecylinder(height, base);//draw_leaf();
     glTranslatef(0.0f, height,0.0f);
     height -=height*0.2f;
     base -=base*0.3f;

   //  for(int a=0; a<3; a++)
     //{
        angle = 20+((rand()%50));
        if(angle >48)
             {angle = -20+((rand()%50));}
        if (height > 1)
        {
           //srand48(savedseed);
           draw_leaf();
           savedseed = (long) drand48()*ULONG_MAX;
           glPushMatrix();
           if (flag1){
             //glRotatef(angle, 1.0f, 0.0f, 1.0f);
             glRotatef(110 + drand48()*40, 0, 1, 0);
             glRotatef(30 + drand48()*20, 0, 0, 1);
           }
           else
           {
             //glRotatef(angle, 0.0f, 1.0f, 1.0f);
             glRotatef(110 + drand48()*40, 0, 1, 0);
             glRotatef(30 + drand48()*20, 0, 0, 1);
           }

           flag1 = !flag1;
           maketree1(height, base); //recursive call
           glPopMatrix();

           srand48(savedseed);
           savedseed = (long) drand48()*ULONG_MAX;
           glPushMatrix();
           if (flag1){
             //glRotatef(angle, 1.0f, 0.0f, 1.0f);
             glRotatef(-130 + drand48()*40, 0, 1, 0);
             glRotatef(30 + drand48()*20, 0, 0, 1);
           }
           else
           {
             //glRotatef(angle, 0.0f, 1.0f, 1.0f);
             glRotatef(-130 + drand48()*40, 0, 1, 0);
             glRotatef(30 + drand48()*20, 0, 0, 1);
           }

           flag1 = !flag1;
           maketree1(height, base); //recursive call
           glPopMatrix();
       }
       else
       {
           glColor3f(1.0f, 1.0f , 1.0f);
           //draw_leaves2();
           //glutSolidSphere(0.3f, 10, 10);

           draw_leaves();glTranslatef(0,0.33,0);glRotated(30,0,1,0);
           draw_leaves();glTranslatef(0,0.33,0);glRotated(40,0,1,0);
           draw_leaves();glTranslatef(0,0.33,0);glRotated(60,0,1,0);
           draw_leaves();glTranslatef(0,0.33,0);glRotated(80,0,1,0);
           draw_leaves();
           //glCallList(leaf);
           //break;
       }
     //}
    glPopMatrix();

  }
 void createTree(){
   call_once=1;
   //index1 = glGenLists (1);
   glNewList(tree, GL_COMPILE);
   glPushMatrix();
   glScalef(0.5,0.5,0.5);
   maketree(4.0,0.4);
   glPopMatrix();
   glEndList();
 }
 void createTree1(){
   call_once1=1;
   //index1 = glGenLists (1);
   glNewList(tree1, GL_COMPILE);
   glPushMatrix();
   glScalef(0.5,0.5,0.5);
   maketree1(4.0,0.4);
   glPopMatrix();
   glEndList();
 }

void place_trees(float x, float y, float z, float rot)
{
   glPushMatrix();
   glTranslatef(x,y,z);
   glRotatef(rot,0,1,0);
   //glScalef(x1,y1,z1);
   glCallList(tree);
   glPopMatrix();
}
void place_trees1(float x, float y, float z, float rot)
{
   glPushMatrix();
   glTranslatef(x,y,z);
   glRotatef(rot,0,1,0);
   //glScalef(x1,y1,z1);
   glCallList(tree1);
   glPopMatrix();
}

 void cone(float base,float height)
{
  GLUquadricObj *cylquad = gluNewQuadric();
  glPushMatrix();
  //glTranslatef(x,y,z);
  gluQuadricNormals(cylquad,GLU_SMOOTH);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glColor3f(1,1,1);
  gluQuadricTexture(cylquad,true);
  glBindTexture(GL_TEXTURE_2D,texture[3]);
  glRotatef(-90,1,0,0);
  //glColor3f(0,1,0);
  gluCylinder(cylquad, base, 0, height, 20, 20 );
  glPopMatrix();
}
 void tree2(float x, float y, float z)
{
  glPushMatrix();
  glNewList(conetree, GL_COMPILE);
  glTranslatef(x,y,z);
  //glScalef(x1,y1,z1);
  makecylinder1(1.5, 0.2);
  glTranslatef(0,0.8,0);
  cone(1,3);
  glTranslatef(0,0.9,0);
  cone(1,3);
  glTranslatef(0,1,0);
  cone(1,2);
  glEndList();
  glPopMatrix();
}

void draw_river_line(int alt){
  glPushMatrix();
  //glTranslatef(0,0,0);draw_tetrahedron1();

  if(alt){

    glTranslatef(2,0,0);draw_tetrahedron2();
    glTranslatef(-2,0,0);draw_tetrahedron2();
    glTranslatef(4,0,0);draw_tetrahedron1();
    glTranslatef(-4,0,0);draw_tetrahedron1();
    glTranslatef(6,0,0);draw_tetrahedron2();
    glTranslatef(-6,0,0);draw_tetrahedron2();
    glTranslatef(8,0,0);draw_tetrahedron1();
    glTranslatef(-8,0,0);draw_tetrahedron1();
    glTranslatef(10,0,0);draw_tetrahedron1();
    glTranslatef(-10,0,0);draw_tetrahedron1();
  }
  else{
    glTranslatef(2,0,0);draw_tetrahedron1();
    glTranslatef(-2,0,0);draw_tetrahedron1();
    glTranslatef(4,0,0);draw_tetrahedron2();
    glTranslatef(-4,0,0);draw_tetrahedron2();
    glTranslatef(6,0,0);draw_tetrahedron1();
    glTranslatef(-6,0,0);draw_tetrahedron2();
    glTranslatef(8,0,0);draw_tetrahedron2();
    glTranslatef(-8,0,0);draw_tetrahedron2();
    glTranslatef(10,0,0);draw_tetrahedron2();
    glTranslatef(-10,0,0);draw_tetrahedron2();
  }

  glPopMatrix();
}
void draw_river(){
  glPushMatrix();
  glTranslatef(0,0,0);draw_river_line(1);
  glTranslatef(0,0,2.5);draw_river_line(0);
  glTranslatef(0,0,3.2);draw_river_line(1);
  glTranslatef(0,0,3.8);draw_river_line(0);
  glPopMatrix();
}

void river(float x,float y,float z){
  glPushMatrix();
  glTranslatef(x,y,z);
  //side walls
  draw_cube(0,-0.8,-2,6,1.7,1,1,1,1,0,0,0,texture[8]);
  draw_cube(0,-0.8,11,6,1.7,1,1,1,1,0,0,0,texture[8]);
  draw_river();
  glPopMatrix();
}

/* Adapted using help from https://www.youtube.com/watch?v=X53C13lm49s*/

static void init_flag(void)
{
    for (int x=0; x<44; x++) {
        for (int y=0; y<44; y++) {
			flagPt[x][y][0] = (double) ((x/3.0)-2);
			flagPt[x][y][1] = (double) ((y/5.0)-3);
			flagPt[x][y][2] = (double) (sin((((x*45)/8.0)/360.0)*2.0*PI));
		}
	}
}

void draw_flag()
{
  unsigned int x, y;
  double hold;
  glPushMatrix();

  glScaled(0.3,0.3,0.3);


  cdraw_cylinder(-1.8,0,-2,15,0.8);
	glColor3f(1, 1, 1);
  glTranslatef(0,7,0);
  glBindTexture(GL_TEXTURE_2D, texture[10]);
  glShadeModel(GL_SMOOTH);

  for (x=0; x<43; x++)
  {
      for (y=0; y<43; y++)
      {
          glBegin(GL_QUADS);
          GL_NORMAL(flagPt[x][y][0], flagPt[x][y][1], flagPt[x][y][2], flagPt[x+1][y][0], flagPt[x+1][y][1], flagPt[x+1][y][2], flagPt[x][y+1][0], flagPt[x][y+1][1], flagPt[x][y+1][2]);

          // GL_NORMAL(flagPt[x][y][0], flagPt[x][y][1], flagPt[x][y][2],
          //           flagPt[x][y+1][0], flagPt[x][y+1][1], flagPt[x][y+1][2],
          //           flagPt[x+1][y+1][0], flagPt[x+1][y+1][1], flagPt[x+1][y+1][2]);
          glTexCoord2f(x/43.0, y/43.0);
          glVertex3f(flagPt[x][y][0], flagPt[x][y][1], flagPt[x][y][2]);

          //glNormal3f( 0, 0, -2);
          glTexCoord2f(x/43.0, (y+1)/43.0);
          glVertex3f(flagPt[x][y+1][0], flagPt[x][y+1][1], flagPt[x][y+1][2]);

          glTexCoord2f((x+1)/43.0, (y+1)/43.0);
          glVertex3f(flagPt[x+1][y+1][0], flagPt[x+1][y+1][1], flagPt[x+1][y+1][2]);

          glTexCoord2f((x+1)/43.0, y/43.0);
          glVertex3f(flagPt[x+1][y][0], flagPt[x+1][y][1], flagPt[x+1][y][2]);
          glEnd();
      }
  }

  glPopMatrix();

	for (y=0; y<44; y++)
  {
		hold = flagPt[0][y][2];

		for (x=0; x<43; x++)
			flagPt[x][y][2] = flagPt[x+1][y][2];

		flagPt[43][y][2] = hold;
	}
}
void call_flag(float x,float y,float z){
  glPushMatrix();
  glTranslatef(x,y,z);
  draw_flag();
  glPopMatrix();
}

void draw_grass(){
   glPushMatrix();

  glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE);
   //glColor4f(1,1,1,1);

   glEnable(GL_TEXTURE_2D);
   //glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glBindTexture(GL_TEXTURE_2D,texture[9]);

   glBegin(GL_QUADS);
   //  Front
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0.0, 0.0);   glVertex3f(-1, -1, +1);
   glTexCoord2f(rep, 0.0);   glVertex3f(+1, -1, +1);
   glTexCoord2f(rep, rep);   glVertex3f(+1, +1, +1);
   glTexCoord2f(0.0, rep);   glVertex3f(-1, +1, +1);

   glEnd();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glPopMatrix();
 }

void draw_conetrees(float x,float y,float z){

  glPushMatrix();
  glTranslatef(x,y,z);
  glScalef(1.5,2.4,1);
  glCallList(conetree);
  glPopMatrix();
}
void d_rocks(){
  GLUquadric *quadric = NULL;
  quadric = gluNewQuadric();
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glColor3f(1,1,1);
  gluQuadricTexture(quadric,1);
  glBindTexture(GL_TEXTURE_2D,texture[13]);
  gluQuadricNormals(quadric,GLU_SMOOTH);

  glPushMatrix();
  glColor3f(0.36 , 0.25 , 0.20);

  glTranslatef(0.5, 0.0, 0.0);
	gluSphere(quadric, 1, 30, 30);

  glTranslatef(-1, 0.0,0.7);
  gluSphere(quadric, 0.6f, 30, 30);

  glTranslatef(2.5, 0.0,-0.4);
  gluSphere(quadric, 1.2f, 30, 30);

  glTranslatef(-3, 0.0,0.4);
  gluSphere(quadric, 1, 30, 30);


	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glPopMatrix();

}
void draw_rocks(float x,float y,float z,float rot ){
  glPushMatrix();
  glTranslatef(x,y,z);
  glRotatef(rot,1,0,0);
  //glScalef();
  d_rocks();
  glPopMatrix();
}
 /*
  *  OpenGL (GLUT) calls this routine to display the scene
  */
void display()
{
   const double len=5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);

   //  Set view angle
     Cx = -2*Sin(th)*Cos(ph);
     Cy = +2*Sin(ph);
     Cz = +2*Cos(th)*Cos(ph);
     gluLookAt(eye_x, eye_y, eye_z, eye_x+Cx, eye_y+Cy, eye_z+Cz, 0, 1, 0);
   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

  //render skybox
   Sky(500);call_flag(-2,8,-15.5);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   // GLfloat fogColour[4]={0.196078,0.6,0.8,1};
   // glFogi(GL_FOG_MODE,GL_LINEAR);
   // glFogfv(GL_FOG_COLOR,fogColour);
   // glFogf(GL_FOG_DENSITY,0.02);
   // glFogf(GL_FOG_START,50);
   // glFogf(GL_FOG_END,300);
   // glHint (GL_FOG_HINT, GL_NICEST);
   // glEnable(GL_FOG);

    //draw_grass();

    //Bowling game
    Bowling_ball();

    shelter(20,0,30);
    shelter(-20,0,30);


    bridge_lane(0,4.5,0);

   	Bowling(0,69,-70,1);
    Bowling(4,69,-82,2);Bowling(-4,69,-82,3);
    Bowling(8,69,-90,4);Bowling(0,69,-90,3);Bowling(-8,69,-90,6);
    Bowling(12,69,-98,7);Bowling(4,69,-98,8);Bowling(-4,69,-98,9);Bowling(-12,69,-98,10);

    river(0,3,0);river(12,3,0);river(-12,3,0);river(21,3,0);river(-21,3,0);river(30,3,0);river(-30,3,0);river(39,3,0);river(-39,3,0);
    river(-48,3,0);river(48,3,0);river(-57,3,0);river(57,3,0);river(-66,3,0);river(66,3,0);river(-75,3,0);river(75,3,0);river(-84,3,0);
    river(84,3,0);river(93,3,0);river(-93,3,0);river(102,3,0);river(-102,3,0);river(111,3,0);river(-111,3,0);river(120,3,0);river(-120,3,0);
    river(-129,3,0);river(129,3,0);river(-138,3,0);river(138,3,0);river(-147,3,0);river(147,3,0);river(-156,3,0);river(156,3,0);river(-167,3,0);

    //fractal  trees
    if(!call_once)
      createTree();
    if(!call_once1)
        createTree1();

    draw_conetrees(40,1,-20); draw_conetrees(27,1,-40); draw_conetrees(23,1,-55); draw_conetrees(10,1,-65);
    place_trees(38,1,-30,90);  place_trees1(30,1,-36,0);   place_trees(22,1,-42,0);   place_trees1(14,1,-48,90);
    draw_rocks(36,0,-27,180);draw_rocks(37,0,-26,90); draw_rocks(26,0,-33,180);draw_rocks(27,0,-32,90);draw_rocks(15,0,-40,180);draw_rocks(16,0,-39,90);

    draw_conetrees(-40,1,-20); draw_conetrees(-27,1,-40); draw_conetrees(-23,1,-55); draw_conetrees(-10,1,-65);
    place_trees(-38,1,-30,90);  place_trees1(-30,1,-36,0);   place_trees(-22,1,-42,0);   place_trees1(-14,1,-48,90);
    draw_rocks(-36,0,-27,180);draw_rocks(-37,0,-26,90); draw_rocks(-26,0,-33,180);draw_rocks(-27,0,-32,90);draw_rocks(-15,0,-40,180);draw_rocks(-16,0,-39,90);

    draw_conetrees(40,1,20); draw_conetrees(27,1,40); draw_conetrees(23,1,55); draw_conetrees(10,1,65);
    place_trees(38,1,30,90);  place_trees1(30,1,36,0);   place_trees(22,1,42,0);   place_trees1(14,1,48,90);
    draw_rocks(36,0,27,180);draw_rocks(37,0,26,90); draw_rocks(26,0,33,180);draw_rocks(27,0,32,90);draw_rocks(15,0,40,180);draw_rocks(16,0,39,90);

    draw_conetrees(-40,1,20); draw_conetrees(-27,1,40); draw_conetrees(-23,1,55); draw_conetrees(-10,1,65);
    place_trees(-38,1,30,90);  place_trees1(-30,1,36,0);   place_trees(-22,1,42,0);   place_trees1(-14,1,48,90);
    draw_rocks(-36,0,27,180);draw_rocks(-37,0,26,90); draw_rocks(-26,0,33,180);draw_rocks(-27,0,32,90);draw_rocks(-15,0,40,180);draw_rocks(-16,0,39,90);

   glDisable(GL_TEXTURE_2D);

   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s and %s",
     th,ph,dim,fov,"Perpective",light?"On":"Off",move?"move":"stop");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f Ex= %lf  Ey= %lf Ez= %lf",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight,eye_x,eye_y,eye_z);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
   }
   //  Render the scene
   ErrCheck("display");
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

void timer(){
  srand(time(NULL));
  cy1=(rand()%5);cy2=(rand()%5);
  if(first){
    cy1 =cy1/10; cy1=0.3*cy1;cy1=1+cy1;
    cy2=0.5;
    first=0;
  }
  else {
    cy1=0.3;
    cy2 =cy2/10; cy2=0.2*cy2;cy2=1+cy2;
    first=1;
  }

  glutTimerFunc(300, timer, 0);
  glutPostRedisplay();
}
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if(move){
     zh = fmod(90*t,360);
     c_y=Cos(zh);
   }

   // bowling
   if((ball_path<(BALL_INIT+1))&& ball_roll)
      ball_path = ball_path - 0.5f;
   if(ball_path<-33&& ball_roll){
     ball_path=BALL_INIT;ball_roll=0;move_pin=0;
     ball_pathx=0;rBall=lBall=0;
   }
      //code
   angle_pin = angle_pin - 0.5f;
   if (angle_pin <= -360.0f)
    angle_pin  = 0.0f;

  angle_rotate = (angle_rotate + 6) % 360;
  move_pin = move_pin + 0.005f;



   glutPostRedisplay();
}
/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
      //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
  //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   // else if (key == GLUT_KEY_F3)
   //    distance = (distance==1) ? 10 : 3;
  //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
     inc = (inc==10)?3:10;
  //  Flip sign
   else if (key == GLUT_KEY_F9)
     one = -one;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
     //  Exit on ESC
     if (ch == 27)
        exit(0);
     //  Reset view angle
     else if (ch == '0')
    {
      th = 180;ph = 0;
      eye_x = 1; eye_y = 9;eye_z = 38;
    }
     //  Toggle axes
     else if (ch == 'x' || ch == 'X')
        axes = 1-axes;
     //  Toggle lighting
     else if (ch == 'l' || ch == 'L')
        light = 1-light;
     //  Toggle light movement
     else if (ch == 'm' || ch == 'M')
        move = 1-move;
     //  Move light
     else if (ch == '<')
        zh += 1;
     else if (ch == '>')
        zh -= 1;
    //  Change light distance
     else if (ch == ','&& distance>1)
       distance -= 1;
     else if (ch == '.')
       distance += 1;
     //  Change field of view angle
     else if (ch == '-' && ch>1)
        fov--;
     else if (ch == '+' && ch<179)
        fov++;
     //  Light elevation
     else if (ch=='[')
        ylight -= 0.1;
     else if (ch==']')
        ylight += 0.1;
     //  Ambient level
     else if (ch=='a' && ambient>0)
        ambient -= 5;
     else if (ch=='A' && ambient<100)
        ambient += 5;
     //  Diffuse level
     else if (ch=='d' && diffuse>0)
        diffuse -= 5;
     else if (ch=='D' && diffuse<100)
        diffuse += 5;
     //  Specular level
     else if (ch=='k' && specular>0)
        specular -= 5;
     else if (ch=='K' && specular<100)
        specular += 5;
     //  Emission level
     else if (ch=='e' && emission>0)
        emission -= 5;
     else if (ch=='E' && emission<100)
        emission += 5;
     //  Shininess level
     else if (ch=='n' && shininess>-1)
        shininess -= 1;
     else if (ch=='N' && shininess<7)
        shininess += 1;

     // navigation
    else if((ch=='q'||ch=='Q')&&eye_y<40) eye_y+=0.3;
    else if((ch=='Z'||ch=='z')&&eye_y>5) eye_y-=0.3;
    else if( ch=='w'||ch=='W' ){
      if(eye_x<=100)
        eye_x+=Cx*0.4;
      if(eye_z<=100)
        eye_z+=Cz*0.4;
    }
    else if( (ch=='s'|| ch=='S') && (fabs(eye_z)<=100 ) ){
      if(eye_x>=-100)
        eye_x-=Cx*0.4;
      if(eye_z>=-100)
        eye_z-=Cz*0.4;
    }


    //Bowling
    else if(ch==32)
      ball_roll=1;
    else if(ch=='3'){
      ball_pathx=3;lBall=1;
    }
    else if(ch=='2'){
      ball_pathx=0;lBall=1;
    }
    else if(ch=='1'){
      ball_pathx=-3;rBall=1;
    }
    else if(ch=='c'  || ch=='C')
      ballCol++;

    if(ballCol==10)ballCol=0;

    if(eye_z>100 )eye_z=100;
    if(eye_z<-100 )eye_z=-100;
    if(eye_x>100 )eye_x=100;
    if(eye_x<-100 )eye_x=-100;

     //  Translate shininess power to value (-1 => 0)
     shiny = shininess<0 ? 0 : pow(2.0,shininess);
    //  Reproject
    Project(fov,asp,dim);
    //  Animate if requested
    glutIdleFunc( ( move || ball_roll )? idle: NULL);
    //glutIdleFunc(ball_roll?idle:NULL);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);

   //  Set projection
   Project(fov,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Bowling Lane - Resort");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);


   //  Load textures .
   texture[0] = LoadTexBMP("textures/sky.bmp",false);
   texture[1] = LoadTexBMP("textures/water.bmp",false);
   texture[2] = LoadTexBMP("textures/sky1.bmp",false);
   texture[3] = LoadTexBMP("textures/ctree.bmp",false);
   texture[4] = LoadTexBMP("textures/wood.bmp",false);
   texture[5] = LoadTexBMP("textures/wood2.bmp",false);
   texture[6] = LoadTexBMP("textures/line.bmp",false);
   texture[7] = LoadTexBMP("textures/leaf.bmp",false);
   texture[8] = LoadTexBMP("textures/brick.bmp",false);
   //texture[9]=LoadTexBMP("textures/grass11.bmp",true);
   texture[10]=LoadTexBMP("textures/cu.bmp",false);
   texture[11]=LoadTexBMP("textures/red.bmp",false);
   texture[12] = LoadTexBMP("textures/tree_bark.bmp",false);
   texture[13] = LoadTexBMP("textures/rock.bmp",false);

   draw_leaf();tree2(0,0,0);
   timer();init_flag();
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
