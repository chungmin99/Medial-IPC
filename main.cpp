#include "Commom/tinyxml/tinyxml.h"
#include "Commom/PolynomialSolver.h"

#include "Simulator/BaseSimulator.h"
#include "Simulator/mipc/MipcSimulator.h"
#include "Simulator/SimulatorFactor.h"

bool detectCudaDevice(double& gpu_mem);
BaseSimulator* importFromConfigFile(const std::string filename);
BaseSimulator* readSimulatorConfigFile(TiXmlElement* subItem);

int main(int argc, char *argv[])
{
	double gpu_mem = 0;
	if (!detectCudaDevice(gpu_mem))
		return 0;

	printf("Reading config file..."); fflush(stdout);
	BaseSimulator* sim = importFromConfigFile("../example/test/test.xml");
	printf("done\n"); fflush(stdout);

	printf("Initializing simulator...\n"); fflush(stdout);
	sim->initialization();
	printf("done\n"); fflush(stdout);

	printf("Running simulator...\n"); fflush(stdout);
	for (int frame = 0; frame < 50; frame++) {
		auto start = std::chrono::system_clock::now();
		sim->run(frame);
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> elapsed_seconds = end-start;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);

		std::cout << "finished computation at " << std::ctime(&end_time)
				<< "elapsed time: " << elapsed_seconds.count() << "s\n";
	}
	printf("done\n"); fflush(stdout);
}

BaseSimulator* importFromConfigFile(const std::string filename)
{
	TiXmlDocument doc(filename.c_str());
	doc.LoadFile();
	if (doc.Error() && doc.ErrorId() == TiXmlBase::TIXML_ERROR_OPENING_FILE) {
		std::cout << "Error: can't read config file !" << std::endl;
		return nullptr;
	}
	TiXmlElement* headerItem = doc.FirstChildElement();
	if (!headerItem)
		return nullptr;
	std::string checkItemName = headerItem->Value();
	if (checkItemName != std::string("SimFramework"))
		return nullptr;

	// readSimulator
	BaseSimulator *sim = readSimulatorConfigFile(headerItem);

	// read OtherInfo
	TiXmlElement* subItem = headerItem->FirstChildElement();

	while (subItem)
	{
		if (!readSimulatorFromConfigFile(sim, filename, subItem))
		{
			std::cout << "Error: can't read config file !!!" << std::endl;
			return nullptr;
		}
		subItem = subItem->NextSiblingElement();
	}
	return sim;
}

BaseSimulator* readSimulatorConfigFile(TiXmlElement* subItem)
{
	TiXmlAttribute * attri = subItem->FirstAttribute();
	std::string simName;
	std::string runPlaformName;
	while (attri)
	{
		std::string attriName = attri->Name();
		if (attriName == std::string("name"))
		{
			simName = attri->Value();
		}
		else if (attriName == std::string("run"))
		{
			runPlaformName = attri->Value();
		}
		attri = attri->Next();
	}

	RunPlatform run = RunPlatform::CPU;
	if (runPlaformName == "OPEMMP")
		run = RunPlatform::OPEMMP;
	else if (runPlaformName == "CUDA")
		run = RunPlatform::CUDA;

	return createSimulator(simName, run);
}

bool detectCudaDevice(double& gpu_mem)
{
	int deviceCount = 0;
	cudaError_t error_id = cudaGetDeviceCount(&deviceCount);
	if (error_id != cudaSuccess) {
		printf("cudaGetDeviceCount returned %d\n-> %s\n",
			static_cast<int>(error_id), cudaGetErrorString(error_id));
		printf("Result = FAIL\n");
		system("pause");
		return false;
	}

	// This function call returns 0 if there are no CUDA capable devices.
	if (deviceCount == 0) {
		printf("There are no available device(s) that support CUDA\n");
		system("pause");
		return false;
	}
	else {

		int dev, driverVersion = 0, runtimeVersion = 0;

		for (dev = 0; dev < deviceCount; ++dev) {
			cudaSetDevice(dev);
			cudaDeviceProp deviceProp;
			cudaGetDeviceProperties(&deviceProp, dev);
			printf("Detected a CUDA Capable device(s): \"%s\" \n", deviceProp.name);
			// Console log
			cudaDriverGetVersion(&driverVersion);
			cudaRuntimeGetVersion(&runtimeVersion);
			printf("CUDA Driver Version / Runtime Version    %d.%d / %d.%d\n",
				driverVersion / 1000, (driverVersion % 100) / 10,
				runtimeVersion / 1000, (runtimeVersion % 100) / 10);
			printf("CUDA Capability Major/Minor version number:   %d.%d\n",
				deviceProp.major, deviceProp.minor);
			printf("\n");

			int size = deviceProp.totalGlobalMem / (1024.0 * 1024.0 * 1024.0);
			printf("GPU Memory:   %d G\n",
				size);
			printf("\n");

			gpu_mem = deviceProp.totalGlobalMem / (1024.0 * 1024.0 * 1024.0);
		}
	}

	return true;

}
