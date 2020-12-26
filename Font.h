#pragma once

#define FONT_DEFAULT_WIDTH (80.f)
#define FONT_DEFAULT_HEIGHT (100.f)

void fontBegin();
void fontEnd();

void fontSetPosition(float _x, float _y);
void fontSetHeight(float _height);
float fontGetWidth();
float fontGetHeight();

float fontGetWeightMin();	//�ݒ�ł���t�H���g�̍ŏ��T�C�Y
float fontGetWeightMax();
void fontSetWeight(float _weight);
float fontGetWeight();

void fontDraw(const char *_format, ...);