#include "FFT.h"

sf::SoundBuffer FFT::buffer;
string FFT::m_path;

FFT::FFT()
{
}

FFT::~FFT() {

}

void FFT::init(string const& path) {
	m_path = path;

	bufferSize = 16384;
	sample.resize(bufferSize);
	for (int i(0); i < bufferSize; i++) window.push_back(0.54 - 0.46*cos(2 * PI*i / (float)bufferSize));

	mark = 0;

	thread = new sf::Thread([&]() {this->load(); });
	thread->launch();

}

void FFT::fft(CArray &x)
{
	const int N = x.size();
	if(N <= 1) return;

	CArray even = x[slice(0,N/2,2)];
	CArray  odd = x[slice(1,N/2,2)];

	fft(even);
	fft(odd);

	for(int i = 0 ; i < N/2 ; i++)
	{
		Complex t = polar(1.0,-2 * PI * i / N) * odd[i];
		x[i] = even[i] + t;
		x[i+N/2] = even[i] - t;
	}
	
}

void FFT::update(float is_time)
{

	mark = is_time * (44100 * 2);

	//cout << is_time << ":" << buffer.getSampleCount() / (44100 * 2.f) << endl;

	if (is_time < buffer.getSampleCount() / (44100 * 2.f) - 1.f)
	{
		for (int i(mark); i < bufferSize + mark; i++)
		{
			sample[i - mark] = Complex(buffer.getSamples()[i] * window[i - mark], 0);	
		}
	}	


	bin = CArray(sample.data(), bufferSize);
	fft(bin);

	int cnt = 0;
	data.clear();
	for (float i(1); i < bufferSize / 4.f; i *= 1.01)
	{
		if (cnt % 4 == 0) data.push_back(0);
		data[data.size() - 1] += (is_time != 0) ? abs(bin[(int)i]) : 0;

		cnt++;
	}
}

vector<int> FFT::getData()
{
	return data;
}

void FFT::load()
{
	buffer.loadFromFile(m_path);
}
