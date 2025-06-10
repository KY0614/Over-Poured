#pragma once

class FpsCounter
{
public:
    FpsCounter();
    void Update();
    void Draw(int x = 0, int y = 0);

private:
    int prevTime_;
    float instantFps_;
};