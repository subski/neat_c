#include "neurolution/agent_io.h"

#include "neurolution/neuron.h"

#include "tools/utils.h"

#include <stdio.h>

void print_agent(Agent* agent)
{
	printf("agent:\n");
	ITER_V(agent->neuronList, neuron_node, neuron, Neuron*,
		   ITER_V(neuron->linkList, link_node, link, Link*,
		   printf("%d -> %d [%.2lf, %d]\n", link->source->id, neuron->id, link->weight, link->enabled);
	);	);

	printf("Neurons: %d | Links: %d | Fitness: %lf\n\n", len(agent->neuronList), len(agent->linkList), agent->fitness);
}

// TODO: maybe different format for neurons like: 'neuron_id': 'incoming connections'
// TODO: load special activation function from file
// TODO: log enabled/disabled neurons and bias
// TODO: log enabled/disabled connections information
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

	fprintf(target_file, "# Connections (source target weight).\n");
	ITER_V(agent->linkList, link_node, link, Link*,
		fprintf(target_file, "%d %d %lf\n", link->source->id, link->target->id, link->weight);
	);

	fprintf(target_file, ";\n\n");

	fprintf(target_file, "# Activation functions (functionId functionCode).\n");
	fprintf(target_file, "0 None\n\n");

	fputc(EOF, target_file);

	fclose(target_file);

	printf("Save agent in file: %s\n", filename);
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

			sscanf(line, "%d\t%d\t%lf\n", &id_source, &id_target, &weight);

			if (!isNeuronInAgent(new_agent, id_source))
			{
				neuron_source = new_BasicNeuron(id_source);
				insert(&new_agent->neuronList, neuron_source);
			}
			else
			{
				neuron_source = getNeuronInAgent(new_agent, id_source);
			}

			if (!isNeuronInAgent(new_agent, id_target))
			{
				neuron_target = new_BasicNeuron(id_target);
				insert(&new_agent->neuronList, neuron_target);
			}
			else
			{
				neuron_target = getNeuronInAgent(new_agent, id_target);
			}

			insert(&new_agent->linkList, new_Link(neuron_source, neuron_target, weight, true));
			
			// r_connections = true;
			continue;
		}

		if (!r_activationFunction)
		{
			r_activationFunction = true;
			continue;
		}
	}

	fclose(target_file);

	printf("Loaded agent from file: %s\n", filename);

	return new_agent;
}

// TODO: function system(...)
int plot_agent(Agent* agent, char pid_str[])
{
	printf("Plotting agent.\n");
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