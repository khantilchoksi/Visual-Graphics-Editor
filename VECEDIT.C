/*
  Vector Editor.
*/

#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include<string.h>
#include<io.h>
#define MAX 10000

int gx,gy,tool=0,color=0,toolp[6][2]={{10+7,132+7},{65+7,132+7},{10+7,187+7},{65+7,187+7},{10+7,243+7},{65+7,243+7}};
float nx1,ny1,nx2,ny2,ncx,ncy,nrx,nry;
union REGS in,out;
FILE *fo,*fn;
int of=0,nf;
int width=509,height=440,wstart=130,hstart=20;
char fna[12];


void dummy(float*);
void canvas();
void showMouse();
void callMouse();
void hideMouse();
int checkClickMouse(int*,int*);
float mod(float);
void drawLine(int,int,int,int);
int modInt(int);
int sign(float);
void push(int,int[],int*);
int pop(int[],int*);
void seedFill(int,int);
void clearTitle();
void setTitle();
void interpret();
void openButtonClicked();
void saveButtonClicked();
void newButtonClicked();
int inRangeX(int,int,int);
int inRangeY(int,int,int);
int inRangeXY(int,int,int,int,int,int);
void selectColor(int);
void selectTool(int);
void mouseClicked(int,int);
void mouseDragged(int,int,int,int);

void main()
{
  int gd=0,gm,x,y,x1,y1,x2,y2,f=0,b=0;

  clrscr();
  initgraph(&gd,&gm,"..\\bgi");
  //setbkcolor(DARKGRAY);
  canvas();


  while(!kbhit())
  {

    b=checkClickMouse(&x,&y);
    if(b&&!f)
    {
      f=1;
      x1=x;
      y1=y;
    }
    if(b&&f)
    {
      x2=x;
      y2=y;
/*      cleardevice();
      canvas();
      showMouse();
      setcolor(color);
      if(tool==0)
	line(x1,y1,x2,y2);
      else if(tool==1)
	rectangle(x1,y1,x2,y2);
      else if(tool==2)
      {
	cx=(x1+x2)/2.0;
	cy=(y1+y2)/2.0;
	rx=mod(x1-x2)/2.0;
	ry=mod(y1-y2)/2.0;
	ellipse(cx,cy,0,360,rx,ry);
      }*/
    }
    if(!b&&f)
    {
      f=0;
      x2=x;
      y2=y;
      if(x1==x2&&y1==y2)
	{
 //         mouseHide();
	  mouseClicked(x1,y1);
   //       mouseShow();
	}
      else if(of)
      {
	mouseDragged(x1,y1,x2,y2);
      }

    }

  }
  getch();
}


void dummy(float *a)
{
  float *b=a;
  dummy(b);
}

void canvas()
{
  float i,j,d;
  int k;

  setcolor(15);
  color=15;
  rectangle(0,0,639,479);
  line(130,1,130,478);
  line(131,20,638,20);
  line(131,460,639,460);
  setcolor(15);
  color=15;
  rectangle(580,1,638,19);
  setcolor(14);
  color=14;
  setfillstyle(01,15);
  floodfill(201,21,15);
  setfillstyle(01,0);
  floodfill(620,10,15);
  settextstyle(3,0,1);
  outtextxy(588,-3,"Paint!");


  setcolor(15);
  //color=15;
			      //color plate
  for(i=10;i<93;i+=27.5)    //vertical
  {
    line(i,351,i,459);
  }

  for(i=350;i<433;i+=27.5)   //horizontal
  {
    line(10,i,119,i);
  }
  line(120,350,120,432.5);
  line(10,460,92.5,460);
  rectangle(100,440,120,460);    //current colour
  color=0;
  setfillstyle(9,color);
  floodfill(110,450,15);
  //filling colors in color box
  i=115;
  j=360;
  d=-27.5;
  for(k=1;k<=15;k++)
  {
    setfillstyle(01,k-1);
    floodfill(i,j,15);
    if(k%4==0)
    {
      i+=d;
      d=-d;
      j+=27.5;

    }
    i+=d;
  }

				 //open and save buttons
  rectangle(10,20,120,45);
  rectangle(10,55,120,80);
  rectangle(10,90,120,115);
  settextstyle(3,0,2);
  outtextxy(40,18,"Open");
  outtextxy(40,53,"Save");
  outtextxy(45,88,"New");

				    //toolbox
  rectangle(10,132.5,120,297.5);
  line(65,133,65,296);
  line(11,187,119,187);
  line(11,243,119,243);

  line(20,170,55,149);       //line tool
  rectangle(75,147,110,172);            //rectangle tool
  ellipse(37,214,0,360,17,12);         //ellipse tool
  settextstyle(7,0,5);
  outtextxy(80,245,"A");
  settextstyle(10,0,3);
  outtextxy(85,187,"F");               //fill color tool
				//cancel tool
  line(25,258,50,283);
  line(25,283,50,258);

  setfillstyle(9,10);
   fillellipse(toolp[tool][0],toolp[tool][1],3,3);
   setcolor(10);

   ellipse(toolp[tool][0],toolp[tool][1],0,360,3,3);

  if(of)
    interpret();
  callMouse();



  /*
  //move tool
  line(85,257,85,277);
  line(86,257,100,271);
  line(85,277,100,272);
  line(91,275,94,284);
  line(94,274,97,283);
  line(94,284,97,283);*/
}

