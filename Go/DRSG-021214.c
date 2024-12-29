/******************************************************************************
 * 
 * FILE: DRSG-021214.c
 * 
 * DESCRIPTION: This distributed program generates the reachability set of a 
 *              Petri net. The states are encoding using canonical firing 
 *              count vectors.
 *
 * AUTHOR: Ruben Carvajal Schiaffino
 *
 * LAST REVISED: Santiago de Chile, 2/12/2014
 *
 *****************************************************************************/ 


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "mpi.h"

#define MPI_data1  MPI_UNSIGNED_CHAR              /* unsigned char */ 
#define MPI_data2  MPI_UNSIGNED                   /* unsigned int */

#define PREFACE_TAG 1
#define IN_TAG      2
#define OUT_TAG     3
#define INHIB_TAG   4
#define TINV_TAG    5
#define Mo_TAG      6
#define MARK_TAG    7

#define HM            214 
#define MAX_T         100
#define MAX_LEVELS   1000                         /* Levels of the RG */

#define MASTER_NODE   0

typedef unsigned char data1;

typedef unsigned int data2;


enum QUEUE_STAT { EMPTY, NORMAL, FULL };

struct queue {
       data1 data_zone[MAX_T];
       data1 begin, end;
};  

struct slot {
       data2 adress, h_m;
};

struct slot *global;

struct queue myqueue;

int *in, *out, *inhib, *invar;

float MAX_SPACE;

FILE *myfile;

data1 *reachset;                                 

data1 *sigma_p, *ssigma, *s, *w;            

data1 verbose = 0;

data1 K, T_TINV, T_STELL, T_TRANS;

data2 *mo;

data2 av_mark;

data2 last_mark = -1;

data2 wh_mark = 0;

data2 current = 0;

data2 whole = 0;

data2 primus = 0;          

data1 *ww0, *ww1, *ww2, *ww3, *ww4, *ww5;

int whoami, hm_are_we;

MPI_Status status;


/*
 * Initializes a queue
 */
void InitQueue(struct queue *q)
{
  q->begin = q->end = 0;
}


/*
 * Verifies the queue status
 */
enum QUEUE_STAT StatusQueue(struct queue q)
{
  if (q.begin == q.end)
     return EMPTY;
  if ((q.end + 1)% MAX_T == q.begin)
     return FULL;
  return NORMAL;
} 


/*
 * Enqueue a data x in q
 */
void InQueue(struct queue *q, data1 x)
{
  q->end = (q->end + 1) % MAX_T;
  q->data_zone[q->end] = x;
}


/*
 * Dequeue a data from q
 */
data1 DeQueue(struct queue *q)
{
  q->begin = (q->begin + 1) % MAX_T;
  return q->data_zone[q->begin];
}


/*
 * Shows the usage of the program
 */
void Usage()
{
  printf("\nUsage : RSGP.exe PN K Mem [-V|-S]\nPN    : Petri Net file description");
  printf("\nK     : Parameter of Mo\nMem   : Available Memory (in Mb)"); 
  printf("\n    -V: Verbose mode\n    -S: Silent mode\n\n");
  fflush(stdout);
  MPI_Finalize();
}


/*
 *
 */
void PrintAll()
{
  register int f, c, k;

  printf("**** From %d ***\n",whoami);
  fflush(stdout);
  if (verbose == 1)
  {
     printf("\n*** IN ***\n");
     fflush(stdout);
     for (f = 0, k = 0; f < T_STELL; f++)
     {
        printf("\n");
	fflush(stdout);
        for ( c = 0; c < T_TRANS; c++, k++)
	{  
           printf(" %d",in[k]);
	   fflush(stdout);
	}   
     }
     printf("\n\n*** OUT ***\n");
     fflush(stdout);
     for (f = 0, k = 0; f < T_STELL; f++)
     {
        printf("\n");
	fflush(stdout);
        for ( c = 0; c < T_TRANS; c++, k++)
	{  
	   printf(" %d",out[k]);
	   fflush(stdout);
	}   
     }
     printf("\n\n*** INHIB ***\n");
     fflush(stdout);
     for (f = 0, k = 0; f < T_STELL; f++)
     {
        printf("\n");
        fflush(stdout);	
        for ( c = 0; c < T_TRANS; c++, k++)
        {	  
           printf(" %d",inhib[k]);
           fflush(stdout);
        }	   	   
     }
     printf("\n\n*** T-Invariants ***\n");
     fflush(stdout);     
     for (f = 0; f < T_TRANS; f++)
     {
        printf("\n");
        fflush(stdout);	
        for (c = 0, k = f; c < T_TINV; c++, k = k + T_TRANS)
        { 	  
           printf(" %d",invar[k]);
           fflush(stdout);
        }	   	   
     }
  }  
  printf("\n\n*** Initial Marking Mo =[ ");
  fflush(stdout);
  for (f = 0; f < T_STELL; f++)
     if (mo[f] != 0)
     {
        if (mo[f] != 1)
	{
           printf(" %d @",mo[f]);
	   fflush(stdout);
	}   
        printf(" P%d,",f+1);
	fflush(stdout);      
     }               
  printf("] ***\n");  
  printf("\n*** Available Markings = %d ***\n\n",av_mark);
  fflush(stdout);  
} 


