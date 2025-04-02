#include "../common.cuh"

int main(int argc, char const *argv[])
{
  // 1.分配主机和设备内存,并初始化
  float *fpHost_A;
  fpHost_A = (float*) malloc(4); // 创建四个字节的主机内存
  memset(fpHost_A, 0, 4); // 主机内存初始化为0

  float *fpDevice_A;
  CUDA_CHECK(cudaMalloc((float**)&fpDevice_A, 4));  // 创建四个字节的设备内存
  CUDA_CHECK(cudaMemset(fpDevice_A, 0, 4));

  // 2.数据从主机传输到设备
  CUDA_CHECK(cudaMemcpy(fpDevice_A, fpHost_A, 4, cudaMemcpyDeviceToHost));

  // 3.释放主机和设备内存
  free(fpHost_A);
  CUDA_CHECK(cudaFree(fpDevice_A));

  // 销毁当前进程中当前设备的所有 CUDA 资源，并将设备恢复到初始状态
  CUDA_CHECK(cudaDeviceReset());
  return 0;
}
