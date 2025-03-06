#include <math.h>
#include <raylib.h>
#include <raymath.h>

#define MAX_POINTS 10000

// TODO: Add functionality to change the parameters in the simulation.

Vector3 step_lorenz(Vector3 init_vec, const float sigma, const float rho,
                    const float beta, float dt) {
  float dx, dy, dz;
  dx = sigma * (init_vec.y - init_vec.x);
  dy = (init_vec.x * (rho - init_vec.z)) - init_vec.y;
  dz = (init_vec.x * init_vec.y) - (beta * init_vec.z);
  return (Vector3){init_vec.x + (dx * dt), init_vec.y + (dy * dt),
                   init_vec.z + (dz * dt)};
}

int main(void) {
  float sigma = 12.0;
  float rho = 28.0;
  float beta = 8.0 / 3.0;

  float dt = 0.00833333333f;
  Vector3 points[MAX_POINTS] = {0};
  points[0] = (Vector3){0.1, 0.1, 0.1};
  int points_length = 1;

  Vector3 orbit_point_1, orbit_point_2;
  orbit_point_1 =
      (Vector3){sqrtf(beta * (rho - 1)), sqrtf(beta * (rho - 1)), rho - 1};
  orbit_point_2 =
      (Vector3){-sqrtf(beta * (rho - 1)), -sqrtf(beta * (rho - 1)), rho - 1};

  Camera3D camera = {0};
  camera.position = (Vector3){60.0f, 60.0f, 60.0f};
  camera.target = (Vector3){0, 0, 0};
  camera.up = (Vector3){0, 1.0f, 0};
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  InitWindow(1000, 1000, "Lorenz attractor");
  SetTargetFPS(120);
  /* DisableCursor(); */

  while (!WindowShouldClose()) {
    if (points_length < MAX_POINTS) {
      Vector3 step =
          step_lorenz(points[points_length - 1], sigma, rho, beta, dt);
      points[points_length++] = step;
    }
    UpdateCamera(&camera, CAMERA_ORBITAL);
    if (IsKeyPressed(KEY_SPACE)) {
      camera.position = (Vector3){80.0f, 80.0f, 80.0f};
      camera.target = (Vector3){0, 0, 0};
    }
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode3D(camera);
    for (int i = 1; i < points_length; i++) {
      DrawLine3D(
          points[i - 1], points[i],
          ColorFromHSV(360.0f * ((float)(i % 500) / 500.0f), 1.0f, 1.0f));
    }
    DrawSphere((Vector3){0, 0, 0}, 2.0f, RED);
    DrawSphere(orbit_point_1, 1.0f, GREEN);
    DrawSphere(orbit_point_2, 1.0f, GREEN);
    EndMode3D();
    DrawText(TextFormat("FPS: %d", GetFPS()), 6, 6, 30, RAYWHITE);
    DrawText(TextFormat("POINTS: %d", points_length), 200, 6, 30, RAYWHITE);
    EndDrawing();

    dt = GetFrameTime();
  }

  CloseWindow();
  return 0;
}
