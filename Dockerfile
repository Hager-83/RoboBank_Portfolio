# Use an official C++ base image
FROM ubuntu:latest

# Install required dependencies
RUN apt-get update && apt-get install -y \
    g++ cmake git

# Set working directory
WORKDIR /app

# Install Google Test from source
RUN git clone https://github.com/google/googletest.git && \
    cd googletest && mkdir build && cd build && \
    cmake .. && make && make install

# Copy your source files into the container
COPY . .

# Compile the main program
RUN g++ -o main main.cpp portfolio.cpp cal.cpp

# Compile the test runner (with Google Test)
RUN g++ -o test_runner portfolio_test.cpp portfolio.cpp  cal.cpp -lgtest -lgtest_main -pthread

# Run tests when the container starts
CMD ["./test_runner"]
