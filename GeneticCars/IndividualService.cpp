#include "IndividualService.h"



IndividualService::IndividualService(float const mutationRate)
	:mutationRate(mutationRate),
	randomEngine(randomDevice())
{
}


IndividualService::~IndividualService()
{
}

Car IndividualService::getRandomCar()
{
	Chromosome randomChromosome = getRandomChromosome();
	Car randomCar = Car(randomChromosome);
	return randomCar;
}

vector<Car> IndividualService::getCrossoveredCars(Car parentA, Car parentB)
{
	vector<float> chromosomeVectorA = convertCarToVector(parentA);
	vector<float> chromosomeVectorB = convertCarToVector(parentB);
	int chromosomeVecotrSize = chromosomeVectorA.size();
	vector<float> chromosomeVectorAB(chromosomeVecotrSize);
	vector<float> chromosomeVectorBA(chromosomeVecotrSize);
	int splitIndex = getRandomInt(1, chromosomeVecotrSize - 1);
	for (int i = 0; i < chromosomeVecotrSize; ++i)
	{
		if (i < splitIndex) 
		{
			chromosomeVectorAB[i] = chromosomeVectorA[i];
			chromosomeVectorBA[i] = chromosomeVectorB[i];
		}
		else {
			chromosomeVectorAB[i] = chromosomeVectorB[i];
			chromosomeVectorBA[i] = chromosomeVectorA[i];
		}
	}
	vector<Car> crossoveredCars;
	Car childAB = convertVectorToCar(chromosomeVectorAB);
	Car childBA = convertVectorToCar(chromosomeVectorBA);
	crossoveredCars.push_back(childAB);
	crossoveredCars.push_back(childBA);
	return crossoveredCars;
}

Car IndividualService::getMutatedCar(Car car)
{
	vector<float> chromosomeVector = convertCarToVector(car);
	for (int i = 0; i < nrPoints; ++i)
	{
		if (checkIfMutate())
		{
			chromosomeVector[i] = getRandomPoint();
		}
	}
	for (int i = nrPoints; i < nrPoints + nrDensity; ++i)
	{
		if (checkIfMutate())
		{
			chromosomeVector[i] = getRandomDensity();
		}
	}
	for (int i = nrPoints + nrDensity; i < nrPoints + nrDensity + nrRadius; ++i)
	{
		if (checkIfMutate())
		{
			chromosomeVector[i] = getRandomRadius();
		}
	}
	Car mutatedCar = convertVectorToCar(chromosomeVector);
	return mutatedCar;
}

bool IndividualService::checkIfMutate()
{
	double mutationValue = getRandomMutationValue();
	return (mutationRate > mutationValue);
}

Chromosome IndividualService::getRandomChromosome()
{
	BodyShape randomBodyShape = getRandomBodyShape();
	vector<Wheel> wheels;
	for (int i = 0; i < NR_OF_WHEELS; ++i)
	{
		Wheel randomWheel = getRandomWheel(i);
		wheels.push_back(randomWheel);
	}
	Chromosome randomChromosome = Chromosome(randomBodyShape, wheels);
	return randomChromosome;
}

BodyShape IndividualService::getRandomBodyShape()
{
	float randomDensity = getRandomDensity();
	vector<ShapePoint> shapePoints;
	for(int i = 0; i < NR_OF_POINTS_IN_BODYSHAPE; ++i)
	{
		ShapePoint randomShapePoint = getRandomShapePoint();
		shapePoints.push_back(randomShapePoint);
	}
	BodyShape randomBodyShape = BodyShape(shapePoints, randomDensity);
	return randomBodyShape;
}

Wheel IndividualService::getRandomWheel(int wheelIndex)
{
	float randomRadius = getRandomRadius();
	float randomDensity = getRandomDensity();
	ShapePoint randomShapePoint = getRandomShapePoint();
	int index = wheelIndex;
	Wheel randomWheel = Wheel(randomRadius, randomDensity, index, randomShapePoint);
	return randomWheel;
}

ShapePoint  IndividualService::getRandomShapePoint()
{
	float x = getRandomPoint();
	float y = getRandomPoint();
	ShapePoint randomShapePoint = ShapePoint(x, y);
	return randomShapePoint;
}

