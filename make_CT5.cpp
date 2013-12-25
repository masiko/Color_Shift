// make_CT5.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

const int BALL		= 1;
const int YELLOW	= 2;

const int MB = 20;
const int MG = 10;
const int MR = 5;

//convert_ct2
const int OB2 = 30;
const int OG2 = 10;
const int OR2 =-5;
	

const int MAX_COLOR_TABLE = 0x01000000;
unsigned short col_tbl[MAX_COLOR_TABLE];

int convert_ct2(){
	FILE* fp;
	
	int id1,id2,id3;

	//カラーテーブル読み込み
	if((fp = fopen("color_table.cnf","rb"))==NULL){
		printf("Cannot Open");
		return -1;
	}	
	fread(col_tbl, sizeof(unsigned short), MAX_COLOR_TABLE, fp);
	fclose(fp);

	//予想色書き込み
	for(int b=0; b<256; b++){
		for(int g=0; g<256; g++){
			for(int r=0; r<256; r++){
				if(col_tbl[b+g*256+r*65536] == BALL){
					for(int i=-1; i<=2; i++){ 
						if(i==0)					continue;
						if(0<=b+i*OB2 && b+i*OB2<=255)	id1 = b+i*OB2;
						else						continue;
						if(1<=g+i*OG2 && g+i*OG2<=255)	id2 = g+i*OG2;
						else						continue;
						if(1<=r+i*OR2 && r+i*OR2<=255)	id3 = r+i*OR2;
						else						continue;

						float g1 = id1/id2;	//	B/G
						float g2 = id2/id3;	//	G/R

						//灰色排除
						if(0.9<g1 && g1<1.1 || 0.9<g2 && g2<1.1 )	continue;
						if(col_tbl[id1+256*id2+65536*id3]  != YELLOW)	col_tbl[id1+256*id2+65536*id3] = -1;
					}
				}
			}
		}
	}

	//フラグを統一
	for(int i=0; i<256; i++){
		for(int j=0; j<256; j++){
			for(int k=0; k<256; k++){
				if(col_tbl[i+j*256+k*65536] == -1)	col_tbl[i+j*256+k*65536] = BALL;
			}
		}
	}

	//マージン書き込み
	for(int b=0; b<256; b++){
		for(int g=0; g<256; g++){
			for(int r=0; r<256; r++){
				if(col_tbl[b+g*256+r*65536] == 1){
					for(int i=-MB; i<MB; i++){
						if(b+i<0 || 255<b+i)	continue;
						else					id1 = b+i;
						for(int j=-MG; j<MG; j++){
							if(g+j<0 || 255<g+j)	continue;
							else					id2 = g+j;
							for(int k=-MR; k<MR; k++){
								if(r+k<0 || 255<r+k)	continue;
								else{
									id3 = r+k;
									if(col_tbl[id1+id2*256+id3*65536]!=1)	col_tbl[id1+id2*256+id3*65536] = 2;
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
				if(col_tbl[i + j*256 + k*65536] == 2)	col_tbl[i+j*256+k*65536] = 1;
			}
		}
	}


	//カラーテーブル書き込み
	if((fp = fopen("color_table.cnf","wb"))==NULL){
		printf("Cannot Open\n");
		return -1;
	}
	fwrite(col_tbl, sizeof(unsigned short), MAX_COLOR_TABLE, fp);
	fclose(fp);

	//カラーテーブル書き込み デバグ用
/*	fp = fopen("color_map_cc2.txt","w");
	for(int i=0; i<256; i++){
		for(int j=0; j<256; j++){
			for(int k=0; k<256; k++){
				if(col_tbl[i+256*j+65536*k] == BALL)		fprintf(fp,"%d,%d,%d\n",i,j,k);
			}
		}
	}
	fprintf(fp,"%d,%d,%d\n",-1,-1,-1);
*/	fclose(fp);
	return -1;
}

int main(){
	convert_ct2();
	return -1;
}