void showMouse()
{
  in.x.ax=1;
	int86(51,&in,&out);
}

void callMouse()
{
  //enabling the mouse
  showMouse();

  //setting the horizontal limits
  in.x.ax=7;
  in.x.cx=0;
  in.x.dx=639;
  int86(51,&in,&out);

  //setting the vertical limits
  in.x.ax=8;
  in.x.cx=20;
  in.x.dx=460;
  int86(51,&in,&out);
}

void hideMouse()
{
  in.x.ax=2;
	int86(51,&in,&out);
}

int checkClickMouse(int *x,int *y)
{
  in.x.ax = 3;
   int86(51,&in,&out);


   *x=out.x.cx;
   *y=out.x.dx;
   return out.x.bx;
}

float mod(float x)
{
	if(x>=0)
		return x;
	else
		return -x;
}

void drawLine(int x1,int y1,int x2,int y2)
{
  int l,i;
  float x,y,dx,dy;
  if(modInt(x2-x1)>=modInt(y2-y1))
    l=modInt(x2-x1);
  else
    l=modInt(y2-y1);

  dx=(float)(x2-x1)/l;
  dy=(float)(y2-y1)/l;
  x=x1+0.5*sign(dx);
  y=y1+0.5*sign(dy);
  

  
  for(i=1;i<=l;++i)
  {
    putpixel(floor(x),floor(y),color);
    x+=dx;
    y+=dy;
  }
}

int sign(float x)
{
  if(x>0)
    return 1;
  else if(x<0)
    return -1;
  else
    return 0;
}

int modInt(int x)
{
	if(x>=0)
		return x;
	else
		return -x;
}

void push(int x,int ar[],int *top)
{	if(*top==MAX-1)
	{	printf("\n STACK OVERFLOW");
		getch();
	}
	else
	{	(*top)++;
		ar[*top]=x;
	}
}

int pop(int ar[],int *top)
{	int x;
	if(*top==-1)
	{	printf("\n STACK UNDERFLOW");
		getch();
		return -1;
	}
	else
	{	x=ar[*top];
		(*top)--;
		return x;
	}
}

void seedFill(int seedx,int seedy)
{
  int stx[MAX],sty[MAX],cblank,cfill,st1[508],top1=-1;
  int i,j,f,y,c1;
  int topx=-1,topy=-1,py1;
  int px,py;
  cblank=getpixel(seedx,seedy);
  push(seedx,stx,&topx);

  cfill=getcolor();

  {
    
    y=seedy;
    c1=getpixel(seedx,y);

    while(c1==cblank)
    {
      push(y,st1,&top1);
      y--;
      c1=getpixel(seedx,y);
    }
    
    y=seedy+1;
    c1=getpixel(seedx,y);
    while(c1==cblank)
    {
      push(y,st1,&top1);
      y++;
      c1=getpixel(seedx,y);
    }
    
  }
  
  
  while(top1!=-1)
  {
    py=pop(st1,&top1);
    push(seedx,stx,&topx);
      while(topx!=-1)
    {
      px=pop(stx,&topx);
		
		if(px==-1)
		{ break; }
		if(getpixel(px,py)==cblank)
		{ putpixel(px,py,cfill);

		}

    if(getpixel(px+1,py)==cblank)		//RIGHT PIXEL
		{	f=1;
      for(i=0;i<=topx;i++)
      {
        if(px+1==stx[topx]&&py==sty[topx])
          {
            f=0;
            break;
          }
      }
      if(f)
      {
        push(px+1,stx,&topx);

      }
		}
    
		if(getpixel(px-1,py)==cblank)		//LEFT PIXEL
		{	f=1;
      for(i=0;i<=topx;i++)
      {
        if(px-1==stx[topx]&&py==sty[topx])
          {
            f=0;
            break;
          }
      }
      if(f)
      {
        push(px-1,stx,&topx);
      }
		}
	} 
  }
}

void clearTitle()
{

}

