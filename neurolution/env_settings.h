#pragma once
#ifndef ENV_SETTINGS_H
#define ENV_SETTINGS_H

// TODO: Read settings from a JSON file.
/*
 * Global settings for the evolution.
 */

#define MAX_NEURON 200
#define FITNESS_GOAL 1000.0
#define INPUT_SIZE 1
#define OUTPUT_SIZE 1
#define MAX_POPULATION 600
#define SPECIE_COUNT 10
#define ELITE_PROP 0.2f
#define ELITE_OFFSPRING true
#define ACT_FUNC leakyReLu

#define pLINK_NEW 0.025
#define pLINK_SHIFT 0.94
#define pLINK_TOGGLE 0.025
#define pNODE_NEW 0.01

#endif