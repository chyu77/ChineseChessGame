
/*设定初始化开关参数*/
int tankstart=0,planstart=0;
int first=1;
union REGS regs;
int init();
int mouseread();
void cursor(),newxy();
int xmin,xmax,ymin,ymax,x_max=639,y_max=479;

/*******************************************************************************
** 描述：加载BMP文件
*******************************************************************************/
/*******************************************************************************
** 函数名称：Set_BMP_Palette_Register
** 函数描述：写入BMP到屏幕中
** 输入参数：无
** 输出参数：无 
*******************************************************************************/
void Set_BMP_Palette_Register(int index,RGB_BMP_ptr color)
{
	outp(PALETTE_MASK,0xff);
	outp(PALETTE_REGISTER_WR,index);
	outp(PALETTE_DATA,color->red);
	outp(PALETTE_DATA,color->green);
	outp(PALETTE_DATA,color->blue);
}
/*******************************************************************************
** 函数名称：Check_Bmp
** 函数描述：BMP检测
** 输入参数：无
** 输出参数：如果有错误，输出相应出错信息并退出
*******************************************************************************/
void Check_Bmp(bmp_picture_ptr bmp_ptr)
{
	if(bmp_ptr->file.bfType!=0x4d42)
{
    printf("Not a BMP file!\n");
	exit(1);
}
if(bmp_ptr->info.biCompression!=0)
{
	printf("Can not display a compressed BMP file!\n");
	exit(1);
}
if(bmp_ptr->info.biBitCount!=8)
{
	printf("Not a index 16color BMP file!\n");
	exit(1);
}
}


void BMP_Load_Screen(char *bmp)
{
	int i,fp;
	bmp_picture bmp256;
	char *file_name;
	if ((fp=open(bmp,O_RDONLY))==1)
     return;

	read(fp,&bmp256.file,sizeof(bitmapfile));
	read(fp,&bmp256.info,sizeof(bitmapinfo));

	Check_Bmp((bmp_picture_ptr)&bmp256);

	for (i=0;i<256;i++)
	{
		read(fp,&bmp256.palette[i].blue,1);
		read(fp,&bmp256.palette[i].green,1);
		read(fp,&bmp256.palette[i].red,1);
		read(fp,&bmp256.palette[i].reserved,1);
		bmp256.palette[i].blue=bmp256.palette[i].blue>>2;
		bmp256.palette[i].green=bmp256.palette[i].green>>2;
		bmp256.palette[i].red=bmp256.palette[i].red>>2;
	}
	for (i=0;i<256;i++)
		Set_BMP_Palette_Register(i,(RGB_BMP_ptr)&bmp256.palette[i]);
	for(i=SCREEN_HEIGHT-1;i>=0;i--)
 {
		lseek(fp,1078+(long)(SCREEN_HEIGHT-i-1)*SCREEN_WIDTH,0);
		read(fp,&video_buffer[i*SCREEN_WIDTH],SCREEN_WIDTH);
 }
	close(fp);

}

void Set_Video_Mode(int mode)
{
    union REGS inregs,outregs;
    inregs.h.ah=0;
    inregs.h.al=(unsigned char)mode;
    int86(0x10,&inregs,&outregs);
}

/*******************************************************************************
** 函数名称：drawmat
** 函数描述：依次：字模指针、点阵大小、起始坐标(x,y)、颜色
** 输入参数：*mat,matsize,x,y,color
** 输出参数：无
*******************************************************************************/
void drawmat(char *mat,int matsize,int x,int y,int color)
{
  int i, j, k, n;
  n = (matsize - 1) / 8 + 1;
  for(j = 0; j < matsize; j++)
    for(i = 0; i < n; i++)
      for(k = 0;k < 8; k++)
        if(mat[j * n + i] & (0x80 >> k))  /*测试为1的位则显示*/
          putpixel(x + i * 8 + k, y + j, color);
}

/*******************************************************************************
** 函数名称：initgr
** 函数描述：BGI初始化
** 输入参数：无
** 输出参数：无
*******************************************************************************/
void initgr(void)
{
  int gd = VGA, gm = VGAHI; 

  initgraph(&gd, &gm, "");
}

