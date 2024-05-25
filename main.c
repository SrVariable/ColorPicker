/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ribana-b <ribana-b@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 19:19:08 by ribana-b          #+#    #+# Malaga      */
/*   Updated: 2024/05/25 10:20:53 by ribana-b         ###   ########.com      */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include <stdio.h>
#include <string.h>

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
	Vector2 leftRecPosition;
	Vector2 rightRecPosition;
} MyColor;

static bool showValue = true;

void	draw_input_box(MyColor *color)
{
	static char text[32];
	Rectangle rec = {X0 + 50, Y0 - 100, 255, 32};

	// Input box
	DrawRectangleRec(rec, color->foreground);

	// Paste / Delete color in the input box
	if (CheckCollisionPointRec(GetMousePosition(), rec))
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			const char *buffer = GetClipboardText();
			if (buffer) memcpy(text, buffer, strlen(buffer));
		}
		else if (IsKeyPressed(KEY_DELETE))
			memset(text, 0, sizeof(text));
	}
	if (text[0])
		DrawText(TextFormat(" %s", text), X0 + 50, Y0 - 100, 32, color->background);
}

void	draw_left_rectangle(MyColor *color)
{
	Rectangle rec = {X0, Y0, 360, 360};
	Rectangle upLeftRec = {X0, Y0, 180, 180};
	Rectangle upRightRec = {X0 + 180, Y0, 180, 180};
	Rectangle downLeftRec = {X0, Y0 + 180, 180, 180};
	Rectangle downRightRec = {X0 + 180, Y0 + 180, 180, 180};
	float radius = 5;
	Vector3 hsv = ColorToHSV(color->current);

	// Set the text color depending on the "theme"
	if (color->isDark) color->text = GetColor(0x303030FF);
	else color->text = GetColor(0x696969FF);

	// Set Saturation
	color->current = ColorFromHSV(hsv.x, (color->leftRecPosition.x - X0) / 360.0f, hsv.z);

	// Set Value
	hsv = ColorToHSV(color->current);
	color->current = ColorFromHSV(hsv.x, hsv.y, 1 - (color->leftRecPosition.y - Y0) / 360.0f);

	hsv.x = color->rightRecPosition.y - 200;
	// Draw the main rectangle
	DrawRectangleGradientEx(upLeftRec, ColorFromHSV(hsv.x, 0.0, 1.0), ColorFromHSV(hsv.x, 0.0, 0.5), ColorFromHSV(hsv.x, 0.5, 0.5), ColorFromHSV(hsv.x, 0.5, 1.0));
	DrawRectangleGradientEx(upRightRec, ColorFromHSV(hsv.x, 0.5, 1.0), ColorFromHSV(hsv.x, 0.5, 0.5), ColorFromHSV(hsv.x, 1.0, 0.5), ColorFromHSV(hsv.x, 1.0, 1.0));
	DrawRectangleGradientEx(downLeftRec, ColorFromHSV(hsv.x, 0.0, 0.5), ColorFromHSV(hsv.x, 0.0, 0.0), ColorFromHSV(hsv.x, 0.5, 0.0), ColorFromHSV(hsv.x, 0.5, 0.5));
	DrawRectangleGradientEx(downRightRec, ColorFromHSV(hsv.x, 0.5, 0.5), ColorFromHSV(hsv.x, 0.5, 0.0), ColorFromHSV(hsv.x, 1.0, 0.0), ColorFromHSV(hsv.x, 1.0, 0.5));

	DrawRectangleLinesEx(rec, 1, color->foreground);

	// Move the circle "pointer" when the cursor collides with the main rectangle
	if (CheckCollisionPointRec(GetMousePosition(), rec))
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			color->leftRecPosition = GetMousePosition();
		if ((IsKeyPressed(KEY_W) || (IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_SHIFT))) && color->leftRecPosition.y > Y0)
			color->leftRecPosition.y -= 1;
		if ((IsKeyPressed(KEY_S) || (IsKeyDown(KEY_S) && IsKeyDown(KEY_LEFT_SHIFT))) && color->leftRecPosition.y < Y0 + 360)
			color->leftRecPosition.y += 1;
		if ((IsKeyPressed(KEY_A) || (IsKeyDown(KEY_A) && IsKeyDown(KEY_LEFT_SHIFT))) && color->leftRecPosition.x > X0)
			color->leftRecPosition.x -= 1;
		if ((IsKeyPressed(KEY_D) || (IsKeyDown(KEY_D) && IsKeyDown(KEY_LEFT_SHIFT))) && color->leftRecPosition.x < X0 + 360)
			color->leftRecPosition.x += 1;
		if (showValue) color->text = color->foreground;
	}

	// Draw the circle "pointer"
	DrawCircleLinesV(color->leftRecPosition, radius, WHITE);
	DrawCircleLinesV(color->leftRecPosition, radius + 1, BLACK);
	if (showValue)
		DrawText(TextFormat("%d %d", (int)color->leftRecPosition.x - X0, (int)color->leftRecPosition.y - Y0), X0, Y0 - 50, 32, color->text);
}

