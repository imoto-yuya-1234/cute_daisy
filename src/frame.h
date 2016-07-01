#pragma once

#include "pebble.h"

static const GPathInfo MINUTE_HAND_POINTS = {
  4, (GPoint []) {
		{0, 0},
    {5, -30},
    {0, -60},
    {-5, -30}
  }
};

static const GPathInfo HOUR_HAND_POINTS = {
  4, (GPoint []){
    {0, 0},
    {5, -20},
    {0, -40},
    {-5, -20}
  }
};

static const GPathInfo HEART_POINTS = {
	10, (GPoint []){
		{0, 0},
		{6, 5},
		{8, 8},
		{6, 12},
		{3, 12},
		{0, 8},
		{-3, 12},
		{-6, 12},
		{-8, 8},
		{-6, 5}
	}
};

static const GPathInfo STAR_POINTS = {
	10, (GPoint []) {
		{0, 0},
		{6, -3},
		{3, 3},
		{8, 6},
		{2, 6},
		{0, 12},
		{-2, 6},
		{-8, 6},
		{-3, 3},
		{-6, -3}
	}
};