/*! \file dodger.c
 *  \brief Dodger Level module code.
 */
/*
 * Copyright (c) Sebastian Krzyszkowiak <dos@dosowisko.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */
#include <stdio.h>
#include <math.h>
#include <allegro5/allegro_primitives.h>
#include "../../gamestates/level.h"
#include "../../utils.h"
#include "../actions.h"
#include "dodger.h"
#include "dodger/actions.h"

void Dodger_Logic(struct Game *game, struct Dodger* data) {
	struct ALLEGRO_KEYBOARD_STATE keyboard;
	al_get_keyboard_state(&keyboard);
	if (data->handle_input) {
		if (data->derpy_angle > 0) { data->derpy_angle -= 0.02; if (data->derpy_angle < 0) data->derpy_angle = 0; }
		if (data->derpy_angle < 0) { data->derpy_angle += 0.02; if (data->derpy_angle > 0) data->derpy_angle = 0; }
		if (al_key_down(&keyboard, ALLEGRO_KEY_UP)) {
			data->derpy_y -= 0.005;
			data->derpy_angle -= 0.03;
			if (data->derpy_angle < -0.15) data->derpy_angle = -0.15;
			/*PrintConsole(game, "Derpy Y position: %f", data->derpy_y);*/
		}
		if (al_key_down(&keyboard, ALLEGRO_KEY_DOWN)) {
			data->derpy_y += 0.005;
			data->derpy_angle += 0.03;
			if (data->derpy_angle > 0.15) data->derpy_angle = 0.15;
			/*PrintConsole(game, "Derpy Y position: %f", data->derpy_y);*/
		}
		/*if ((data->derpy_y > 0.6) && (data->flying)) {
			SelectDerpySpritesheet(game, "run");
			data->flying = false;
			data->sheet_speed = tps(game, 60*0.0020/data->speed);
		}
		else if ((data->derpy_y <= 0.6) && (!data->flying)) {
			SelectDerpySpritesheet(game, "fly");
			data->flying = true;
			data->sheet_speed = tps(game, 60*2.4);
		}
		if (!data->flying) data->sheet_speed = tps(game, 60*0.0020/data->speed); */
		if (data->derpy_y < 0) data->derpy_y=0;
		else if (data->derpy_y > 0.8) data->derpy_y=0.8;

		data->derpy_y += data->derpy_angle / 30;
	}

	int derpyx = data->derpy_x*game->viewport.height*1.6;
	int derpyy = data->derpy_y*game->viewport.height;
	int derpyw = al_get_bitmap_width(data->derpy);
	int derpyh = al_get_bitmap_height(data->derpy);
	int derpyo = game->viewport.height*1.6*0.1953125-al_get_bitmap_width(data->derpy); /* offset */
	struct Obstacle *tmp = data->dodger.obstacles;
	while (tmp) {
		/*PrintConsole(game, "DRAWING %f %f", tmp->x, tmp->y);*/
		int x = (tmp->x/100.0)*game->viewport.width;
		int y = (tmp->y/100.0)*game->viewport.height;
		int w = 0, h = 0;
		if (tmp->bitmap) {
			w = al_get_bitmap_width(*(tmp->bitmap))/tmp->cols;
			h = al_get_bitmap_height(*(tmp->bitmap))/tmp->rows;
		}
		if (x > -w) {
			/*if (!tmp->hit)*/
			if ((((x>=derpyx+0.38*derpyw+derpyo) && (x<=derpyx+0.94*derpyw+derpyo)) || ((x+w>=derpyx+0.38*derpyw+derpyo) && (x+w<=derpyx+0.94*derpyw+derpyo)) || ((x<=derpyx+0.38*derpyw+derpyo) && (x+w>=derpyx+0.94*derpyw+derpyo))) &&
					(((y>=derpyy+0.26*derpyh) && (y<=derpyy+0.76*derpyh)) || ((y+h>=derpyy+0.26*derpyh) && (y+h<=derpyy+0.76*derpyh)) || ((y<=derpyy+0.26*derpyh) && (y+h>=derpyy+0.76*derpyh)))) {
				tmp->hit=true;
			}

			if (tmp->anim_speed) {
				tmp->tmp_pos+=1;
				if (tmp->tmp_pos >= tmp->anim_speed) {
					tmp->pos++;
					tmp->tmp_pos = 0;
				}
				if (tmp->pos>=tmp->cols*tmp->rows-tmp->blanks) tmp->pos=0;
			}

			if (tmp->hit) {
				if (tmp->points>0) tmp->bitmap = NULL;
				data->hp+=0.0002*tmp->points*(((1-data->speed_modifier)/2.0)+1);
				if (data->hp>1) data->hp=1;
				//PrintConsole(game, "POINTS: %d, %f", tmp->points, tps(game, 60*0.0002*tmp->points*data->speed_modifier));
				if ((data->hp<=0) && (!data->failed)) {
					data->failed = true;
					data->handle_input = false;
					data->speed_modifier = 1;
					TM_AddBackgroundAction(&LevelFailed, NULL, 0, "levelfailed");
				}
			}
			tmp->x -= data->speed*data->speed_modifier*tmp->speed*100*al_get_bitmap_width(data->stage)/(float)game->viewport.width;
			if (tmp->callback) tmp->callback(game, tmp);
			tmp = tmp->next;
		} else {
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			else
				data->dodger.obstacles = tmp->next;
			struct Obstacle *t = tmp;
			tmp = tmp->next;
			free(t);
		}
	}
	/*if (colision) data->hp-=tps(game, 60*0.002);*/

}

