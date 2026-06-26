#include "interface.cpp"
int main() {
    auto app = ScreenInteractive::Fullscreen();
    DequeDemoApp demo;
    app.Loop(demo.getRoot());
    return 0;
}
