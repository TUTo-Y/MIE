#include <stdio.h>
#include <math.h>
#include <locale.h>

#define PI 3.141592653589793238462643383279502884197169399375

#define kernel_size 13 // 13核的高斯模糊
#define s 2.0         // 通常为1.0~10.0, 值越高越模糊

double gaussian(double x, double sigma)
{
    return exp(-(x * x) / (2.0 * sigma * sigma)) / sqrt(2. * PI * sigma * sigma);
}

int main()
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    double sum = 0.0;

    int min = -(kernel_size / 2);
    int max = (kernel_size / 2);
    printf("|----------------测试高斯模糊核----------------|\n");
    for (int i = min; i <= max; i++)
    {
        double gaussian_value = gaussian(i, s);
        printf("[%2d] : %lf\n", i, gaussian_value);
        sum += gaussian_value;
    }
    printf("\n总和 : %lf\n", sum);
    printf("|----------------高斯模糊核----------------|\n");
    printf("\n核数量为 : %d\n", kernel_size);
    printf("\n赋值 :[%2d]  float[](", max+1);
    for (int i = 0; i <= max; i++)
    {
        double gaussian_value = gaussian(i, s);
        if(i<max)
            printf("%lf, ", gaussian_value);
        else
            printf("%lf", gaussian_value);
    }
    printf(")\n");
    return 0;
}