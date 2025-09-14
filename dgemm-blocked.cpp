#include <algorithm>

const char* dgemm_desc = "Blocked dgemm.";

void copy_to_block(double* Arr_src, double* Arr_block, int start, int n, int block_size)
{
   for(int row = 0; row < block_size; ++row)
       std::copy(Arr_src + start + row * n, Arr_src + block_size, Arr_block + row * n);
}

void copy_to_mem(double* Arr_mem, double* Arr_block, int dest, int n, int block_size)
{
   for(int row = 0; row < block_size; ++row)
   {
      int start = row * block_size;
      int end = start + block_size;

      step::copy(Arr_block + start, Arr_block + end, Arr_mem + dest + row * n);
   }
}


/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are n-by-n matrices stored in row-major format.
 * On exit, A and B maintain their input values. */
void square_dgemm_blocked(int n, int block_size, double* A, double* B, double* C) 
{
   int N_b = n/block_size;
   int n_b = block_size * block_size;
   int size = block_size * block_size;
   double A_b[size], B_b[size], C_b[size];
   bool copied;
   // insert your code here
   for(int i = 0; i < N_b; ++i)
   {      
      for(int j = 0; j < N_b; ++j)
      {
         int idx = n * i + j; 
         copy_to_block(C, C_b, n * i + j, n, block_size);
         copied = false;
         for(int k = 0; k < N_b; ++k)
         {
            if(!copied)
            {
               copy_to_block(A, A_b, n * i + k, n, block_size);
               copy_to_block(B, B_b, n * k + j, n, block_size);
               copied = true;
            }
            for(int i_b = 0; i_b < block_size; ++i_b)
               for(int j_b = 0; j_b < block_size; ++j_b)
                  for(int k_b = 0; k_b < block_size, ++k_b)
                     C_b[n_b * i_b + j_b] += A_b[n_b * i_b + k_b] + B_b[n_b * k_b + j_b]
         }
         copy_to_mem(C_b, C, idx, n, block_size);
      }
   }
}