void setTitle()
{

}

void interpret()
{
   char *inst,*str;
   float x1,y1,x2,y2,cx,cy,rx,ry,seedx,seedy;

   int oc;


   while(fgets(inst,100,fo)!=NULL)
   {
      sscanf(inst,"%d",&oc);
      switch(oc)
      {
         case 0:
              sscanf(inst,"%d %f %f %f %f %d",&oc,&nx1,&ny1,&nx2,&ny2,&color);
              setcolor(color);
              selectColor(color);
              x1=nx1*width+wstart;
              y1=ny1*height+hstart;
              x2=nx2*width+wstart;
              y2=ny2*height+hstart;
              line(x1,y1,x2,y2);

         break;

         case 1:
              sscanf(inst,"%d %f %f %f %f %d",&oc,&nx1,&ny1,&nx2,&ny2,&color);
              setcolor(color);
              selectColor(color);
              x1=nx1*width+wstart;
              y1=ny1*height+hstart;
              x2=nx2*width+wstart;
              y2=ny2*height+hstart;
              rectangle(x1,y1,x2,y2);
         break;

         case 2:
              sscanf(inst,"%d %f %f %f %f %d",&oc,&ncx,&ncy,&nrx,&nry,&color);
              setcolor(color);
              selectColor(color);
              cx=ncx*width+wstart;
              cy=ncy*height+hstart;
              rx=nrx*width;
              ry=nry*height;
              ellipse(cx,cy,0,360,rx,ry);
         break;

         case 3:
              sscanf(inst,"%d %f %f %d",&oc,&seedx,&seedy,&color);
              setcolor(color);
              selectColor(color);
              setfillstyle(1,color);
              seedx=seedx*width+wstart;
              seedy=seedy*height+hstart;
              hideMouse();
              floodfill(seedx,seedy,0);
              showMouse();
         break;

         case 4:
              //clear canvas and file
         break;

         case 5:
              sscanf(inst,"%d %f %f %d %s",&oc,&nx1,&ny1,&color,str);
              setcolor(color);
              selectColor(color);
              x1=nx1*width+wstart;
              y1=ny1*height+hstart;
              outtextxy(x1,y1,str);
         break;
      }
   }
}

void openButtonClicked()
{
   int gd=0,gm;
   gotoxy(130,0);
   closegraph();
   printf("Enter file name: ");
   gets(fna);
   initgraph(&gd,&gm,"..\\bgi");
   canvas();
//   strcat(fna,".df");
   fo=fopen(fna,"r+");
   interpret();
   of=1;
   //clearTitlfe();
   //setTitle();

}

void saveButtonClicked()
{
   fclose(fo);
}

void newButtonClicked()
{
  int gd=0,gm;
   gotoxy(130,0);
   closegraph();
   printf("Enter file name: ");
   gets(fna);
   initgraph(&gd,&gm,"..\\bgi");
   canvas();
   fo=fopen(fna,"w+");
   of=1;
   //clearTitle();
   //setTitle(fna);
}

int inRangeX(int x,int x1,int x2)
{
  if(x1<x&&x<x2)
    return 1;
  else if(x2<x&&x<x1)
    return 1;
  else
    return 0;
}

int inRangeY(int y,int y1,int y2)
{
  if(y1<y&&y<y2)
    return 1;
  else if(y2<y&&y<y1)
    return 1;
  else
    return 0;
}

int inRangeXY(int x,int y,int x1,int y1,int x2,int y2)
{
  return inRangeX(x,x1,x2)&&inRangeY(y,y1,y2);
}

void selectColor(int x)
{           //color set by inbuilt setcolor(int) has scope limited to
            //the respective block; so we've used global variable
            //color to make selected color available throught the program
   color=x;
    
   setfillstyle(9,color);
   floodfill(110,450,15);
}

void selectTool(int x)
{

   setfillstyle(9,0);
  fillellipse(toolp[tool][0],toolp[tool][1],3,3);
   setcolor(0);
   color=0;
   ellipse(toolp[tool][0],toolp[tool][1],0,360,3,3);
   tool=x;
   setfillstyle(9,10);
   fillellipse(toolp[tool][0],toolp[tool][1],3,3);
   setcolor(10);

   ellipse(toolp[tool][0],toolp[tool][1],0,360,3,3);
}

