/* $Id: fence.c,v 1.11 2003-03-27 21:40:30 d3h325 Exp $ */
#include "armcip.h"
#include "armci.h"
#include "copy.h"

#include <stdio.h>

#ifdef CLUSTER
   char *_armci_fence_arr;
#endif

#ifdef GA_USE_VAMPIR
#include "armci_vampir.h"
#endif

void armci_init_fence()
{
#ifdef DATA_SERVER
# ifdef GM /*when fence moves to ds-shared, fence_init would become common*/
     armci_gm_fence_init();
#endif
     _armci_fence_arr=calloc(armci_nproc,1);
     if(!_armci_fence_arr)armci_die("armci_init_fence: calloc failed",0);
#endif
}

void ARMCI_Fence(int proc)
{
#ifdef GA_USE_VAMPIR
     vampir_begin(ARMCI_FENCE,__FILE__,__LINE__);
 if (armci_me != proc)
        vampir_start_comm(proc,armci_me,0,ARMCI_FENCE);
#endif
#if defined(DATA_SERVER) && !(defined(GM) && defined(ACK_FENCE))
     if(_armci_fence_arr[proc] && (armci_nclus >1)){
         
           int cluster = armci_clus_id(proc);
           int master=armci_clus_info[cluster].master;

           armci_rem_ack(cluster);

           /* one ack per cluster node suffices */
           bzero(_armci_fence_arr+master, armci_clus_info[cluster].nslave); 

     }
#else
     FENCE_NODE(proc);
#endif
#ifdef GA_USE_VAMPIR
     if (armci_me != proc) 
        vampir_end_comm(proc,armci_me,0,ARMCI_FENCE);
     vampir_end(ARMCI_FENCE,__FILE__,__LINE__);
#endif
}

void _armci_amina_allfence()
{
#define MAX_HNDL 12
armci_hdl_t ah[MAX_HNDL];
armci_hdl_t *h;
int buf, c=0,p,i;
extern void** memlock_table_array;

     if(!memlock_table_array) armci_die("armci_internal_allfence: NULL ptr",0);

     for(p=0;p<armci_nproc;p++)

       if(_armci_fence_arr[p] && (armci_nclus >1)){

           int cluster = armci_clus_id(p);
           int master=armci_clus_info[cluster].master;

           h = ah+(c%MAX_HNDL);
           if(c>MAX_HNDL) ARMCI_Wait(h);
           
           ARMCI_INIT_HANDLE(h);
            
           ARMCI_NbGet(memlock_table_array+master, &buf, sizeof(int), master,  h);

           /* one ack per cluster node suffices */
           bzero(_armci_fence_arr+master, armci_clus_info[cluster].nslave);

#if 0
           printf("%d fencing %d %d\n",armci_me,p,c); fflush(stdout);
#endif

           c++;
       }
        
       for(i=0; i< MIN(c,MAX_HNDL); i++) ARMCI_Wait(ah+i);
}       

void ARMCI_AllFence()
{
#if defined(LAPI) || defined(CLUSTER)
     int p;
#endif
#ifdef GA_USE_VAMPIR
     vampir_begin(ARMCI_ALLFENCE,__FILE__,__LINE__);
#endif
#ifdef _CRAYMPP
     if(cmpl_proc != -1) FENCE_NODE(cmpl_proc);
#elif defined(LAPI) || defined(CLUSTER)
#if defined(GM) && !defined(ACK_FENCE)
     _armci_amina_allfence(); 
#else
     for(p=0;p<armci_nproc;p++)ARMCI_Fence(p);
#endif
#endif
#ifdef GA_USE_VAMPIR
     vampir_end(ARMCI_ALLFENCE,__FILE__,__LINE__);
#endif
}

