#include <assert.h>  
#include <stdlib.h>  
#include <time.h>  
#include <GL/glut.h>      
#include <GL/gl.h>     
#include <GL/glu.h>     
#include <time.h>     
#include <windows.h>
#include <mmsystem.h>//����������ͷ�ļ�
#include <tchar.h> 
#include <stdio.h> 
/*���尴����ASCII�� */
#define ESCAPE 27  
#define SPACE 32

/* ��С���� */
#define ISNAKE_WIDTH  15  
#define ISNAKE_HEIGHT 15  

/* ������ */
#define DIR_UP         0  
#define DIR_DOWN       1  
#define DIR_LEFT       2  
#define DIR_RIGHT      3  

/* ��ɫ���壨RGB�� */
#define  CLR_BLACK      0.0f, 0.0f, 0.0f  
#define  CLR_RED        1.0f, 0.0f, 0.0f  
#define  CLR_GREEN      0.0f, 1.0f, 0.0f  
#define  CLR_YELLOW     1.0f, 1.0f, 0.0f  
#define  CLR_BLUE       0.0f, 0.0f, 1.0f  
#define  CLR_MEGENTA    1.0f, 0.0f, 1.0f  
#define  CLR_CYAN       0.0f, 1.0f, 1.0f  
#define  CLR_WHITE      1.0f, 1.0f, 1.0f  

/* ʱ��ӿ���Ҫ�Ե���ʳ����� */
#define  MIN_FOOD       10     
/* ̰��������ʱ��,ms */
#define  TIME_BASE      300  
/* ̰�������ʱ��,ms */
#define  TIME_MIN       50  
/* ÿ�μӿ�ʱ�����̵�λ,ms */
#define  TIME_UPUNIT    50  

/*�ϰ�����ʧʱ��*/
#define TIME_SHOWBLOCK 5000

typedef struct _RGB {
	float r;
	float g;
	float b;
}RGB;

typedef struct Position {
	int x;
	int y;
}Pos;

typedef struct _SnakeNode {
	int    color;
	int    index_x;
	int    index_y;
	struct _SnakeNode* prev;
	struct _SnakeNode* next;
}SnakeNode;


struct _Snake {
	SnakeNode* head;
	SnakeNode* tail;
	int num;
} _snake;

RGB  _color[] = {
	{ CLR_RED },{ CLR_GREEN } ,{ CLR_BLUE },{ CLR_YELLOW },
	{ CLR_CYAN },{ CLR_MEGENTA },{ CLR_WHITE },{ CLR_BLACK },
};

/*���÷���*/
int _score = 0;
/* 0, 1, 2, 3�ֱ������������ */
int _direction = DIR_RIGHT;  /* Ҫ���Ƶķ��� */
int _move_dir = DIR_RIGHT;   /* ��ǰ�ƶ����� */
int _dir_x[4] = { -1, 1, 0, 0 };
int _dir_y[4] = { 0, 0,-1, 1 };

/* ��ʱʱ�� */
int _timer = TIME_BASE;
/* ��Ϸ������־ */
int _finish_flag = 0;
/*��Ϸ���¿�ʼ��־*/
int _restartGame = 0;
/* ��ͣ��־ */
int _pause_flag = 0;
/* ʳ������ */
Pos _food_pos;
int _food_clr_id;//ʳ�����ɫid
/*�ϰ�������*/
Pos _block_pos;
/* The number of our GLUT window */
int window;
int wnd_width;
int wnd_height;

/* �ƶ������Ͻ����һ���ڵ��Ϊͷ�ڵ㣬���ݷ���ı����� */
void SnakeMove(int direction)
{
	/* ��ǰͷ��λ�� */
	int x = _snake.head->index_x;
	int y = _snake.head->index_y;
	int c = _snake.head->color;
	SnakeNode* node = _snake.head;
	/* �ƶ�֮ǰ�ı���ɫ */
	for (; node->next != NULL; node = node->next) {
		node->color = node->next->color;
	}
	node->color = c;
	/* ��β�ͼӵ�ͷ�� */
	_snake.tail->prev->next = NULL;
	_snake.head->prev = _snake.tail;
	_snake.tail->next = _snake.head;
	_snake.head = _snake.tail;
	_snake.tail = _snake.tail->prev;
	_snake.head->prev = NULL;
	_snake.tail->next = NULL;
	/* ���� */
	_snake.head->index_x = x + _dir_x[direction];
	_snake.head->index_y = y + _dir_y[direction];
	/*�Ƴ���Ļ�Ĵ���*/
	if (_snake.head->index_x < 0) { _snake.head->index_x = ISNAKE_HEIGHT - 1; }
	if (_snake.head->index_x > ISNAKE_HEIGHT - 1) { _snake.head->index_x = 0; }
	if (_snake.head->index_y < 0) { _snake.head->index_y = ISNAKE_WIDTH - 1; }
	if (_snake.head->index_y > ISNAKE_WIDTH - 1) { _snake.head->index_y = 0; }

	_move_dir = _direction;
}
/*��������,��ʱû���õ�*/
// ASCII�ַ��ܹ�ֻ��0��127��һ��128���ַ�
#define MAX_CHAR        128

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
					   // Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}
