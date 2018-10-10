#include "FFT.h"

FFT::FFT(string const& _path,int const& _bufferSize)
{
	path = _path ;
	if(!buffer.loadFromFile(path)) cout<<"Unable to load buffer"<<endl ;

	sound.setBuffer(buffer) ;
	sound.setLoop(true);
	sound.play();

	sampleRate = buffer.getSampleRate()*buffer.getChannelCount() ;
	sampleCount = buffer.getSampleCount() ;
	if(_bufferSize < sampleCount) bufferSize = _bufferSize ;
	else bufferSize = sampleCount ;
	mark = 0 ;

	for(int i(0) ; i < bufferSize ; i++) window.push_back(0.54-0.46*cos(2*PI*i/(float)bufferSize)) ;
	sample.resize(bufferSize) ;
}

void FFT::hammingWindow()
{
	mark = sound.getPlayingOffset().asSeconds()*sampleRate ;
	if(mark+bufferSize < sampleCount)
	{
		for(int i(mark) ; i < bufferSize+mark ; i++)
		{
			sample[i-mark] = Complex(buffer.getSamples()[i]*window[i-mark],0) ;
		}
	}
}
void FFT::fft(CArray &x)
{
	const int N = x.size();
	if(N <= 1) return;

	CArray even = x[slice(0,N/2,2)];
	CArray  odd = x[slice(1,N/2,2)];

	fft(even);
	fft(odd);

	for(int k = 0 ; k < N/2 ; k++)
	{
		Complex t = polar(1.0,-2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k+N/2] = even[k] - t;
	}
}

void FFT::positions(Vector2f * x) {
	hammingWindow();
	bin = CArray(sample.data(), bufferSize);
	fft(bin);
	int j = 0;
	for (float i(3); i < min(bufferSize / 2.f, 20000.f); i *= 1.01)
	{
		Vector2f samplePosition(log(i) / log(min(bufferSize / 2.f, 20000.f)), abs(bin[(int)i]));
		x[j] = samplePosition;
		j++;
	}
}