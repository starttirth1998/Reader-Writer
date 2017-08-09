#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t *mutex;
pthread_attr_t attr;
pthread_t *tid;
pthread_t writer_tid;
int n,start,buffer_size,tot;
int **buffer;

int write_num(int num)
{
  for(int start=0;start<buffer_size;start++)
  {
    if(buffer[start][0] == 0)
    {
      pthread_mutex_lock(&mutex[start]);
      buffer[start][0] = num;
      for(int i=1;i<=n;i++)
      {
        buffer[start][i] = 0;
      }
      //pthread_mutex_unlock(&mutex[start]);
      return start;
    }
  }
  return -1;
}

int read_num(int rd,int num)
{
  int flag=0;
  if(buffer[num][0] == 0)
    return -1;
  else if(buffer[num][rd] == 1)
    return 1;
  buffer[num][rd] = 1;
  for(int i=1;i<=n;i++)
  {
    if(buffer[num][i] == 0)
    {
      flag = 1;
      break;
    }
  }
  if(flag == 0)
  {
    for(int i=0;i<=n;i++)
    {
      buffer[num][i] = 0;
    }
  }
  return 0;
}

int read_all(int rd)
{
  for(int i=0;i<buffer_size;i++)
  {
    if((buffer[i][rd] == 0) && (buffer[i][0] != 0))
    {
      //printf("%d %d %d\n",i,rd,buffer[i][rd]);
      return 0;
    }
  }
  return 1;
}

void *writer(void *par)
{
  int num;
  while(tot)
  {
    sleep(1);
    num = rand()+1;
    int val = write_num(num);
    //pthread_mutex_lock(&mutex[val]);
    if(val >= 0)
    {
      tot--;
      printf("WRITER: Wrote %d at %d\n",num,val);
      pthread_mutex_unlock(&mutex[val]);
    }
    else
      printf("WRITER: No space to write\n");
  }
  return NULL;
}

void *reader(void *rd)
{
  int num,val;
  int *par = (int *)rd;
  while(1)
  {
    if((tot <= 0) && (read_all(*par) == 1))
    {
      return NULL;
    }
    sleep(1);
    num = rand()%buffer_size;
    pthread_mutex_lock(&mutex[num]);
    val = read_num(*par,num);
    if(val == -1)
      printf("READER %d: No Data available at %d\n",*par,num);
    else if(val == 1)
      printf("READER %d: Has already read %d\n",*par,num);
    else if(val == 0)
      printf("READER %d: Read at %d\n",*par,num);
    pthread_mutex_unlock(&mutex[num]);
    //printf("R:%d %d\n",*par,num);
  }
}

int main(int argc,char *argv[])
{
  int i;
  int *arg;
  printf("BUFFER SIZE: ");
  scanf("%d",&buffer_size);
  printf("Number of Readers: ");
  scanf("%d",&n);
  printf("Number of items to write: ");
  scanf("%d",&tot);
  buffer = (int **)malloc((buffer_size+5)*sizeof(int *));
  arg = (int *)malloc((n+5)*sizeof(int));
  tid = (pthread_t *)malloc((n+5)*sizeof(pthread_t));
  mutex = (pthread_mutex_t *)malloc((n+5)*sizeof(pthread_mutex_t));
  for(i=0;i<buffer_size;i++)
  {
    buffer[i] = (int *)malloc((n+5)*sizeof(int));
    buffer[i][0] = 0;
  }
  for(i=0;i<=n;i++)
  {
    pthread_mutex_init(&mutex[i],NULL);
  }
  pthread_attr_init(&attr);
  //start = 0;
  pthread_create(&writer_tid,&attr,writer,NULL);
  for(i=1;i<=n;i++)
  {
    arg[i] = i;
    pthread_create(&tid[i],&attr,reader,&arg[i]);
  }
  pthread_join(writer_tid, 0);
  for(i=1;i<=n;i++)
    pthread_join(tid[i], 0);
  //sleep(20);
  return 0;
}
