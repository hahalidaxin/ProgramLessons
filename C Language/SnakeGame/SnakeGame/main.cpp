#include <assert.h>  
#include <stdlib.h>  
#include <time.h>  
#include <GL/glut.h>      
#include <GL/gl.h>     
#include <GL/glu.h>     
#include <time.h>     
#include <windows.h>
#include <mmsystem.h>//播放声音的头文件
#include <tchar.h> 
#include <stdio.h> 
/*定义按键的ASCII码 */
#define ESCAPE 27  
#define SPACE 32

/* 大小定义 */
#define ISNAKE_WIDTH  15  
#define ISNAKE_HEIGHT 15  

/* 方向定义 */
#define DIR_UP         0  
#define DIR_DOWN       1  
#define DIR_LEFT       2  
#define DIR_RIGHT      3  

/* 颜色定义（RGB） */
#define  CLR_BLACK      0.0f, 0.0f, 0.0f  
#define  CLR_RED        1.0f, 0.0f, 0.0f  
#define  CLR_GREEN      0.0f, 1.0f, 0.0f  
#define  CLR_YELLOW     1.0f, 1.0f, 0.0f  
#define  CLR_BLUE       0.0f, 0.0f, 1.0f  
#define  CLR_MEGENTA    1.0f, 0.0f, 1.0f  
#define  CLR_CYAN       0.0f, 1.0f, 1.0f  
#define  CLR_WHITE      1.0f, 1.0f, 1.0f  

/* 时间加快所要吃到的食物个数 */
#define  MIN_FOOD       10     
/* 贪吃蛇最慢时间,ms */
#define  TIME_BASE      300  
/* 贪吃蛇最块时间,ms */
#define  TIME_MIN       50  
/* 每次加快时间缩短单位,ms */
#define  TIME_UPUNIT    50  

/*障碍物消失时间*/
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

/*设置分数*/
int _score = 0;
/* 0, 1, 2, 3分别代表上下左右 */
int _direction = DIR_RIGHT;  /* 要控制的方向 */
int _move_dir = DIR_RIGHT;   /* 当前移动方向 */
int _dir_x[4] = { -1, 1, 0, 0 };
int _dir_y[4] = { 0, 0,-1, 1 };

/* 定时时间 */
int _timer = TIME_BASE;
/* 游戏结束标志 */
int _finish_flag = 0;
/*游戏重新开始标志*/
int _restartGame = 0;
/* 暂停标志 */
int _pause_flag = 0;
/* 食物坐标 */
Pos _food_pos;
int _food_clr_id;//食物的颜色id
/*障碍物坐标*/
Pos _block_pos;
/* The number of our GLUT window */
int window;
int wnd_width;
int wnd_height;

/* 移动，不断将最后一个节点变为头节点，根据方向改变坐标 */
void SnakeMove(int direction)
{
	/* 当前头部位置 */
	int x = _snake.head->index_x;
	int y = _snake.head->index_y;
	int c = _snake.head->color;
	SnakeNode* node = _snake.head;
	/* 移动之前改变颜色 */
	for (; node->next != NULL; node = node->next) {
		node->color = node->next->color;
	}
	node->color = c;
	/* 将尾巴加到头部 */
	_snake.tail->prev->next = NULL;
	_snake.head->prev = _snake.tail;
	_snake.tail->next = _snake.head;
	_snake.head = _snake.tail;
	_snake.tail = _snake.tail->prev;
	_snake.head->prev = NULL;
	_snake.tail->next = NULL;
	/* 方向 */
	_snake.head->index_x = x + _dir_x[direction];
	_snake.head->index_y = y + _dir_y[direction];
	/*移出屏幕的处理*/
	if (_snake.head->index_x < 0) { _snake.head->index_x = ISNAKE_HEIGHT - 1; }
	if (_snake.head->index_x > ISNAKE_HEIGHT - 1) { _snake.head->index_x = 0; }
	if (_snake.head->index_y < 0) { _snake.head->index_y = ISNAKE_WIDTH - 1; }
	if (_snake.head->index_y > ISNAKE_WIDTH - 1) { _snake.head->index_y = 0; }

	_move_dir = _direction;
}
/*绘制字体,暂时没有用到*/
// ASCII字符总共只有0到127，一共128种字符
#define MAX_CHAR        128

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
					   // 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}