/*
 *  Prints only the non-zero elements of the state sigma
 */ 
void PrintMarkP(data2 *sigma, data2 l)
{
   register int f;
   
   printf("From %d M_P[%5d] = [",whoami,l);
   fflush(stdout);        
   for (f = 0; f < T_STELL; f++)
      if (sigma[f] != 0)
      { 
         if (sigma[f] != 1)
         {
            printf(" %d @",sigma[f]);
            fflush(stdout);
         }          
         printf(" P%d,",f+1);
         fflush(stdout);         
      }        
   printf("] \n\n");
   fflush(stdout);      
}     


/*
 * Prints only the non-zero elements of the state sigma.
 * sigma is encoded as firing count vector 
 */
void PrintMarkT(data1 *sigma, data2 l)
{
   register int f;
    
   printf("From %d M_T[%5d] = [",whoami,l);
   fflush(stdout);   
   for (f = 0; f < T_TRANS; f++)
      if (sigma[f] != 0)
      { 
         if (sigma[f] != 1)
         {
            printf("%d @",sigma[f]);
            fflush(stdout); 
         }         
         printf(" T%d,",f+1);
         fflush(stdout);     
      }        
   printf("] \n\n");
   fflush(stdout);      
}   

/*
 * Prints the amount of elements in the level of the graph
 */
void PrintGraph() 
{
     printf("From %d %3d %8d %8d\n",whoami,whole,last_mark,global[whole].h_m);
     fflush(stdout);    
}        


/*
 * Prints error messages 
 */
void Error(data1 code, char *message)
{
  switch (code)
  {
     case 1:{ printf("\007 Error, Memory Full ...");
              fflush(stdout);
              break;
            }
     case 2:{ printf("\007 Error, Search Tree Full...");
              fflush(stdout);
            }  
  }     
  printf("From %d %s  ",whoami,message);
  fflush(stdout);  
  printf("\n\n Last Mark = %d \n\n",last_mark);  
  fflush(stdout);  
  MPI_Finalize();
  exit(1);  
}  


/*
 * Computes the enabled transitions
 */
data2 CompEnabT(data2 *vig)
{
  register int f, t, k; 
  data1 ok;
  data1 hm = 0;

  InitQueue(&myqueue);           
  for (t = 0; t < T_TRANS; t++)     
  {
     ok = 1;
     for (f = 0, k = t; f < T_STELL && ok; f++, k = k + T_TRANS)
     {
        if (in[k] != 0 && vig[f] == 0)
           ok = 0;
        if (inhib[k] != 0 && vig[f] != 0)
           ok = 0;
     }
     if (ok)
     {      
       InQueue(&myqueue,t);
       hm++;
     }  
  }
  return hm;
} 


/* 
 * Computes the minimal representation of a state
 */