void RestartGame() { 
	_score = 0;
	_finish_flag = 0;
	_restartGame = 1;
	_direction = DIR_RIGHT;//���÷���
	_move_dir = DIR_RIGHT;
	_timer = TIME_BASE;//�����ٶ�
	/*��������*/
	SnakeNode* node;

	int i = 0;
	for (node = _snake.head; i < 4; node = node->next, i++) {
		node->index_x = ISNAKE_HEIGHT / 2;
		node->index_y = ISNAKE_WIDTH / 2 - i;
		if (i == 3) {
			_snake.tail = node;
		}
	}
	//ɾ������Ľڵ�
	SnakeNode* tmp;
	while (node != NULL) {
		tmp = node->next;
		free(node);
		node = tmp;
	}
	_snake.tail->next = NULL;
}

/*��Ϸ���������*/
void GameOver() {
	PlaySound(_T("data/game_over.wav"), NULL, SND_ASYNC);
	glutSetWindowTitle("GAMEOVER");
	HWND hwnd = FindWindow(NULL, _T("GAMEOVER"));
	int ret = MessageBox(hwnd, _T("��Ϸ�������Ƿ�ʼ���棿"), _T("����"), MB_YESNO | MB_ICONQUESTION);
	switch (ret)
	{
	case IDYES:
		//���¿�ʼ
		RestartGame();
		break;
	case IDNO:
		//��ʾ����
		char str[256];
		sprintf_s(str, "���ķ�����:%d", _score);
		MessageBox(NULL, str, "�Ǻ�", MB_OK);
		break;

	default:
		break;
	}
}

/*
* �ж���Ϸ�Ƿ����
*/
int IsFinish(void)
{
	int x, y;
	SnakeNode* node;
	x = _snake.head->index_x + _dir_x[_direction];
	y = _snake.head->index_y + _dir_y[_direction];

	/*�ж�ͷ�Ƿ������ϰ���*/
	if (x == _block_pos.x && y == _block_pos.y) {
		GameOver();
		return 1;
	}
	/*�ж�ͷ�Ƿ���������*/
	for (node = _snake.head->next; node != NULL; node = node->next) {
		if ((node->index_x == x && node->index_y == y)) {
			GameOver();
			return 1;
		}
	}

	return 0;
}

