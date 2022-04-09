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
float gAng =0.0f;    // angulo para rotacao 
float gMatriz[3][3];


void circulo(float r, float ang, float pp[3])
{
	pp[0] = (float)(r * cos(ang));
	pp[1] = (float)(r * sin(ang));
	pp[2] = (float)0.0;
}


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
	nVertices=0;   // zero pontos
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
//    gluPerspective(60.0, 1.0, 0.1, 1000.0);
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

	glColor3f(1.0, 0.0, 0.0);

	// vertical line

	glBegin(GL_LINE_STRIP);
		glVertex2f(-windW, 0);
		glVertex2f(windW, 0);
    glEnd();

	// horizontal line 

    glBegin(GL_LINE_STRIP);
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
     //  option: 
     //            1: transla��o
     //            2: Rota��o
     //            3: Scalamento
     //            4: Cisalha
	 //		       5: espelho
	

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
// void translate(float dx, float dy)
// {
//      int i;   
//      for (i=0; i<MAXVERTEXS; i++) {
// 		pvertex[i].v[0] += dx;
// 		pvertex[i].v[1] += dy;
// 	}  
// }

void calCentro(void)
{
     int i;
    // computando o centroide
     gCen[0] = gCen[1] = 0.0f;    
     for (i=0; i<gNumVert; i++) {
		gCen[0] += pvertex[i].v[0];
		gCen[1] += pvertex[i].v[1];
	 }
     gCen[0] /= gNumVert;     
     gCen[1] /= gNumVert;     
}

void translaCentro(int t)
{
     int i;   
     // translada para centro
     for (i=0; i<gNumVert; i++) {
		pvertex[i].v[0] += (t * gCen[0]);
		pvertex[i].v[1] += (t * gCen[1]);
	 }    
}

// void rotate(float dx, float dy)
// {
//      int i; 
//      float oo, teta, xy[3]; 
     
//      // calculo do angulo 
//      // seja vetor do centro para o vertice: vv 
//      // dd = (dx, dy) é o vetor deslocalmento do mouse
//      // o = vv x dd  (produto vetorial)
//      // se o positivo ==> rota antihorario;  
//      // se o negativo ==> rota horario
 
//      calCentro();
//      translaCentro(-1);
     
//      // determinando o angulo
//      oo = pvertex[gIndVert].v[1] * dx - pvertex[gIndVert].v[0] * dy;
//      teta = oo;
//      if(oo>0.0f){
//         teta = -1.0f * oo; 
// 	 }
            
//      // rota em teta para lado oo         
//      for (i=0; i<MAXVERTEXS; i++) {
// 		xy[0] = pvertex[i].v[0];
// 		xy[1] = pvertex[i].v[1];
// 		pvertex[i].v[0] = xy[0] * cos(teta) - xy[1] * sin(teta);
// 		pvertex[i].v[1] = xy[0] * sin(teta) + xy[1] * cos(teta);		
// 	 } 	 
//      translaCentro(1);     
// }

// void scale(float dx, float dy)
// {
//      int i;
//      float sx, sy;
     
//      calCentro();
//      translaCentro(-1);   
//      // scalando...
     
//      sx = sy = 1.0f;
//      if(fabs(pvertex[gIndVert].v[0]) > 0.01f)
//         sx = 1.0f + dx / pvertex[gIndVert].v[0];
//      if(fabs(pvertex[gIndVert].v[1]) > 0.01f)
//         sy = 1.0f + dy / pvertex[gIndVert].v[1];     
//      for (i=0; i<MAXVERTEXS; i++) {
// 		    pvertex[i].v[0] *= sx;		
// 		    pvertex[i].v[1] *= sy;
// 	 }
              
//      translaCentro(1);                     
// }

// void shear(float dx, float dy)
// {
//      int i;
//      float sx, sy, xy[3];
     
//      sx = 0.001f*dx;
//      sy = 0.001f*dy;
//      if(dx>dy)
//      {
//          if(fabs(pvertex[gIndVert].v[0])>0.1f)
//             sx =  dx / pvertex[gIndVert].v[0];
//      }
//      else
//      {
//          if(fabs(pvertex[gIndVert].v[1])>0.1f)
//             sy =  dy / pvertex[gIndVert].v[1]; 
//      }
         
//      calCentro();
//      translaCentro(-1);          
     
//       // rota em teta para lado oo         
//      for (i=0; i<MAXVERTEXS; i++) {
// 		xy[0] = pvertex[i].v[0];
// 		xy[1] = pvertex[i].v[1];
// 		pvertex[i].v[0] = xy[0] + xy[1] * sx;
// 		pvertex[i].v[1] = xy[0] * sy + xy[1];		
// 	 } 	     
         
//      translaCentro(1);   
// }

// void reflection(int x, int y)
// {
//     int i;   
// 	if(x < 0) {
// 			for (i=0; i<MAXVERTEXS; i++) {
// 				pvertex[i].v[0] *= -1;
// 				pvertex[i].v[1] *= 1;
// 			}
// 	 }
// 	if(y < 0) {
// 			for (i=0; i<MAXVERTEXS; i++) {
// 				pvertex[i].v[0] *= 1;
// 				pvertex[i].v[1] *= -1;
// 			}
// 	 }
// }

// NOVAS TRANSFORMAÇÕES


