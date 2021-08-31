#include "Buffer.h"

using namespace Measurement;

Buffer::Buffer(int bufferSize)
    : _bufferSize(bufferSize) {
        _bufferPtr = new float[_bufferSize];
    }

Buffer::~Buffer(){
    delete[] _bufferPtr;
}

int Buffer::getBufferSize() { return _bufferSize; }
int Buffer::getCurrentIndex() { return _currentIndex; }

float Buffer::getMeasurement(int index) {
  auto i = _getRealIndex(index);
  return _bufferPtr[i];
}

void Buffer::addMeasurement(float value) {
  auto i = _getRealIndex(_currentIndex);
  _currentIndex++;
  _bufferPtr[i] = value;
}

int Buffer::_getRealIndex(int index) { return index % _bufferSize; }

void Buffer::clearBuffer() { _currentIndex = 0; }