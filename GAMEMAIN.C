#include<graphics.h>
#include<io.h>
#include<stdio.h>
#include<dos.h>
#include<string.h>
#include<math.h>
#include<stdio.h>
#include<bios.h>
#include<mem.h>
#include<fcntl.h>
#include<stdlib.h>
#include<conio.h>

#define SCREEN_HEIGHT 200
#define SCREEN_WIDTH 320
#define PALETTE_MASK 0x3c6
#define PALETTE_REGISTER_RD 0x3c7
#define PALETTE_REGISTER_WR 0x3c8
#define PALETTE_DATA 0x3c9
#define VGA256 0x13
#define TEXT_MODE 0x03
#define lowerp 0x1970
#define lowerh 0x2368
#define lowerl 0x266c
#define closegr closegraph

#include "GAMEDATA.C"
#include "GAMEBASE.C"

 /*******************************************************************************
** 函数名称：main
** 函数描述：主函数
** 输入参数：无
** 输出参数：无
*******************************************************************************/
main()    
{
    union REGS regs;  /*必须放在第一行*/
    Set_Video_Mode(VGA256);
    BMP_Load_Screen("256.bmp");
    zuixuan();
    initgr();           /* BGI初始化      */
    startpicture();     /* 启动界面       */
	cartoon();
	intel();
    cleardevice();
    setbkcolor(10);
    drawgameline();
    preparestage();
    gamestart();
    bioskey(0);
}