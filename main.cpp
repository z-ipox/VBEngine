#include "VulkanBomb/VBEngine.hpp"

using namespace std;

int main() {
    VBEngine engine;
    
    if (!engine.Init("VBEngine", 800, 600)) {
        cout << "CRITICAL ERROR: Engine failed to initialize!\n";
        return -1; 
    }

    engine.RunTriangle2D<true>();
    return 0;
}