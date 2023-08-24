# Use a C++ base image
FROM gcc:latest

# Set the working directory in the container
WORKDIR /app

# Copy the server source code into the container
COPY server.cpp .

# Compile the server program
RUN g++ -o server server.cpp -lpthread

# Command to run the server when the container starts
CMD ["./server"]