void Dodger_Draw(struct Game *game, struct Dodger* data) {
	int derpyx = data->derpy_x*game->viewport.height*1.6;
	int derpyy = data->derpy_y*game->viewport.height;
	int derpyw = al_get_bitmap_width(data->derpy);
	int derpyh = al_get_bitmap_height(data->derpy);
	int derpyo = game->viewport.height*1.6*0.1953125-al_get_bitmap_width(data->derpy); /* offset */
	bool colision = false;
	struct Obstacle *tmp = data->dodger.obstacles;
	while (tmp) {
		/*PrintConsole(game, "DRAWING %f %f", tmp->x, tmp->y);*/
		int x = (tmp->x/100.0)*game->viewport.width;
		int y = (tmp->y/100.0)*game->viewport.height;
		int w = 0, h = 0;
		if (tmp->bitmap) {
			w = al_get_bitmap_width(*(tmp->bitmap))/tmp->cols;
			h = al_get_bitmap_height(*(tmp->bitmap))/tmp->rows;
		}
		if (x > -w) {
			if ((tmp->hit) && (tmp->points<0)) {
				colision = true;
			}

			if (tmp->bitmap) {
				ALLEGRO_BITMAP* subbitmap = al_create_sub_bitmap(*(tmp->bitmap),w*(tmp->pos%tmp->cols), h*(tmp->pos/tmp->cols),w,h);
				al_draw_rotated_bitmap(subbitmap,w/2.0, h/2.0, x+w/2.0,y+h/2.0, tmp->angle, 0);
				al_destroy_bitmap(subbitmap);
			}

			/*al_draw_bitmap(*(tmp->bitmap), x, y, 0);*/
			if (data->debug_show_sprite_frames) al_draw_rectangle(x, y, x+w, y+h, al_map_rgba(255,0,0,255), 3);

			tmp = tmp->next;
		} else {
			if (tmp->next)
				tmp->next->prev = tmp->prev;
			if (tmp->prev)
				tmp->prev->next = tmp->next;
			else
				data->dodger.obstacles = tmp->next;
			struct Obstacle *t = tmp;
			tmp = tmp->next;
			free(t);
		}
	}
	/*if (colision) data->hp-=tps(game, 60*0.002);*/

	al_set_target_bitmap(data->derpy);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(*(data->derpy_sheet),al_get_bitmap_width(data->derpy)*(data->sheet_pos%data->sheet_cols),al_get_bitmap_height(data->derpy)*(data->sheet_pos/data->sheet_cols),al_get_bitmap_width(data->derpy), al_get_bitmap_height(data->derpy),0,0,0);
	al_set_target_bitmap(al_get_backbuffer(game->display));

	al_draw_tinted_rotated_bitmap(data->derpy, al_map_rgba(255,255-colision*255,255-colision*255,255), al_get_bitmap_width(data->derpy), al_get_bitmap_height(data->derpy)/2, derpyx+game->viewport.height*1.6*0.1953125, derpyy + al_get_bitmap_height(data->derpy)/2, data->derpy_angle, 0);

	/*		if ((((x>=derpyx+0.36*derpyw) && (x<=derpyx+0.94*derpyw)) || ((x+w>=derpyx+0.36*derpyw) && (x+w<=derpyx+0.94*derpyw))) &&
		(((y>=derpyy+0.26*derpyh) && (y<=derpyy+0.76*derpyh)) || ((y+h>=derpyy+0.26*derpyh) && (y+h<=derpyy+0.76*derpyh)))) {
*/
	if (data->debug_show_sprite_frames) {
		al_draw_rectangle(derpyx+derpyo, derpyy, derpyx+derpyw+derpyo, derpyy+derpyh, al_map_rgba(0,255,0,255), 3);
		al_draw_rectangle(derpyx+0.38*derpyw+derpyo, derpyy+0.26*derpyh, derpyx+0.94*derpyw+derpyo, derpyy+0.76*derpyh, al_map_rgba(0,0,255,255), 3);
	}
}

