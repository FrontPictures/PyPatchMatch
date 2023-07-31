#pragma once

#include "Mat.h"
#include <cassert>
#include <iostream>

class MaskedImage {
public:
    MaskedImage() : m_image(), m_mask(), m_global_mask(), m_image_grady(), m_image_gradx(), m_image_grad_computed(false) {
        // pass
    }
    MaskedImage(pm::Mat image, pm::Mat mask) : m_image(image), m_mask(mask), m_image_grad_computed(false) {
        // pass
    }
    MaskedImage(pm::Mat image, pm::Mat mask, pm::Mat global_mask) : m_image(image), m_mask(mask), m_global_mask(global_mask), m_image_grad_computed(false) {
        // pass
    }
    MaskedImage(pm::Mat image, pm::Mat mask, pm::Mat global_mask, pm::Mat grady, pm::Mat gradx, bool grad_computed) :
        m_image(image), m_mask(mask), m_global_mask(global_mask),
        m_image_grady(grady), m_image_gradx(gradx), m_image_grad_computed(grad_computed) {
        // pass
    }
    MaskedImage(int width, int height, int type = PM_8UC3)
        : m_global_mask(), m_image_grady(), m_image_gradx()
    {
        m_image = pm::Mat(pm::Size(width, height), type);
        m_image = pm::Scalar::all(0);

        m_mask = pm::Mat(pm::Size(width, height), PM_8U);
        m_mask = pm::Scalar::all(0);
    }
    inline MaskedImage clone() {
        return MaskedImage(
            m_image.clone(), m_mask.clone(), m_global_mask.clone(),
            m_image_grady.clone(), m_image_gradx.clone(), m_image_grad_computed
        );
    }

    inline pm::Size size() const {
        return m_image.size();
    }
    inline const pm::Mat &image() const {
        return m_image;
    }
    inline const pm::Mat &mask() const {
        return m_mask;
    }
    inline const pm::Mat &global_mask() const {
        return m_global_mask;
    }
    inline const pm::Mat &grady() const {
        assert(m_image_grad_computed);
        return m_image_grady;
    }
    inline const pm::Mat &gradx() const {
        assert(m_image_grad_computed);
        return m_image_gradx;
    }

    inline void init_global_mask_mat() {
        m_global_mask = pm::Mat(m_mask.size(), PM_8U);
        m_global_mask.setTo(pm::Scalar(0));
    }
    inline void set_global_mask_mat(const pm::Mat &other) {
        m_global_mask = other;
    }

    inline bool is_masked(int y, int x) const {
        return static_cast<bool>(m_mask.at<unsigned char>(y, x));
    }
    inline bool is_globally_masked(int y, int x) const {
        return !m_global_mask.empty() && static_cast<bool>(m_global_mask.at<unsigned char>(y, x));
    }
    inline void set_mask(int y, int x, bool value) {
        m_mask.at<unsigned char>(y, x) = static_cast<unsigned char>(value);
    }
    inline void set_global_mask(int y, int x, bool value) {
        m_global_mask.at<unsigned char>(y, x) = static_cast<unsigned char>(value);
    }
    inline void clear_mask() {
        m_mask.setTo(pm::Scalar(0));
    }

    inline const unsigned char *get_image(int y, int x) const {
        return m_image.ptr<unsigned char>(y, x);
    }
    inline unsigned char *get_mutable_image(int y, int x) {
        return m_image.ptr<unsigned char>(y, x);
    }

    inline unsigned char get_image(int y, int x, int c) const {
        return m_image.ptr<unsigned char>(y, x)[c];
    }
    inline int get_image_int(int y, int x, int c) const {
        return static_cast<int>(m_image.ptr<unsigned char>(y, x)[c]);
    }

    bool contains_mask(int y, int x, int patch_size) const;
    MaskedImage downsample() const;
    MaskedImage upsample(int new_w, int new_h) const;
    MaskedImage upsample(int new_w, int new_h, const pm::Mat &new_global_mask) const;
    void compute_image_gradients();
    void compute_image_gradients() const;

    static const pm::Size kDownsampleKernelSize;
    static const int kDownsampleKernel[6];

private:
	pm::Mat m_image;
	pm::Mat m_mask;
    pm::Mat m_global_mask;
    pm::Mat m_image_grady;
    pm::Mat m_image_gradx;
    bool m_image_grad_computed = false;
};