data1 *CompuMin(data2 c_urrent, data1 t)
{
  register int f, c, res, ok, k; 

  c = c_urrent * T_TRANS;
  for (f = 0; f < T_TRANS; f++, c++)
  {  
     ssigma[f] = reachset[c]; 
     sigma_p[f] = 0; 
  }   
  ssigma[t]++;    
  for (c = 0; c < T_TINV; c++)
  {
     ok = 1;
     k = c * T_TRANS;
     for (f = 0; f < T_TRANS && ok; f++, k++)
     {
        res = ssigma[f] - invar[k];
        if (res < 0)
           ok = 0;
	sigma_p[f] = res;
     }    
     if (ok)
        break;
  }
  if (c != T_TINV) 
     for (f = 0; f < T_TRANS; f++)
     {
        if (sigma_p[f] < ssigma[f])
	   return sigma_p;
        if (sigma_p[f] > ssigma[f])
           break;
     }
  for (f = 0; f < T_TRANS; f++)
     sigma_p[f] = ssigma[f];    
  return sigma_p;
} 


/*
 * Compares two markings
 */
int Strcmp(data1 *a, int j)
{
  register int i;
  
  for (i = 0; i < T_TRANS && a[i] == reachset[j]; i++, j++); 
  if (i == T_TRANS)
     return 0;   
  else           
     return a[i] - reachset[j];     
} 
     

/*
 * Search a tuple using binary search
 */     
int Bsearch(data1 *sigma, data2 atlevel)
{
  register int high, i, low, j, k, res;

  i = global[atlevel].adress;
  j = i * T_TRANS;  
  res = Strcmp(sigma,j);
  if (res < 0)                                    /* Add at the end of the Tree */
     return -1;
  if (res == 0)
     return 0;   
  low = global[atlevel-1].adress;
  high = global[atlevel].adress + 1; 
  for ( ; high-low > 1; )
  {
     i = (high + low) / 2;
     j = i * T_TRANS;   
     res = Strcmp(sigma,j);
     if (res == 0)
        return 0;               
     if (res < 0)
        low = i;
     else
        high = i;                
  }          
  j = low * T_TRANS;
  res = Strcmp(sigma,j);
  if (res == 0)
     return 0;                                    /* Found */
  if (res > 0)
     return low+1;     
  return high;                                    /* Not Found and insert in position */        
} 

     
/*
 * Insert a tuple in the tree at position r_es. If r_es = -1 insert at the end
 */ 
void Insert(data1 *sigma, int r_es, data2 atlevel)
{
  register int i, j, o_ld, n_ew;
  
  i = global[atlevel].adress;  
  if (r_es == -1)                                 /* insert at the end */
     i = last_mark * T_TRANS;
  else
  {
     i = r_es * T_TRANS;
     n_ew = last_mark * T_TRANS + (T_TRANS - 1);
     o_ld = last_mark * T_TRANS - 1;
     for ( ; o_ld >= i; o_ld--, n_ew--)
        reachset[n_ew] = reachset[o_ld];
  }       
  for (j = 0; j < T_TRANS; j++, i++)
     reachset[i] = sigma[j];           
}


/*
 * Decides at which level of the graph is needed to
 * perform the search of a given mark in order to decide insert or not 
 */  
data2 AddMarking(data1 *sigma)
{
  register int i, j, r_es;
  data1 ok;

  ok = 0;
  r_es = -1;  
  if (global[whole].h_m == 0)                     /* First on its level */
     ok = 1;
  else
  {
     r_es = Bsearch(sigma,whole);
     if (r_es != 0)                               /* New marking */
        ok = 1;
  }         
  if (ok)         
  {
     last_mark++;
     wh_mark++;
     if (last_mark > av_mark)
        Error(2,"Add_marking()");
     Insert(sigma,r_es,whole);
     global[whole].adress = last_mark;
     global[whole].h_m++; 
     return 1;
  }
  else
     return 0;         
}


/*
 *
 */
data2 Computes(data1 *sigma)
{
  register int i;
  data2 r;
  
  r = 0;
  for (i = 0; i < T_TRANS; i++)
     r = r + sigma[i];
  return r;
}  
       

int Pack(data1 *sigma, data1 *states, int n, int dest)
{
  register int c, val_m;
  
  val_m = HM * T_TRANS + 2;
  if (n < val_m - 3)
     for (c = 0; c < T_TRANS; c++, n++)
        states[n] = sigma[c];     
  else
  {
     states[val_m-1] = n / T_TRANS;
     states[val_m-2] = 2; 
     MPI_Send(states,val_m,MPI_data1,dest,MARK_TAG,MPI_COMM_WORLD);
     n = 0;
     for (c = 0; c < T_TRANS; c++, n++)
        states[n] = sigma[c];
  }
  return n;
}


