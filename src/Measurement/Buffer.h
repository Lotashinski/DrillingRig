#ifndef _BUFFER_H
#define _BUFFER_H

namespace Measurement {

class Buffer {
private:
  //   static const auto _BUFFER_SIZE = 512;

  int _bufferSize;
  int _currentIndex = 0;
  float *_bufferPtr;

  int _getRealIndex(int index);

  // It`s don`t compiled
  // Buffer &operator=(Buffer) = default;
  Buffer &operator=(const Buffer &) = default;
  Buffer &operator=(Buffer &&) = default;

public:
  Buffer(int bufferSize);
  ~Buffer();

  int getBufferSize();
  int getCurrentIndex();

  float getMeasurement(int index);
  void addMeasurement(float value);

  void clearBuffer();
};

} // namespace Measurement

#endif