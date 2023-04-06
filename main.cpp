#include <fstream>
#include <iostream>
#include <vector>
#include "SFML/Graphics.hpp"

using namespace std;

class mnist_database
{
public:
	int num_pixels = 60000 * 28 * 28;
	int num_images = 60000;
	vector<unsigned char> images;
	vector<unsigned char> labels;

	mnist_database()
	{
		ifstream image_file;
		ifstream label_file;
		char byte = '\0';

		// load in the digit images
		image_file.open("IDX/train-images.idx3-ubyte", ios::binary);
		if (!image_file.is_open())
		{
			cout << "Oh no! I couldn't open this file! :c\n";
		}
		for (int i = 0; i < 16; i++) // Skip the first 16 bytes
		{
			image_file.read(&byte, 1);
		}
		for (int i = 0; i < num_pixels; i++) // Read the rest of the bytes
		{
			image_file.read(&byte, 1);
			images.push_back((unsigned char)byte);
		}

		// load in the digit labels
		label_file.open("IDX/train-labels.idx1-ubyte", ios::binary);
		if (!label_file.is_open())
		{
			cout << "Oh no! I couldn't open this file! :c\n";
		}
		for (int i = 0; i < 8; i++)
		{
			label_file.read(&byte, 1);
		}
		for (int i = 0; i < num_images; i++)
		{
			label_file.read(&byte, 1);
			labels.push_back((unsigned char)byte);
		}

		image_file.close();
		label_file.close();
	}

	unsigned char color(int index, int i, int j)
	{
		return images[index * 28 * 28 + i * 28, j];
	}

	unsigned char label(int index)
	{
		return labels[index];
	}
};

class neuron
{
public:
	float activation = 0.0f;

	int index = 0;

	int number_of_weights = 0;
	vector<float> weights;

	neuron(int num_weights, int ind)
	{
		index = ind;
		number_of_weights = num_weights;
		for (int i = 0; i < number_of_weights; i++)
		{
			weights.push_back(random_float());
		}
	}

	float random_float()
	{
		return (float)rand() / (float)RAND_MAX;
	}

	float& operator[](size_t index)
	{
		return weights[index];
	}
};


class layer
{
public:
	vector<neuron> neurons;

	int index = 0;
	int number_of_neurons = 0;
	int number_of_weights_per_neuron = 0;

	bool is_input_layer = false;
	bool is_output_layer = false;

	layer(int ind, int* neurons_per_layer, bool input, bool output)
	{
		index = ind;
		number_of_neurons = neurons_per_layer[index];

		is_input_layer = input;
		is_output_layer = output;

		for (int i = 0; i < number_of_neurons; i++)
		{
			if (!is_input_layer)
			{
				number_of_weights_per_neuron = neurons_per_layer[index - 1];
			}
			neuron neuron(number_of_weights_per_neuron, i);
			neurons.push_back(neuron);
		}
	}

	neuron& operator[](size_t index)
	{
		return neurons[index];
	}
};

class neural_network
{
public:
	mnist_database* ptr_mnist_data = nullptr;

	vector<layer> layers;

	int number_of_layers = 4;
	int neurons_per_layer[4] = { 28 * 28, 20, 20, 10 };

	neural_network(mnist_database* ptr_mnist)
	{
		ptr_mnist_data = ptr_mnist;

		for (int i = 0; i < number_of_layers; i++)
		{
			layer layer(i, neurons_per_layer, i == 0, i == number_of_layers - 1);
			layers.push_back(layer);
		}
	}

	void percieve(int index)
	{
		set_input_activations(index);
		propagate_activations();
		back_propagate_activation_margins();
	}

	void set_input_activations(int index)
	{
		layer& input_layer = layers[0];
		mnist_database& mnist_database = *ptr_mnist_data;

		for (neuron& neuron : input_layer.neurons)
		{
			int i = neuron.index / 28;
			int j = neuron.index % 28;

			neuron.activation = (float)mnist_database.color(index, i, j) / 255.0f;
		}
	}

	void propagate_activations()
	{

	}

	void back_propagate_activation_margins()
	{

	}

	layer& operator[](size_t index)
	{
		return layers[index];
	}
};

class network_visualiser
{
public:
	neural_network* ptr_neural_net = nullptr;

	network_visualiser(neural_network* ptr_nn) : ptr_neural_net(ptr_nn) {}

	void draw(sf::RenderWindow& window, int index)
	{
		neural_network& neural_network = *ptr_neural_net;
		mnist_database& mnist_database = *neural_network.ptr_mnist_data;
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Hello SFML", sf::Style::Close);
	sf::Event event;

	mnist_database mnist_database;
	neural_network neural_network(&mnist_database);
	network_visualiser network_visualiser(&neural_network);

	int index = 0;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		neural_network.percieve(index);

		window.clear(sf::Color(0, 0, 0));
		network_visualiser.draw(window, index);
		window.display();

		index++;
		if (index == 60000)
		{
			index = 0;
		}
	}

	return 0;
}

