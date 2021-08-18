#ifndef ENV_SETTINGS_H
#define ENV_SETTINGS_H

// TODO: Read settings from a JSON file.
/*
 * Global settings for the evolution.
 */
struct env_settings
{
    const unsigned MaxNeuron;
    const double fitnessGoal;
} ENV_SETTINGS = { 200, 14.0 };

typedef struct env_settings env_settings;

#endif