/*
 *
 */
void EndLevel(int a, data1 *states)
{
  register int val_m; 
  
  val_m = HM * T_TRANS + 2;  
  states[val_m-2] = 2;
  states[val_m-1] = a / T_TRANS;
  MPI_Send(states,val_m,MPI_data1,a,MARK_TAG,MPI_COMM_WORLD);   
}


/*
 * Generates the RS 
 */
void Process()
{
  register int f, c, j, i, k, how_many, who, ff, val_m;
  int pp0, pp1, pp2, pp3, pp4, pp5;
  data2 where, hm, aux;
  data1 t, notyet, code_rec; 
  data2 *mo_p;
  data1 *ww;
  float E_cpu;
  clock_t cs, ce; 
  
    
  val_m = HM * T_TRANS + 2;
  if ((mo_p = calloc(T_STELL,sizeof(data2))) == NULL)
     Error(1,"Process()");
  if ((ww = calloc(val_m,sizeof(data1))) == NULL)
     Error(1,"Process()");                
  if ((ww0 = calloc(val_m,sizeof(data1))) == NULL)  
     Error(1,"Process()");     
  if ((ww1 = calloc(val_m,sizeof(data1))) == NULL)  
     Error(1,"Process()");        
  if ((ww2 = calloc(val_m,sizeof(data1))) == NULL)  
     Error(1,"Process()");        
  if ((ww3 = calloc(val_m,sizeof(data1))) == NULL)  
     Error(1,"Process()");     
  if ((ww4 = calloc(val_m,sizeof(data1))) == NULL)  
     Error(1,"Process()");        
  if ((ww5 = calloc(val_m,sizeof(data1))) == NULL)  
     Error(1,"Process()");   
  if (whoami == MASTER_NODE)
  {
     cs = clock();
     printf("\nStarting ... Verbose = %d\n\n",verbose);
     fflush(stdout);
     last_mark = 0;
     global[0].h_m = 1;
     how_many = CompEnabT(mo);
     for (f = 1, ff = 0; f <= how_many; f++)
     {
        t = DeQueue(&myqueue);         
        w = CompuMin(current,t);
        who = Computes(w); 
        for (c = 0; c < T_TRANS; c++, ff++)
           ww[ff] = w[c];               
     }
     ww[val_m-2] = 1;                             /* End of Sequence */
     ww[val_m-1] = how_many;
     who = (whoami + 1) % hm_are_we;
     MPI_Send(ww,val_m,MPI_data1,who,MARK_TAG,MPI_COMM_WORLD);
     if (verbose != 2)  
        PrintGraph();                 
     whole++;
     current++;
     wh_mark++;
  }   
  notyet = 1;
  how_many = 0;
  while (notyet)
  { 
     MPI_Recv(ww,val_m,MPI_data1,MPI_ANY_SOURCE,MARK_TAG,MPI_COMM_WORLD,&status);
     code_rec = ww[val_m-2];    
     while (code_rec == 0 || code_rec == 1)
     {     
        for (c = 1; c <= ww[val_m-1]; c++)
        {
           for (i = 0, f = (c - 1) * T_TRANS; i < T_TRANS; i++, f++)
              w[i] = ww[f];
           where = AddMarking(w);
           if (where > 0)
              how_many++;
        }   
        if (code_rec == 1)
           break;
        MPI_Recv(ww,val_m,MPI_data1,MPI_ANY_SOURCE,MARK_TAG,MPI_COMM_WORLD,&status);
        code_rec = ww[val_m-2];                    
     }
     if (code_rec == 1)
     {
        notyet = 0;
        ff = pp0 = pp1 = pp2 = pp3 = pp4 = pp5 = 0;
        for (i = 1; i <= how_many; i++)
        {        
           for (f = 0; f < T_STELL; f++)
              mo_p[f] = mo[f];
           c = current * T_TRANS;               
           for (j = 0; j < T_TRANS; c++, j++)
              if (reachset[c] != 0)
                 for (f = 0, k = j; f < T_STELL; f++, k = k + T_TRANS)
                 {
                    where = reachset[c];
                    mo_p[f] = mo_p[f] + where * (out[k] - in[k]); 
                 }      
           hm = CompEnabT(mo_p);         
           for (f = 1; f <= hm; f++)
           {
              t = DeQueue(&myqueue);         
              w = CompuMin(current,t);
              who = Computes(w);
              if (who > whole * hm_are_we + whoami)
              {
                 notyet = 1;
                 if (ff < val_m - 3)
                    for (c = 0; c < T_TRANS; c++, ff++)
                       ww[ff] = w[c]; 
                 else           
                 {
                    ww[val_m-1] = ff / T_TRANS;
                    ww[val_m-2] = 0;
                    who = who % hm_are_we; 
                    MPI_Send(ww,val_m,MPI_data1,who,MARK_TAG,MPI_COMM_WORLD);
                    ff = 0;
                    for (c = 0; c < T_TRANS; c++, ff++)
                       ww[ff] = w[c];                     
                 }                                     
              }   
              else
              {                       
                 who = who % hm_are_we;
		 if (who == whoami)
		 {
		    who = Computes(w);
		    who = who / hm_are_we;
		    where = Bsearch(w,who);
                 }
		 else
		 {		    
                    if (who == 0)
                       pp0 = Pack(w,ww0,pp0,who);
                    if (who == 1)   
                       pp1 = Pack(w,ww1,pp1,who);
                    if (who == 2)   
                       pp2 = Pack(w,ww2,pp2,who);
                    if (who == 3)
                       pp3 = Pack(w,ww3,pp3,who);
                    if (who == 4)   
                       pp4 = Pack(w,ww4,pp4,who);
                    if (who == 5)   
                       pp5 = Pack(w,ww5,pp5,who);
                 }		                                                       
              }   
           }
           current++;
        }
        how_many = 0;
        if (verbose != 2) 
           PrintGraph();
        whole++;
        if (notyet == 1)
        {
           ww[val_m-2] = 1;
           ww[val_m-1] = ff / T_TRANS;
           who = (whoami + 1) % hm_are_we;
           MPI_Send(ww,val_m,MPI_data1,who,MARK_TAG,MPI_COMM_WORLD);           
        }
        if (pp0 != 0)
           EndLevel(0,ww0);
        if (pp1 != 0)   
           EndLevel(1,ww1);
        if (pp2 != 0)   
           EndLevel(2,ww2);
        if (pp3 != 0)
           EndLevel(3,ww3);
        if (pp4 != 0)   
           EndLevel(4,ww4);
        if (pp5 != 0)   
           EndLevel(5,ww5);                                           
     }   
     if (code_rec == 2)
     {
        for (c = 1; c <= ww[val_m-1]; c++)
        {
           for (i = 0, f = (c - 1) * T_TRANS; i < T_TRANS; i++, f++)
              w[i] = ww[f];
           who = Computes(w);
           who = who / hm_are_we;
           where = Bsearch(w,who);   
        }         
     }   
     if (code_rec == 3) 
     {
        last_mark++;
        printf("\n[%d] %d End Reached. Last Mark = %d\n",whoami,status.MPI_SOURCE,last_mark);
        fflush(stdout); 
        free(reachset);       
        break;   
     }  
  }
  if (notyet == 0)
  {
     ww[val_m-2] = 3;
     last_mark++;
     printf("\n [%d] Finishing... Last Mark = %d\n",whoami,last_mark); 
     fflush(stdout);
     for (f = 0; f < hm_are_we; f++)
        if (f != whoami)
           MPI_Send(ww,val_m,MPI_data1,f,MARK_TAG,MPI_COMM_WORLD);
  }
  if (whoami == MASTER_NODE)
  {
     ce = clock();
     E_cpu = (float)(ce - cs) / CLOCKS_PER_SEC;
     printf("\n\nElapsed CPU Time = %f Seconds \n",E_cpu);
     fflush(stdout);
  }         
}          


