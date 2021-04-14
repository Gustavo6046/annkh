# Neural network training design and implementation

## Roadmap

What follows is a concise layout of the plans being considered
for implementing fitting of neural network models in annkh.

### Training Context

A network should be able to be trained with supervised learning
methods prior to usage, but also should be able to learn during
usage with unsupervised/reinforcement learning methods.

### Scoring

Reinforcement learning methods (which include [evolutionary
strategies](evolve-strats.training.design])) may take an additional
parameter, a rule to be used to deem a neural network's fitness
during training.

### Fitting

There ought to be multiple ways to update a neural network to
maximize fitness, ranging from backpropagation to evolutionary
strategies. We value computational performance and conceptual
simplicity over popularity of usage in the industry and trades.