/*******************************************************************************
** 函数名称：startpicture
** 函数描述：显示启动画面
** 输入参数：无
** 输出参数：无
*******************************************************************************/
void startpicture(void)
{

    int buttons,xm,ym,x0,y0,x,y;
    char str[100];

    int i;
    setbkcolor(LIGHTBLUE);
/*drawmat 参数 依次：字模指针、点阵大小、起始坐标(x,y)、颜色*/
/*欢乐中国暗棋*/
    drawmat(huan64M,64,60,50,6);
    drawmat(le64M,64,140,50,7);
    drawmat(zhong64M,64,220,50,8);
    drawmat(guo64M,64,300,50,5);
    drawmat(an64M,64,380,50,12);
    drawmat(qi64M,64,460,50,11);

/*吕红兵指导*/
    drawmat(lv24M,24,100,350,11);
    drawmat(hong24M,24,130,350,11);
    drawmat(bing24M,24,160,350,11);
    drawmat(zhi24M,24,200,350,11);
    drawmat(dao24M,24,230,350,11);
    
/*温天宇李凯 制作*/
    drawmat(wen24M,24,100,380,11);
    drawmat(tian24M,24,130,380,11);
    drawmat(yu24M,24,160,380,11);
    drawmat(li24M,24,210,380,11);
    drawmat(kai24M,24,240,380,11);
    drawmat(zhi24S,24,310,380,11);
    drawmat(zuo24M,24,340,380,11);
/*浙江大学*/
    drawmat(zhe32M,32,200,420,11);
    drawmat(jiang32M,32,250,420,11);
    drawmat(da32M,32,300,420,11);
    drawmat(xue32M,32,350,420,11);
/*画帅和将*/
    setcolor(6);
    setfillstyle(1,14);
    fillellipse(173,234,53,53);
    fillellipse(396,234,53,53);
    setfillstyle(1,6);
    fillellipse(173,234,50,50);
    fillellipse(396,234,50,50);
    drawmat(shuai64H,64,140,207,4);
    drawmat(jiang64H,64,366,204,11);
/*画按钮*/
    drawmat(kai32H,32,443,343,1);
    drawmat(shi32H,32,483,343,1);
    drawmat(you32H,32,523,343,1);
    drawmat(xi32H,32,563,343,1);
    drawmat(jie32H,32,443,403,1);
    drawmat(shu32H,32,483,403,1);
    drawmat(you32H,32,523,403,1);
    drawmat(xi32H,32,563,403,1);
    
    xmin=2;
    xmax=x_max-1;
    ymin=8;
    ymax=y_max-2;
    setwritemode(XOR_PUT);
    if(init(xmin,xmax,ymin,ymax)==0 )   /*  调用init函数对鼠标器初始化*/
    {
        printf("Mouse or Mouse Driver Absent,P1ease install!");         
                delay(5000);
                exit(1);
    }
    x=320;y=240;
    cursor(x,y);    /*  置十字光标在屏幕中心。  */
    for(;;) 
        {
        newxy(&x,&y,&buttons);
        if(x>=440&& x<=600 &&y>=400&&y<=440&& buttons) /*十字光标移到结束处时*/
            {
            exit(0);    /*  回到系统*/
        }
        if(x>=440&& x<=600 &&y>=340&&y<=380&& buttons) /*十字光标移到开始t处时*/
            {
	    return;    /* 开始游戏*/
        }
    }
    
}
/*******************************************************************************
** 函数名称：cursor
** 函数描述：画光标
** 输入参数：无
** 输出参数：无
*******************************************************************************/
void cursor(int x,int y)
{
    int x1,x2,y1,y2;
    x1=x-7;
    x2=x+7;
    y1=y-6;
    y2=y+6;
	setfillstyle(1,0);
    line(x1,y,x2,y);
    line(x,y1,x,y2);
}
/*******************************************************************************
** 函数名称：init
** 函数描述：鼠标器初始化
** 输入参数：无
** 输出参数：无，当返回值为0时(即返回参数为0)，
表示未安装成功，这可能是鼠标器或驱动程序末安装。这时程序将显示Mouse or Mouse Driver Absent，并回到系统。
*******************************************************************************/
int init(int xmi,int xma,int ymi,int yma)   
{
    int retcode;
    regs.x.ax=0;
    int86(51,&regs,&regs);
    retcode=regs.x.ax;
    if(retcode==0)
        return 0;   /*  返回0表示鼠标或鼠标驱动程序未安装   */
    regs.x.ax=7;
    regs.x.cx=xmi;
    regs.x.dx=xma;
    int86(51,&regs,&regs);
    regs.x.ax=8;
    regs.x.cx=ymi;
    regs.x.dx=yma;
    int86(51,&regs,&regs);  /*  表示鼠标器和驱动程序已安装  */
    return retcode;
}
/*******************************************************************************
** 函数名称：mouseread
** 函数描述：读鼠标的位置和按钮状态
** 输入参数：鼠标的x、y位置值将由指针mx和my给出，而按钮状态则由mbutt指针给出
** 输出参数：无
*******************************************************************************/
int mouseread(int *mx,int *my,int *mbutt)    
{
    int xx0=*mx,yy0=*my,but0=0,mb;
    int xnew, ynew;
    do  {
            regs.x.ax=3;
            int86(51,&regs,&regs);
            xnew=regs.x.cx;
            ynew=regs.x.dx;
            *mbutt=regs.x.bx;
    }   while(xnew==xx0 && ynew==yy0 && *mbutt == but0 );
    *mx=xnew;
    *my=ynew;
    mb=(*mbutt);
    if((mb==1)||(mb==2)) return 1;  /*键按下*/
    else
        return 0;
}
/*******************************************************************************
** 函数名称：newxy
** 函数描述：检测鼠标的移动
** 输入参数：鼠标的x、y位置值将由指针mx和my给出，而按钮状态则由mbutt指针给出
** 输出参数：鼠标的x,y位置
*******************************************************************************/
void newxy(int *mx,int *my,int *mbutt)
{
    int ch,xx0=*mx,yy0=*my,x,y;
    int xm,ym;
    ch=mouseread(&xm,&ym,mbutt);
    switch (ch) {
    case 0:
        cursor(xx0,yy0);
        cursor(xm,ym);
        break;
    case 1:
        cursor(xx0,yy0);
        cursor(xm,ym);
        break;
    }
    *mx=xm;
    *my=ym;
}

