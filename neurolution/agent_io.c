#include "neurolution/agent_io.h"

#include "neurolution/neuron.h"
#include "neurolution/mutations.h"

#include "tools/utils.h"

#include "data_structures/clist.h"
#include "data_structures/vector.h"

#include "CPPN/activations.h"

#include <stdio.h>

void print_agent(Agent* agent)
{
	printf("agent:\n");
	CY_ITER_DATA(agent->neuronList, neuron_node, neuron, Neuron*,
		printf("neuron %d => value : %lf\n", neuron->id, neuron->value);
		CY_ITER_DATA(neuron->linkList, link_node, link, Link*,
		   printf("(%d>%d) : %d -> %d [%.2lf, %d]\n", neuron->id, link->id, link->source->id, neuron->id, link->weight, link->enabled);
		);
		printf("\n");
	);

	printf("Neurons: %d | Links: %d | Fitness: %lf | Weights: %lf\n\n", cy_len(agent->neuronList), cy_len(agent->linkList), agent->fitness, agent_weight(agent));
}

// TODO: maybe different format for neurons like: 'neuron_id': 'incoming connections'
// TODO: load special activation function from file
// TODO: log enabled/disabled neurons and bias
// TODO: retrive inputs/outputs links and save them in inputVector/outputVector
bool save_agent(char filename[], Agent* agent)
{
	FILE* target_file = fopen(filename, "w");
	if (target_file == NULL)
	{
		fprintf(stderr, "Cannot write agent data in file '%s'.", filename);
		return false;
	}

	fprintf(target_file, "# Input and output node counts.\n");
	fprintf(target_file, "%d\t%d\n\n", agent->inputVector.count, agent->outputVector.count);

	fprintf(target_file, "# Connections (source target weight enabled).\n");
	CY_ITER_DATA(agent->linkList, link_node, link, Link*,
		fprintf(target_file, "%d\t%d\t%lf\n", link->source->id, link->target->id, link->weight);
	);

	fprintf(target_file, ";\n\n");

	fprintf(target_file, "# Activation functions (functionId functionCode).\n");
	fprintf(target_file, "0 None\n\n");

	fputc(EOF, target_file);

	fclose(target_file);

	//printf("Save agent in file: %s\n", filename);
	return true;
}

Agent* load_agent(char filename[])
{
	Agent* new_agent;
	char line[255];

	uint32_t inputSize = 0, outputSize = 0;
	uint32_t id_source, id_target;
	Neuron* neuron_source;
	Neuron* neuron_target;
	double weight;
	// uint32_t activationFunctionId;
	// char activationFunctionName[255];

	bool r_ioSize = false, r_connections = false, r_activationFunction = false;

	FILE* target_file = fopen(filename, "r");

	while (fgets(line, sizeof(line), target_file) != NULL)
	{
		if (line[0] == '#' || line[0] == '\n')
			continue;
		
		if (!r_ioSize)
		{
			sscanf(line, "%d\t%d\n", &inputSize, &outputSize);
			new_agent = new_Agent(inputSize, outputSize);

			r_ioSize = true;
			continue;
		}

		if (!r_connections)
		{
			if (line[0] == ';')
			{
				r_connections = true;
				continue;
			}

			#pragma warning(disable : 4477)
			sscanf(line, "%d\t%d\t%lf\n", &id_source, &id_target, &weight);

			if (!isNeuronInAgent(new_agent, id_source+1))
			{
				neuron_source = new_BasicNeuron(id_source+1, &ACT_FUNC);
				cy_insert(&new_agent->neuronList, neuron_source);
			}
			else
			{
				neuron_source = getNeuronInAgent(new_agent, id_source+1);
			}

			if (!isNeuronInAgent(new_agent, id_target+1))
			{
				neuron_target = new_BasicNeuron(id_target+1, &ACT_FUNC, 0);
				cy_insert(&new_agent->neuronList, neuron_target);
			}
			else
			{
				neuron_target = getNeuronInAgent(new_agent, id_target+1);
			}

			cy_insert(&new_agent->linkList, new_Link(neuron_source, neuron_target, weight, 1));
			continue;
		}

		if (!r_activationFunction)
		{
			r_activationFunction = true;
			continue;
		}
	}

	fclose(target_file);

	CY_ITER_DATA(new_agent->neuronList, neuron_node, neuron, Neuron*,
		if (neuron->id <= inputSize)
		{
			VEC(new_agent->inputVector, Neuron*, neuron->id - 1) = neuron;
			neuron->type = INPUT_TYPE;
		}
		else if (neuron->id <= inputSize + outputSize)
		{
			VEC(new_agent->outputVector, Neuron*, neuron->id - inputSize - 1) = neuron;
			neuron->type = OUTPUT_TYPE;
		}
	);

	//printf("Loaded agent from file: %s\n", filename);
	return new_agent;
}

