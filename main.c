/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ribana-b <ribana-b@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:19:08 by ribana-b          #+#    #+# Malaga      */
/*   Updated: 2024/05/25 01:44:41 by ribana-b         ###   ########.com      */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define X0 200
#define Y0 200
#define FPS 60
#define PRINT_HEX(COLOR) "#%02X%02X%02X", (COLOR).r, (COLOR).g, (COLOR).b
#define PRINT_RGB(COLOR) "(%u, %u, %u)", (COLOR).r, (COLOR).g, (COLOR).b

typedef struct
{
	Color background;
	Color foreground;
	Color text;
	Color current;
	bool isDark;
} MyColor;

static bool showValue = true;

void	draw_left_rectangle(MyColor *color)
{
	Rectangle rec = {X0, Y0, 360, 360};
	float radius = 5;
	static Vector2 currentPosition = {X0, Y0};
	Vector3 hsv = ColorToHSV(color->current);

	// Set the text color depending on the "theme"
	if (color->isDark) color->text = GetColor(0x303030FF);
	else color->text = GetColor(0x696969FF);

	// Set Saturation
	color->current = ColorFromHSV(hsv.x, (currentPosition.x - X0) / 360.0f, hsv.z);
	// Set Value
	hsv = ColorToHSV(color->current);
	color->current = ColorFromHSV(hsv.x, hsv.y, (currentPosition.y - Y0) / 360.0f);

	// Draw the main rectangle
	DrawRectangleRec(rec, color->current);

	// Move the circle "pointer" when the cursor collides with the main rectangle
	if (CheckCollisionPointRec(GetMousePosition(), rec))
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			currentPosition = GetMousePosition();
		if ((IsKeyPressed(KEY_W) || (IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_SHIFT))) && currentPosition.y > Y0)
			currentPosition.y -= 1;
		if ((IsKeyPressed(KEY_S) || (IsKeyDown(KEY_S) && IsKeyDown(KEY_LEFT_SHIFT))) && currentPosition.y < Y0 + 360)
			currentPosition.y += 1;
		if ((IsKeyPressed(KEY_A) || (IsKeyDown(KEY_A) && IsKeyDown(KEY_LEFT_SHIFT))) && currentPosition.x > X0)
			currentPosition.x -= 1;
		if ((IsKeyPressed(KEY_D) || (IsKeyDown(KEY_D) && IsKeyDown(KEY_LEFT_SHIFT))) && currentPosition.x < X0 + 360)
			currentPosition.x += 1;
		if (showValue) color->text = color->foreground;
	}


	// Draw the circle "pointer"
	DrawCircleLinesV(currentPosition, radius, WHITE);
	DrawCircleLinesV(currentPosition, radius + 1, BLACK);
	if (showValue)
		DrawText(TextFormat("%d %d", (int)currentPosition.x - X0, (int)currentPosition.y - Y0), X0, Y0 - 50, 32, color->text);
}

void	draw_right_rectangle(MyColor *color)
{
	float size = 360.0 / 6;
	Vector2 offset = {400, 50};
	static Vector2 currentPosition = {X0 + 300, Y0};
	Rectangle rec = {X0 + offset.x, Y0, 50, size * 6};
	Rectangle rec2 = {currentPosition.x - (currentPosition.x - (X0 + offset.x)),
						currentPosition.y - 2.5, 50, 5};
	Vector3 hsv = ColorToHSV(color->current);

	// Set the text color depending on the "theme"
	if (color->isDark) color->text = GetColor(0x303030FF);
	else color->text = GetColor(0x696969FF);

	// Set the Hue
	color->current = ColorFromHSV(currentPosition.y - 200.0f, hsv.y, hsv.z);

	// Draw first 3 sections (From red to cyan)
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 0, 50, size, GetColor(0xFF0000FF), GetColor(0xFFFF00FF));
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 1, 50, size + 0.5, GetColor(0xFFFF00FF), GetColor(0x00FF00FF));
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 2, 50, size, GetColor(0x00FF00FF), GetColor(0x00FFFFFF));

	// Draw last 3 sections (From cyan to red)
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 3, 50, size + 0.5, GetColor(0x00FFFFFF), GetColor(0x0000FFFF));
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 4, 50, size, GetColor(0x0000FFFF), GetColor(0xFF00FFFF));
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 5, 50, size, GetColor(0xFF00FFFF), GetColor(0xFF0000FF));

	// Move the rectangle "pointer" when the cursor is colliding with the main rectangle
	if (CheckCollisionPointRec(GetMousePosition(), rec))
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			currentPosition = GetMousePosition();
		if ((IsKeyPressed(KEY_W) || (IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_SHIFT))) && currentPosition.y > Y0)
			currentPosition.y -= 1;
		if ((IsKeyPressed(KEY_S) || (IsKeyDown(KEY_S) && IsKeyDown(KEY_LEFT_SHIFT))) && currentPosition.y < Y0 + size * 6)
			currentPosition.y += 1;
		if (showValue) color->text = color->foreground;
	}

	// Draw the rectangle "pointer"
	DrawRectangleLinesEx(rec2, 1, color->foreground);
	if (showValue)
		DrawText(TextFormat("%d", (int)currentPosition.y - Y0), X0 + offset.x, Y0 - offset.y, 32, color->text);
}

void	draw_switch_theme_button(MyColor *color)
{
	float radius = 10;
	Vector2 center = {WIDTH - 50, 50};

	if (color->isDark)
	{
		DrawCircleV(center, 10, color->foreground);
		color->background = BLACK;
		color->foreground = RAYWHITE;
	}
	else
	{
		DrawCircleV(center, radius, color->foreground);
		color->background = RAYWHITE;
		color->foreground = BLACK;
	}
	if (CheckCollisionPointCircle(GetMousePosition(), center, radius) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			color->isDark = !color->isDark;
}

int	main(void)
{
	Rectangle rec = {200, 200, 100, 100};
	MyColor color = {
		.background = WHITE,
		.foreground = GetColor(0x696969FF),
		.text = RAYWHITE,
		.current = GetColor(0XFF0000FF),
		.isDark = true,
	};
	bool isHex = true;
	InitWindow(WIDTH, HEIGHT, "ColorPicker");
	SetTargetFPS(FPS);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		{
			ClearBackground(color.background);
			draw_left_rectangle(&color);
			draw_right_rectangle(&color);
			draw_switch_theme_button(&color);
			DrawRectangle(X0 + 45, 42, 30, 30, color.current);
			if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){X0 + 45, 40, 255 + 60, 32}) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				// Copy the color
				if (isHex) SetClipboardText(TextFormat(PRINT_HEX(color.current)));
				else SetClipboardText(TextFormat(PRINT_RGB(color.current)));
				if (IsKeyDown(KEY_LEFT_SHIFT))
					isHex = !isHex;
			}
			if (isHex) DrawText(TextFormat(PRINT_HEX(color.current)), X0 + 100, 40, 32, color.foreground);
			else DrawText(TextFormat(PRINT_RGB(color.current)), X0 + 100, 40, 32, color.foreground);
			if (IsKeyPressed(KEY_SPACE)) showValue = !showValue;
		}
		EndDrawing();
	}
	CloseWindow();
	return (0);
}
