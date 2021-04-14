# Neural network training via [evolutionary strategies](https://openai.com/blog/evolution-strategies/)

Evolutionary strategies work by updating an entire set of weights
by jittering several copies and interpolating towards better ones and
away from worse ones, rather than by inferring weight changes through
backpropagation.

> **Note:** here, evolutionary strategies are classified - and thus
regarded - as a _reinforcement learning_ algorithm, despite
being, in the linked article, "pitted" against RL; what is
not made clear there is that it is pitted against **standard**
reinforcement learning algorithms; that is, fitting a neural
network to reduce a Q-function and backpropagating those changes,
rather than operating directly on the parameters (in this case
neural network weights) themselves as proposed by ES.


