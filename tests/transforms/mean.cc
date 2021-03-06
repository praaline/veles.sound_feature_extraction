/*! @file complex_to_real.cc
 *  @brief Tests for sound_feature_extraction::transforms::Mean.
 *  @author Shapichev Alexey <a.shapichev@samsung.com>
 *  @version 1.0
 *
 *  @section Notes
 *  This code partially conforms to <a href="http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml">Google C++ Style Guide</a>.
 *
 *  @section Copyright
 *  Copyright © 2013 Samsung R&D Institute Russia
 *
 *  @section License
 *  Licensed to the Apache Software Foundation (ASF) under one
 *  or more contributor license agreements.  See the NOTICE file
 *  distributed with this work for additional information
 *  regarding copyright ownership.  The ASF licenses this file
 *  to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance
 *  with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing,
 *  software distributed under the License is distributed on an
 *  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *  KIND, either express or implied.  See the License for the
 *  specific language governing permissions and limitations
 *  under the License.
 */


#include <cmath>
#include "src/transforms/mean.h"
#include "tests/transforms/transform_test.h"

using sound_feature_extraction::formats::ArrayFormatF;
using sound_feature_extraction::formats::FixedArray;
using sound_feature_extraction::BuffersBase;
using sound_feature_extraction::transforms::Mean;

class MeanTest : public TransformTest<Mean> {
 public:
  int Size;

  virtual void SetUp() {
    set_types({ sound_feature_extraction::transforms::kMeanTypeArithmetic,
                sound_feature_extraction::transforms::kMeanTypeGeometric });
    Size = 486;
    SetUpTransform(1, Size, 18000);
    for (int i = 0; i < Size; i++) {
      (*Input)[0][i] = i + 1;
    }
  }
};

#define EPSILON 0.15f

#define ASSERT_EQF(a, b) ASSERT_NEAR(a, b, EPSILON)

TEST_F(MeanTest, Do) {
  set_use_simd(false);
  Do((*Input)[0], &(*Output)[0]);

  float amean = 0.f;
  float gmean = 1.f, tmp = 1.f;
  for (int j = 0; j < Size; j++) {
    float val = (*Input)[0][j];
    amean += val;
    float multmp = tmp * val;
    if (multmp == std::numeric_limits<float>::infinity()) {
      gmean *= powf(tmp, 1.f / Size);
      tmp = val;
    } else {
      tmp = multmp;
    }
  }
  amean /= Size;
  gmean *= powf(tmp, 1.f / Size);
  ASSERT_EQF(amean, ((*Output)[0])
      [sound_feature_extraction::transforms::kMeanTypeArithmetic]);
  ASSERT_EQF(gmean, ((*Output)[0])
      [sound_feature_extraction::transforms::kMeanTypeGeometric]);
  ASSERT_EQF(amean,
             Do(false, (*Input)[0], Size,
                sound_feature_extraction::transforms::kMeanTypeArithmetic));
  ASSERT_EQF(gmean,
             Do(false, (*Input)[0], Size,
                sound_feature_extraction::transforms::kMeanTypeGeometric));
}

TEST_F(MeanTest, DoSimd) {
  set_use_simd(true);
  Do((*Input)[0], &(*Output)[0]);

  float amean = 0.f;
  float gmean = 1.f, tmp = 1.f;
  for (int j = 0; j < Size; j++) {
    float val = (*Input)[0][j];
    amean += val;
    float multmp = tmp * val;
    if (multmp == std::numeric_limits<float>::infinity()) {
      gmean *= powf(tmp, 1.f / Size);
      tmp = val;
    } else {
      tmp = multmp;
    }
  }
  amean /= Size;
  gmean *= powf(tmp, 1.f / Size);
  ASSERT_EQF(amean, ((*Output)[0])
      [sound_feature_extraction::transforms::kMeanTypeArithmetic]);
  ASSERT_EQF(gmean, ((*Output)[0])
      [sound_feature_extraction::transforms::kMeanTypeGeometric]);
  ASSERT_EQF(amean,
             Do(false, (*Input)[0], Size,
                sound_feature_extraction::transforms::kMeanTypeArithmetic));
  ASSERT_EQF(gmean,
             Do(false, (*Input)[0], Size,
                sound_feature_extraction::transforms::kMeanTypeGeometric));
}

