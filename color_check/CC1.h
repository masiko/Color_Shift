#include "iostream"
#include "string.h"
#include "cv.h"
#include "mycv.h"
#include "highgui.h"


class Color_Check{
private:
	
public:
	Color_Check();
	int set_image();
	int detect_ball();
	int detect_field(float, float, float, int);
	int is_on_green(int,int);
	int set_GUI();
	void set_MarginB(int);
	void set_MarginG(int);
	void set_MarginR(int);
	void set_OffsetB(int);
	void set_OffsetG(int);
	void set_OffsetR(int);
	int convert_ct2();
	
	IplImage* img;
	IplImage* dst;
	FILE*     fp;
	char	  fname[10][20];
	char	  count;

	char	  MB;
	char	  MG;
	char	  MR;
	char	  OB;
	char	  OG;
	char	  OR;
	
};