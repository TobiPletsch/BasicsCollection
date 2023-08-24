# Use a C++ base image
FROM gcc:latest

# Set the working directory in the container
WORKDIR /app

# Copy the client source code into the container
COPY client.cpp .

# Compile the client program
RUN g++ -o client client.cpp -lpthread

# Command to run the client when the container starts
CMD ["./client"]
