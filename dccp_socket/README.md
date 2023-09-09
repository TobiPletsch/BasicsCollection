# Socket Communication with Docker Compose

This project demonstrates a simple socket-to-socket communication using DCCP protocol with the support of Docker Compose.

## Prerequisites

- Docker
- Docker Compose

## Project Structure

- `Dockerfile.server`: Dockerfile to set up the server environment.
- `Dockerfile.client`: Dockerfile to set up the client environment.
- `server.c`: Server program written in C to establish a DCCP socket.
- `client.c`: Client program written in C to communicate with the server via a DCCP socket.
- `docker-compose.yml`: Docker Compose file to orchestrate the deployment of server and client.
- `README.md`: This file, containing setup and run instructions.

## Setup & Installation

1. Ensure Docker and Docker Compose are installed on your system.
2. Clone this repository to your local system.
3. Navigate to the project directory where the Dockerfiles and `docker-compose.yml` are located.

## Building & Running the Project

1. Build and start the containers by running:
   
   ```sh
   docker-compose up --build


## Viewing TCPDump

The server container has `tcpdump` installed. If you wish to trace the communication using `tcpdump`, you might need to adjust the Dockerfile or docker-compose file to grant necessary privileges.

## Stopping the Project

To stop the containers, use the following command:

```sh
docker-compose down

