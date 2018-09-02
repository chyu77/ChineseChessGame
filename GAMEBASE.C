
/*�趨��ʼ�����ز���*/
int tankstart=0,planstart=0;
int first=1;
union REGS regs;
int init();
int mouseread();
void cursor(),newxy();
int xmin,xmax,ymin,ymax,x_max=639,y_max=479;

/*******************************************************************************
** ����������BMP�ļ�
*******************************************************************************/
/*******************************************************************************
** �������ƣ�Set_BMP_Palette_Register
** ����������д��BMP����Ļ��
** �����������
** ����������� 
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
** �������ƣ�Check_Bmp
** ����������BMP���
** �����������
** �������������д��������Ӧ������Ϣ���˳�
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
** �������ƣ�drawmat
** �������������Σ���ģָ�롢�����С����ʼ����(x,y)����ɫ
** ���������*mat,matsize,x,y,color
** �����������
*******************************************************************************/
void drawmat(char *mat,int matsize,int x,int y,int color)
{
  int i, j, k, n;
  n = (matsize - 1) / 8 + 1;
  for(j = 0; j < matsize; j++)
    for(i = 0; i < n; i++)
      for(k = 0;k < 8; k++)
        if(mat[j * n + i] & (0x80 >> k))  /*����Ϊ1��λ����ʾ*/
          putpixel(x + i * 8 + k, y + j, color);
}

/*******************************************************************************
** �������ƣ�initgr
** ����������BGI��ʼ��
** �����������
** �����������
*******************************************************************************/
void initgr(void)
{
  int gd = VGA, gm = VGAHI; 

  initgraph(&gd, &gm, "");
}

