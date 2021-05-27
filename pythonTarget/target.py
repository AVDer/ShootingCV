import math

import numpy as np
import cv2
import center_search
import ray_search


class Target:

    kBlurConstant = 30
    kThreshold = 50
    kWindowName = 'target'
    kBlurTbName = 'Blur'
    kThresholdTbName = 'Threshold'
    kImageSwitchName = 'Switch'

    def blur_changed(self, b):
        self.blur_constant_ = b if b > 0 else self.kBlurConstant

    def threshold_changed(self, t):
        self.threshold_ = t if t > 0 else self.kThreshold

    def selection_changed(self, t):
        self.image_selected_ = t

    def __init__(self, provider):
        self.provider_ = provider
        self.original_image_ = np.ndarray(shape=(1, 1))
        self.blurry_image_ = np.ndarray(shape=(1, 1))
        self.show_image_ = np.ndarray(shape=(1, 1))
        self.target_width_ = 0
        self.target_height_ = 0
        self.target_center_ = (0, 0)
        self.image_center_ = (0, 0)
        self.step_x_ = 0
        self.step_y_ = 0
        self.step_h_ = 0
        self.blur_constant_ = self.kBlurConstant
        self.threshold_ = self.kThreshold
        self.image_selected_ = 2

    def load(self):
        self.next_frame()
        (self.target_height_, self.target_width_, _) = self.original_image_.shape
        self.image_center_ = (self.target_width_ // 2, self.target_height_ // 2)
        cv2.namedWindow(self.kWindowName)
        cv2.setMouseCallback(self.kWindowName, self.draw_circle)
        cv2.createTrackbar(self.kBlurTbName, self.kWindowName, 0, 100, self.blur_changed)
        cv2.createTrackbar(self.kThresholdTbName, self.kWindowName, 0, 255, self.threshold_changed)
        cv2.createTrackbar(self.kImageSwitchName, self.kWindowName, 0, 2, self.selection_changed)
        cv2.setTrackbarPos(self.kBlurTbName, self.kWindowName, self.kBlurConstant)
        cv2.setTrackbarPos(self.kThresholdTbName, self.kWindowName, self.kThreshold)
        switch = '0 : Original  \n1 : Blurry \n2 : Show'


    def next_frame(self):
        self.original_image_ = self.provider_.get_frame()
        self.blurry_image_ = cv2.cvtColor(self.original_image_, cv2.COLOR_BGR2GRAY)
        self.blurry_image_ = cv2.blur(self.blurry_image_, (self.blur_constant_, self.blur_constant_))
        self.show_image_ = cv2.cvtColor(self.blurry_image_, cv2.COLOR_GRAY2BGR)

    def detect_center(self):
        black_range = center_search.detect_center(self.original_image_, self.image_center_,
                                                  self.blur_constant_, self.threshold_)
        self.target_center_ = ((black_range['x_max'] + black_range['x_min']) // 2,
                               (black_range['y_max'] + black_range['y_min']) // 2)
        black_range = center_search.detect_center(self.original_image_, self.target_center_,
                                                  self.blur_constant_, self.threshold_)
        self.target_center_ = ((black_range['x_max'] + black_range['x_min']) // 2,
                               (black_range['y_max'] + black_range['y_min']) // 2)
        # print('Target center = {}'.format(self.target_center_))

    def detect_7(self):
        black_range = center_search.detect_center(self.original_image_, self.target_center_,
                                                  self.blur_constant_, self.threshold_)
        axis7x = black_range['x_max'] - black_range['x_min']
        axis7y = black_range['y_max'] - black_range['y_min']
        self.step_x_ = axis7x // 8
        self.step_y_ = axis7y // 8
        self.step_h_ = (float(self.step_x_) + float(self.step_y_)) / 2.0

    def detect_ray(self):
        # rect = ray_search.get_ray_square(self.original_image_)
        # cv2.rectangle(self.show_image_, rect[0], rect[1], (0, 0, 255))
        rayPos = ray_search.get_ray_square(self.original_image_)
        self.add_mark(rayPos, (0, 0, 255))
        delta_x = rayPos[0] - self.target_center_[0]
        delta_y = rayPos[1] - self.target_center_[1]
        # print('deltax = {}, deltay = {}'.format(delta_x, delta_y))
        if self.step_h_ == 0:
            return
        return {'r': np.sqrt(delta_x**2 + delta_y**2) / self.step_h_, 't': np.arctan2(float(delta_y), float(delta_x))}

    def store_image(self, type):
        if type == 'original':
            cv2.imwrite('out_image.png', self.original_image_)
        if type == 'blurry':
            cv2.imwrite('out_image.png', self.blurry_image_)
        if type == 'augmented':
            cv2.imwrite('out_image.png', self.show_image_)

    def add_grid(self):
        self.add_mark(self.image_center_, (0, 0, 255))
        for s in range(10, 0, -1):
            cv2.ellipse(self.show_image_, self.target_center_, (self.step_x_ * s, self.step_y_ * s),
                        0, 0, 360, (0, 0, 255))
    
    def set_pixel(self, pixel, color):
        self.show_image_[pixel] = color

    def add_mark(self, center, color):
        cv2.circle(self.show_image_, center, 10, color, -1)
    
    def show(self):
        if self.image_selected_ == 0:
            cv2.imshow('target', self.original_image_)
        elif self.image_selected_ == 1:
            cv2.imshow('target', self.blurry_image_)
        else:
            cv2.imshow('target', self.show_image_)

    # mouse callback function
    @staticmethod
    def draw_circle(event, x, y, _, __):
        if event == cv2.EVENT_LBUTTONDBLCLK:
            pass
            # self.image_center_ = (x, y)
            # self.detect_center()
            # print(self.blurry_image_.item((y, x)))