void	draw_right_rectangle(MyColor *color)
{
	float size = 360.0 / 6;
	Vector2 offset = {400, 50};
	Rectangle rec = {X0 + offset.x, Y0, 50, size * 6};
	Rectangle rec2 = {color->rightRecPosition.x - (color->rightRecPosition.x - (X0 + offset.x)),
						color->rightRecPosition.y - 2.5, 50, 5};
	Vector3 hsv = ColorToHSV(color->current);

	// Set the text color depending on the "theme"
	if (color->isDark) color->text = GetColor(0x303030FF);
	else color->text = GetColor(0x696969FF);

	// Set the Hue
	color->current = ColorFromHSV(color->rightRecPosition.y - 200.0f, hsv.y, hsv.z);

	// Draw first 3 sections (From red to cyan)
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 0, 50, size, GetColor(0xFF0000FF), GetColor(0xFFFF00FF));
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 1, 50, size + 0.5, GetColor(0xFFFF00FF), GetColor(0x00FF00FF));
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 2, 50, size, GetColor(0x00FF00FF), GetColor(0x00FFFFFF));

	// Draw last 3 sections (From cyan to red)
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 3, 50, size + 0.5, GetColor(0x00FFFFFF), GetColor(0x0000FFFF));
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 4, 50, size, GetColor(0x0000FFFF), GetColor(0xFF00FFFF));
	DrawRectangleGradientV(X0 + offset.x, Y0 + size * 5, 50, size, GetColor(0xFF00FFFF), GetColor(0xFF0000FF));

	DrawRectangleLinesEx(rec, 1, color->foreground);

	// Move the rectangle "pointer" when the cursor is colliding with the main rectangle
	if (CheckCollisionPointRec(GetMousePosition(), rec))
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
			color->rightRecPosition = GetMousePosition();
		if ((IsKeyPressed(KEY_W) || (IsKeyDown(KEY_W) && IsKeyDown(KEY_LEFT_SHIFT))) && color->rightRecPosition.y > Y0)
			color->rightRecPosition.y -= 1;
		if ((IsKeyPressed(KEY_S) || (IsKeyDown(KEY_S) && IsKeyDown(KEY_LEFT_SHIFT))) && color->rightRecPosition.y < Y0 + size * 6)
			color->rightRecPosition.y += 1;
		if (showValue) color->text = color->foreground;
	}

	// Draw the rectangle "pointer"
	DrawRectangleLinesEx(rec2, 1, color->foreground);
	if (showValue)
		DrawText(TextFormat("%d", (int)color->rightRecPosition.y - Y0), X0 + offset.x, Y0 - offset.y, 32, color->text);
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
		.leftRecPosition = {X0, Y0},
		.rightRecPosition = {X0 + 300, Y0},
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
			draw_input_box(&color);
			if (isHex) DrawText(TextFormat(PRINT_HEX(color.current)), X0 + 100, 40, 32, color.foreground);
			else DrawText(TextFormat(PRINT_RGB(color.current)), X0 + 100, 40, 32, color.foreground);
			if (IsKeyPressed(KEY_SPACE)) showValue = !showValue;
		}
		EndDrawing();
	}
	CloseWindow();
	return (0);
}
