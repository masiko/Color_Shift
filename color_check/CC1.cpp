#include "stdafx.h"
#include "CC1.h"

#define FLG_BALL 1
#define FLG_GREEN 0x80

const int MAX_COLOR_TABLE = 0x01000000;
unsigned short col_tbl[MAX_COLOR_TABLE];



Color_Check::Color_Check(){
	
	fp = fopen("Image_names.txt","r");
	count = 0;

	for(int i=0; i<10; i++){
		fscanf(fp,"%s",fname[i]);
	}
	MB =
	MG =
	MR = 0;
	OB = 0;
	OG = 0;
	OR = 0;

	fclose(fp);
}

int Color_Check::set_image(){
	img = cvLoadImage(fname[count]);
	if(!img)		return 0;
	dst =  cvCloneImage(img);
	return 1;
}

int Color_Check::detect_ball(){
	FILE* cp;
	char save_name[30];
	int id1,id2,id3;

	//カラーテーブル読み込み
	if((cp = fopen("color_table_c.cnf","rb"))==NULL){
		printf("Cannot Open");
	}	
	fread(col_tbl, sizeof(unsigned short), MAX_COLOR_TABLE, cp);
	fclose(cp);

	for(int y=0; y<img->height; y++){
		for(int x =0; x<img->width; x++){
			id1 = (unsigned char)img->imageData[y*img->widthStep + 3*x];
			id2 = (unsigned char)img->imageData[y*img->widthStep + 3*x + 1];
			id3 = (unsigned char)img->imageData[y*img->widthStep + 3*x + 2];
	
			if(col_tbl[id1 + id2*256 + id3*65536] == FLG_BALL){
				if(is_on_green(x,y)){
					dst->imageData[y*dst->widthStep + 3*x]		= //id1;
					dst->imageData[y*dst->widthStep + 3*x+1]	= //id2;
					dst->imageData[y*dst->widthStep + 3*x+2]	= 255;//id3;
				}else{
					dst->imageData[y*dst->widthStep + 3*x]		=
					dst->imageData[y*dst->widthStep + 3*x+1]	=
					dst->imageData[y*dst->widthStep + 3*x+2]	= 0;
				}
			}else{
				dst->imageData[y*dst->widthStep + 3*x]		=
				dst->imageData[y*dst->widthStep + 3*x+1]	=
				dst->imageData[y*dst->widthStep + 3*x+2]	= 0;
			}
		}
	}
	strcpy(save_name,fname[count]);
	strncat(save_name,"_ball.png",10);
	cvSaveImage(save_name,dst);
	cout<<"end";
//	count++;
	return 1;
}

int Color_Check::is_on_green(int a, int b){
	char flag = 0;
	int id1,id2,id3;
	int i;
	float B,G,R;

	for(int x=0; x<a; x++){	
		id1 = (unsigned char)(img->imageData[b*img->widthStep + 3*(x)]);
		id2 = (unsigned char)(img->imageData[b*img->widthStep + 3*(x) + 1]);
		id3 = (unsigned char)(img->imageData[b*img->widthStep + 3*(x) + 2]);

		i = id1 + id2 + id3;
		B = (float)id1/i;
		G = (float)id2/i;
		R = (float)id3/i;

		if(detect_field(B,G,R,i)){
//		if(col_tbl[id1 + id2*256 + id3*65536] == FLG_GREEN){
			flag = 1;
			break;
		}
	}
	if(!flag)	return 0;

	for(int x=img->width-1; x>a; x--){
		id1 = (unsigned char)(img->imageData[b*img->widthStep + 3*(x)]);
		id2 = (unsigned char)(img->imageData[b*img->widthStep + 3*(x) + 1]);
		id3 = (unsigned char)(img->imageData[b*img->widthStep + 3*(x) + 2]);
		//if(col_tbl[id1 + id2*256 + id3*65536] == FLG_GREEN)	return 1;
		if(detect_field(B,G,R,i))	return 1;
	}
	return 0;
}

int Color_Check::detect_field(float b, float g, float r, int i){
	if(i<200 || 600<i)		return 0;
	if(0.4 <g && g<0.5){
		if(0.15 <r && r<0.25){
			if(0.25 <b && b<0.35){
				return 1;
			}
		}
	}
	return 0;
}

