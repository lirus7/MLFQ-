#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
struct process
{
  time_t start;
  time_t end;
  int tm;
  int ong;
  int que;
  bool fin;
};
int slice=15;
time_t global;
time_t upd;
int q[5]={0};
bool check()
{
  upd=time(NULL);
  if(upd-global>slice)
  {
    global=upd;
    return true;
  }
  return false;
}
int findmin(int q)
{
  for(int i=0;i<5;i++)
    if(q[i]>0)
      return i;
  return 10;
}
struct process p[25];
pthread_t threads[25];
int k=0;
void startpro()
{
  int curr=10;
  if(check())
    priority_boost();
  curr=findmin();
  if(curr==10)
    printf("\n All processes ended \n");
  else
  {
    while(q[curr]!=0)
      {
        for(int j=0;j<k;j++)
          if(p[j].que==curr && !(p[j].fin))
            {
              p[j].ong++;
              if(p[j].tm==p[j].ong)
                  {
                    p[j].fin=true;
                    p[j].end=time(NULL);
                    q[curr]--;
                  }
              else
                {
                  if(curr!=4);
                    {
                      q[curr+1]++;
                      p[j].que+=1;
                      q[curr]--;
                    }
                }
            curr=findmin();
            }
        if(check())
          priority_boost();
      }
  }
}
void priority_boost()
{
  printf("priority_boost called and all processes are updated to the 1st queue \n");
  for(int i=0;i<k;i++)
    if(!p[i].fin)
      p[i].que=0;
}
void createpro(int tm)
{
  p[k].start=time(NULL);
  p[k].tm=tm;
  p[k].ong=0;
  p[k].fin=0;
  p[k++].que=0;
  q[0]++;
  startpro();
}
void display_queue(int y)
{
  printf("The processes in this queue are --\n");
  for(int i=0;i<k;i++)
    if(p[i].que==y)
      printf("%d ",i);
  printf("\n");
}
void display_burst(int k)
{
  if(p[k].fin)
    printf("The burst time was --- %d  and it ended in %d queue \n",p[k].end-p[k].start,p[k].que);
  else
    printf("The process is still running \n");
}
int main()
{
  int x,y;
  global=time (NULL);
  while(1)
  {
  printf("  1.Create a new process provide the running time \n  2.Display the proceses in the queue number \n  3.Burst time \n  4.Display the queue of certain process\n");
  scanf("%d",&x);
  if(x==1)
  {
    scanf("%d",&y);
    createpro(y);
  }
  else if(x==2)
  {
    scanf("%d",&y);
    display_queue(y);
  }
  else if(x==3)
  {
    scanf("%d",&y);
    display_burst(y);
  }
  else if(x==4)
  {
    scanf("%d",&y);
    printf("The queue number is  %d \n",p[y].que);
  }
  startpro();
  }
  return 0;
}
