/*
  ==============================================================================

    multi_sine_osc.h
    Created: 24 Aug 2018 11:48:18pm
    Author:  Alexandre Arsenault

  ==============================================================================
*/

#pragma once

#include <cmath>
#include <vector>
#include <iostream>

template <typename T>
inline T lineair_interpole(T y1, T y2, T mu)
{
  return y1 + mu * (y2 - y1);
}

//#define N_SINE 160
#define N_SINE 1200
#define TABLE_SIZE 4096

class multi_sine_osc2 {
public:
  static constexpr float kTwoPi = 2.0f * M_PI;
  static constexpr float kOneOverTwoPi = 1.0f / kTwoPi;

  multi_sine_osc2() {
    _sine_table = create_sine_table(TABLE_SIZE);

    _freqs.resize(N_SINE);
    _phases.resize(N_SINE);
    _gains.resize(N_SINE);

    for(int i = 0; i < N_SINE; i++) {
      int n = i + 1;
      _freqs[i] = 160.0f * n;
      _phases[i] = 0.0f;
      _gains[i] = 1.0f - float(n) / N_SINE;
      
      std::cout << _freqs[i] << " " << _gains[i] << std::endl;
    }
  }

  void process(float** outputs, unsigned int buffer_size) {
    for(int i = 0; i < buffer_size; i++) {
      const float v = get_next();
      outputs[0][i] = 0.3 * v;
      outputs[1][i] = 0.3 * v;
    }
  }

  private:
  std::vector<float> _freqs;
  std::vector<float> _phases;
  std::vector<float> _gains;

  unsigned int _table_size = TABLE_SIZE;
  float _f_table_size = TABLE_SIZE;
  float _f_table_size_m1 = TABLE_SIZE - 1;
  float _two_pi_over_sr = kTwoPi / 44100.0f;
  float _one_over_sr = 1.0f / 44100.0f;
  std::vector<float> _sine_table;
  
  float get_next() {
    float sum = 0.0f;

    for(int i = 0; i < _phases.size(); i++) {
      const float phi = _phases[i];
      const float dk = phi * _f_table_size_m1;
      const unsigned int pos_left = dk;
      const float frac = phi + dk - pos_left;

      // Update phase.
      const float df = phi + _one_over_sr * _freqs[i];
      _phases[i] = df - (long)df;

      const float plv = _sine_table[pos_left];
      sum += (plv + frac * (_sine_table[pos_left + 1] - plv)) * _gains[i];
    }

    return sum;
  }

  // ---- 2
//  inline void update_phase() {
////    _phase += _one_over_sr * _freq;
////    _phase -= (long)_phase;
//
//    const float df = _phase + _one_over_sr * _freq;
//    _phase = df - (long)df;
//  }
//
//  float get_next() {
////    const float v = _phase * kOneOverTwoPi;
//    const float v = _phase;
//    unsigned int pos_left = v * _f_table_size_m1;
//    float frac = v * _f_table_size - pos_left;
//
//    unsigned int pos_right = pos_left + 1;
//    if (pos_right == _table_size) {
//      pos_right = 0;
//    }
//
//    update_phase();
//    return lineair_interpole<float>(_sine_table[pos_left], _sine_table[pos_right], frac);
//
//
//  }
//
//  float get_next() {
//    const float v = 1.0 - _phase * kOneOverTwoPi;
//    _phase += ((kTwoPi / 44100.0) * _freq);
//
//    while (_phase > kTwoPi) {
//      _phase -= kTwoPi;
//    }
//    while (_phase < 0.0) {
//      _phase += kTwoPi;
//    }
//
//    const unsigned int table_size = 2048;
//    unsigned int pos_left = v * ((float)table_size - 1.0f);
//    float frac = v * (float)table_size - pos_left;
//
//    unsigned int pos_right = pos_left + 1;
//    if (pos_right == table_size) {
//      pos_right = 0;
//    }
//
//    return lineair_interpole<float>(_sine_table[pos_left], _sine_table[pos_right], frac);
//  }

  
  std::vector<float> create_sine_table(unsigned int buffer_size) {
    const float one_over_buffer_size = 1.0f / (float)buffer_size;
    std::vector<float> table;
    table.resize(buffer_size + 1);

    const float mult_value = kTwoPi * one_over_buffer_size;

    for(int i = 0; i < buffer_size + 1; i++) {
        table[i] = std::sin(i * mult_value);
    }
    
    return table;
  }
};

