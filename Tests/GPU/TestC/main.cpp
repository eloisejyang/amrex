#include <cuda_device_runtime_api.h>
#include <thrust/device_vector.h>

#include <iostream>
#include <AMReX.H>
#include <AMReX_Print.H>

#include <AMReX_Utility.H>
#include <AMReX_Managed.H>
#include <AMReX_Device.H>
#include <AMReX_Geometry.H>
#include <AMReX_ArrayLim.H>
#include <AMReX_Vector.H>
#include <AMReX_IntVect.H>
#include <AMReX_BaseFab_f.H>
#include <AMReX_CudaAllocators.H>

// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

using namespace amrex;

int main (int argc, char* argv[])
{
    std::cout << "**********************************\n";
//    Device::initialize_device();
    amrex::Initialize(argc, argv);
    amrex::Print() << "amrex::Initialize complete." << "\n";

    // ===================================
    // Simple cuda action to make sure all tests have cuda.
    // Allows nvprof to return data.

    int devices;
    cudaGetDeviceCount(&devices);
    amrex::Print() << "Hello world from AMReX version " << amrex::Version() << ". GPU devices: " << devices << "\n";
    amrex::Print() << "**********************************\n"; 

    // ===================================

    int* n_cpy;
    cudaMallocManaged(&n_cpy, sizeof(int));
    *n_cpy = 0;

    amrex::Print() << "n_cpy before = " << *n_cpy << std::endl << std::endl;


    AMREX_SIMPLE_L_LAUNCH(1,1,
    [=] AMREX_CUDA_DEVICE ()
    {
       *n_cpy = 1;
    });

    int n;
    Device::synchronize();
    cudaMemcpy(&n, n_cpy, sizeof(int), cudaMemcpyDeviceToHost);
    amrex::Print() << "n after = " << n << std::endl << std::endl;
//    amrex::Print() << "n/n_cpy after = " << n << "/" << *n_cpy << std::endl << std::endl;

    cudaFree(n_cpy);
    amrex::Print() << std::endl << "*************************************\n";

    amrex::Finalize();
}
