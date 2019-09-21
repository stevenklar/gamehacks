#include "pch.h"
#include "OpenGL.h"
#include "Game.h"

bool OpenGL::WorldToScreen(Vec3 vPos, Vec3* pvOut)
{
	ScreenSettings* pScreenSettings = ScreenSettings::GetInstance();
	if (!IsValidPtr(pScreenSettings))
		return false;

	glmatrixf* mvpmatrix = reinterpret_cast<glmatrixf*>(0x501AE8);

	float mX = (float)pScreenSettings->m_Width / 2.0F;
	float mY = (float)pScreenSettings->m_Height / 2.0F;

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

bool OpenGL::OutdatedWorldToScreen(float* matrix, Vector3 worldPosition, Vector3& screenPosition)
{
	// transform viewport to screen
	auto *pScreenSettings = ScreenSettings::GetInstance();
	float w = (float)pScreenSettings->m_Width / 2.0f;
	float h = (float)pScreenSettings->m_Height / 2.0f;

	// calculate clip coords
	Vector4 clipCoords = { 0, 0, 0, 0 };
	clipCoords.x = worldPosition.x * matrix[0] + worldPosition.y * matrix[4] + worldPosition.z * matrix[8] + matrix[12];
	clipCoords.y = worldPosition.x * matrix[1] + worldPosition.y * matrix[5] + worldPosition.z * matrix[9] + matrix[13];
	clipCoords.z = worldPosition.x * matrix[2] + worldPosition.y * matrix[6] + worldPosition.z * matrix[10] + matrix[14];
	clipCoords.w = worldPosition.x * matrix[3] + worldPosition.y * matrix[7] + worldPosition.z * matrix[11] + matrix[15];
	
	// worldPosition should'nt be behind otherwise it would be rendered twice like a mirror
	if (clipCoords.w < 0.01f)
		return false;

	// perspective divison = divide clip.W = normalizedDeviceCoordinates
	screenPosition.x = w + (w * clipCoords.x / clipCoords.w);
	screenPosition.y = h - (h * clipCoords.y / clipCoords.w);
	screenPosition.z = clipCoords.w;

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