/*******************************************************************************
** 函数名称：drawgameline
** 函数描述：画游戏界面
** 输入参数：无
** 输出参数：无
*******************************************************************************/
void drawgameline(void)
{
  int i ;
 for ( i = 0 ; i<14 ; i++ ){
    setlinestyle( 0 , 0 , 3 ) ;
    setcolor (RED) ;
    line( gameline [ i ][ 0 ] , gameline [ i ][ 1 ] , gameline [ i ][ 2 ] , gameline [ i ][ 3 ] ) ;
	}
  	setlinestyle(0,0,3);
	setcolor(BLUE);
	line(35,35,35,325);
	line(35,35,605,35);
	line(35,325,605,325);
	line(605,35,605,325);
	setcolor(BROWN);
	for(i=0;i<32;i++){
	setcolor(6);
		setfillstyle(1,6);
		fillellipse(gamepixy[i][0],gamepixy[i][1],30,30);
		drawbox(i);}
		
	setlinestyle( 0, 0, 3);
	setcolor(BROWN);
	line(60,380,60,425);
	line(60,380,220,380);
	line(110,380,110,425);
	line(220,380,220,425);
	line(60,425,220,425);
	
	line(240,380,240,425);
	line(240,380,400,380);
	line(290,380,290,425);
	line(400,380,400,425);
	line(240,425,400,425);
	drawmat(feng32HH,32,125,385,4);
	drawmat(jie32HH,32,170,385,4);
	drawmat(chun32HH,32,310,385,1);
	drawmat(ge32HH,32,355,385,1);
	setcolor(15);
	setfillstyle(1,15);
	fillellipse(86,405,14,14);
}
/*******************************************************************************
** 函数名称：drawbox
** 函数描述：画棋子
** 输入参数：坐标i
** 输出参数：无
*******************************************************************************/
int drawbox (int i){
	switch(StateMap[i][0])
	{
	case 5:
		setcolor(10);
		setfillstyle(1,6);
		fillellipse(gamepixy[i][0],gamepixy[i][1],32,32);
		break;
	case 1:
		setcolor(10);
		setfillstyle(1,6);
		fillellipse(gamepixy[i][0],gamepixy[i][1],32,32);
		setcolor(15);
		setfillstyle(1,6);
		fillellipse(gamepixy[i][0],gamepixy[i][1],30,30);
		switch(StateMap[i][1]){
			case 1:drawmat(che48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,4);break;
			case 2:drawmat(ma48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,4);break;
			case 3:drawmat(pao148J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,4);break;
			case 4:drawmat(xiang148J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,4);break;
			case 5:drawmat(shi148J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,4);break;
			case 6:drawmat(shuai48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,4);break;
			case 7:drawmat(bing48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,4);break;
			}
		break;
	case 2:
		setcolor(10);
		setfillstyle(1,6);
		fillellipse(gamepixy[i][0],gamepixy[i][1],32,32);
		setcolor(15);
		setfillstyle(1,6);
		fillellipse(gamepixy[i][0],gamepixy[i][1],30,30);
		switch(StateMap[i][1]){
			case 1:drawmat(che48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,1);break;
			case 2:drawmat(ma48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,1);break;
			case 3:drawmat(pao248J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,1);break;
			case 4:drawmat(xiang248J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,1);break;
			case 5:drawmat(shi248J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,1);break;
			case 6:drawmat(jiang48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,1);break;
			case 7:drawmat(zu48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,1);break;
			}
		break;
	case 3:
	case 4:
		setcolor(10);
		setfillstyle(1,6);
		fillellipse(gamepixy[i][0],gamepixy[i][1],32,32);
		setcolor(6);
		setfillstyle(1,6);
		fillellipse(gamepixy[i][0],gamepixy[i][1],30,30);
		break;
	}
	
}
/*******************************************************************************
** 函数名称：preparestage
** 函数描述：布阵
** 输入参数：无
** 输出参数：无
*******************************************************************************/
void preparestage (void) {
 int i,x,y,k,j ;
 int chance=Change;
 srand(time(NULL));/*打乱顺序*/
 for  ( i=1 ; i<=chance ; i++ )
 {
    x=rand ( )% 32;
    y=rand ( )% 32;
    k= ListCareer [ x ][ 0 ] ; ListCareer [ x ][ 0 ] = ListCareer [ y ][ 0 ] ; ListCareer [ y ][ 0 ] = k ;
    k= ListCareer [ x ][ 1 ] ; ListCareer [ x ][ 1 ] = ListCareer [ y ][ 1 ] ; ListCareer [ y ][ 1 ] = k ;
 }
 
 for (i=0;i<32;i++)
 {
  if  ( ListCareer [ i ][ 0 ] == 1 )
  {
    StateMap[ i ][ 0 ] = 3;
    StateMap[ i ][ 1 ] = ListCareer [ i ][ 1 ] ;
  }
  else
  { 
    StateMap[ i ][ 0 ] = 4;
    StateMap[ i ][ 1 ] = ListCareer [ i ][ 1 ] ;
   }
  }
}
/*******************************************************************************
** 函数名称：zuixuan
** 函数描述：开场音乐：最炫民族风！
** 输入参数：无
** 输出参数：无
*******************************************************************************/
void zuixuan()
{
	int i;
	for(i=0;i<22;i++){
		sound(freq[i]);
		delay(85*dely[i]);
		nosound();
		delay(20);
		}
}
/*******************************************************************************
这些是在下棋时的音效
*******************************************************************************/
void openmid()
{
sound(2000);
delay(100);
nosound();
}

