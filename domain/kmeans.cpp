//
// Created by biosvos on 8/9/22.
//

#include <cassert>
#include <cstddef>
#include <cmath>
#include <utility>
#include "kmeans.h"

KMeans::KMeans() {}

Centroids KMeans::Run(int k, const Observations &observations) {
    auto centroids = DetermineCentroids(k, observations);

    for (int i = 0; i < 10; ++i) {
        for (const auto &item: observations) {
            auto cent = centroids.FindNearest(distance_, item);
            cent.Add(item);
        }
        centroids = centroids.Adjust(average_);
    }

    return centroids;
}

Centroids KMeans::DetermineCentroids(int k, const Observations &observations) {
    auto obs = sampling_->Sampling(k, observations);
    Centroids ret;
    for (const auto &item: obs) {
        ret.Add(Centroid(item));
    }

    return ret;
}

Observation::Observation(int num) : elements_({static_cast<long double>(num)}) {}

Observation::Observation(std::initializer_list<int> arr) {
    elements_.reserve(arr.size());
    for (const auto &item: arr) {
        elements_.push_back(static_cast<long double>(item));
    }
}

size_t Observation::Size() {
    return elements_.size();
}

std::vector<long double>::iterator Observation::begin() {
    return elements_.begin();
}

std::vector<long double>::iterator Observation::end() {
    return elements_.end();
}

void Observations::Add(const Observation &observation) {
    observations_.push_back(observation);
}

std::vector<Observation>::const_iterator Observations::begin() const {
    return observations_.cbegin();
}

std::vector<Observation>::const_iterator Observations::end() const {
    return observations_.cend();
}

long double SimpleDistance::Distance(Observation a, Observation b) {
    assert(a.Size() == 1);
    assert(b.Size() == 1);
    auto ait = a.begin();
    auto bit = b.begin();

    return std::abs(*ait - *bit);
}

Centroid::Centroid(Observation center) : center_(std::move(center)) {}

long double Centroid::CalculateDistance(DistanceStrategy *distance, Observation observation) const {
    return distance->Distance(center_, observation);
}

void Centroid::Add(const Observation &observation) {
    belongings_.Add(observation);
}

Centroid Centroid::Adjust(AverageStrategy *average) const {
    auto obs = average->Average(belongings_);
    return Centroid(obs);
}

void Centroids::Add(const Centroid &centroid) {
    centroids_.push_back(centroid);
}

Centroid Centroids::FindNearest(DistanceStrategy *distance, const Observation &observation) {
    long double min = std::numeric_limits<long double>::max();
    int min_idx = 0;
    for (int i = 0; i < centroids_.size(); ++i) {
        auto tmp = centroids_[i].CalculateDistance(distance, observation);
        if (min > tmp) {
            min = tmp;
            min_idx = i;
        }
    }
    return centroids_[min_idx];
}

Centroids Centroids::Adjust(AverageStrategy *average) {
    Centroids n;
    for (const auto &item: centroids_) {
        n.Add(item.Adjust(average));
    }
    return n;
}
