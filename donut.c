#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 100
#define HEIGHT 60
#define MAJOR_RADIUS 1
#define MINOR_RADIUS 2
#define DISTANCE_FROM_VIEWER 50
#define FACTOR 5

void renderDonut(double A, double B);

int main()
{
  double A = 0, B = 0;

  while (1)
  {
    renderDonut(A, B);
    A += 0.04;
    B += 0.02;

    usleep(30000);
  }

  return 0;
}

void renderDonut(double A, double B)
{
  int SCREEN_SIZE = WIDTH * HEIGHT;
  char output[SCREEN_SIZE];
  float zbuffer[SCREEN_SIZE];

  memset(output, ' ', SCREEN_SIZE);
  memset(zbuffer, 0, SCREEN_SIZE * sizeof(float));

  for (double theta = 0; theta < 2 * M_PI; theta += 0.07)
  {
    for (double phi = 0; phi < 2 * M_PI; phi += 0.02)
    {
      double cosTheta = cos(theta);
      double sinTheta = sin(theta);
      double cosPhi = cos(phi);
      double sinPhi = sin(phi);

      // Coordinates in 3D space (X, Y, Z)
      double x = (MINOR_RADIUS + MAJOR_RADIUS * cosTheta) * (cosPhi * cos(A) + sinPhi * sin(A) * sin(B));
      double y = (MINOR_RADIUS + MAJOR_RADIUS * cosTheta) * (sinPhi * cos(A) - cosPhi * sin(A) * sin(B));
      double z = MINOR_RADIUS * sinTheta * cos(B) - cos(A) * sinPhi;

      // Perspective Projection
      double ooz = 1 / (z + FACTOR); // 1/z for perspective effect

      // Coordinates on 2D screen
      int xp = (int)(WIDTH / 2 + DISTANCE_FROM_VIEWER * ooz * x);
      int yp = (int)(HEIGHT / 2 - DISTANCE_FROM_VIEWER * ooz * y);

      // Simple lighting calculation
      float light = cosPhi * cosTheta * sin(B) - cos(A) * cosPhi * sinTheta * cos(B);

      if (light > 0 && xp >= 0 && xp < WIDTH && yp >= 0 && yp < HEIGHT)
      {
        if (ooz > zbuffer[yp * WIDTH + xp])
        {
          zbuffer[yp * WIDTH + xp] = ooz;
          output[yp * WIDTH + xp] = ".,-~:;=!*#$@"[(int)(light * 12)];
        }
      }
    }
  }

  // Print the frame
  printf("\x1b[H"); // Move cursor to the top-left corner of the terminal
  for (int k = 0; k < SCREEN_SIZE; k++)
  {
    putchar(k % WIDTH ? output[k] : '\n'); // Add newline at the end of each row
  }
}