void cancelmid()
{
sound(300);
delay(100);
nosound();
}
void pressmid()
{
sound(500);
delay(100);
nosound();
}

void stepmid()
{
sound(1000);
delay(100);
nosound();
}

void killmid()
{
sound(1500);
delay(100);
nosound();
}
/*******************************************************************************
** 函数名称：keguan
** 函数描述：游戏音乐：客官不可以
** 输入参数：~
** 输出参数：~
*******************************************************************************/
void keguan(void){
	int i;
	for(i=0;i<13;i++){
		sound(freq1[i]);
		delay(110*dely1[i]);
		nosound();
		delay(20);
	}
}
/*******************************************************************************
** 函数名称：intel
** 函数描述：动画转场音乐：intel广告曲
** 输入参数：~
** 输出参数：~
*******************************************************************************/
void intel(void){
	int i;
	for(i=0;i<5;i++){
		sound(freq2[i]);
		delay(110*dely2[i]);
		nosound();
		delay(20);
	}
}
/*******************************************************************************
** 函数名称：canarrive
** 函数描述：判断是否能移动
** 输入参数：初始坐标start，目标坐标final
** 输出参数：1表示可以，0表示不可以
*******************************************************************************/
int canarrive ( int start , int final ){
	int i;
	if((StateMap[final][0]==3)&&(StateMap[final][0]==4))return 0;
	else if((start-final==1)&&(start!=0)&&(start!=8)&&(start!=16)&&(start!=24))return 1;
	else if((final-start==1)&&(start!=7)&&(start!=15)&&(start!=23)&&(start!=31))return 1;
	else if((start-final==8)&&(start>=8))return 1;
	else if((final-start==8)&&(start<=23))return 1;
	return 0;
}
/*******************************************************************************
** 函数名称：trydraw
** 函数描述：测试
** 输入参数：鼠标的x、y位置值将由指针mx和my给出，而按钮状态则由mbutt指针给出
** 输出参数：鼠标的x,y位置
*******************************************************************************/
void trydraw(void){
	int i,flag,p;
	for(i=0;i<32;i++){
		p=StateMap[i][0];
		if(StateMap[i][0]==3)flag=0;
		if(StateMap[i][0]==4)flag=1;
		switch(StateMap[i][1]){
			case 1:drawmat(che48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;
			case 2:drawmat(ma48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;
			case 3:{
				if(flag==0){drawmat(pao148J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				if(flag==1){drawmat(pao248J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				}
			case 4:{
				if(flag==0){drawmat(xiang148J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				if(flag==1){drawmat(xiang248J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				}
			case 5:{
				if(flag==0){drawmat(shi148J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				if(flag==1){drawmat(shi248J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				}
			case 6:{
				if(flag==0){drawmat(shuai48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				if(flag==1){drawmat(jiang48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				}
			case 7:{
				if(flag==0){drawmat(bing48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				if(flag==1){drawmat(zu48J,48,gamepixy[i][0]-24,gamepixy[i][1]-24,p);break;}
				}
			}
		}
}
/*******************************************************************************
** 函数名称：compare
** 函数描述：比较两个棋子那个获胜
** 输入参数：两个棋子的信息X,Y
** 输出参数：是否获胜的信息(bool)
*******************************************************************************/
int compare(int x,int y)
{
	int atk,def;
	atk = x;
	def = y;
	if(atk==1){
		if((def==1)||(def==2)||(def==3)||(def==7))return 1;
		else return 0;}
	else if(atk==2){
		if((def==2)||(def==3)||(def==7))return 1;
		else return 0;}
	else if(atk==3)return 1;
	else if(atk==4){
		if((def==1)||(def==2)||(def==3)||(def==4)||(def==7))return 1;
		else return 0;}
	else if(atk==5){
		if((def==1)||(def==2)||(def==3)||(def==4)||(def==5)||(def==7))return 1;
		else return 0;}
	else if(atk==6){
		if((def==1)||(def==2)||(def==3)||(def==4)||(def==5)||(def==6))return 1;
		else return 0;}
	else if(atk==7){
		if((def==6)||(def==7))return 1;
		else return 0;}
	return 0;
}
/*******************************************************************************
** 函数名称：delete
** 函数描述：擦除棋子
** 输入参数：棋子的坐标
** 输出参数：无
*******************************************************************************/
int delete(int i)
{
		setcolor(10);
		setfillstyle(1,10);
		fillellipse(gamepixy[i][0],gamepixy[i][1],32,32);
}

/*******************************************************************************
** 函数名称：doubledrawbox
** 函数描述：画选定的棋子
** 输入参数：坐标i
** 输出参数：无
*******************************************************************************/
void doubledrawbox(int i)
{
	setlinestyle( 0 , 0 , 3 ) ;
    setcolor(14);
	circle(gamepixy[i][0],gamepixy[i][1],30);
}
/*******************************************************************************
** 函数名称：drawpoint
** 函数描述：画轮到谁走棋
** 输入参数：Sstep
** 输出参数：无 
*******************************************************************************/
int drawpoint(int i){
	if(i%2==1){
		setfillstyle(1,15);
		fillellipse(268,405,14,14);
		setfillstyle(1,10);
		fillellipse(86,405,14,14);
		}
	else{
		setfillstyle(1,10);
		fillellipse(268,405,14,14);
		setfillstyle(1,15);
		fillellipse(86,405,14,14);
		}
}
/*******************************************************************************
** 函数名称：renshu
** 函数描述：认输
** 输入参数：无
** 输出参数：无 
*******************************************************************************/
void renshu(void)
{
    setfillstyle ( 1 , 15 ) ;
    bar ( 200 , 200 , 400 , 300 ) ;
    setcolor ( 2 ) ;
    rectangle ( 201 , 201 , 399 , 299 ) ;
    setcolor ( 13 ) ;
    rectangle ( 203 , 203 , 397 , 297 ) ;
    drawmat(ren32WW,32,220 , 230 ,13);
    drawmat(shu32WW,32,260 , 230 ,13);
    drawmat(la32WW,32,300 , 230 ,13);
    drawmat(plaint32WW,32,340 , 230 ,13);
    keguan();
    if(Sstep%2!=0){redwin();}
    else {bluewin();}
}

/*******************************************************************************
** 函数名称：gamestart
** 函数描述：游戏主体
** 输入参数：无
** 输出参数：无 
*******************************************************************************/
void gamestart(void){
 int  i , x , compared , j , k ,anjian;
    int buttons,xm,ym,x0,y0,mousex,mousey;
    drawmat(jie32H,32,443,403,1);
    drawmat(shu32H,32,483,403,1);
    drawmat(you32H,32,523,403,1);
    drawmat(xi32H,32,563,403,1);
    First = 0;
    xmin=2;
    xmax=x_max-1;
    ymin=8;
    ymax=y_max-2;
    setwritemode(XOR_PUT);
    mousex=320;mousey=240;
    cursor(mousex,mousey);    /*  置十字光标在屏幕中心。  */

    Remain[0]=16;
    Remain[1]=16;
    while(bioskey(1)==0){
        newxy(&mousex,&mousey,&buttons);
        if(mousex>=440&& mousex<=600&&mousey>=400&&mousey<=440&& buttons)
        exit(0); /*十字光标移到结束处时*/
    	{
            x = -1 ;
            for ( i = 0 ; i<32 ; i++ ) 
    		{
                if (( mousex >= Map [ i ][ 0 ] ) && ( mousey >= Map [ i ][ 1 ] ) && (mousex <= Map [ i ][ 2 ] ) && ( mousey <= Map [ i ][ 3 ] )&&buttons)
        		{ 
                          x = i ;
                          break ;
        		}
    		}
            if ( x== -1 ) 
    		{
    			
                continue ;
    		}
            switch(StateMap[x][0])
            { 
            case 5 :if ( First == 0 ) break;
    			
        		else
        		{
                    if(1)/*先手是我方*/
        			{ 
                        if ( canarrive ( FirstNum , x )==1 )/*能够移动*/
        				{
                            StateMap [ x ][ 0 ] = StateMap [ FirstNum ][ 0 ];
                            StateMap [ x ][ 1 ] = StateMap [ FirstNum ][ 1 ];
                            StateMap [ FirstNum ][ 0 ]= 5 ;
                            StateMap [ FirstNum ][ 1 ]= -1 ;
                        	drawbox ( x ) ;
                            delete ( FirstNum ) ;
                    		stepmid();
                			nostep++;
                            if(nostep>16)pingju();
                    		Sstep ++ ;
                        	drawpoint(Sstep);
                    		First = 0 ;
                			break;
        				}
            			else
            			{  
                            drawbox(FirstNum) ;
                			First=0;
                    		cancelmid();
                			break ; 
        				}
        			}
            		else 
        			{
                        drawbox ( FirstNum ) ;
                		First=0;
                    	cancelmid();
                		break ;  
        			}
        		}
    			
            case 4 : case 3 :
    		{
            if(First!=0){
                drawbox(FirstNum) ;
            	First=0;
                cancelmid();
            	break;
        		}
        	else{
                StateMap [ x ][ 0 ] = StateMap [ x ][ 0 ]- 2 ;
                drawbox (x) ;
                openmid();
                Sstep ++ ;
                nostep=0;
                drawpoint(Sstep);
                First = 0 ;
            	break ;
        		}
    		}
            case 1 : case 2 :
            if ( First == 0 ) 
                {
                    if  (Sstep%2==(StateMap[x][0]-1))/*判断是否为我方的棋*/
        			{ 
                    	First = 1 ;
                    	FirstNum = x ;
                    	pressmid();
                        doubledrawbox ( FirstNum ) ;
            			break;
        			}
                    else break ; 
                }
        	else{

                if ((StateMap[FirstNum][0])==(StateMap[x][0])) /*判断目标棋子是不是我方的*/
        		{
                    drawbox ( FirstNum ) ;
                	First = 0;
            		break ;
                }
                else
                    if (0){/*判断起始棋子是不是我方的*/
                        drawbox ( FirstNum ) ;
                        First = 0 ;
                    	cancelmid();
                        break ;
                        }
                    else 
                	{
                    compared = compare(StateMap[FirstNum][1],StateMap[x][1]);
                    if ( canarrive ( FirstNum , x )==1 ) /*判断能否通行*/{

                        if(compared==1)/*胜*/{
                            if(StateMap[x][0]%2==0)Remain[0]--;
                        	else Remain[1]--;
                            StateMap[x][0] = StateMap[FirstNum][0];
                            StateMap[x][1] = StateMap[FirstNum][1];
                            StateMap[FirstNum][0]=5;
                    		drawbox(x);
                    		killmid();
                    		Sstep ++ ;
                			nostep=0;
                        	drawpoint(Sstep);
                    		First = 0;
                        	delete(FirstNum);
                            if(Remain[0]==0)redwin();
                            if(Remain[1]==0)bluewin();
        				}
            			else{
                			First=0;
                            drawbox ( FirstNum ) ;
                    		cancelmid();
                			break;
        				}
        			}
            		else
        			{
                		First=0;
                        drawbox ( FirstNum ) ;
                    	cancelmid();
            			break;
        			}
        		}
                    }
    		}
    	}
    }anjian = bioskey(0);
    if(anjian==lowerp)renshu();
    if(anjian==lowerh)takehelp();
    if(anjian==lowerl)bluewin();
}

 /******************************************************************************
**函数名称：cartoon
**函数描述：游戏开始时的过场动画(“开”字和“战”字互相碰撞）
**输入参数：无
**输出参数：无
******************************************************************************/
void cartoon(void)
{
int i,x0,y0,a0,b0;
    int size;
    void *buffer;

    cleardevice();	
    setbkcolor(8);
    setfillstyle(1,14);
    setlinestyle(0,0,4);

    fillellipse(25,200,25,15);
    size=imagesize(0,175,50,225);
    buffer=malloc(size);
    getimage(0,175,50,225,buffer);
    putimage(570,175,buffer,COPY_PUT);

    for(i=0;i<=270/10;i++)
    {	
        putimage(0+i*10,175,buffer,COPY_PUT);
        putimage(570-i*10,175,buffer,COPY_PUT);
        cleardevice();

    }
    setfillstyle(1,14);
    fillellipse(320,200,5,5);
    delay(200);
    for(i=1;i<=6;i++)
    {
        fillellipse(320,200,10*i,10*i);
        delay(100-15*i);
    }
    delay(100);
    cleardevice();
    setfillstyle(1,4);
    for(i=0;i<=6;i++)
    {
        fillellipse(320,200,60-10*i,60-10*i);
        cleardevice();
    }
    drawmat(kai64W,64,255,170,4);
    drawmat(zhan64W,64,320,170,4);
}

/*******************************************************************************
** 函数名称：redwin
** 函数描述：红色胜
** 输入参数：无
** 输出参数：无 
*******************************************************************************/
int redwin(void){
   int buttons,mousex,mousey;
   setfillstyle ( 1 , 4 ) ;
   bar ( 200 , 200 , 400 , 300 ) ;
   setcolor ( 2 ) ;
   rectangle ( 201 , 201 , 399 , 299 ) ;
   setcolor ( 13 ) ;
   rectangle ( 203 , 203 , 397 , 297 ) ;
   drawmat(feng32MQ,32,220,215,15);
   drawmat(jie32MQ,32,260,215,15);
   drawmat(ba32MQ,32,300,215,15);
   drawmat(qi32MQ,32,340,215,15);

   setfillstyle ( 1 , 12 ) ;
   bar(230,270,290,290);
   bar(320,270,380,290);
   rectangle(231,271,289,289);
   rectangle(321,271,379,289);
   setcolor(2);
   outtextxy(239,277,"Quit");
   outtextxy(325,277,"Restart");
   while ( 1 ) {
    newxy(&mousex,&mousey,&buttons);
    if (buttons== 1 ) 
    {
      if (( mousex>=230 ) && ( mousex <= 280 ) && ( mousey >=270 ) && ( mousey <=290 ))
      {
        closegraph();
        exit (0);
      }
      else 
      if (( mousex>=320 ) && ( mousex <=370 ) && ( mousey >= 270 ) && ( mousey <= 290 ))
      {
        closegraph();
        initgr();
        startpicture();
		cartoon();
		intel();
		cleardevice();
        setbkcolor(10);
		preparestage();
        drawgameline();
        gamestart();
        break;
      } 
    }
  }
}
/*******************************************************************************
** 函数名称：bluewin
** 函数描述：蓝色胜
** 输入参数：无
** 输出参数：无 
*******************************************************************************/
int bluewin(void){
   int buttons,mousex,mousey;
   setfillstyle ( 1 , 1 ) ;
   bar ( 200 , 200 , 400 , 300 ) ;
   rectangle ( 201 , 201 , 399 , 299 ) ;
   setcolor ( 13 ) ;
   rectangle ( 203 , 203 , 397 , 297 ) ;
   drawmat(chun32MQ,32,220,215,15);
   drawmat(ge32MQ,32,260,215,15);
   drawmat(wei32MQ,32,300,215,15);
   drawmat(wu32MQ,32,340,215,15);
   setfillstyle ( 1 , 9 ) ;
   bar(230,270,290,290);
   bar(320,270,380,290);
   rectangle(231,271,289,289);
   rectangle(321,271,379,289);
   setcolor(2);
   outtextxy(239,277,"Quit");
   outtextxy(325,277,"Restart");
   while ( 1 ) {
    newxy(&mousex,&mousey,&buttons);
    if (buttons==1) 
    {
      if (( mousex>=230 ) && ( mousex <= 280 ) && ( mousey >=270 ) && ( mousey <=290 ))
      {
        closegraph();
        exit (0);
      }
      else 
      if (( mousex>=320 ) && ( mousex <=370 ) && ( mousey >= 270 ) && ( mousey <= 290 ))
      {
        closegraph();
        initgr();
        startpicture();
		cartoon();
		intel();
		cleardevice();
        setbkcolor(10);
		preparestage();
        drawgameline();
        gamestart();
        break;
      } 
    }
   }
}
/*******************************************************************************
** 函数名称：pingju
** 函数描述：平局
** 输入参数：无
** 输出参数：无
*******************************************************************************/
int pingju(void){
   int buttons,mousex,mousey;
   setfillstyle ( 1 , 15) ;
   bar ( 200 , 200 , 400 , 300 ) ;
   rectangle ( 201 , 201 , 399 , 299 ) ;
   setcolor ( 13 ) ;
   rectangle ( 203 , 203 , 397 , 297 ) ;
   drawmat(bu32MQ,32,220,215,2);
   drawmat(fen32MQ,32,260,215,2);
   drawmat(shang32MQ,32,300,215,2);
   drawmat(xia32MQ,32,340,215,2);
   setfillstyle ( 1 , 14 ) ;
   bar(230,270,290,290);
   bar(320,270,380,290);
   rectangle(231,271,289,289);
   rectangle(321,271,379,289);
   setcolor(2);
   outtextxy(239,277,"Quit");
   outtextxy(325,277,"Restart");
   while ( 1 ) {
    newxy(&mousex,&mousey,&buttons);
    if ( buttons == 1 )
    {
      if ((mousex>=230 ) && ( mousex <= 280 ) && ( mousey >=270 ) && ( mousey <=290 ))
      {
        closegraph();
        exit (0);
      }
      else
      if (( mousex>=320 ) && ( mousex <=370 ) && ( mousey >= 270 ) && ( mousey <= 290 ))
      {
        closegraph();
        initgr();
        startpicture();
		cartoon();
		intel();
		cleardevice();
        setbkcolor(10);
		preparestage();
        drawgameline();
        gamestart();
        break;
      } 
    }
  }
 }
 int takehelp(void)
 {
	Set_Video_Mode(VGA256);
    BMP_Load_Screen("257.bmp");
	delay(5000);
	initgr();
	cleardevice();
    setbkcolor(10);
    drawgameline();
    gamestart();
}
