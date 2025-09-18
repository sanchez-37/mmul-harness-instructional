#include <iostream>
#include <algorithm>

const char* dgemm_desc = "Blocked dgemm.";

void print_arr(double* arr, int n)
{
    for(int i = 0; i < n; ++i)
    {
       for(int j = 0; j < n; ++j)
       {
          std::cout << arr[i * n + j] << ", ";
       }
       std::cout << std::endl;
    }
}

void copy_to_block(double* Arr_src, double* Arr_block, int begin, int n, int block_size)
{
   // std::cout << "starting block copy at: " << begin << std::endl;
   for(int row = 0; row < block_size; ++row)
   {
      int start = begin + row * n;
      int end = start + block_size;
      // std::cout << "Copying " << end - start << " blocks of data." << std::endl;
      std::copy(Arr_src + start, Arr_src + end, Arr_block + row * block_size);
   }
   // std::cout << "Done copying.." << std::endl;
}

void copy_to_mem(double* Arr_block, double* Arr_mem, int dest, int n, int block_size)
{
   for(int row = 0; row < block_size; ++row)
   {
      int start = row * block_size;
      int end = start + block_size;

      // std::cout << "Copying " << end - start << " blocks of data." << std::endl;
      std::copy(Arr_block + start, Arr_block + end, Arr_mem + dest + row * n);
   }
}


/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are n-by-n matrices stored in row-major format.
 * On exit, A and B maintain their input values. */
void square_dgemm_blocked(int n, int block_size, double* A, double* B, double* C) 
{
   std::cout << "Working with block_size: " << block_size << "X" << block_size << std::endl;
   int N_b = n/block_size;
   // std::cout << "N_b:" << N_b << std::endl;
   // int n_b = block_size * block_size;
   double* A_b = new double[block_size * block_size];
   double* B_b = new double[block_size * block_size];
   double* C_b = new double[block_size * block_size];
 
   int i = 0;
   int j = 0; 
   int k = 0;
   // insert your code here
   for(i = 0; i < N_b; ++i)
   {      
      for(j = 0; j < N_b; ++j)
      {
         // int idx = n * i + j; 
         // std::cout << "Begin copy C block process." << std::endl;
         copy_to_block(C, C_b, n * i * block_size + j * block_size, n, block_size);

         // if (i == 0 && j == 0)
         // {
         //    print_arr(C_b, block_size);
         // }

         for(k = 0; k < N_b; ++k)
         {
            // std::cout << "Begin copy A block process." << std::endl;
            // std::cout << "Block A_" << i << "," << j << "," << k  << ": " << std::endl;
            // printf("Block A_%d,%d,%d\n", i, j, k);
            // int a_block_idx = n * i * block_size + k * block_size;
            // std::cout << "a_block_idx: " << a_block_idx << std::endl;
            copy_to_block(A, A_b, n * i * block_size + k * block_size, n, block_size);
            // std::cout << "Begin copy B block process." << std::endl;
            // std::cout << "Block B_" << j + k + 1 << ": " << std::endl;
            // std::cout << "Block B_" << i << "," << j << "," << k  << ": " << std::endl;
            copy_to_block(B, B_b, n * k * block_size + j * block_size, n, block_size);
            // std::cout << "Begin block mmul process." << std::endl;

            for(int i_b = 0; i_b < block_size; ++i_b)
               for(int j_b = 0; j_b < block_size; ++j_b)
                  for(int k_b = 0; k_b < block_size; ++k_b)
                     C_b[block_size * i_b + j_b] += A_b[block_size * i_b + k_b] * B_b[block_size * k_b + j_b];
         }
         
         // std::cout << "Begin copy C_b to C process." << std::endl;
         copy_to_mem(C_b, C, n * i * block_size + j * block_size, n, block_size);
      }
   }

   delete A_b;
   delete B_b;
   delete C_b;
}