/*
* �ж��Ƿ���ʳ��
* �з���1�� ���򷵻�0
*/
int HasFood(void)
{
	SnakeNode* node = _snake.head;
	/*�ж���ͷҪ�Ե�*/
	if (node->index_x + _dir_x[_direction] == _food_pos.x
		&& node->index_y + _dir_y[_direction] == _food_pos.y
		) {
		return 1;
	}
	/*�ж����Ѿ�����ʳ��*/
	while (node != NULL) {
		if (node->index_x == _food_pos.x &&
			node->index_y == _food_pos.y) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}

/*��ʳ���̰���ߵ�ͷ������һ���ڵ�*/
void SnakeEat(int x, int y)
{
	//��������
	PlaySound(_T("data/getPoint.wav"), NULL, SND_ASYNC);
	//������1����ʾ����
	_score++;
	char str[256];
	sprintf_s(str, "���ķ�����:%d", _score);
	glutSetWindowTitle(str);

	SnakeNode* node = (SnakeNode*)malloc(sizeof(SnakeNode));
	node->index_x = x;
	node->index_y = y;
	_snake.head->prev = node;
	node->next = _snake.head;
	node->prev = NULL;
	_snake.head = node;
	_snake.num++;
	_snake.head->color = _food_clr_id;

}

int CorrectFood(int x, int y)
{
	SnakeNode* node = _snake.head;
	for (; node != NULL; node = node->next) {
		if (node->index_x == x && node->index_y == y) {
			return 0;
		}
	}
	return 1;
}

void RandomFood(void)
{
	int x, y;
	srand(time(NULL));
	do {
		x = (double)rand() / RAND_MAX * ISNAKE_HEIGHT - 1;
		y = (double)rand() / RAND_MAX * ISNAKE_WIDTH - 1;
	} while (!CorrectFood(x, y));
	_food_clr_id = (double)rand() / RAND_MAX * (sizeof(_color) / sizeof(_color[0]) - 2);//��ɫ���
	_food_pos.x = x;
	_food_pos.y = y;
}

/*�ж��ϰ����λ���Ƿ���ߣ�ʳ���ص�*/
int isBlockPosRight(double x, double y)
{
	if (x == _food_pos.x && y == _food_pos.y) {
		return 1;
	}
	SnakeNode* node;
	for (node = _snake.head; node != NULL; node = node->next) {
		if (node->index_x == x && node->index_y == y) {
			return 1;
		}
	}
	return 0;
}

/*�ϰ�����ֶ�ʱ��*/
void BlockShowTimer(int value)
{
	if (!value && !_pause_flag) {
		int x, y;
		srand(time(NULL));
		do {
			x = (double)rand() / RAND_MAX * ISNAKE_HEIGHT - 1;
			y = (double)rand() / RAND_MAX * ISNAKE_WIDTH - 1;
		} while (isBlockPosRight(x, y));
		_block_pos.x = x;
		_block_pos.y = y;

		glutTimerFunc(TIME_SHOWBLOCK, BlockShowTimer, _finish_flag);//��֤��ʱ����ѭ������
	}
}

/*
* ��ʼ��һ������n���������
*/
void SnakeInit(int n)
{
	int i;
	SnakeNode* node;
	_snake.head = _snake.tail = (SnakeNode*)malloc(sizeof(SnakeNode));
	_snake.head->index_x = ISNAKE_HEIGHT / 2;//̰����ͷ��λ��
	_snake.head->index_y = ISNAKE_WIDTH / 2;
	_snake.head->color = 0;
	/*����β���ڵ�*/
	for (i = 0; i < n - 1; ++i) {
		node = (SnakeNode*)malloc(sizeof(SnakeNode));
		node->index_x = _snake.tail->index_x;
		node->index_y = _snake.tail->index_y - 1;
		node->color = (i + 1) % 7;
		node->prev = _snake.tail;
		_snake.tail->next = node;
		_snake.tail = node;
	}
	_snake.head->prev = NULL;
	_snake.tail->next = NULL;
	_snake.num = n;
	RandomFood();
	_direction = DIR_RIGHT;

	//��ʼ���ϰ����λ��
	int x, y;
	srand(time(NULL));
	do {
		x = (double)rand() / RAND_MAX * ISNAKE_HEIGHT - 1;
		y = (double)rand() / RAND_MAX * ISNAKE_WIDTH - 1;
	} while (isBlockPosRight(x, y));
	_block_pos.x = x;
	_block_pos.y = y;
}


/*
* OpenGL��ʼ������
*/
void InitGL(int Width, int Height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // ����������ɫ 
	glClearDepth(1.0);              // ����Ȼ���ָ����ʼֵ
	glDepthFunc(GL_LESS);               // �����������ֵС�ڲο�ֵ����ͨ��  
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0f, (GLdouble)Width, 0.0f, (GLdouble)Height);
}

/*�����ڸı�ʱ���� */
void ReSizeGLScene(int Width, int Height)
{
	if (Height == 0)                //�ô��ڵĸ�ʼ�ղ�Ϊ0 
		Height = 1;

	glViewport(0, 0, Width, Height);      // ���ı䴰�ڴ�Сʱ�����ӿ����� 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0f, (GLdouble)Width, 0.0f, (GLdouble)Height);//��������ߣ�����Ӧ�������Ż�����
}

/* �����ߡ���ͼ��ʳ�� */
void DrawGLScene()
{
	int i, j, clr;
	static double unit_x = 2.0f / ISNAKE_WIDTH;
	static double unit_y = 2.0f / ISNAKE_HEIGHT;
	SnakeNode* node = _snake.head;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // ����������ɫ  

	glLoadIdentity();             // ����ǰ���Ƶ���Ļ����  								
	glTranslatef(-1.0f, 1.0f, 0.0f);
	/*����ǰ���涼���������*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*������*/
	glBegin(GL_QUADS);
	glColor3f(CLR_WHITE);
	for (i = 0; i < ISNAKE_HEIGHT; ++i) {
		for (j = 0; j < ISNAKE_WIDTH; ++j) {
			glVertex3f((j)* unit_x, -(i)* unit_y, 0.0f);
			glVertex3f((j + 1) * unit_x, -(i)* unit_y, 0.0f);
			glVertex3f((j + 1) * unit_x, -(i + 1) * unit_y, 0.0f);
			glVertex3f((j)* unit_x, -(i + 1) * unit_y, 0.0f);
		}
	}
	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	/* ���� */
	while (node != NULL) {
		/*��һ���������һ������*/
		clr = node->color;
		glColor3f(_color[clr].r, _color[clr].g, _color[clr].b);
		glVertex3f((node->index_y) * unit_x, -(node->index_x) * unit_y, 0.0f);
		glVertex3f((node->index_y + 1) * unit_x, -(node->index_x) * unit_y, 0.0f);
		glVertex3f((node->index_y + 1) * unit_x, -(node->index_x + 1) * unit_y, 0.0f);
		glVertex3f((node->index_y) * unit_x, -(node->index_x + 1) * unit_y, 0.0f);

		node = node->next;
	}
	/* ��ʳ��ĸ��� */
	glColor3f(_color[_food_clr_id].r, _color[_food_clr_id].g, _color[_food_clr_id].b);
	glVertex3f((_food_pos.y) * unit_x, -(_food_pos.x) * unit_y, 0.0f);
	glVertex3f((_food_pos.y + 1) * unit_x, -(_food_pos.x) * unit_y, 0.0f);
	glVertex3f((_food_pos.y + 1) * unit_x, -(_food_pos.x + 1) * unit_y, 0.0f);
	glVertex3f((_food_pos.y) * unit_x, -(_food_pos.x + 1) * unit_y, 0.0f);


	/*���ϰ���*/
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f((_block_pos.y) * unit_x, -(_block_pos.x) * unit_y, 0.0f);
	glVertex3f((_block_pos.y + 1) * unit_x, -(_block_pos.x) * unit_y, 0.0f);
	glVertex3f((_block_pos.y + 1) * unit_x, -(_block_pos.x + 1) * unit_y, 0.0f);
	glVertex3f((_block_pos.y) * unit_x, -(_block_pos.x + 1) * unit_y, 0.0f);
	glEnd();

	glutSwapBuffers();
}