// TODO: function system(...)
int plot_agent(Agent* agent, char pid_str[])
{
#ifdef _WIN32
	char cmd[255] = "";          // command that will launch 'plot.py'
	char unique_id[39] = "";	 // unique id used as title of the cmd window
	char tasklist_cmd[255] = ""; // command to get the pid of a given window
	char pid_tmp[255] = "";      // store the new process pid temporary
	char tmp_file[255] = "";     // store the temporary agent file generated

	FILE* pipe; // used to read the output of windows commands
	
    // generate a unique ID
	pipe = _popen("powershell '{'+[guid]::NewGuid().ToString()+'}'", "r");
	fgets(unique_id, 39, pipe);
	_pclose(pipe);

    // begining of the 'cmd' in the form: 'start "<unique_id>" python '
	strcat(cmd, "start \"");
	strcat(cmd, unique_id);
	strcat(cmd, "\" python ");
	
    // save temp agent file at '<BIN_PATH>/<unique_id>'
	strcat(tmp_file, BIN_PATH);
	strcat(tmp_file, unique_id);
	save_agent(tmp_file, agent);

    // continue the 'cmd' with '<BIN_PATH>/plot.py <BIN_PATH>/<unique_id>'
	strcat(cmd, BIN_PATH);
	strcat(cmd, "plot.py ");
	strcat(cmd, tmp_file);

    // call the command
	// printf(cmd);
	system(cmd);

    // retrieve our command process PID
	strcat(tasklist_cmd, "tasklist /v /fo csv | findstr /i \"");
	strcat(tasklist_cmd, unique_id);
	strcat(tasklist_cmd, "\"");

	pipe = _popen(tasklist_cmd, "r");
	fgets(pid_tmp, 15, pipe);
	fgets(pid_tmp, 20, pipe);
	_pclose(pipe);

    // extract PID from the output of 'tasklist'
	for (int i = 0; i < 20; i++)
	{
		if (pid_tmp[i] == '"' || pid_tmp[i] == '\0')
		{
			pid_tmp[i] = '\0';
			break;
		}
	}

    // return the pid as a string if needed
	if (pid_str != NULL)
		strcpy(pid_str, pid_tmp);

	return atoi(pid_tmp);

#endif
	return 0;
}

void show_agent(Agent* agent)
{
	char pid[255];
    plot_agent(agent, pid);
    system("Pause");
    plot_close(pid);
}

void plot_close(char* pid_str)
{
	if (pid_str[0] == '\0') return;
	char cmd[100] = "taskkill /F /PID ";    
	strcat(cmd, pid_str);
	system(cmd);
}

void plot_fileclear()
{
	char cmd[255] = "start cmd /c del ";
    strcat(cmd, BIN_PATH);
    strcat(cmd, "{*}");
    system(cmd);
}

void interactive_agent_plot(uint32_t inputSize, uint32_t outputSize)
{
	Agent* agent = new_Agent(inputSize, outputSize);

	#if _WIN32

    char inp = '\0';
    char pid[32];
    char cmd[100];
    while (inp != 'q')
    {
        plot_agent(agent, pid);
        printf(">");
        scanf(" %c", &inp);

        strcpy(cmd, "start taskkill /F /PID ");    
        strcat(cmd, pid);
        system(cmd); 

        switch (inp)
        {
            case 'l':
                mutate_link_add(agent);
                break;
            case 's':
                mutate_link_shift(agent, 0.2);
                break;
            case 't':
                mutate_link_toggle(agent);
                break;
            case 'n':
                mutate_neuron_insert(agent, NULL);
                break;
            default:
            break;
        }
    }
    
#endif // !_WIN32

	free_agent(&agent);
}