float IndividualService::getRandomPoint()
{
	float randomPoint = getRandomFloat(POINT_MIN, POINT_MAX);
	return randomPoint;
}

float IndividualService::getRandomDensity()
{
	float randomDensity = getRandomFloat(DENSITY_MIN, DENSITY_MAX);
	return randomDensity;
}

float IndividualService::getRandomRadius()
{
	float randomRadius = getRandomFloat(RADIUS_MIN, RADIUS_MAX);
	return randomRadius;
}

float IndividualService::getRandomFloat(float min, float max)
{
	uniform_real_distribution<float> dist(min, max);
	float randomFloat = dist(randomEngine);
	return randomFloat;
}

double IndividualService::getRandomMutationValue()
{
	double randomMutationValue = getRandomDouble(MUTATION_RATE_MIN, MUTATION_RATE_MAX);
	return randomMutationValue;
}

double IndividualService::getRandomDouble(double min, double max)
{
	uniform_real_distribution<float> dist(min, max);
	double randomDouble = dist(randomEngine);
	return randomDouble;
}

int IndividualService::getRandomInt(int min, int max)
{
	uniform_int_distribution<int> dist(min, max);
	double randomInt = dist(randomEngine);
	return randomInt;
}

Car IndividualService::convertVectorToCar(vector<float> chromosomeVector)
{
	// BodyShape
	vector<ShapePoint> bsShapePoints;
	for (int i = 0; i < NR_OF_POINTS_IN_BODYSHAPE; ++i)
	{
		int index = i * 2;
		float x = chromosomeVector[index];
		float y = chromosomeVector[++index];
		ShapePoint sp = ShapePoint(x, y);
		bsShapePoints.push_back(sp);
	}
	float bsDensity = chromosomeVector[nrPoints];
	BodyShape bs = BodyShape(bsShapePoints, bsDensity);

	// Wheels
	vector<Wheel> wheels;
	vector<ShapePoint> whShapePoints;
	vector<float> whDensities;
	vector<float> whRadiuses;
	for (int i = NR_OF_POINTS_IN_BODYSHAPE; i < nrPoints / 2; ++i)
	{
		int index = i * 2;
		float x = chromosomeVector[index];
		float y = chromosomeVector[++index];
		ShapePoint sp = ShapePoint(x, y);
		whShapePoints.push_back(sp);
	}
	for (int i = nrPoints + 1; i < nrPoints + nrDensity; ++i)
	{
		float whDensity = chromosomeVector[i];
		whDensities.push_back(whDensity);
	}
	for (int i = nrPoints + nrDensity; i < nrPoints + nrDensity + nrRadius; ++i)
	{
		float whRadius = chromosomeVector[i];
		whRadiuses.push_back(whRadius);
	}
	for (int i = 0; i < NR_OF_WHEELS; ++i)
	{
		Wheel wh = Wheel(
			whRadiuses[i],
			whDensities[i],
			i,
			whShapePoints[i]
		);
		wheels.push_back(wh);
	}

	// Car
	Chromosome chromosome = Chromosome(bs, wheels);
	Car car = Car(chromosome);
	return car;
}

vector<float> IndividualService::convertCarToVector(Car car)
{
	vector<float> chromosomeVector;

	Chromosome chromosome = car.getChromosome();
	BodyShape bodyShape = chromosome.getBodyShape();

	// POINTS
	for (ShapePoint sp : bodyShape.getShapePoints())
	{
		float x = sp.getX();
		float y = sp.getY();
		chromosomeVector.push_back(x);
		chromosomeVector.push_back(y);
	}
	vector<Wheel> wheels = chromosome.getWheels();
	for (Wheel wh : wheels)
	{
		ShapePoint sp = wh.getShapePoint();
		float x = sp.getX();
		float y = sp.getY();
		chromosomeVector.push_back(x);
		chromosomeVector.push_back(y);
	}
	
	// DENSITIES
	float bsDensity = bodyShape.getDensity();
	chromosomeVector.push_back(bsDensity);
	for (Wheel wh : wheels)
	{
		float whDensity = wh.getDensity();
		chromosomeVector.push_back(whDensity);
	}

	// RADIUSES
	for (Wheel wh : wheels)
	{
		float whRadius = wh.getRadius();
		chromosomeVector.push_back(whRadius);
	}
	return chromosomeVector;
}