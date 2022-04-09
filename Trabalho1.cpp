#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "GL/glut.h" 

#define MAXVERTEXS 30  // numero maximo de v�rtices do poligono (espa�o disponivel)
#define PI 3.141572
#define ZERO 0.00001;

GLenum doubleBuffer;   
 
struct polygon 
{
    float v[3];
} pvertex[MAXVERTEXS];
 
// pontos poligonos
int nVertices = 0;
int jaPoligono = 0;
//-----------------

int windW, windH;    // comprimento da janela: Width  e Height

int gOpera = 0;      // tipo de operacao: O: gera poligono; 1: transalacao; 2:...
int tipoPoligono;    // poligo por vertices ou por arestas
int gIndVert = -1;   // numero de v�rtices do poligono definido
int gNumVert = 0; 
float gCen[3];       // centroido do poligono
float gAng = 0.0f;    // angulo para rotacao 
float gMatriz[3][3];


int clipVertex(int x, int y)
{
	int i;
	float d;
	gIndVert=-1;
	for (i=0; i<gNumVert; i++) {
		d = sqrt(pow((pvertex[i].v[0]-x), 2.0) + pow((pvertex[i].v[1]-y), 2.0));
		if(d < 3.0){
			gIndVert = i;
			break;
		}
	}
	return gIndVert;
}

void init(void)
{
	int i;
	tipoPoligono = GL_POINTS;
	jaPoligono = 0;
	nVertices=0;  
	gNumVert = 0; 

	for(i=0; i<MAXVERTEXS; i++)
	{
		pvertex[i].v[0] = 0.0f;
		pvertex[i].v[1] = 0.0f;
		pvertex[i].v[2] = 1.0f;
	}

	gAng = (2.0f * PI) / 180.0f;
}

static void Reshape(int width, int height)
{
    windW = width/2;
    windH = height/2;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-windW, windW, -windH, windH);

    glMatrixMode(GL_MODELVIEW);
}

static void Key(unsigned char key, int x, int y)
{
    switch (key) 
	{
      case 27:
			exit(0);
    }
}

void coord_line(void)
{
    glLineWidth(1);

	// vertical line
	glBegin(GL_LINE_STRIP);
		glColor3f(1.0, 0.0, 0.0);
		glVertex2f(-windW, 0);
		glVertex2f(windW, 0);
    glEnd();

	// horizontal line 

    glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 0.0, 1.0);
		glVertex2f(0, -windH);
		glVertex2f(0, windH);
    glEnd();
}

void PolygonDraw(void)
{
	int i;

	glColor3f(0.0, 0.0, 0.0); 

	glPolygonMode(GL_FRONT_AND_BACK, tipoPoligono);

	glBegin(tipoPoligono);
	for(i=0; i<gNumVert; i++)
	{
		glVertex2fv(pvertex[i].v);
	}
	glEnd();
}

static void Draw(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

	coord_line();

	PolygonDraw();

	if(gIndVert>-1) {
		glColor3f(1.0, 0.0, 0.0);
		glPointSize(3);
		glBegin(GL_POINTS); 
			glVertex2fv(pvertex[gIndVert].v);
		glEnd();
	}

    if (doubleBuffer) 
	{
	   glutSwapBuffers(); 
    } else {
	   glFlush();      
    } 
}

static void Args(int argc, char **argv)
{
    GLint i;

    doubleBuffer = GL_FALSE;

    for (i = 1; i < argc; i++) 
	{
	   if (strcmp(argv[i], "-sb") == 0)   
	   {
	      doubleBuffer = GL_FALSE;
	   } else if (strcmp(argv[i], "-db") == 0) 
	   {
	      doubleBuffer = GL_TRUE;
	   }
    }
}


void procegVertMenuEvents(int option) 
{
	switch (option) 
	{
		case 1 : 
			{
				tipoPoligono = GL_LINE_LOOP;
				jaPoligono = 1;
				break;
			}
		case 2 : 
			init();
			break;
	}
	glutPostRedisplay();
}


void subMenuEvents(int option) 
{
	gOpera = option;
	glutPostRedisplay();
}  
 
void createGLUTMenus() 
{
	int menu, submenu;

	submenu = glutCreateMenu(subMenuEvents);
		glutAddMenuEntry("Translation", 1);
		glutAddMenuEntry("Rotation", 2);	
		glutAddMenuEntry("Scale", 3);	
		glutAddMenuEntry("Shear", 4);
		glutAddMenuEntry("Reflection", 5);

	menu = glutCreateMenu(procegVertMenuEvents);
		glutAddMenuEntry("Limpar", 2);
		glutAddMenuEntry("Gerar poligno", 1);

	glutAddSubMenu("Transformation",submenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);		
	
}

void matrizIdentidade(void) {
	gMatriz[0][0] = 1.0f;   gMatriz[0][1] = 0.0f;    gMatriz[0][2] = 0.0f;
	gMatriz[1][0] = 0.0f;   gMatriz[1][1] = 1.0f;    gMatriz[1][2] = 0.0f;
	gMatriz[2][0] = 0.0f;   gMatriz[2][1] = 0.0f;    gMatriz[2][2] = 1.0f;
}

void calCentro(float cc[])
{
     int i;
     cc[0] = cc[1] = cc[2] = 0.0f;    
     for (i=0; i<gNumVert; i++) {
		cc[0] += pvertex[i].v[0];
		cc[1] += pvertex[i].v[1];
		cc[2] += pvertex[i].v[2];
	 }
     cc[0] /= gNumVert;     
     cc[1] /= gNumVert;     
     cc[2] /= gNumVert;   
}



