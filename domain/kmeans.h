//
// Created by biosvos on 8/9/22.
//

#ifndef KMEANS_KMEANS_H
#define KMEANS_KMEANS_H

#include <vector>


class Observation {
public:
    Observation(int num);

    Observation(std::initializer_list<int> arr);

    size_t Size();

    std::vector<long double>::iterator begin();

    std::vector<long double>::iterator end();

private:
    std::vector<long double> elements_;
};


class Observations {
public:
    void Add(const Observation &observation);

    std::vector<Observation>::const_iterator begin() const;

    std::vector<Observation>::const_iterator end() const;

private:
    std::vector<Observation> observations_;
};

class DistanceStrategy {
public:
    virtual long double Distance(Observation a, Observation b) = 0;
};

class SimpleDistance : DistanceStrategy {
    long double Distance(Observation a, Observation b) override;

};

class SamplingStrategy {
public:
    virtual Observations Sampling(int k, Observations observations) = 0;
};

class AverageStrategy {
public:
    virtual Observation Average(Observations observations) = 0;
};

class Centroid {
public:
    explicit Centroid(Observation center);

    void Add(const Observation &observation);

    long double CalculateDistance(DistanceStrategy *distance, Observation observation) const;
    Centroid Adjust(AverageStrategy *average) const;

private:
    Observation center_;
    Observations belongings_;
};

class Centroids {
public:
    void Add(const Centroid &centroid);

    Centroid FindNearest(DistanceStrategy *distance, const Observation &observation);

    Centroids Adjust(AverageStrategy *average);

private:
    std::vector<Centroid> centroids_;
};


class KMeans {
public:
    KMeans();

    Centroids Run(int k, const Observations &observations);

private:
    DistanceStrategy *distance_;
    AverageStrategy *average_;
    SamplingStrategy *sampling_;

    Centroids DetermineCentroids(int k, const Observations &observations);
};

#endif //KMEANS_KMEANS_H
