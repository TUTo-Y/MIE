#include <stdio.h>
#include <math.h>
#include <locale.h>

#define PI 3.141592653589793238462643383279502884197169399375

#define kernel_size 33 // 33核的高斯模糊
#define s 10.0         // 通常为1.0~10.0, 值越高越模糊

double gaussian(double x, double sigma)
{
    return exp(-(x * x) / (2.0 * sigma * sigma)) / sqrt(2. * PI * sigma * sigma);
}

int main()
{
    setlocale(LC_ALL, "zh_CN.UTF-8");
    double gaussian_value[kernel_size] = {0};

    printf("|----------------计算高斯模糊核----------------|\n");
    double sum = 0.0;
    int min = -(kernel_size / 2);
    int max = (kernel_size / 2);
    printf("原高斯核:");
    for (int i = min; i <= max; i++)
    {
        gaussian_value[i - min] = gaussian(i, s);
        printf("[%2d] : %lf\n", i, gaussian_value[i - min]);
        sum += gaussian_value[i - min];
    }
    printf("\n总和 : %lf\n", sum);
    puts("");

    printf("|----------------高斯核归一化----------------|\n");
    double sum2 = 0.0;
    printf("高斯核归一化:");
    for (int i = min; i <= max; i++)
    {
        gaussian_value[i - min] /= sum;
        printf("[%2d] : %lf\n", i, gaussian_value[i - min]);
        sum2 += gaussian_value[i - min];
    }
    printf("\n总和 : %lf\n", sum2);
    puts("");

    printf("|----------------高斯模糊核----------------|\n");
    printf("\n核数量为 : %d\n", kernel_size);
    printf("\n赋值 :[%2d]  float[](", max + 1);
    for (int i = 0; i <= max; i++)
    {
        if (i < max)
            printf("%lf, ", gaussian_value[i - min]);
        else
            printf("%lf", gaussian_value[i - min]);
    }
    printf(")\n");
    return 0;
}