#include <stdio.h>
#include <math.h>


#define N 729
#define reps 100 
#include <omp.h> 

double a[N][N]={0}, b[N][N]={0}, c[N]={0};
int jmax[N];  


void init1(void);
void init2(void);
void loop1(void);
void loop2(void);
void valid1(void);
void valid2(void);


int main(int argc, char *argv[]) { 
  FILE *pFile;

  double start1,start2,end1,end2,time1,time2;
  int r,i=0;

  pFile = fopen ("myfile.txt" , "w");
  if (pFile == NULL) perror ("Error opening file");

  for(i=0;i<10;i++)
  {
  init1(); 
  start1 = omp_get_wtime(); 
  for (r=0; r<reps; r++){ 
    loop1();
  } 
  end1  = omp_get_wtime();  

  valid1(); 
  time1 = (float)(end1-start1);
  // printf("Total time for %d reps of loop 1 = %f\n",reps, time1); 
 
  
  init2(); 
  start2 = omp_get_wtime(); 
  // for (r=0; r<reps; r++){ 
  //   loop2();
  // } 
  end2  = omp_get_wtime(); 

  valid2(); 
  time2 = (float)(end2-start2);
  // printf("Total time for %d reps of loop 2 = %f\n",reps, time2);
  // printf("This file is functional\n"); 
  fprintf(pFile,"%d\t%lf\t%lf\n",i+1,time1,time2);  
  }
  printf("\n\t\t\tWritten to 'myfile.txt'\n\n"); 
  fclose(pFile);
} 


void init1(void){
  
    int i,j;  
  
    for (i=0; i<N; i++){ 
      for (j=0; j<N; j++){ 
        a[i][j] = 0.0; 
        b[i][j] = 3.142*(i+j); 
      }
    }
}

void init2(void){ 
  int i,j, expr; 

  for (i=0; i<N; i++){ 
    expr =  i%( 3*(i/30) + 1); 
    if ( expr == 0) { 
      jmax[i] = N;
    }
    else {
      jmax[i] = 1; 
    }
    c[i] = 0.0;
  }

  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      b[i][j] = (double) (i*j+1) / (double) (N*N); 
    }
  }
 
} 

void loop1(void) { 
  int i,j; 
    
#pragma omp parallel for default(none) private(i,j) shared(a,b)
    for (i=0; i<N; i++){ 
      for (j=N-1; j>i; j--){
        a[i][j] += cos(b[i][j]);
      }  //for2 end
    }//for1 end
 }//void end

void loop2(void) {

  int i,j,k; 
  double rN2;
  rN2 = 1.0 / (double) (N*N);
  
#pragma omp parallel for default(none) private(i,j,k) shared(c,b,jmax,rN2)
    for (i=0; i<N; i++){ 
      for (j=0; j < jmax[i]; j++){
        for(k=0;k<j;k++){
          c[i] += (k+1) * log (b[i][j]) * rN2;
         } //for3 end   
      } //for2 end
    }//for1 end
}//void end


void valid1(void) { 
  int i,j; 
  double suma; 
  
  suma= 0.0; 
  for (i=0; i<N; i++){ 
    for (j=0; j<N; j++){ 
      suma += a[i][j];
    }
  }
  // printf("Loop 1 check: Sum of a is %lf\n", suma);

} 


void valid2(void) { 
  int i; 
  double sumc; 
  
  sumc= 0.0; 
  for (i=0; i<N; i++){ 
    sumc += c[i];
  }
  // printf("Loop 2 check: Sum of c is %f\n", sumc);
} 
 

