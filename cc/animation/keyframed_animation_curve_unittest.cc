// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/animation/keyframed_animation_curve.h"

#include "cc/animation/transform_operations.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace cc {
namespace {

void ExpectTranslateX(double translate_x, const gfx::Transform& transform) {
  EXPECT_FLOAT_EQ(translate_x, transform.matrix().getDouble(0, 3));
}

void ExpectBrightness(double brightness, const FilterOperations& filter) {
  EXPECT_EQ(1u, filter.size());
  EXPECT_EQ(FilterOperation::BRIGHTNESS, filter.at(0).type());
  EXPECT_FLOAT_EQ(brightness, filter.at(0).amount());
}

// Tests that a float animation with one keyframe works as expected.
TEST(KeyframedAnimationCurveTest, OneFloatKeyframe) {
  scoped_ptr<KeyframedFloatAnimationCurve> curve(
      KeyframedFloatAnimationCurve::Create());
  curve->AddKeyframe(
      FloatKeyframe::Create(0.0, 2.f, scoped_ptr<TimingFunction>()));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(-1.f));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(0.f));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(0.5f));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(1.f));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(2.f));
}

// Tests that a float animation with two keyframes works as expected.
TEST(KeyframedAnimationCurveTest, TwoFloatKeyframe) {
  scoped_ptr<KeyframedFloatAnimationCurve> curve(
      KeyframedFloatAnimationCurve::Create());
  curve->AddKeyframe(
      FloatKeyframe::Create(0.0, 2.f, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(1.0, 4.f, scoped_ptr<TimingFunction>()));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(-1.f));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(0.f));
  EXPECT_FLOAT_EQ(3.f, curve->GetValue(0.5f));
  EXPECT_FLOAT_EQ(4.f, curve->GetValue(1.f));
  EXPECT_FLOAT_EQ(4.f, curve->GetValue(2.f));
}

// Tests that a float animation with three keyframes works as expected.
TEST(KeyframedAnimationCurveTest, ThreeFloatKeyframe) {
  scoped_ptr<KeyframedFloatAnimationCurve> curve(
      KeyframedFloatAnimationCurve::Create());
  curve->AddKeyframe(
      FloatKeyframe::Create(0.0, 2.f, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(1.0, 4.f, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(2.0, 8.f, scoped_ptr<TimingFunction>()));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(-1.f));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(0.f));
  EXPECT_FLOAT_EQ(3.f, curve->GetValue(0.5f));
  EXPECT_FLOAT_EQ(4.f, curve->GetValue(1.f));
  EXPECT_FLOAT_EQ(6.f, curve->GetValue(1.5f));
  EXPECT_FLOAT_EQ(8.f, curve->GetValue(2.f));
  EXPECT_FLOAT_EQ(8.f, curve->GetValue(3.f));
}

