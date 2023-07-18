#pragma once

#include <vector>

#include "masked_image.h"
#include "nnf.h"
#include "Mat.h"

class Inpainting {
public:
    Inpainting(pm::Mat image, pm::Mat mask, const PatchDistanceMetric *metric);
    Inpainting(pm::Mat image, pm::Mat mask, pm::Mat global_mask, const PatchDistanceMetric *metric);
    pm::Mat run(bool verbose = false, bool verbose_visualize = false, unsigned int random_seed = 1212);

private:
    void _initialize_pyramid(void);
    MaskedImage _expectation_maximization(MaskedImage source, MaskedImage target, int level, bool verbose);
    void _expectation_step(const NearestNeighborField &nnf, bool source2target, pm::Mat &vote, const MaskedImage &source, bool upscaled);
    void _maximization_step(MaskedImage &target, const pm::Mat &vote);

    MaskedImage m_initial;
    std::vector<MaskedImage> m_pyramid;

    NearestNeighborField m_source2target;
    NearestNeighborField m_target2source;
    const PatchDistanceMetric *m_distance_metric;
};