/*******************************************************************************
** �������ƣ�startpicture
** ������������ʾ��������
** �����������
** �����������
*******************************************************************************/
void startpicture(void)
{

    int buttons,xm,ym,x0,y0,x,y;
    char str[100];

    int i;
    setbkcolor(LIGHTBLUE);
/*drawmat ���� ���Σ���ģָ�롢�����С����ʼ����(x,y)����ɫ*/
/*�����й�����*/
    drawmat(huan64M,64,60,50,6);
    drawmat(le64M,64,140,50,7);
    drawmat(zhong64M,64,220,50,8);
    drawmat(guo64M,64,300,50,5);
    drawmat(an64M,64,380,50,12);
    drawmat(qi64M,64,460,50,11);

/*�����ָ��*/
    drawmat(lv24M,24,100,350,11);
    drawmat(hong24M,24,130,350,11);
    drawmat(bing24M,24,160,350,11);
    drawmat(zhi24M,24,200,350,11);
    drawmat(dao24M,24,230,350,11);
    
/*������� ����*/
    drawmat(wen24M,24,100,380,11);
    drawmat(tian24M,24,130,380,11);
    drawmat(yu24M,24,160,380,11);
    drawmat(li24M,24,210,380,11);
    drawmat(kai24M,24,240,380,11);
    drawmat(zhi24S,24,310,380,11);
    drawmat(zuo24M,24,340,380,11);
/*�㽭��ѧ*/
    drawmat(zhe32M,32,200,420,11);
    drawmat(jiang32M,32,250,420,11);
    drawmat(da32M,32,300,420,11);
    drawmat(xue32M,32,350,420,11);
/*��˧�ͽ�*/
    setcolor(6);
    setfillstyle(1,14);
    fillellipse(173,234,53,53);
    fillellipse(396,234,53,53);
    setfillstyle(1,6);
    fillellipse(173,234,50,50);
    fillellipse(396,234,50,50);
    drawmat(shuai64H,64,140,207,4);
    drawmat(jiang64H,64,366,204,11);
/*����ť*/
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
    if(init(xmin,xmax,ymin,ymax)==0 )   /*  ����init�������������ʼ��*/
    {
        printf("Mouse or Mouse Driver Absent,P1ease install!");         
                delay(5000);
                exit(1);
    }
    x=320;y=240;
    cursor(x,y);    /*  ��ʮ�ֹ������Ļ���ġ�  */
    for(;;) 
        {
        newxy(&x,&y,&buttons);
        if(x>=440&& x<=600 &&y>=400&&y<=440&& buttons) /*ʮ�ֹ���Ƶ�������ʱ*/
            {
            exit(0);    /*  �ص�ϵͳ*/
        }
        if(x>=440&& x<=600 &&y>=340&&y<=380&& buttons) /*ʮ�ֹ���Ƶ���ʼt��ʱ*/
            {
	    return;    /* ��ʼ��Ϸ*/
        }
    }
    
}
/*******************************************************************************
** �������ƣ�cursor
** ���������������
** �����������
** �����������
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
** �������ƣ�init
** �����������������ʼ��
** �����������
** ����������ޣ�������ֵΪ0ʱ(�����ز���Ϊ0)��
��ʾδ��װ�ɹ�������������������������ĩ��װ����ʱ������ʾMouse or Mouse Driver Absent�����ص�ϵͳ��
*******************************************************************************/
int init(int xmi,int xma,int ymi,int yma)   
{
    int retcode;
    regs.x.ax=0;
    int86(51,&regs,&regs);
    retcode=regs.x.ax;
    if(retcode==0)
        return 0;   /*  ����0��ʾ���������������δ��װ   */
    regs.x.ax=7;
    regs.x.cx=xmi;
    regs.x.dx=xma;
    int86(51,&regs,&regs);
    regs.x.ax=8;
    regs.x.cx=ymi;
    regs.x.dx=yma;
    int86(51,&regs,&regs);  /*  ��ʾ����������������Ѱ�װ  */
    return retcode;
}
/*******************************************************************************
** �������ƣ�mouseread
** ����������������λ�úͰ�ť״̬
** �������������x��yλ��ֵ����ָ��mx��my����������ť״̬����mbuttָ�����
** �����������
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
    if((mb==1)||(mb==2)) return 1;  /*������*/
    else
        return 0;
}
/*******************************************************************************
** �������ƣ�newxy
** ������������������ƶ�
** �������������x��yλ��ֵ����ָ��mx��my����������ť״̬����mbuttָ�����
** �������������x,yλ��
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
** �������ƣ�drawgameline
** ��������������Ϸ����
** �����������
** �����������
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
** �������ƣ�drawbox
** ����������������
** �������������i
** �����������
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
** �������ƣ�preparestage
** ��������������
** �����������
** �����������
*******************************************************************************/
void preparestage (void) {
 int i,x,y,k,j ;
 int chance=Change;
 srand(time(NULL));/*����˳��*/
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
** �������ƣ�zuixuan
** �����������������֣���������磡
** �����������
** �����������
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
��Щ��������ʱ����Ч
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
** �������ƣ�keguan
** ������������Ϸ���֣��͹ٲ�����
** ���������~
** ���������~
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
** �������ƣ�intel
** ��������������ת�����֣�intel�����
** ���������~
** ���������~
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
** �������ƣ�canarrive
** �����������ж��Ƿ����ƶ�
** �����������ʼ����start��Ŀ������final
** ���������1��ʾ���ԣ�0��ʾ������
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
** �������ƣ�trydraw
** ��������������
** �������������x��yλ��ֵ����ָ��mx��my����������ť״̬����mbuttָ�����
** �������������x,yλ��
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
** �������ƣ�compare
** �����������Ƚ����������Ǹ���ʤ
** ����������������ӵ���ϢX,Y
** ����������Ƿ��ʤ����Ϣ(bool)
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
** �������ƣ�delete
** ������������������
** ������������ӵ�����
** �����������
*******************************************************************************/
int delete(int i)
{
		setcolor(10);
		setfillstyle(1,10);
		fillellipse(gamepixy[i][0],gamepixy[i][1],32,32);
}

/*******************************************************************************
** �������ƣ�doubledrawbox
** ������������ѡ��������
** �������������i
** �����������
*******************************************************************************/
void doubledrawbox(int i)
{
	setlinestyle( 0 , 0 , 3 ) ;
    setcolor(14);
	circle(gamepixy[i][0],gamepixy[i][1],30);
}
/*******************************************************************************
** �������ƣ�drawpoint
** �������������ֵ�˭����
** ���������Sstep
** ����������� 
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
** �������ƣ�renshu
** ��������������
** �����������
** ����������� 
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
** �������ƣ�gamestart
** ������������Ϸ����
** �����������
** ����������� 
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
    cursor(mousex,mousey);    /*  ��ʮ�ֹ������Ļ���ġ�  */

    Remain[0]=16;
    Remain[1]=16;
    while(bioskey(1)==0){
        newxy(&mousex,&mousey,&buttons);
        if(mousex>=440&& mousex<=600&&mousey>=400&&mousey<=440&& buttons)
        exit(0); /*ʮ�ֹ���Ƶ�������ʱ*/
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
                    if(1)/*�������ҷ�*/
        			{ 
                        if ( canarrive ( FirstNum , x )==1 )/*�ܹ��ƶ�*/
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
                    if  (Sstep%2==(StateMap[x][0]-1))/*�ж��Ƿ�Ϊ�ҷ�����*/
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

                if ((StateMap[FirstNum][0])==(StateMap[x][0])) /*�ж�Ŀ�������ǲ����ҷ���*/
        		{
                    drawbox ( FirstNum ) ;
                	First = 0;
            		break ;
                }
                else
                    if (0){/*�ж���ʼ�����ǲ����ҷ���*/
                        drawbox ( FirstNum ) ;
                        First = 0 ;
                    	cancelmid();
                        break ;
                        }
                    else 
                	{
                    compared = compare(StateMap[FirstNum][1],StateMap[x][1]);
                    if ( canarrive ( FirstNum , x )==1 ) /*�ж��ܷ�ͨ��*/{

                        if(compared==1)/*ʤ*/{
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
**�������ƣ�cartoon
**������������Ϸ��ʼʱ�Ĺ�������(�������ֺ͡�ս���ֻ�����ײ��
**�����������
**�����������
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
** �������ƣ�redwin
** ������������ɫʤ
** �����������
** ����������� 
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
** �������ƣ�bluewin
** ������������ɫʤ
** �����������
** ����������� 
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
** �������ƣ�pingju
** ����������ƽ��
** �����������
** �����������
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