// Tests that a float animation with multiple keys at a given time works sanely.
TEST(KeyframedAnimationCurveTest, RepeatedFloatKeyTimes) {
  scoped_ptr<KeyframedFloatAnimationCurve> curve(
      KeyframedFloatAnimationCurve::Create());
  curve->AddKeyframe(
      FloatKeyframe::Create(0.0, 4.f, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(1.0, 4.f, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(1.0, 6.f, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(2.0, 6.f, scoped_ptr<TimingFunction>()));

  EXPECT_FLOAT_EQ(4.f, curve->GetValue(-1.f));
  EXPECT_FLOAT_EQ(4.f, curve->GetValue(0.f));
  EXPECT_FLOAT_EQ(4.f, curve->GetValue(0.5f));

  // There is a discontinuity at 1. Any value between 4 and 6 is valid.
  float value = curve->GetValue(1.f);
  EXPECT_TRUE(value >= 4 && value <= 6);

  EXPECT_FLOAT_EQ(6.f, curve->GetValue(1.5f));
  EXPECT_FLOAT_EQ(6.f, curve->GetValue(2.f));
  EXPECT_FLOAT_EQ(6.f, curve->GetValue(3.f));
}

// Tests that a transform animation with one keyframe works as expected.
TEST(KeyframedAnimationCurveTest, OneTransformKeyframe) {
  scoped_ptr<KeyframedTransformAnimationCurve> curve(
      KeyframedTransformAnimationCurve::Create());
  TransformOperations operations;
  operations.AppendTranslate(2.f, 0.f, 0.f);
  curve->AddKeyframe(
      TransformKeyframe::Create(0.f, operations, scoped_ptr<TimingFunction>()));

  ExpectTranslateX(2.f, curve->GetValue(-1.f));
  ExpectTranslateX(2.f, curve->GetValue(0.f));
  ExpectTranslateX(2.f, curve->GetValue(0.5f));
  ExpectTranslateX(2.f, curve->GetValue(1.f));
  ExpectTranslateX(2.f, curve->GetValue(2.f));
}

// Tests that a transform animation with two keyframes works as expected.
TEST(KeyframedAnimationCurveTest, TwoTransformKeyframe) {
  scoped_ptr<KeyframedTransformAnimationCurve> curve(
      KeyframedTransformAnimationCurve::Create());
  TransformOperations operations1;
  operations1.AppendTranslate(2.f, 0.f, 0.f);
  TransformOperations operations2;
  operations2.AppendTranslate(4.f, 0.f, 0.f);

  curve->AddKeyframe(TransformKeyframe::Create(
      0.f, operations1, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(TransformKeyframe::Create(
      1.f, operations2, scoped_ptr<TimingFunction>()));
  ExpectTranslateX(2.f, curve->GetValue(-1.f));
  ExpectTranslateX(2.f, curve->GetValue(0.f));
  ExpectTranslateX(3.f, curve->GetValue(0.5f));
  ExpectTranslateX(4.f, curve->GetValue(1.f));
  ExpectTranslateX(4.f, curve->GetValue(2.f));
}

// Tests that a transform animation with three keyframes works as expected.
TEST(KeyframedAnimationCurveTest, ThreeTransformKeyframe) {
  scoped_ptr<KeyframedTransformAnimationCurve> curve(
      KeyframedTransformAnimationCurve::Create());
  TransformOperations operations1;
  operations1.AppendTranslate(2.f, 0.f, 0.f);
  TransformOperations operations2;
  operations2.AppendTranslate(4.f, 0.f, 0.f);
  TransformOperations operations3;
  operations3.AppendTranslate(8.f, 0.f, 0.f);
  curve->AddKeyframe(TransformKeyframe::Create(
      0.f, operations1, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(TransformKeyframe::Create(
      1.f, operations2, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(TransformKeyframe::Create(
      2.f, operations3, scoped_ptr<TimingFunction>()));
  ExpectTranslateX(2.f, curve->GetValue(-1.f));
  ExpectTranslateX(2.f, curve->GetValue(0.f));
  ExpectTranslateX(3.f, curve->GetValue(0.5f));
  ExpectTranslateX(4.f, curve->GetValue(1.f));
  ExpectTranslateX(6.f, curve->GetValue(1.5f));
  ExpectTranslateX(8.f, curve->GetValue(2.f));
  ExpectTranslateX(8.f, curve->GetValue(3.f));
}

// Tests that a transform animation with multiple keys at a given time works
// sanely.
TEST(KeyframedAnimationCurveTest, RepeatedTransformKeyTimes) {
  scoped_ptr<KeyframedTransformAnimationCurve> curve(
      KeyframedTransformAnimationCurve::Create());
  // A step function.
  TransformOperations operations1;
  operations1.AppendTranslate(4.f, 0.f, 0.f);
  TransformOperations operations2;
  operations2.AppendTranslate(4.f, 0.f, 0.f);
  TransformOperations operations3;
  operations3.AppendTranslate(6.f, 0.f, 0.f);
  TransformOperations operations4;
  operations4.AppendTranslate(6.f, 0.f, 0.f);
  curve->AddKeyframe(TransformKeyframe::Create(
      0.f, operations1, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(TransformKeyframe::Create(
      1.f, operations2, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(TransformKeyframe::Create(
      1.f, operations3, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(TransformKeyframe::Create(
      2.f, operations4, scoped_ptr<TimingFunction>()));

  ExpectTranslateX(4.f, curve->GetValue(-1.f));
  ExpectTranslateX(4.f, curve->GetValue(0.f));
  ExpectTranslateX(4.f, curve->GetValue(0.5f));

  // There is a discontinuity at 1. Any value between 4 and 6 is valid.
  gfx::Transform value = curve->GetValue(1.f);
  EXPECT_GE(value.matrix().getDouble(0.f, 3.f), 4);
  EXPECT_LE(value.matrix().getDouble(0.f, 3.f), 6);

  ExpectTranslateX(6.f, curve->GetValue(1.5f));
  ExpectTranslateX(6.f, curve->GetValue(2.f));
  ExpectTranslateX(6.f, curve->GetValue(3.f));
}

// Tests that a filter animation with one keyframe works as expected.
TEST(KeyframedAnimationCurveTest, OneFilterKeyframe) {
  scoped_ptr<KeyframedFilterAnimationCurve> curve(
      KeyframedFilterAnimationCurve::Create());
  FilterOperations operations;
  operations.Append(FilterOperation::CreateBrightnessFilter(2.f));
  curve->AddKeyframe(
      FilterKeyframe::Create(0.f, operations, scoped_ptr<TimingFunction>()));

  ExpectBrightness(2.f, curve->GetValue(-1.f));
  ExpectBrightness(2.f, curve->GetValue(0.f));
  ExpectBrightness(2.f, curve->GetValue(0.5f));
  ExpectBrightness(2.f, curve->GetValue(1.f));
  ExpectBrightness(2.f, curve->GetValue(2.f));
}

// Tests that a filter animation with two keyframes works as expected.
TEST(KeyframedAnimationCurveTest, TwoFilterKeyframe) {
  scoped_ptr<KeyframedFilterAnimationCurve> curve(
      KeyframedFilterAnimationCurve::Create());
  FilterOperations operations1;
  operations1.Append(FilterOperation::CreateBrightnessFilter(2.f));
  FilterOperations operations2;
  operations2.Append(FilterOperation::CreateBrightnessFilter(4.f));

  curve->AddKeyframe(FilterKeyframe::Create(
      0.f, operations1, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(FilterKeyframe::Create(
      1.f, operations2, scoped_ptr<TimingFunction>()));
  ExpectBrightness(2.f, curve->GetValue(-1.f));
  ExpectBrightness(2.f, curve->GetValue(0.f));
  ExpectBrightness(3.f, curve->GetValue(0.5f));
  ExpectBrightness(4.f, curve->GetValue(1.f));
  ExpectBrightness(4.f, curve->GetValue(2.f));
}

// Tests that a filter animation with three keyframes works as expected.
TEST(KeyframedAnimationCurveTest, ThreeFilterKeyframe) {
  scoped_ptr<KeyframedFilterAnimationCurve> curve(
      KeyframedFilterAnimationCurve::Create());
  FilterOperations operations1;
  operations1.Append(FilterOperation::CreateBrightnessFilter(2.f));
  FilterOperations operations2;
  operations2.Append(FilterOperation::CreateBrightnessFilter(4.f));
  FilterOperations operations3;
  operations3.Append(FilterOperation::CreateBrightnessFilter(8.f));
  curve->AddKeyframe(FilterKeyframe::Create(
      0.f, operations1, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(FilterKeyframe::Create(
      1.f, operations2, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(FilterKeyframe::Create(
      2.f, operations3, scoped_ptr<TimingFunction>()));
  ExpectBrightness(2.f, curve->GetValue(-1.f));
  ExpectBrightness(2.f, curve->GetValue(0.f));
  ExpectBrightness(3.f, curve->GetValue(0.5f));
  ExpectBrightness(4.f, curve->GetValue(1.f));
  ExpectBrightness(6.f, curve->GetValue(1.5f));
  ExpectBrightness(8.f, curve->GetValue(2.f));
  ExpectBrightness(8.f, curve->GetValue(3.f));
}

// Tests that a filter animation with multiple keys at a given time works
// sanely.
TEST(KeyframedAnimationCurveTest, RepeatedFilterKeyTimes) {
  scoped_ptr<KeyframedFilterAnimationCurve> curve(
      KeyframedFilterAnimationCurve::Create());
  // A step function.
  FilterOperations operations1;
  operations1.Append(FilterOperation::CreateBrightnessFilter(4.f));
  FilterOperations operations2;
  operations2.Append(FilterOperation::CreateBrightnessFilter(4.f));
  FilterOperations operations3;
  operations3.Append(FilterOperation::CreateBrightnessFilter(6.f));
  FilterOperations operations4;
  operations4.Append(FilterOperation::CreateBrightnessFilter(6.f));
  curve->AddKeyframe(FilterKeyframe::Create(
      0.f, operations1, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(FilterKeyframe::Create(
      1.f, operations2, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(FilterKeyframe::Create(
      1.f, operations3, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(FilterKeyframe::Create(
      2.f, operations4, scoped_ptr<TimingFunction>()));

  ExpectBrightness(4.f, curve->GetValue(-1.f));
  ExpectBrightness(4.f, curve->GetValue(0.f));
  ExpectBrightness(4.f, curve->GetValue(0.5f));

  // There is a discontinuity at 1. Any value between 4 and 6 is valid.
  FilterOperations value = curve->GetValue(1.f);
  EXPECT_EQ(1u, value.size());
  EXPECT_EQ(FilterOperation::BRIGHTNESS, value.at(0).type());
  EXPECT_GE(value.at(0).amount(), 4);
  EXPECT_LE(value.at(0).amount(), 6);

  ExpectBrightness(6.f, curve->GetValue(1.5f));
  ExpectBrightness(6.f, curve->GetValue(2.f));
  ExpectBrightness(6.f, curve->GetValue(3.f));
}

// Tests that the keyframes may be added out of order.
TEST(KeyframedAnimationCurveTest, UnsortedKeyframes) {
  scoped_ptr<KeyframedFloatAnimationCurve> curve(
      KeyframedFloatAnimationCurve::Create());
  curve->AddKeyframe(
      FloatKeyframe::Create(2.0, 8.f, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(0.0, 2.f, scoped_ptr<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(1.0, 4.f, scoped_ptr<TimingFunction>()));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(-1.f));
  EXPECT_FLOAT_EQ(2.f, curve->GetValue(0.f));
  EXPECT_FLOAT_EQ(3.f, curve->GetValue(0.5f));
  EXPECT_FLOAT_EQ(4.f, curve->GetValue(1.f));
  EXPECT_FLOAT_EQ(6.f, curve->GetValue(1.5f));
  EXPECT_FLOAT_EQ(8.f, curve->GetValue(2.f));
  EXPECT_FLOAT_EQ(8.f, curve->GetValue(3.f));
}

// Tests that a cubic bezier timing function works as expected.
TEST(KeyframedAnimationCurveTest, CubicBezierTimingFunction) {
  scoped_ptr<KeyframedFloatAnimationCurve> curve(
      KeyframedFloatAnimationCurve::Create());
  curve->AddKeyframe(FloatKeyframe::Create(
      0.0,
      0.f,
      CubicBezierTimingFunction::Create(0.25f, 0.f, 0.75f, 1.f)
          .PassAs<TimingFunction>()));
  curve->AddKeyframe(
      FloatKeyframe::Create(1.0, 1.f, scoped_ptr<TimingFunction>()));

  EXPECT_FLOAT_EQ(0.f, curve->GetValue(0.f));
  EXPECT_LT(0.f, curve->GetValue(0.25f));
  EXPECT_GT(0.25f, curve->GetValue(0.25f));
  EXPECT_NEAR(curve->GetValue(0.5f), 0.5f, 0.00015f);
  EXPECT_LT(0.75f, curve->GetValue(0.75f));
  EXPECT_GT(1.f, curve->GetValue(0.75f));
  EXPECT_FLOAT_EQ(1.f, curve->GetValue(1.f));
}

}  // namespace
}  // namespace cc
