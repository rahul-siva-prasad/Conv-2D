#include <stdio.h>
#include <stdlib.h>

#define ARRAY_2D_SIZE(arr) sizeof(arr)/sizeof(arr[0])
#define OUTPUT_KERNEL_SIZE(_input_,_kernel_,_padding_,_stride_) (((_input_-_kernel_+2*_padding_)/_stride_)+1)

typedef struct S_CONV_2D_DATA{
    int* conv2D_array;
    int array_size;
}CONV_2D_DATA;

CONV_2D_DATA* conv2D(int* image_arr, int image_size, int* kernel, int kernel_size,int stride,int padding,int pad_value,int bias);

int image_arr[4][4] = {
    {1,   2,  3,  4},
    {5,   6,  7,  8},
    {9,  10, 11, 12},
    {13, 14, 15, 16}
};

int kernel[2][2] ={
    {1, 1},
    {1, 1}
};
/*int kernel[3][3] = {
    {0, -1,  0},
    {-1, 5, -1},
    {0, -1,  0}
};*/

CONV_2D_DATA* conv2D(int* image_arr, int image_size, int* kernel, int kernel_size,int stride,int padding,int pad_value,int bias) {
    int output_size = OUTPUT_KERNEL_SIZE(image_size,kernel_size,padding,stride);

    CONV_2D_DATA* ptr = malloc(sizeof(CONV_2D_DATA));
    ptr->conv2D_array = malloc(sizeof(int)*output_size * output_size);
    ptr->array_size = output_size;
    int output_index =0;

    for(int row_image=0-padding;row_image<output_size+stride-padding-1;row_image+=stride){
        for(int col_image=0-padding;col_image<output_size+stride-padding-1;col_image+=stride){
            int convolved_value =0;
            for(int row_kernel = 0;row_kernel < kernel_size;row_kernel++){
                int row_start = row_image+row_kernel;
                for(int col_kernel = 0;col_kernel < kernel_size;col_kernel++){
                    int col_start       = col_image + col_kernel;
                    int image_cell_val;
                    if((row_start <0 || row_start >=image_size) || ( col_start <0 || col_start >=image_size))
                        image_cell_val = pad_value;
                    else 
                        image_cell_val  = image_arr [row_start  * image_size  + col_start];
                    int kernel_cell_val = kernel    [row_kernel * kernel_size + col_kernel];
                    convolved_value     += (image_cell_val*kernel_cell_val);
                }
            }
            ptr->conv2D_array[output_index++] = convolved_value + bias;
        }
    }
    int max_buffer =0;
    for(int row=0;row<output_size;row++){
        int current_buffer=0;
        for(int col=0;col<output_size;col++){
            current_buffer += printf("|\t%d\t|",ptr->conv2D_array[row*output_size+col]);
            current_buffer += 5*2;
        }
        if(current_buffer> max_buffer)
            max_buffer = current_buffer;
        int temp = max_buffer;
        printf("\n");
        while(temp--)
            printf("-");
        printf("\n");
    }
    return ptr;
}

int main() {
    int image_size = ARRAY_2D_SIZE(image_arr);
    int kernel_size = ARRAY_2D_SIZE(kernel);   
    int bias=0;//5
    int stride =1;//1
    int padding =1;
    int pad_value =0;
    CONV_2D_DATA*ptr = conv2D((int*)image_arr, image_size, (int*)kernel, kernel_size,stride,padding,pad_value,bias);
    free(ptr->conv2D_array);
    free(ptr);
    return 0;
}