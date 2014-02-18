// test_color_check.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "iostream"
#include "CC1.h"

Color_Check cc;

void set_MarginB(int val){
	cc.set_MarginB(val);
	//�o��
	cvShowImage("img",cc.img);
	cvShowImage("dst",cc.dst);
		
}

void set_MarginG(int val){
	cc.set_MarginG(val);
	//�o��
	cvShowImage("img",cc.img);
	cvShowImage("dst",cc.dst);
		
}

void set_MarginR(int val){
	cc.set_MarginR(val);
	//�o��
	cvShowImage("img",cc.img);
	cvShowImage("dst",cc.dst);
		
}

void set_Procese(int val){
	if(val){
		cc.set_GUI();
		cvShowImage("img",cc.img);
		cvShowImage("dst",cc.dst);
		cvSetTrackbarPos("Go","Console",0);
	}
}


void set_OffsetB(int val){
	cc.set_OffsetB(val-10);
	//�o��
	cvShowImage("img",cc.img);
	cvShowImage("dst",cc.dst);
}

void set_OffsetG(int val){
	cc.set_OffsetG(val-10);
	//�o��
	cvShowImage("img",cc.img);
	cvShowImage("dst",cc.dst);
}

void set_OffsetR(int val){
	cc.set_OffsetR(val-10);
	//�o��
	cvShowImage("img",cc.img);
	cvShowImage("dst",cc.dst);
}

int main(){
	FILE* Lp;
	char Logname[30];
	//�E�B���h�E�AGUI�쐬
	cvNamedWindow("img");
	cvNamedWindow("dst");
	cvNamedWindow("Console");

	cvCreateTrackbar("Blue_O","Console",0,100, set_OffsetB);
	cvCreateTrackbar("Green_O","Console",0,100, set_OffsetG);
	cvCreateTrackbar("Red_O","Console",0,100, set_OffsetR);
	cvCreateTrackbar("Blue_M","Console",0,100, set_MarginB);
	cvCreateTrackbar("Green_M","Console",0,100, set_MarginG);
	cvCreateTrackbar("Red_M","Console",0,100, set_MarginR);
	cvCreateTrackbar("Go","Console",0,1, set_Procese);

	cc.set_image();
	cvShowImage("img",cc.img);
//	cvShowImage("dst",cc.dst);

	for(int i=0; i<5; i++){
		//���O�o��
		strcpy(Logname,cc.fname[cc.count]);
		strncat(Logname, ".txt", 10);
		cvWaitKey(0);
		Lp = fopen(Logname, "w");
		fprintf(Lp,"%d,%d,%d,%d,%d,%d",cc.OB, cc.OG, cc.OR, cc.MB, cc.MG, cc.MR);
		fclose(Lp);
		//�Ώۉ摜ID�X�V
		cc.count++;
	}
	return 0;
}