void RestartGame() { 
	_score = 0;
	_finish_flag = 0;
	_restartGame = 1;
	_direction = DIR_RIGHT;//重置方向
	_move_dir = DIR_RIGHT;
	_timer = TIME_BASE;//重置速度
	/*重置蛇身*/
	SnakeNode* node;

	int i = 0;
	for (node = _snake.head; i < 4; node = node->next, i++) {
		node->index_x = ISNAKE_HEIGHT / 2;
		node->index_y = ISNAKE_WIDTH / 2 - i;
		if (i == 3) {
			_snake.tail = node;
		}
	}
	//删除后面的节点
	SnakeNode* tmp;
	while (node != NULL) {
		tmp = node->next;
		free(node);
		node = tmp;
	}
	_snake.tail->next = NULL;
}

/*游戏结束后调用*/
void GameOver() {
	PlaySound(_T("data/game_over.wav"), NULL, SND_ASYNC);
	glutSetWindowTitle("GAMEOVER");
	HWND hwnd = FindWindow(NULL, _T("GAMEOVER"));
	int ret = MessageBox(hwnd, _T("游戏结束，是否开始重玩？"), _T("哈哈"), MB_YESNO | MB_ICONQUESTION);
	switch (ret)
	{
	case IDYES:
		//重新开始
		RestartGame();
		break;
	case IDNO:
		//显示分数
		char str[256];
		sprintf_s(str, "您的分数是:%d", _score);
		MessageBox(NULL, str, "呵呵", MB_OK);
		break;

	default:
		break;
	}
}

/*
* 判断游戏是否结束
*/
int IsFinish(void)
{
	int x, y;
	SnakeNode* node;
	x = _snake.head->index_x + _dir_x[_direction];
	y = _snake.head->index_y + _dir_y[_direction];

	/*判断头是否碰到障碍物*/
	if (x == _block_pos.x && y == _block_pos.y) {
		GameOver();
		return 1;
	}
	/*判断头是否碰到身子*/
	for (node = _snake.head->next; node != NULL; node = node->next) {
		if ((node->index_x == x && node->index_y == y)) {
			GameOver();
			return 1;
		}
	}

	return 0;
}

/*
* 判断是否有食物
* 有返回1， 无则返回0
*/
int HasFood(void)
{
	SnakeNode* node = _snake.head;
	/*判断蛇头要吃到*/
	if (node->index_x + _dir_x[_direction] == _food_pos.x
		&& node->index_y + _dir_y[_direction] == _food_pos.y
		) {
		return 1;
	}
	/*判断蛇已经经过食物*/
	while (node != NULL) {
		if (node->index_x == _food_pos.x &&
			node->index_y == _food_pos.y) {
			return 1;
		}
		node = node->next;
	}
	return 0;
}

/*吃食物，在贪吃蛇的头部增加一个节点*/
void SnakeEat(int x, int y)
{
	//播放声音
	PlaySound(_T("data/getPoint.wav"), NULL, SND_ASYNC);
	//分数加1，显示分数
	_score++;
	char str[256];
	sprintf_s(str, "您的分数是:%d", _score);
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
	_food_clr_id = (double)rand() / RAND_MAX * (sizeof(_color) / sizeof(_color[0]) - 2);//颜色随机
	_food_pos.x = x;
	_food_pos.y = y;
}

/*判定障碍物的位置是否和蛇，食物重叠*/
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

/*障碍物出现定时器*/
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

		glutTimerFunc(TIME_SHOWBLOCK, BlockShowTimer, _finish_flag);//保证计时器的循环调用
	}
}

/*
* 初始化一个带有n个方块的蛇
*/
void SnakeInit(int n)
{
	int i;
	SnakeNode* node;
	_snake.head = _snake.tail = (SnakeNode*)malloc(sizeof(SnakeNode));
	_snake.head->index_x = ISNAKE_HEIGHT / 2;//贪吃蛇头部位置
	_snake.head->index_y = ISNAKE_WIDTH / 2;
	_snake.head->color = 0;
	/*创建尾部节点*/
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

	//初始化障碍物的位置
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
* OpenGL初始化函数
*/
void InitGL(int Width, int Height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // 设置清屏颜色 
	glClearDepth(1.0);              // 给深度缓冲指定初始值
	glDepthFunc(GL_LESS);               // 如果输入的深度值小于参考值，则通过  
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0f, (GLdouble)Width, 0.0f, (GLdouble)Height);
}

/*当窗口改变时调用 */
void ReSizeGLScene(int Width, int Height)
{
	if (Height == 0)                //让窗口的高始终不为0 
		Height = 1;

	glViewport(0, 0, Width, Height);      // 当改变窗口大小时调用视口设置 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0f, (GLdouble)Width, 0.0f, (GLdouble)Height);//定义比例尺，以适应窗口缩放或扩大
}