/*
 *
 */
void StepTwo()
{
  register int f;
  int s_info[4];  

  if (whoami == MASTER_NODE)
  {
     s_info[0] = T_STELL; s_info[1] = T_TRANS; 
     s_info[2] = T_TINV; s_info[3] = verbose; 
     for (f = 1; f < hm_are_we; f++)
     {
        MPI_Send(&MAX_SPACE,1,MPI_FLOAT,f,PREFACE_TAG,MPI_COMM_WORLD);     
        MPI_Send(&s_info,4,MPI_INT,f,PREFACE_TAG,MPI_COMM_WORLD);
        MPI_Send(in,T_STELL * T_TRANS,MPI_INT,f,IN_TAG,MPI_COMM_WORLD);
        MPI_Send(out,T_STELL * T_TRANS,MPI_INT,f,OUT_TAG,MPI_COMM_WORLD);
        MPI_Send(inhib,T_STELL * T_TRANS,MPI_INT,f,INHIB_TAG,MPI_COMM_WORLD);        
        MPI_Send(invar,T_TRANS * T_TINV,MPI_INT,f,TINV_TAG,MPI_COMM_WORLD);        
        MPI_Send(mo,T_STELL,MPI_data2,f,Mo_TAG,MPI_COMM_WORLD);
     }                   
  }
  else
  {
     MPI_Recv(&MAX_SPACE,1,MPI_FLOAT,MASTER_NODE,PREFACE_TAG,MPI_COMM_WORLD,&status);   
     MPI_Recv(&s_info,4,MPI_INT,MASTER_NODE,PREFACE_TAG,MPI_COMM_WORLD,&status);
     T_STELL = s_info[0];
     T_TRANS = s_info[1];
     T_TINV = s_info[2];
     verbose = s_info[3];  
     if ((mo = calloc(T_STELL,sizeof(data2))) == NULL)
        Error(1,"StepTwo()");   
     if ((sigma_p = calloc(T_TRANS,sizeof(data1))) == NULL)
        Error(1,"StepTwo()"); 
     if ((ssigma = calloc(T_TRANS,sizeof(data1))) == NULL)
        Error(1,"StepTwo()"); 
     if ((w = calloc(T_TRANS,sizeof(data1))) == NULL)
        Error(1,"StepTwo"); 
     if ((s = calloc(T_STELL,sizeof(data1))) == NULL)
        Error(1,"StepTwo"); 
     f = (MAX_SPACE * 1000000) / T_TRANS;
     av_mark = f;     
     f = f * T_TRANS; 
     if ((reachset = calloc(f ,sizeof(data1))) == NULL)
        Error(1,"StepTwo()");     
     if ((global = calloc(MAX_LEVELS,sizeof(struct slot))) == NULL)
        Error(1,"StepTwo()");  
     if ((in = calloc(T_STELL * T_TRANS,sizeof(int))) == NULL)
        Error(1,"StepTwo()");
     if ((out = calloc(T_STELL * T_TRANS,sizeof(int))) == NULL)
        Error(1,"StepTwo()");     
     if ((inhib = calloc(T_STELL * T_TRANS,sizeof(int))) == NULL)
        Error(1,"StepTwo()");   
     if ((invar = calloc(T_TRANS * T_TINV,sizeof(int))) == NULL)
        Error(1,"StepTwo()");  
     MPI_Recv(in,T_STELL * T_TRANS,MPI_INT,MASTER_NODE,IN_TAG,MPI_COMM_WORLD,&status);
     MPI_Recv(out,T_STELL * T_TRANS,MPI_INT,MASTER_NODE,OUT_TAG,MPI_COMM_WORLD,&status);
     MPI_Recv(inhib,T_STELL * T_TRANS,MPI_INT,MASTER_NODE,INHIB_TAG,MPI_COMM_WORLD,&status);        
     MPI_Recv(invar,T_TRANS * T_TINV,MPI_INT,MASTER_NODE,TINV_TAG,MPI_COMM_WORLD,&status); 
     MPI_Recv(mo,T_STELL,MPI_data2,MASTER_NODE,Mo_TAG,MPI_COMM_WORLD,&status);             
     printf("\nFrom %d Available Markings = %d\n",whoami,av_mark);
     fflush(stdout);
  }  
}
 