/*
* ��ʱ����Ӧ����
*/
void OnTimer(int value)
{
	if (!value && !_pause_flag) {
		if (HasFood()) {
			SnakeEat(_food_pos.x, _food_pos.y);
			RandomFood();
		}
		if (!IsFinish() || _restartGame) {
			_restartGame = 0;
			SnakeMove(_direction);
		}
		else {
			_finish_flag = 1;
		}
		_timer = (TIME_BASE - TIME_UPUNIT * _snake.num / MIN_FOOD);//�������ųԵ�ʳ������ӣ��ٶȵļӿ�
		_timer = _timer > TIME_MIN ? _timer : TIME_MIN;//�������ʱ��
		glutTimerFunc(_timer, OnTimer, _finish_flag);//��֤��ʱ����ѭ������
	}
}


void keyPressed(unsigned char key, int x, int y)
{
	/* ѡ����ʵļ�����Ӧʱ�� */
	//Sleep(100);

	if (key == ESCAPE) {
		glutDestroyWindow(window);
		exit(0);
	}

	switch (key) {
		/*���¿ո��ֹͣ*/
	case SPACE:
		_pause_flag = !_pause_flag;
		//��������Timer
		if (!_pause_flag) {
			glutTimerFunc(_timer, OnTimer, _finish_flag);
		}
		break;
	default:
		break;
	}
}

/*
* �����
*/
void OnDirection(int key, int x, int y)
{
	if (_pause_flag) { return; }

	//�任����������෴�����򲻱任
	switch (key) {
	case GLUT_KEY_UP:   if (_move_dir != DIR_DOWN) { _direction = DIR_UP; }
						break;
	case GLUT_KEY_DOWN: if (_move_dir != DIR_UP) { _direction = DIR_DOWN; }
						break;
	case GLUT_KEY_LEFT: if (_move_dir != DIR_RIGHT) { _direction = DIR_LEFT; }
						break;
	case GLUT_KEY_RIGHT:if (_move_dir != DIR_LEFT) { _direction = DIR_RIGHT; }
						break;
	default:
		break;
	}
}


int main(int argc, char** argv)
{
	SnakeInit(4);	//��snake���г�ʼ��
	glutInit(&argc, argv);	//OpenGL��ʼ��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH); //��ʼ���ƶ��������ڵĴ���ģʽ
	glutInitWindowSize(500, 500);	//��ʼ�����ڴ�С
	glutInitWindowPosition(0, 0);	//���ô���λ��
	window = glutCreateWindow("̰����");
	glutDisplayFunc(&DrawGLScene);//���ڻ���ʱ����
	glutIdleFunc(&DrawGLScene);//����û���¼�����ʱ����
	glutReshapeFunc(&ReSizeGLScene); //�ı䴰���¼�����
	glutKeyboardFunc(&keyPressed);//��ͨ�����ļ���
	glutSpecialFunc(OnDirection);//���ⰴ���ļ���
	glutTimerFunc(_timer, OnTimer, _finish_flag);	//��ʱ������_timerʱ�����һ��OnTimer
	glutTimerFunc(TIME_SHOWBLOCK, BlockShowTimer, _finish_flag);	//�����ϰ�����ֵĶ�ʱ��
	InitGL(500, 500);	//�Զ���OpenGl��ʼ������
	glutMainLoop();

	return 1;
}