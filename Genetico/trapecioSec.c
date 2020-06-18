#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  double a = 0.0, b = 1.5, h, x_i, approx;
  int n = 10000;

  h = (b - a) / n;

  approx = (cos(a) + cos(b)) / 2.0;

  for (int i = 0; i <= n-1; i++) {
    x_i = a + i * h;
    approx += cos(x_i);
  }
  approx = h * approx;
  printf("Resultado con n = %d: %f\n", n, approx);
}