void operaTransforma(int position) {
	float temp[3];
	int i, j;

	printf("\n-------------------------------\n");
	for(i=0; i<3; i++) {
		temp[i] = 0.0f;
		printf("TEMP[%d]=%f ", i, temp[i]);
		for(j=0; j<3; j++){
			temp[i] = temp[i] + gMatriz[i][j] * pvertex[position].v[j];
	 		printf("\ntemp[%d] = temp[%d] + gMatriz[%d][%d] * v[%d]", i, i, i, j, j);
	 		printf("\n%f = %f + %f * %f", temp[i],temp[i], gMatriz[i][j], pvertex[position].v[j]);
	 		printf("\n\n");
		}
	}

	for( i = 0; i < 3; i++){
		pvertex[position].v[i] = temp[i];
	}	
	printf("\n-------------------------------\n");
}


void translate(float dx, float dy)
{	
	matrizIdentidade();  
     
    gMatriz[0][2] = dx;
    gMatriz[1][2] = dy;
    
	for (int i = 0; i < gNumVert; i++) {
		operaTransforma(i);
	}
}

void rotate(float dx, float dy)
{
    int i; 
    float oo, teta, vc[3]; 
 
    calCentro(vc);            
    translate(-1*vc[0], -1*vc[1]);    
    
    oo = pvertex[gIndVert].v[1] * dx - pvertex[gIndVert].v[0] * dy;
    teta = gAng;                
    if(oo > 0.0f) {
        teta = -1.0f * gAng; 		 
	}

 	matrizIdentidade();  
 	
	gMatriz[0][0] = cos(teta);    
	gMatriz[0][1] = -sin(teta);
 	gMatriz[1][0] = sin(teta);
	gMatriz[1][1] = cos(teta);
 	 
    for (i=0; i<gNumVert; i++){
			operaTransforma(i);
	}
 	 
    translate(vc[0], vc[1]);  
}

void scale(float dx, float dy){

	int i; 
    float vc[3], Sx, Sy; 
    
    calCentro(vc);   
    translate(-1*vc[0], -1*vc[1]);    
     

    Sx = Sy = 1.0f;
    if(fabs(pvertex[gIndVert].v[0]) > 0.01f){
		Sx = 1.0f + dx / pvertex[gIndVert].v[0];
	}
		
    if(fabs(pvertex[gIndVert].v[1]) > 0.01f){
		Sy = 1.0f + dy / pvertex[gIndVert].v[1];
	}
 
 	matrizIdentidade();  
 	gMatriz[0][0] = Sx;
 	gMatriz[1][1] = Sy;
 	 
	for (i=0; i<gNumVert; i++){
		operaTransforma(i);
	}
 	 
    translate(vc[0], vc[1]); 
}


void shear(float dx, float dy)
{
    int i;
    float Sx, Sy, vc[3];
     

	calCentro(vc);           
    translate(-1*vc[0], -1*vc[1]);

    Sx = 0.001f*dx;
    Sy = 0.001f*dy;
    if(dx > dy){
        if(fabs(pvertex[gIndVert].v[0])>0.1f)
            Sx = 1.0f + dx / pvertex[gIndVert].v[0];

    } else {
        if(fabs(pvertex[gIndVert].v[1])>0.1f)
 	       Sy = 1.0f +  dy / pvertex[gIndVert].v[1]; 
    }

	matrizIdentidade();
	gMatriz[0][1] = Sx; 
	gMatriz[1][0] = Sy;      
   
   	 for (i=0; i<gNumVert; i++){
			operaTransforma(i);
	 }
 	 
     translate(vc[0], vc[1]); 
}

void reflect(float dx, float dy){
  	int i;
    
	matrizIdentidade();

	if(fabs(dx) > fabs(dy)){
		gMatriz[0][0] = -1;
		gMatriz[1][1] = 1;       
	}
	if(fabs(dy) > fabs(dx)){
		gMatriz[0][0] = 1;
		gMatriz[1][1] = -1;       
	}
   
   	 for (i=0; i<gNumVert; i++){
			operaTransforma(i);
	 }
}


void motion(int x, int y)
{
	int i;
	float dx, dy;
	if(gIndVert >-1) {
		x=x-windW; 
		y=windH-y;
		dx = x - pvertex[gIndVert].v[0];
		dy = y - pvertex[gIndVert].v[1];
	    switch (gOpera)
	    {
               case 1: translate(dx, dy); break;     
               case 2: rotate(dx, dy); break;   
               case 3: scale(dx, dy); break;   
               case 4: shear(dx, dy); break;     
			   case 5: reflect(x, y); break;     
        }
		Draw();
	}
}

void mouse(int button, int state, int x, int y)
{ 
	
	if(state == GLUT_UP)
	{
        printf("\n jaPoligono %d ", jaPoligono);
		if(button == GLUT_LEFT_BUTTON) 
		{

			if(jaPoligono==0)
			{
				x = x - windW; 
				y = windH - y;

				glPointSize(3);

				pvertex[gNumVert].v[0] = (float)x;
				pvertex[gNumVert].v[1] = (float)y;
				pvertex[gNumVert].v[2] = 1.0f;
				gNumVert++;
				gIndVert= (clipVertex(x, y)); 
			}
		} 
		else 
		if(button == GLUT_RIGHT_BUTTON)
		{
			if(nVertices>0)
			{
				jaPoligono = 1;
				tipoPoligono = GL_LINE;
			}
		}		
	}
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
    GLenum type;

    glutInit(&argc, argv);
    Args(argc, argv);

    type = GLUT_RGB;
    type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;

    glutInitDisplayMode(type);
    glutInitWindowSize(600, 500);
    glutCreateWindow("Basic Program Using Glut and Gl");

	init();

    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Key);
    glutDisplayFunc(Draw);

	glutMotionFunc(motion);
	glutMouseFunc(mouse);

	createGLUTMenus();  

    glutMainLoop();

	return (0);
}
