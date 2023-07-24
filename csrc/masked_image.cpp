#include "masked_image.h"
#include <algorithm>
#include <iostream>

const pm::Size MaskedImage::kDownsampleKernelSize = pm::Size(6, 6);
const int MaskedImage::kDownsampleKernel[6] = {1, 5, 10, 10, 5, 1};

bool MaskedImage::contains_mask(int y, int x, int patch_size) const {
    auto mask_size = size();
    for (int dy = -patch_size; dy <= patch_size; ++dy) {
        for (int dx = -patch_size; dx <= patch_size; ++dx) {
            int yy = y + dy, xx = x + dx;
            if (yy >= 0 && yy < mask_size.height && xx >= 0 && xx < mask_size.width) {
                if (is_masked(yy, xx) && !is_globally_masked(yy, xx)) return true;
            }
        }
    }
    return false;
}

MaskedImage MaskedImage::downsample() const {
    const auto &kernel_size = MaskedImage::kDownsampleKernelSize;
    const auto &kernel = MaskedImage::kDownsampleKernel;

    const auto size = this->size();
    const auto new_size = pm::Size(size.width / 2, size.height / 2);

    auto ret = MaskedImage(new_size.width, new_size.height, m_image.type());
    if (!m_global_mask.empty()) ret.init_global_mask_mat();
    int channels = m_image.channels();
    for (int y = 0; y < size.height - 1; y += 2) {
        for (int x = 0; x < size.width - 1; x += 2) {
            int ksum = 0;
            int *colors = new int[channels]{};
            bool is_gmasked = true;

            for (int dy = -kernel_size.height / 2 + 1; dy <= kernel_size.height / 2; ++dy) {
                for (int dx = -kernel_size.width / 2 + 1; dx <= kernel_size.width / 2; ++dx) {
                    int yy = y + dy, xx = x + dx;
                    if (yy >= 0 && yy < size.height && xx >= 0 && xx < size.width) {
                        if (!is_globally_masked(yy, xx)) {
                            is_gmasked = false;
                        }
                        if (!is_masked(yy, xx)) {
                            auto source_ptr = get_image(yy, xx);
                            int k = kernel[kernel_size.height / 2 - 1 + dy] * kernel[kernel_size.width / 2 - 1 + dx];
                            for (size_t c = 0; c < channels; c++) {
                                colors[c] += source_ptr[c] * k;
                            }
                            ksum += k;
                        }
                    }
                }
            }

            if (ksum > 0) {
                for (size_t c = 0; c < channels; c++) {
                    colors[c] /= ksum;
                }
            }

            if (!m_global_mask.empty()) {
                ret.set_global_mask(y / 2, x / 2, is_gmasked);
            }
            if (ksum > 0) {
                auto target_ptr = ret.get_mutable_image(y / 2, x / 2);
                for (size_t c = 0; c < channels; c++) {
                    target_ptr[c] = colors[c];
                }
                ret.set_mask(y / 2, x / 2, 0);
            } else {
                ret.set_mask(y / 2, x / 2, 1);
            }

            delete[] colors;
        }
    }

    return ret;
}

MaskedImage MaskedImage::upsample(int new_w, int new_h) const {
    const auto size = this->size();
    auto ret = MaskedImage(new_w, new_h, m_image.type());
    if (!m_global_mask.empty()) ret.init_global_mask_mat();
    int channels = m_image.channels();
    for (int y = 0; y < new_h; ++y) {
        for (int x = 0; x < new_w; ++x) {
            int yy = y * size.height / new_h;
            int xx = x * size.width / new_w;

            if (is_globally_masked(yy, xx)) {
                ret.set_global_mask(y, x, 1);
                ret.set_mask(y, x, 1);
            } else {
                if (!m_global_mask.empty()) ret.set_global_mask(y, x, 0);

                if (is_masked(yy, xx)) {
                    ret.set_mask(y, x, 1);
                } else {
                    auto source_ptr = get_image(yy, xx);
                    auto target_ptr = ret.get_mutable_image(y, x);
                    for (int c = 0; c < channels; ++c)
                        target_ptr[c] = source_ptr[c];
                    ret.set_mask(y, x, 0);
                }
            }
        }
    }

    return ret;
}

MaskedImage MaskedImage::upsample(int new_w, int new_h, const pm::Mat &new_global_mask) const {
    auto ret = upsample(new_w, new_h);
    ret.set_global_mask_mat(new_global_mask);
    return ret;
}

void MaskedImage::compute_image_gradients() {
    if (m_image_grad_computed) {
        return;
    }

    const auto size = m_image.size();
    int channels = m_image.channels();
    //int type = channels == 3 ? PM_8UC3 : PM_8UC1;
    int type = PM_MAKETYPE(PM_8U, channels);
    m_image_grady = pm::Mat(size, type);
    m_image_gradx = pm::Mat(size, type);
    m_image_grady = pm::Scalar::all(0);
    m_image_gradx = pm::Scalar::all(0);

    for (int i = 1; i < size.height - 1; ++i) {
        const auto *ptr = m_image.ptr<unsigned char>(i, 0);
        const auto *ptry1 = m_image.ptr<unsigned char>(i + 1, 0);
        const auto *ptry2 = m_image.ptr<unsigned char>(i - 1, 0);
        const auto *ptrx1 = m_image.ptr<unsigned char>(i, 0) + channels;
        const auto *ptrx2 = m_image.ptr<unsigned char>(i, 0) - channels;
        auto *mptry = m_image_grady.ptr<unsigned char>(i, 0);
        auto *mptrx = m_image_gradx.ptr<unsigned char>(i, 0);
        for (int j = channels; j < size.width * channels - channels; ++j) {
            mptry[j] = (ptry1[j] / 2 - ptry2[j] / 2) + 128;
            mptrx[j] = (ptrx1[j] / 2 - ptrx2[j] / 2) + 128;
        }
    }

    m_image_grad_computed = true;
}

void MaskedImage::compute_image_gradients() const {
    const_cast<MaskedImage *>(this)->compute_image_gradients();
}