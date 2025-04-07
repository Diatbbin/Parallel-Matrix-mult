#include <vector>
#include <iostream>
#include <random>
#include <functional>
#include <chrono>
#include <hpx/algorithm.hpp>
#include <hpx/execution.hpp>
#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>

std::mt19937 gen;

void print_matrix(std::vector<int> const& M, std::size_t rows, std::size_t cols,
    char const* message);

int hpx_main(hpx::program_options::variables_map& vm);

using namespace hpx::program_options;
options_description cmdline("usage: " HPX_APPLICATION_STRING " [options]");

int main(int argc, char* argv[]) 
{
    cmdline.add_options()
        ("help", "Print help message")
        ("n",
        hpx::program_options::value<std::size_t>()->default_value(3),
        "Number of rows of first matrix")
        ("m",
        hpx::program_options::value<std::size_t>()->default_value(3),
        "Number of columns of first matrix (equal to the number of rows of "
        "second matrix)")
        ("k",
        hpx::program_options::value<std::size_t>()->default_value(3),
        "Number of columns of second matrix")
        ("seed",
        hpx::program_options::value<unsigned int>(),
        "The random number generator seed to use for this run")
        ("l",
        hpx::program_options::value<int>()->default_value(0),
        "Lower limit of range of values")
        ("u",
        hpx::program_options::value<int>()->default_value(10),
        "Upper limit of range of values");
    hpx::init_params init_args;
    init_args.desc_cmdline = cmdline;

    return hpx::init(hpx_main, argc, argv, init_args);
}

int hpx_main(hpx::program_options::variables_map& vm)
{
    using element_type = int;

    if (vm.count("help")) {
        std::cout << cmdline << "\n";  
        return hpx::finalize(); 
    }

    std::size_t const rowsA = vm["n"].as<std::size_t>();
    std::size_t const colsA = vm["m"].as<std::size_t>();
    std::size_t const rowsB = colsA;
    std::size_t const colsB = vm["k"].as<std::size_t>();
    std::size_t const rowsOutput = rowsA;
    std::size_t const colsOutput = colsB;

    // Initialize matrices A and B
    std::vector<int> A(rowsA * colsA);
    std::vector<int> B(rowsB * colsB);
    std::vector<int> Output(rowsOutput * colsOutput);

    // Define seed
    unsigned int seed = std::random_device{}();
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    gen.seed(seed);
    std::cout << "using seed: " << seed << std::endl;

    // Define range of values
    int const lower = vm["l"].as<int>();
    int const upper = vm["u"].as<int>();

    // Matrices have random values in the range [lower, upper]
    std::uniform_int_distribution<element_type> dis(lower, upper);
    auto generator = std::bind(dis, gen);
    hpx::ranges::generate(A, generator);
    hpx::ranges::generate(B, generator);

    auto startTime = std::chrono::high_resolution_clock::now();

     // Perform sequential matrix multiplication
     for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            Output[i * colsOutput + j] = 0;
            for (int k = 0; k < rowsB; ++k) {
                Output[i * colsOutput + j] += A[i * colsA + k] * B[k * colsB + j];
            };
        };
    };

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTime - startTime;
    std::cout << "Sequential Matrix multiplication took " << elapsed.count() << " seconds\n";

    startTime = std::chrono::high_resolution_clock::now();

    // Perform parallel matrix multiplication
    hpx::experimental::for_loop(hpx::execution::par, 0, rowsA, [&](auto i) {
        hpx::experimental::for_loop(0, colsB, [&](auto j) {
            Output[i * colsOutput + j] = 0;
            hpx::experimental::for_loop(0, rowsB, [&](auto k) {
                Output[i * colsOutput + j] += A[i * colsA + k] * B[k * colsB + j];
            });
        });
    });

    endTime = std::chrono::high_resolution_clock::now();
    elapsed = endTime - startTime;
    std::cout << "Parallel Matrix multiplication took " << elapsed.count() << " seconds\n";

    // Print all 3 matrices
    //print_matrix(A, rowsA, colsA, "A");
    //print_matrix(B, rowsB, colsB, "B");
    //print_matrix(Output, rowsOutput, colsOutput, "R");

    return hpx::finalize();
}

void print_matrix(std::vector<int> const& M, std::size_t rows, std::size_t cols,
    char const* message)
{
    std::cout << "\nMatrix " << message << " is:" << std::endl;
    for (std::size_t i = 0; i < rows; i++)
    {
        for (std::size_t j = 0; j < cols; j++)
            std::cout << M[i * cols + j] << " ";
        std::cout << "\n";
    }
}