TEST_F(MeanTest, DoCase1) {
  float data[] {
       14.000000, 12.147677, 7.239147,  1.920708,  5.957093,  9.768391,
       10.661250, 8.767072,  5.146831,  1.545739,
       2.542431,  4.095103,  4.268193,  3.487323,  2.419142,  1.679975,
       1.614317,  1.932667,  2.242195,  2.394188,
       2.421191,  2.678555,  3.710246,  5.343172,  6.749712,  7.024582,
       5.638990,  3.404799,  4.893416,  8.849478,
       11.533978, 11.468283, 8.327876,  3.028759,  3.826278,  8.811692,
       11.308797, 10.646174, 7.323343,  2.797879,
       2.428811,  5.215474,  6.156565,  5.400490,  3.810879,  2.359146,
       1.500396,  1.038677,  1.125559,  1.527126,
       1.637754,  1.585411,  1.999519,  2.711942,  3.145981,  3.151658,
       3.015997,  3.240598,  4.112591,  5.435161,
       6.637612,  6.981294,  5.896019,  3.464489,  2.449490,  5.461041,
       7.942804,  8.417841,  6.677486,  3.596703,
       2.589705,  4.832062,  6.014294,  5.606078,  4.626289,  4.552700,
       5.004514,  4.624925,  3.121680,  1.513250,
       2.142658,  2.988013,  2.727604,  1.585353,  0.509548,  0.912408,
       0.951881,  1.048167,  2.098451,  3.050678,
       3.265496,  2.714679,  2.034716,  2.226161,  2.983628,  3.635599,
       4.015833,  3.936847,  3.112669,  1.524254,
       1.105299,  2.855775,  3.761478,  3.377796,  2.582373,  3.663632,
       5.529967,  6.274077,  5.218201,  2.618501,
       1.319440,  4.050600,  5.615703,  5.424427,  3.769471,  1.476823,
       0.505356,  1.479253,  1.335812,  0.505446,
       0.457828,  0.686575,  0.090077,  1.139614,  2.380332,  3.019486,
       2.706917,  1.538594,  0.000000,  1.284005,
       1.865358,  1.694431,  1.121461,  0.597073,  0.554776,  1.616257,
       3.098090,  4.259238,  4.394960,  3.152024,
       0.963656,  2.548137,  4.992091,  6.150478,  5.615703,  3.635787,
       0.969018,  1.447651,  2.887414,  3.110871,
       2.414879,  1.442621,  0.789162,  0.516361,  0.830651,  1.468186,
       1.716659,  1.220493,  0.256707,  1.473497,
       2.533228,  2.742794,  2.051204,  1.263791,  1.886841,  2.594487,
       2.423380,  1.473448,  1.130886,  2.106285,
       2.655293,  2.586874,  2.647682,  3.307341,  3.804050,  3.440928,
       2.142658,  0.595667,  1.673491,  2.725870,
       2.911726,  2.433653,  1.831784,  1.464580,  1.052485,  0.361370,
       0.638862,  1.238473,  1.297600,  1.001795,
       1.338005,  2.103705,  2.449490,  2.144198,  1.560320,  1.557979,
       1.920745,  1.877495,  1.616533,  2.071501,
       2.909563,  3.201581,  2.572886,  1.163561,  0.576751,  1.924525,
       2.485965,  2.130837,  1.125559,  0.198161,
       0.928041,  1.240121,  1.267145,  1.458520,  1.674948,  1.475499,
       0.767147,  0.634006,  1.587356,  2.181869,
       2.211601,  1.919891,  1.841200,  2.045926,  2.025416,  1.494594,
       0.567949,  0.809057,  1.855393,  2.598861,
       2.877227,  2.632328,  1.914737,  1.100587,  1.402948,  2.319586,
       2.835637,  2.710121,  2.109199,  1.587058,
       1.614317,  1.706014,  1.474589,  1.340186,  1.834860,  2.353730,
       2.316422,  1.580782,  0.418109,  1.134244,
       2.182069,  2.637870,  2.458133,  1.933196,  1.648814,  1.842487,
       2.000000
  };
  Size = sizeof(data) / sizeof(data[0]);
  set_types({ sound_feature_extraction::transforms::kMeanTypeGeometric });
  SetUpTransform(1, Size, 18000);
  memcpy((*Input)[0], data, sizeof(data));
  Do((*Input)[0], &(*Output)[0]);
  ASSERT_NE(std::numeric_limits<float>::infinity(), (*Output)[0][1]);
}

