#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <complex>
#include <valarray>
#include <math.h>

const double PI = 3.141592653589793238460 ;

using namespace std ;
using namespace sf ;

typedef complex<double> Complex;
typedef valarray<Complex> CArray;

class FFT
{
public:
	FFT(string const& _path,int const& _bufferSize);

	void hammingWindow() ;
	void fft(CArray &x) ;
	void positions(Vector2f *);

	string path ;
	SoundBuffer buffer ;
	Sound sound ;

	vector<Complex> sample ;
	vector<float> window ;
	CArray bin ;
	
	int sampleRate ;
	int sampleCount ;
	int bufferSize ;
	int mark ;
};