int Color_Check::set_GUI(){
	//カラーテーブル書き換え
	if( !convert_ct2())	return 0;

	//画像を評価
	if(!set_image()) return 0;
	detect_ball();

	return 1;
}

void Color_Check::set_MarginB(int val){
	MB = val;
}

void Color_Check::set_MarginG(int val){
	MG = val;
}

void Color_Check::set_MarginR(int val){
	MR = val;
}

void Color_Check::set_OffsetB(int val){
	OB = val;
}

void Color_Check::set_OffsetG(int val){
	OG = val;
}

void Color_Check::set_OffsetR(int val){
	OR = val;
}

int Color_Check::convert_ct2(){
	FILE* fp;
	
	int id1,id2,id3;

	//カラーテーブル読み込み
	if((fp = fopen("color_table.cnf","rb"))==NULL){
		printf("Cannot Open");
		return 0;
	}	
	fread(col_tbl, sizeof(unsigned short), MAX_COLOR_TABLE, fp);
	fclose(fp);

	//予想色書き込み
	for(int b=0; b<256; b++){
		for(int g=0; g<256; g++){
			for(int r=0; r<256; r++){
				if(col_tbl[b+g*256+r*65536] == FLG_BALL){
					for(int i=0; i<=1; i++){ 
						if(i==0)					continue;
						if(0<=b+i*OB && b+i*OB<=255)	id1 = b+i*OB;
						else						continue;
						if(1<=g+i*OG && g+i*OG<=255)	id2 = g+i*OG;
						else						continue;
						if(1<=r+i*OR && r+i*OR<=255)	id3 = r+i*OR;
						else						continue;

						int dis = id1+id2+id3;
						float g1 = id1/dis;	//	B/I
						float g2 = id2/dis;	//	G/I

						//他の色指定なし
						if(col_tbl[id1+256*id2+65536*id3] != 0)		continue;
						//灰色排除
						if( g1>=0.27 && 0.3<=g2 && g2<0.4 )	continue;
						//白黒排除
						if(150<dis && dis<600)	col_tbl[id1+256*id2+65536*id3] = 10;
					}	
				}
			}
		}
	}

	//フラグを統一
	for(int i=0; i<256; i++){
		for(int j=0; j<256; j++){
			for(int k=0; k<256; k++){
				if(col_tbl[i+j*256+k*65536] == 10)	col_tbl[i+j*256+k*65536] = FLG_BALL;
			}
		}
	}

	//マージン書き込み
	for(int b=0; b<256; b++){
		for(int g=0; g<256; g++){
			for(int r=0; r<256; r++){
				if(col_tbl[b+g*256+r*65536] == 1){
					for(int i=-MB; i<MB; i++){
						if(b+i<=0 || 255<b+i)	continue;
						else					id1 = b+i;
						for(int j=-MG; j<MG; j++){
							if(g+j<=0 || 255<g+j)	continue;
							else					id2 = g+j;
							for(int k=-MR; k<MR; k++){
								if(r+k<=0 || 255<r+k)	continue;
								else{
									id3 = r+k;
									float g1 = id1/id2;	//	B/G
									float g2 = id2/id3;	//	G/R
	
									//他の色指定なし
									if(col_tbl[id1+256*id2+65536*id3] != 0)		continue;
									//灰色排除
									if( g1>=0.27 && 0.3<=g2 && g2<0.4 )	continue;
									//白黒排除
									int dis = id1+id2+id3;
									if(250<dis && dis<600)	col_tbl[id1+256*id2+65536*id3] = 10;
								}
							}
						}
					}
				}
			}
		}
	}

	//フラグを統一
	for(int i=0; i<256; i++){
		for(int j=0; j<256; j++){
			for(int k=0; k<256; k++){
				if(col_tbl[i + j*256 + k*65536] == 10)	col_tbl[i+j*256+k*65536] = FLG_BALL;
			}
		}
	}


	//カラーテーブル書き込み

	fp = fopen("color_table_c.cnf","wb");
	fwrite(col_tbl, sizeof(unsigned short), MAX_COLOR_TABLE, fp);
	fclose(fp);

	return 1;
}