struct Dodger* Dodger_Load(struct Game *game, int current_level) {
	struct Dodger *data = malloc(sizeof(struct Dodger));
	data->dodger.obstacles = NULL;
	data->character = CreateCharacter(game, "derpy");
	RegisterSpritesheet(game, data->character, "walk");
	RegisterSpritesheet(game, data->character, "stand");
	RegisterSpritesheet(game, data->character, "fly");
	RegisterSpritesheet(game, data->character, "run");
	return data;
}

void Dodger_Keydown(struct Game *game, struct Dodger* data, ALLEGRO_EVENT *ev) {
	if (data->handle_input) {
		if (ev->keyboard.keycode==ALLEGRO_KEY_LEFT) {
			data->speed_modifier = 0.75;
		} else if (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT) {
			data->speed_modifier = 1.3;
		}
	}
}

void Dodger_ProcessEvent(struct Game *game, struct Dodger* data, ALLEGRO_EVENT *ev) {
	if (data->handle_input) {
		if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode==ALLEGRO_KEY_LEFT)) {
			data->speed_modifier = 1;
			struct ALLEGRO_KEYBOARD_STATE keyboard;
			al_get_keyboard_state(&keyboard);
			if (al_key_down(&keyboard, ALLEGRO_KEY_RIGHT)) {
				data->speed_modifier = 1.3;
			}
		} else if ((ev->type==ALLEGRO_EVENT_KEY_UP) && (ev->keyboard.keycode==ALLEGRO_KEY_RIGHT)) {
			data->speed_modifier = 1;
			struct ALLEGRO_KEYBOARD_STATE keyboard;
			al_get_keyboard_state(&keyboard);
			if (al_key_down(&keyboard, ALLEGRO_KEY_LEFT)) {
				data->speed_modifier = 0.75;
			}
		}
	}
}

inline int Dodger_PreloadSteps(void) {
	return 7;
}

void Dodger_PreloadBitmaps(struct Game *game, struct Dodger* data, void (*progress)(struct Game*, float)) {
	//PROGRESS_INIT(Dodger_PreloadSteps());
	data->dodger.obst_bmps.pie1 = LoadScaledBitmap(game, "levels/dodger/pie1.png", game->viewport.height*1.6*0.1, game->viewport.height*0.08);

	data->dodger.obst_bmps.pie2 = LoadScaledBitmap(game, "levels/dodger/pie2.png", game->viewport.height*1.6*0.1, game->viewport.height*0.08);

	data->dodger.obst_bmps.pig = LoadScaledBitmap(game, "levels/dodger/pig.png", (int)(game->viewport.height*1.6*0.15)*3, (int)(game->viewport.height*0.2)*3);

	data->dodger.obst_bmps.screwball = LoadScaledBitmap(game, "levels/dodger/screwball.png", (int)(game->viewport.height*0.2)*4*1.4, (int)(game->viewport.height*0.2)*4);

	data->dodger.obst_bmps.muffin = LoadScaledBitmap(game, "levels/dodger/muffin.png", game->viewport.height*1.6*0.07, game->viewport.height*0.1);

	data->dodger.obst_bmps.cherry = LoadScaledBitmap(game, "levels/dodger/cherry.png", game->viewport.height*1.6*0.03, game->viewport.height*0.08);

	data->dodger.obst_bmps.badmuffin = LoadScaledBitmap(game, "levels/dodger/badmuffin.png", game->viewport.height*1.6*0.07, game->viewport.height*0.1);

}

void Dodger_UnloadBitmaps(struct Game *game, struct Dodger* data) {
	al_destroy_bitmap(data->dodger.obst_bmps.pie1);
	al_destroy_bitmap(data->dodger.obst_bmps.pie2);
	al_destroy_bitmap(data->dodger.obst_bmps.pig);
	al_destroy_bitmap(data->dodger.obst_bmps.cherry);
	al_destroy_bitmap(data->dodger.obst_bmps.muffin);
	al_destroy_bitmap(data->dodger.obst_bmps.badmuffin);
	al_destroy_bitmap(data->dodger.obst_bmps.screwball);
}

void Dodger_Unload(struct Game *game, struct Dodger* data) {
	struct Obstacle *t = data->dodger.obstacles;
	if (t) {
		while (t->next) {
			if (t->prev) free(t->prev);
			t = t->next;
		}
		free(t);
	}
	struct Spritesheet *tmp, *s = data->derpy_sheets;
	tmp = s;
	while (s) {
		tmp = s;
		s = s->next;
		free(tmp);
	}
	DestroyCharacter(game, data->character);
	free(data);
}

void Dodger_Resume(struct Game *game, struct Dodger* data) {}
void Dodger_Pause(struct Game *game, struct Dodger* data) {}
