A machine learning project that uses a website where you draw numbers that gets sent to and parsed by a neural network.
The NN then tells which number it thinks it is and returns it. 

The communication looks like this:
Website -> REST API -> C-daemon -> NN
and then goes backwards when the NN has a result.


TODO:

Get backpropagation to work.

Database for for storage of NN values so it doesn't need to be retrained if server goes down.

Better visual appearance on website.

Tidy up code and make some parts cleaner and smaller.