void calCentro(float cc[])
{
     int i;
    // computando o centroide
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


void matrizIdentidade(void) {
	gMatriz[0][0] = 1.0f;   gMatriz[0][1] = 0.0f;    gMatriz[0][2] = 0.0f;
	gMatriz[1][0] = 0.0f;   gMatriz[1][1] = 1.0f;    gMatriz[1][2] = 0.0f;
	gMatriz[2][0] = 0.0f;   gMatriz[2][1] = 0.0f;    gMatriz[2][2] = 1.0f;
}

void operaTransforma(int position) {
	float temp[3];
	int i, j;
	  // temp = Matriz x vetor


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
	  // copia vetor resultando no vetor original

	for(i=0; i<3; i++){
		printf("%f, ", temp[i]);
		pvertex[position].v[i] = temp[i];
	}	
	printf("\n-------------------------------\n");
}


void translate(float dx, float dy)
{
	int i;
    matrizIdentidade();    // gera a matriz de identidade gMatriz
     
     	// preenche matriz translacao:  [ 1  0  dx |  0  1  dy |  0  0  1 ] 
    gMatriz[0][2] = dx;
    gMatriz[1][2] = dy;
    
    	// opera transformação de cada vetor vértice
	for (i=0; i<gNumVert; i++) {
		printf("\n\n vetor %d: (%f, %f)", i, pvertex[i].v[0], pvertex[i].v[1]);
		operaTransforma(i);
	}
}

void rotate(float dx, float dy)
{
     int i; 
     float oo, teta, vc[3]; 
     
     // calculo do angulo 
     // ----------------------------------------------
     // seja vetor do centro para o vertice: vv 
     // dd = (dx, dy) é o vetor deslocalmento do mouse
     // oo = vv x dd  (produto vetorial)
     // se oo positivo ==> rota antihorario;  
     // se oo negativo ==> rota horario
     // ----------------------------------------------
 
     calCentro(vc);                    // calculo vetor vc ao centro geométric do polígono
     translate(-1*vc[0], -1*vc[1]);    // translada o polígo ao origem em -vc
     
     // determinando o angulo: 
     
	    // produto vetorial --> (v[0], v[1]) x (dx, dy)  
     oo = pvertex[gIndVert].v[1] * dx - pvertex[gIndVert].v[0] * dy;
     
     	// oo é esacalar positivo (horaria) ou negativo (anti-horaria)

     teta = gAng;                // angulo constante definido no init
     if(oo>0.0f) {
        teta = -1.0f * gAng; 		 
	 }

	// printf("oo: %f \ngAng: %f \nteta: %f", oo, gAng, teta);
	// 	// Define a matriz de Rotacao	 
 	 matrizIdentidade();  
 	 gMatriz[0][0] = cos(teta);    gMatriz[0][1] = -sin(teta);
 	 gMatriz[1][0] = sin(teta);    gMatriz[1][1] = cos(teta);
 	 
    // 	// opera transformação Rotacao de cada vetor vértice
	 for (i=0; i<gNumVert; i++){
			operaTransforma(i);
	 }
 	 
     translate(vc[0], vc[1]);     // o poligo é tranladado para sua posicao original
}

// -------- verifica se um ponto (x, y) posia se considerar um vértice do polígono
// ---------------------------------------------------------------------------------

void scale(float dx, float dy){
	int i; 
    float oo, teta, vc[3], Sx, Sy; 
     
     // calculo do angulo 
     // ----------------------------------------------
     // seja vetor do centro para o vertice: vv 
     // dd = (dx, dy) é o vetor deslocalmento do mouse
     // oo = vv x dd  (produto vetorial)
     // se oo positivo ==> rota antihorario;  
     // se oo negativo ==> rota horario
     // ----------------------------------------------
 
     calCentro(vc);                    // calculo vetor vc ao centro geométric do polígono
     translate(-1*vc[0], -1*vc[1]);    // translada o polígo ao origem em -vc
     
     // determinando o angulo: 
     
	    // produto vetorial --> (v[0], v[1]) x (dx, dy)  
    Sx = Sy = 1.0f;
     if(fabs(pvertex[gIndVert].v[0]) > 0.01f)
        Sx = 1.0f + dx / pvertex[gIndVert].v[0];
     if(fabs(pvertex[gIndVert].v[1]) > 0.01f)
        Sy = 1.0f + dy / pvertex[gIndVert].v[1];
     for (i=0; i<gNumVert; i++) {
            pvertex[i].v[0] *= Sx;
            pvertex[i].v[1] *= Sy;
     }

 	 matrizIdentidade();  
 	 gMatriz[0][0] = Sx;
 	 gMatriz[1][1] = Sy;
 	 
    // 	// opera transformação Rotacao de cada vetor vértice
	 for (i=0; i<gNumVert; i++){
			operaTransforma(i);
	 }
 	 
     translate(vc[0], vc[1]); 
}


void shear(float dx, float dy)
{
     int i;
     float sx, sy, vc[3];
     

	 calCentro(vc);                    // calculo vetor vc ao centro geométric do polígono
     translate(-1*vc[0], -1*vc[1]);

     sx = 0.001f*dx;
     sy = 0.001f*dy;
     if(dx>dy)
     {
         if(fabs(pvertex[gIndVert].v[0])>0.1f)
            sx =  dx / pvertex[gIndVert].v[0];
     }
     else
     {
         if(fabs(pvertex[gIndVert].v[1])>0.1f)
            sy =  dy / pvertex[gIndVert].v[1]; 
     }

	matrizIdentidade();
	gMatriz[0][1] = sx; 
	gMatriz[1][0] = sy;      
   
   	 for (i=0; i<gNumVert; i++){
			operaTransforma(i);
	 }
 	 
     translate(vc[0], vc[1]); 
}

void reflect(float dx, float dy){
  	int i;
    
	// FAZENDO PARA X


	matrizIdentidade();

	if(dx > 151 || dx < -151){
		gMatriz[0][0] = -1;
		gMatriz[1][1] = 1;       
	}
	if(dy > 151 || dy < -151){
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

//				glColor3f(0.0, 1.0, 0.0);
				glPointSize(3);
//				glBegin(GL_POINTS); 
//				glVertex2i(x, y);
//				glEnd();

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
