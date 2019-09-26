#include "pch.h"
#include "OpenGL.h"

float Get3dDistance(Vector3 myCoords, Vector3 enemyCoords)
{
	return static_cast<float>(
		sqrt(
			pow(float(enemyCoords.x - myCoords.x), 2.0) +
			pow(float(enemyCoords.y - myCoords.y), 2.0) +
			pow(float(enemyCoords.z - myCoords.z), 2.0)
		)
	);
}

void GetResolution(int &height, int &width)
{
	RECT rWindow;
	HWND fWindow = GetForegroundWindow();
	GetWindowRect(fWindow, &rWindow);
	
	height = rWindow.right;
	width = rWindow.bottom;
}

bool OpenGL::WorldToScreen(Vec3 vPos, Vec3* pvOut, int width, int height)
{
	glmatrixf* mvpmatrix = reinterpret_cast<glmatrixf*>(0x501AE8);

	float mX = (float)width / 2.0F;
	float mY = (float)height / 2.0F;

	float x = mvpmatrix->transformx(vPos);
	float y = mvpmatrix->transformy(vPos);
	float z = mvpmatrix->transformz(vPos);
	float w = mvpmatrix->transformw(vPos);

	if (w < 0.01F)
		return false;

	pvOut->x = mX + (mX * x / w);
	pvOut->y = mY - (mY * y / w);
	pvOut->z = w;

	return true;
}


void OpenGL::DrawBox(float x, float y, RGB color, float width, float height)
{
	glLineWidth(3);
	glBegin(GL_LINE_LOOP); //begin drawing outline rect
	glColor4f(0, 0, 0, 1); //black, yes alpha
	glVertex2f(x - (width / 2), y); // bottom left
	glVertex2f(x - (width / 2), y - height); //top left
	glVertex2f(x + (width / 2), y - height); //top right
	glVertex2f(x + (width / 2), y); //bottom right
	glEnd();

	glLineWidth(1);
	glBegin(GL_LINE_LOOP);//begin drawing color rect
	glColor4f(color.red, color.green, color.blue, color.alpha);//color, yes alpha
	glVertex2f(x - (width / 2), y); // bottom left
	glVertex2f(x - (width / 2), y - height); //top left
	glVertex2f(x + (width / 2), y - height); //top right
	glVertex2f(x + (width / 2), y); //bottom right
	glEnd();
}

void OpenGL::DrawBoxEx(float x, float y, float width, float height, bool enemy)
{
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);//begin drawing color rect

	x += 800;
	y += 600;

	if (enemy) {
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	}

	glVertex2f(x - (width / 2), y); // bottom left
	glVertex2f(x - (width / 2), y - height); //top left
	glVertex2f(x + (width / 2), y - height); //top right
	glVertex2f(x + (width / 2), y); //bottom right

	glEnd();
}

void OpenGL::DrawSnaplines(float x, float y, RGB color, float SWidth, float SHeight)
{
	glLineWidth(3);
	glBegin(GL_LINES); //begin drawing snapline outline
	glColor4f(0, 0, 0, 1); //black, yes alpha
	glVertex2f(x, y); //enemies feet
	glVertex2f(SWidth / 2, SHeight); //half screen width, bottom of screen
	glEnd();
	glLineWidth(1);
	glBegin(GL_LINES); //begin drawing color snapline
	glColor4f(color.red, color.green, color.blue, color.alpha);//color, yes alpha
	glVertex2f(x, y); //enemies feet
	glVertex2f(SWidth / 2, SHeight); //half screen width, bottom of screen
	glEnd();
}

void OpenGL::DrawRect(int x, int y, int x2, int y2, RenderFunctions::Color32 color, bool bFilled)
{
	glDepthMask(GL_FALSE);
	glDisable(GL_TEXTURE_2D);

	glColor4f(color.r, color.g, color.b, color.a);

	glBegin(bFilled ? GL_QUADS : GL_LINE_LOOP);

	glVertex2f(x, y);
	glVertex2f(x2, y);
	glVertex2f(x2, y2);
	glVertex2f(x, y2);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glDepthMask(GL_TRUE);
}


void OpenGL::DrawHealthBar(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat health, RenderFunctions::Color32 color)
{
	//Outline
	OpenGL::DrawRect(x, y, x + w, y + h, RenderFunctions::Color32(0.0F, 0.0F, 0.0F, 1.0F), false);

	//Content
	OpenGL::DrawRect(x + 1, y + 1, x + (w * health) - 1, y + h - 1, color, true);
}

void OpenGL::DrawString(int x, int y, int r, int g, int b, char* pText, ...)
{
	va_list va_alist;

	char buf[256];

	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);

	float extraScale = 0.3F;

	glScalef(extraScale, extraScale, 1);

	//EngineDrawString(buf, x / extraScale, y / extraScale, r, g, b);

	glScalef(1 / extraScale, 1 / extraScale, 1);
}

float OpenGL::GetDistance(Vector3 myCoords, Vector3 enemyCoords)
{
	return sqrt(
		powf(enemyCoords.x - myCoords.x, 2.0) +
		powf(enemyCoords.y - myCoords.y, 2.0) +
		powf(enemyCoords.z - myCoords.z, 2.0)
	);
}

void SetupOrtho()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void RestoreGL()
{
	glPopMatrix();
	glPopAttrib();
}