/* 绘制蛇、地图和食物 */
void DrawGLScene()
{
	int i, j, clr;
	static double unit_x = 2.0f / ISNAKE_WIDTH;
	static double unit_y = 2.0f / ISNAKE_HEIGHT;
	SnakeNode* node = _snake.head;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 设置清屏颜色  

	glLoadIdentity();             // 将当前点移到屏幕中心  								
	glTranslatef(-1.0f, 1.0f, 0.0f);
	/*设置前后面都用线来填充*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	/*画格子*/
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
	/* 画蛇 */
	while (node != NULL) {
		/*画一个蛇身体的一个格子*/
		clr = node->color;
		glColor3f(_color[clr].r, _color[clr].g, _color[clr].b);
		glVertex3f((node->index_y) * unit_x, -(node->index_x) * unit_y, 0.0f);
		glVertex3f((node->index_y + 1) * unit_x, -(node->index_x) * unit_y, 0.0f);
		glVertex3f((node->index_y + 1) * unit_x, -(node->index_x + 1) * unit_y, 0.0f);
		glVertex3f((node->index_y) * unit_x, -(node->index_x + 1) * unit_y, 0.0f);

		node = node->next;
	}
	/* 画食物的格子 */
	glColor3f(_color[_food_clr_id].r, _color[_food_clr_id].g, _color[_food_clr_id].b);
	glVertex3f((_food_pos.y) * unit_x, -(_food_pos.x) * unit_y, 0.0f);
	glVertex3f((_food_pos.y + 1) * unit_x, -(_food_pos.x) * unit_y, 0.0f);
	glVertex3f((_food_pos.y + 1) * unit_x, -(_food_pos.x + 1) * unit_y, 0.0f);
	glVertex3f((_food_pos.y) * unit_x, -(_food_pos.x + 1) * unit_y, 0.0f);


	/*画障碍物*/
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f((_block_pos.y) * unit_x, -(_block_pos.x) * unit_y, 0.0f);
	glVertex3f((_block_pos.y + 1) * unit_x, -(_block_pos.x) * unit_y, 0.0f);
	glVertex3f((_block_pos.y + 1) * unit_x, -(_block_pos.x + 1) * unit_y, 0.0f);
	glVertex3f((_block_pos.y) * unit_x, -(_block_pos.x + 1) * unit_y, 0.0f);
	glEnd();

	glutSwapBuffers();
}

/*
* 定时器响应函数
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
		_timer = (TIME_BASE - TIME_UPUNIT * _snake.num / MIN_FOOD);//设置随着吃的食物的增加，速度的加快
		_timer = _timer > TIME_MIN ? _timer : TIME_MIN;//设置最快时间
		glutTimerFunc(_timer, OnTimer, _finish_flag);//保证计时器的循环调用
	}
}


void keyPressed(unsigned char key, int x, int y)
{
	/* 选择合适的键盘响应时间 */
	//Sleep(100);

	if (key == ESCAPE) {
		glutDestroyWindow(window);
		exit(0);
	}

	switch (key) {
		/*按下空格键停止*/
	case SPACE:
		_pause_flag = !_pause_flag;
		//重新启动Timer
		if (!_pause_flag) {
			glutTimerFunc(_timer, OnTimer, _finish_flag);
		}
		break;
	default:
		break;
	}
}

/*
* 方向键
*/
void OnDirection(int key, int x, int y)
{
	if (_pause_flag) { return; }

	//变换方向，如果是相反方向则不变换
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
	SnakeInit(4);	//对snake进行初始化
	glutInit(&argc, argv);	//OpenGL初始化
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH); //初始化制定创建窗口的创建模式
	glutInitWindowSize(500, 500);	//初始化窗口大小
	glutInitWindowPosition(0, 0);	//设置窗口位置
	window = glutCreateWindow("贪吃蛇");
	glutDisplayFunc(&DrawGLScene);//窗口绘制时调用
	glutIdleFunc(&DrawGLScene);//窗口没有事件发生时调用
	glutReshapeFunc(&ReSizeGLScene); //改变窗口事件监听
	glutKeyboardFunc(&keyPressed);//普通按键的监听
	glutSpecialFunc(OnDirection);//特殊按键的监听
	glutTimerFunc(_timer, OnTimer, _finish_flag);	//定时器，隔_timer时间调用一次OnTimer
	glutTimerFunc(TIME_SHOWBLOCK, BlockShowTimer, _finish_flag);	//调用障碍物出现的定时器
	InitGL(500, 500);	//自定义OpenGl初始化函数
	glutMainLoop();

	return 1;
}