void mouseClicked(int x,int y)
{
  char str[20];
  if(inRangeXY(x,y,10,20,120,45))
    openButtonClicked();                      //open file
  else if(inRangeXY(x,y,10,55,120,80))
    saveButtonClicked();                  //save file
  else if(inRangeXY(x,y,10,90,120,105))
    newButtonClicked();                   //create new file
  else if(inRangeXY(x,y,10,132,65,187))
    selectTool(0);                               //line selectTool
  else if(inRangeXY(x,y,65,132,120,187))
    selectTool(1);                               //rectangle selectTool
  else if(inRangeXY(x,y,10,187,65,243))
    selectTool(2);                               //ellipse selectTool
  else if(inRangeXY(x,y,65,187,120,243))
    selectTool(3);                               //fill selectTool
  else if(inRangeXY(x,y,10,243,65,297))
    {
      if(of)
      {cleardevice();
      
      freopen(fna,"w+",fo);
      canvas();
      selectTool(4);}
    }                               //clear all
  else if(inRangeXY(x,y,65,243,120,297))
    selectTool(5);                               //text tool
  else if(inRangeX(x,10,37))
  {
    if(inRangeY(y,350,377))
      selectColor(3);
    else if(inRangeY(y,377,405))
      selectColor(4);
    else if(inRangeY(y,405,432))
      selectColor(11);
    else if(inRangeY(y,432,460))
      selectColor(12);
    //printf("%d",getcolor());
  }
  else if(inRangeX(x,37,65))
  {
    if(inRangeY(y,350,377))
      selectColor(2);
    else if(inRangeY(y,377,405))
      selectColor(5);
    else if(inRangeY(y,405,432))
      selectColor(10);
    else if(inRangeY(y,432,460))
      selectColor(13);
          //printf("%d",getcolor());
  }
  else if(inRangeX(x,65,92))
  {
      if(inRangeY(y,350,377))
      selectColor(1);
    else if(inRangeY(y,377,405))
      selectColor(6);
    else if(inRangeY(y,405,432))
      selectColor(9);
    else if(inRangeY(y,432,460))
      selectColor(14);
          //printf("%d",getcolor());
  }
  else if(inRangeX(x,92,120))
  {
    if(inRangeY(y,350,377))
      selectColor(0);
    else if(inRangeY(y,377,405))
      selectColor(7);
    else if(inRangeY(y,405,432))
      selectColor(8);
          //printf("%d",getcolor());
  }
  else if(inRangeXY(x,y,130,20,639,460)&&of)
  {
/*    struct fillsettingstype fillinfo;
    getfillsettings(&fillinfo);*/
    if(tool==3)
    {
          //printf("%d",getcolor());
      setfillstyle(1,color);
      hideMouse();
//      seedFill();
      floodfill(x,y,0);
      showMouse();
      nx1=(float)(x-wstart)/width;
      ny1=(float)(y-hstart)/height;
      fprintf(fo,"3 %f %f %d\n",nx1,ny1,color);
    }
    else if(tool==5)
      {
        int gd=0,gm;
        gotoxy(130,460);
        closegraph();
        printf("Enter string: ");
        gets(str);
        initgraph(&gd,&gm,"..\\bgi");
        freopen(fna,"r+",fo);
        canvas();
        outtextxy(x,y,str);
        nx1=(float)(x-wstart)/width;
        ny1=(float)(y-hstart)/height;
        fprintf(fo,"5 %f %f %d %s\n",nx1,ny2,color,str);
      }
    //seedFill(x,y);
  }
  
  
}


void mouseDragged(int x1,int y1,int x2,int y2)
{
  float rx,ry,cx,cy;

    setcolor(color);
      if(tool==0)
        {
          line(x1,y1,x2,y2);          
          nx1=(float)(x1-wstart)/width;
          nx2=(float)(x2-wstart)/width;
          ny1=(float)(y1-hstart)/height;
          ny2=(float)(y2-hstart)/height;
          fprintf(fo,"0 %f %f %f %f %d\n",nx1,ny1,nx2,ny2,color);
        }
      else if(tool==1)
        {
          rectangle(x1,y1,x2,y2);
          nx1=(float)(x1-wstart)/width;
          nx2=(float)(x2-wstart)/width;
          ny1=(float)(y1-hstart)/height;
          ny2=(float)(y2-hstart)/height;
          fprintf(fo,"1 %f %f %f %f %d\n",nx1,ny1,nx2,ny2,color);
        }
      else if(tool==2)
      {
        cx=(x1+x2)/2.0;
        cy=(y1+y2)/2.0;
        rx=mod(x1-x2)/2.0;
        ry=mod(y1-y2)/2.0;
        ellipse(cx,cy,0,360,rx,ry);
        ncx=(float)(cx-wstart)/width;
        nrx=(float)(rx)/width;
        ncy=(float)(cy-hstart)/height;
        nry=(float)(ry)/height;
        fprintf(fo,"2 %f %f %f %f %d\n",ncx,ncy,nrx,nry,color);
      }
}