TEST_F(MeanTest, DoCase2) {
  float data[] {
       0.000000, 174534.921875, 363713.125000, 158088.109375, 76958.414062,
135526.343750, 144309.203125, 58393.785156, 33235.437500, 44237.996094,
73492.367188, 41352.394531, 50468.804688, 9648.817383, 18265.562500,
11295.250000, 47312.875000, 155001.906250, 121486.257812, 12168.852539,
15219.207031, 6229.041992, 27532.867188, 101634.257812, 70659.492188,
5231.288086, 820.821655, 6008.090332, 11406.963867, 28007.910156,
21704.867188, 8948.859375, 1396.351562, 3701.672119, 7176.165527, 10110.354492,
6907.112793, 6100.512207, 12691.864258, 19925.082031,
156647.015625, 272904.562500, 79817.570312, 10876.429688, 12387.514648,
6738.259277, 9335.102539, 15629.714844, 2855.738525, 8059.276855,
3588.575439, 10778.733398, 83133.617188, 80811.500000, 7010.745117, 6236.867188,
5523.004395, 1072.870605, 14273.165039, 7559.005859,
6105.828613, 392.527161, 1929.515869, 9103.837891, 44630.539062, 31082.943359,
3356.623779, 3941.327637, 2943.267334, 9331.450195,
18075.236328, 9949.333008, 2371.245361, 6895.093262, 6327.318359, 2068.149902,
2294.762451, 950.338135, 2381.619629, 4169.156250,
3243.211426, 2408.314941, 707.504944, 3463.633057, 3992.360596, 1481.886963,
597.379639, 3389.233643, 2797.511475, 1224.815063,
633.191589, 4191.746582, 2785.958740, 1778.388062, 4858.080078, 6312.399902,
7069.471191, 4078.877197, 3256.672363, 1470.807495,
3507.032959, 1868.728027, 972.252075, 1954.370728, 286.370209, 2366.099121,
5586.568848, 4368.388184, 4485.822266, 1865.775269,
1309.446533, 3239.059326, 4093.988281, 945.786133, 2779.017090, 8761.343750,
8763.137695, 2979.692627, 2809.864746, 2513.969727,
3433.806152, 2057.458252, 3527.171143, 2384.616211, 1599.854980, 544.953552,
178.092911, 1005.292908, 3501.218262, 2550.821045,
1691.368042, 923.711853, 887.410461, 941.327209, 1048.781860, 865.366882,
1141.654175, 2235.934326, 3593.646973, 1681.757812,
902.951843, 2083.257812, 1447.812866, 3760.597656, 2087.944580, 3271.651367,
4136.012207, 1617.045898, 1669.840576, 793.239929,
1910.080078, 2064.263672, 2721.561279, 1037.418579, 1871.753418, 598.323120,
1544.380371, 1109.546875, 2128.314453, 778.795898,
2366.198730, 2099.548096, 624.754150, 692.551331, 2672.342773, 3808.154541,
4166.220703, 1567.795410, 1186.400146, 928.393250,
1296.710449, 927.605408, 889.132568, 3100.520020, 5465.414062, 2218.909424,
3679.416016, 1445.503662, 983.549316, 1417.571289,
121.046036, 1487.266602, 2024.660522, 2452.508545, 1924.500000, 1355.417603,
1507.671509, 884.640137, 708.225647, 406.998932,
1514.321045, 1436.739624, 3010.937500, 6069.840332, 3404.605957, 40.761181,
923.983032, 766.219604, 562.850098, 1062.388550,
699.756714, 1585.637451, 750.565125, 567.358521, 854.308289, 1028.857666,
722.610229, 411.536743, 450.221100, 330.053101,
988.240479, 204.808395, 832.946960, 349.143402, 293.554443, 206.467712,
525.647583, 318.814209, 379.623230, 238.549683,
451.286591, 102.414032, 472.854156, 353.080322, 311.802032, 296.761566,
314.935150, 318.023590, 383.252960, 301.106781,
296.946350, 342.141510, 302.027588, 337.416687, 313.299835, 320.481018,
316.254730, 331.969971, 305.680176, 302.738312,
322.392029, 311.618988, 307.117310, 318.374573, 310.265594, 308.569946,
323.758423, 306.137848, 304.607819, 312.569366,
308.892578, 319.420044, 323.295959, 312.145782, 307.832825, 322.155792,
310.000000
  };
  Size = sizeof(data) / sizeof(data[0]);
  set_types({ sound_feature_extraction::transforms::kMeanTypeGeometric });
  SetUpTransform(1, Size, 18000);
  memcpy((*Input)[0], data, sizeof(data));
  Do((*Input)[0], &(*Output)[0]);
  ASSERT_NE(std::numeric_limits<float>::infinity(), (*Output)[0][1]);
}

#define EXTRA_PARAM sound_feature_extraction::transforms::kMeanTypeArithmetic
#define CLASS_NAME MeanTest
#define ITER_COUNT 500000
#define NO_OUTPUT
#define BENCH_NAME BenchmarkArithmetic
#include "tests/transforms/benchmark.inc"

#undef EXTRA_PARAM
#define EXTRA_PARAM sound_feature_extraction::transforms::kMeanTypeGeometric
#undef BENCH_NAME
#define BENCH_NAME BenchmarkGeometric
#include "tests/transforms/benchmark.inc"