/*
 * Alloc & initializes the data structures 
 */
void StepOne(char *myf)
{
  register int f;

  myfile = fopen(myf,"r");
  fscanf(myfile,"%d",&T_STELL);                   /* # Places */
  fscanf(myfile,"%d",&T_TRANS);                   /* # Transitions */
  if ((mo = calloc(T_STELL,sizeof(data2))) == NULL)
     Error(1,"StepOne()");   
  if ((sigma_p = calloc(T_STELL,sizeof(data1))) == NULL)
     Error(1,"StepOne()"); 
  if ((ssigma = calloc(T_TRANS,sizeof(data1))) == NULL)
     Error(1,"StepOne()"); 
  if ((w = calloc(T_TRANS,sizeof(data1))) == NULL)
     Error(1,"StepOne()"); 
  if ((s = calloc(T_STELL,sizeof(data1))) == NULL)
     Error(1,"StepOne()"); 
  f = (MAX_SPACE * 1000000) / T_TRANS;
  av_mark = f;
  f = f * T_TRANS;
  if ((reachset = calloc(f,sizeof(data1))) == NULL)
     Error(1,"StepOne()");     
  if ((global = calloc(MAX_LEVELS,sizeof(struct slot))) == NULL)
     Error(1,"StepOne()"); 
  if ((in = calloc(T_STELL * T_TRANS,sizeof(int))) == NULL)
     Error(1,"StepOne()");
  if ((out = calloc(T_STELL * T_TRANS,sizeof(int))) == NULL)
     Error(1,"StepOne()");     
  if ((inhib = calloc(T_STELL * T_TRANS,sizeof(int))) == NULL)
     Error(1,"StepOne()");   
} 
 
 
/*
 * Reads the configuration of the PN
 */