class multi_sine_osc {
public:
  static constexpr float kTwoPi = 2.0f * M_PI;
  static constexpr float kOneOverTwoPi = 1.0f / kTwoPi;

  multi_sine_osc() {
    _sine_table = create_sine_table(2048);
  }
  
  void process(float** outputs, unsigned int buffer_size) {
    for(int i = 0; i < buffer_size; i++) {
//      _phase += ((2.0f * M_PI / 44100.0) * _freq);
      
//      outputs[0][i] = std::sin(_phase);
//      outputs[1][i] = std::sin(_phase);
      const float v = get_next();
      outputs[0][i] = v;
      outputs[1][i] = v;
    }
  }
  
  private:
  float _freq = 500.0f;
  float _phase = 0.0f;
  unsigned int _table_size = 2048;
  float _f_table_size = 2048.0f;
  float _f_table_size_m1 = 2047.0f;
  float _two_pi_over_sr = kTwoPi / 44100.0f;
  float _one_over_sr = 1.0f / 44100.0f;
  std::vector<float> _sine_table;
  
  // ---- 3
  float get_next() {
    const float dk = _phase * _f_table_size_m1;
    const unsigned int pos_left = dk;
    const float frac = (_phase + dk) - pos_left;

    // Update phase.
    const float df = _phase + _one_over_sr * _freq;
    _phase = df - (long)df;
    
    const float plv = _sine_table[pos_left];
    return plv + frac * (_sine_table[pos_left + 1] - plv);
//    return lineair_interpole<float>(_sine_table[pos_left], _sine_table[pos_right], frac);
  }
  // ---- 2
//  inline void update_phase() {
////    _phase += _one_over_sr * _freq;
////    _phase -= (long)_phase;
//
//    const float df = _phase + _one_over_sr * _freq;
//    _phase = df - (long)df;
//  }
//
//  float get_next() {
////    const float v = _phase * kOneOverTwoPi;
//    const float v = _phase;
//    unsigned int pos_left = v * _f_table_size_m1;
//    float frac = v * _f_table_size - pos_left;
//
//    unsigned int pos_right = pos_left + 1;
//    if (pos_right == _table_size) {
//      pos_right = 0;
//    }
//
//    update_phase();
//    return lineair_interpole<float>(_sine_table[pos_left], _sine_table[pos_right], frac);
//
//
//  }
//
//  float get_next() {
//    const float v = 1.0 - _phase * kOneOverTwoPi;
//    _phase += ((kTwoPi / 44100.0) * _freq);
//
//    while (_phase > kTwoPi) {
//      _phase -= kTwoPi;
//    }
//    while (_phase < 0.0) {
//      _phase += kTwoPi;
//    }
//
//    const unsigned int table_size = 2048;
//    unsigned int pos_left = v * ((float)table_size - 1.0f);
//    float frac = v * (float)table_size - pos_left;
//
//    unsigned int pos_right = pos_left + 1;
//    if (pos_right == table_size) {
//      pos_right = 0;
//    }
//
//    return lineair_interpole<float>(_sine_table[pos_left], _sine_table[pos_right], frac);
//  }

  
  std::vector<float> create_sine_table(unsigned int buffer_size) {
    const float one_over_buffer_size = 1.0f / (float)buffer_size;
    std::vector<float> table;
    table.resize(buffer_size + 1);

    const float mult_value = kTwoPi * one_over_buffer_size;

    for(int i = 0; i < buffer_size + 1; i++) {
        table[i] = std::sin(i * mult_value);
    }
    
    return table;
  }
};
