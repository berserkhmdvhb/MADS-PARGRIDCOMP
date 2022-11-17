#include <stdio.h>
#include <omp.h>
int main(){
printf("Hello from process: %d\n", omp_get_thread_num());
#pragma omp parallel for
  for(int i=0;i<10;i++){
    printf("%i\n",i);
  }
  
  return 0;
}

    
