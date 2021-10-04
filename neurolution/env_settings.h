#pragma once
#ifndef ENV_SETTINGS_H
#define ENV_SETTINGS_H

// TODO: Read settings from a JSON file.
/*
 * Global settings for the evolution.
 */

#define MAX_NEURON 200
#define FITNESS_GOAL 13.0
#define INPUT_SIZE 3
#define OUTPUT_SIZE 1
#define MAX_POPULATION 150
#define SPECIE_COUNT 10
#define ELITE_PROP 0.3f
#define ELITE_OFFSPRING true
#define ACT_FUNC leakyReLu

#endif