void Read()
{
  int f, c, tot, valf, valc;
  
  fscanf(myfile,"%d",&c);  
  for (f = 0, tot = 0; f < T_STELL; f++)
     for (c = 0; c < T_TRANS; c++, tot++)
        fscanf(myfile,"%1d",&in[tot]);	
  fscanf(myfile,"%d",&c);	
  for (f = 0, tot = 0; f < T_STELL; f++)
     for (c = 0; c < T_TRANS; c++, tot++)
        fscanf(myfile,"%1d",&out[tot]);
  fscanf(myfile,"%d",&c);	
  for (f = 0, tot = 0; f < T_STELL; f++)
     for (c = 0; c < T_TRANS; c++, tot++)
        fscanf(myfile,"%1d",&inhib[tot]);
  fscanf(myfile,"%d",&c);		
  fscanf(myfile,"%d",&T_TINV);                    /*  # T-Invariants */    
  if ((invar = calloc(T_TRANS * T_TINV,sizeof(int))) == NULL)
     Error(1,"Read()");          
  fscanf(myfile,"%d",&c);	
  for (f = 0; f < T_TRANS; f++)
     for (c = 0; c < T_TINV; c++)
     {
        tot = f + c * T_TRANS;
        fscanf(myfile,"%1d",&invar[tot]);
     }		  
  fscanf(myfile,"%d",&c);	
  fscanf(myfile,"%d",&tot);     
  fscanf(myfile,"%d",&c);	
  for (f = 1; f <= tot; f++)
  {
     fscanf(myfile,"%d",&valf);                   /* token position */
     fscanf(myfile,"%d",&valc);                   /* cardinality   */
     if (valc == 0)                               /* => valf has a generic marking */
        mo[valf-1] = K;
     else  
        mo[valf-1] = valc;
  }
  fclose(myfile);
}  


/*
 *
 */
void main( int argc, char *argv[])
{
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int me;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&hm_are_we);
  MPI_Comm_rank(MPI_COMM_WORLD,&whoami);
  MPI_Get_processor_name(processor_name,&me);
  printf("Process [%d] Alive on %s\n",whoami,processor_name);
  fflush(stdout);
  if (whoami == MASTER_NODE)  
     if (argc >= 4 && argc <= 5)
     {
        if (argc == 5)
        {
           if (strcmp(argv[4],"-V") == 0)
              verbose = 1;
           if (strcmp(argv[4],"-S") == 0)
              verbose = 2;
        }      
        K = atoi(argv[2]);              
        MAX_SPACE = atof(argv[3]);
        StepOne(argv[1]);
        Read();
        PrintAll();     
     }    
     else
        Usage();
  StepTwo();
  MPI_Barrier(MPI_COMM_WORLD);
  Process();
  MPI_Finalize();      
}
