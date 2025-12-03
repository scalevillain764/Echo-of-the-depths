#pragma once
#include <iostream>

enum MenuState { Play, Exit, None }; // Ћокальное состо€ние меню
enum GameState { Menu_gs, Playing, Exit_gs }; // состо€ние игры
enum Locations { // локации
	first,
	second,
	third,
	count,
	hub,
	wastelands_of_ash,
	forests_of_echo,
	broken_lands,
	memoeris_of_cold
};

enum FadeStage {
	NoneFade,
	FadeToBlack,
	ShowStory,
	ShowLabel,
	FadeOutLabel
};